// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import cmajor.systemx.sxutil;
import cmajor.systemx.sxcdb.debugger;
import cmajor.systemx.sxcdb.break_point;
import util;
import std.core;

void InitApplication()
{
    util::Init();
    cmajor::systemx::kernel::Init(true);
}

void DoneApplication()
{
    cmajor::systemx::kernel::Done();
    util::Done();
}

std::string Version()
{
    return "5.1.0";
}

void PrintHelp()
{
    std::cout << "System X command line debugger version " << Version() << "\n";
    std::cout << "Usage: sxcdb [options] FILE.x" << "\n";
    std::cout << "Debug program FILE.x" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bool verbose = false;
        bool programFileSeen = false;
        std::vector<std::string> args;
        std::vector<std::string> env;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
                }
                else if (arg == "--verbose")
                {
                    verbose = true;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                        case 'h':
                        {
                            PrintHelp();
                            return 0;
                        }
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                    }
                }
            }
            else if (!programFileSeen)
            {
                programFileSeen = true;
                std::string filePath = cmajor::systemx::SearchBin(arg);
                args.push_back(filePath);
            }
            else
            {
                args.push_back(arg);
            }
        }
        if (!programFileSeen)
        {
            throw std::runtime_error("program not set");
        }
        cmajor::systemx::machine::Machine machine;
        cmajor::systemx::kernel::Kernel::Instance().SetMachine(&machine);
        cmajor::systemx::kernel::Kernel::Instance().Start();
        cmajor::systemx::kernel::Process* process = cmajor::systemx::kernel::ProcessManager::Instance().CreateProcess();
        process->SetFilePath(util::GetFullPath(args[0]));
        process->SetUMask(cmajor::systemx::machine::UMask());
        if (util::Path::GetFileName(process->FilePath()) != "sh.x")
        {
            process->SetUID(cmajor::systemx::machine::UID());
            process->SetGID(cmajor::systemx::machine::GID());
        }
        cmajor::systemx::kernel::Load(process, args, env, machine);
        if (verbose)
        {
            std::cout << "debugging '" << args[0] << "'..." << std::endl;;
        }
        cmajor::systemx::sxcdb::Debugger debugger(&machine, process);
        process->SetDebugger(&debugger);
        cmajor::systemx::object::Symbol* entryPointSymbol = process->GetSymbolTable()->GetSymbol("@entry_point");
        if (entryPointSymbol)
        {
            debugger.AddBreakPoint(cmajor::systemx::sxcdb::BreakPoint(entryPointSymbol->Start()));
        }
        else
        {
            throw std::runtime_error("program entry point not found");
        }
        machine.Start();
        uint8_t exitCode = 0;
        cmajor::systemx::kernel::ProcessManager::Instance().WaitForProcessesExit();
        if (machine.HasException())
        {
            try
            {
                machine.CheckExceptions();
            }
            catch (const std::exception& ex)
            {
                exitCode = 255;
                std::cout << ex.what() << std::endl;
            }
        }
        if (exitCode == 0)
        {
            exitCode = process->ExitCode();
        }
        cmajor::systemx::kernel::WriteToTerminal("'" + args[0] + "' exited with code " + std::to_string(static_cast<int>(exitCode)) + "\n", process);
        cmajor::systemx::kernel::Kernel::Instance().Stop();
        if (machine.Started())
        {
            machine.Exit();
        }
        
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    DoneApplication();
    return 0;
}