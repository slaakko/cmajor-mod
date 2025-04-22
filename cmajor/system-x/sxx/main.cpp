// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.systemx.machine;
import cmajor.systemx.executor.console;
import cmajor.systemx.kernel;
import cmajor.systemx.sxutil;
import soul.xml.xpath;
import util;
import std.filesystem;
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

const char* version = "5.1.0";

void PrintHelp()
{
    std::cout << "System X Executor version " << version << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: sxx [options] [ FILE.x [ arguments for FILE.x ] ]" << std::endl;
    std::cout << std::endl;
    std::cout << "Run FILE.x in the System X virtual machine." << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
    std::cout << "--debug=MODE | -d=MODE" << std::endl;
    std::cout << "  Set debug mode to MODE (default mode is 0=no kernel debugging)." << std::endl;
}

int main(int argc, const char** argv)
{
    uint8_t exitCode = 0;
    cmajor::systemx::executor::Console console;
    try
    {
        InitApplication();
        std::vector<std::string> args;
        std::vector<std::string> env;
        bool programFileSeen = false;
        bool verbose = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (!programFileSeen && arg.starts_with("--"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--debug")
                        {
                            int debugMode = std::stoi(components[1]);
                            cmajor::systemx::kernel::SetDebugMode(debugMode);
                        }
                        else
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    if (arg == "--help")
                    {
                        PrintHelp();
                        return 1;
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
            }
            else if (!programFileSeen && arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-d")
                        {
                            int debugMode = std::stoi(components[1]);
                            cmajor::systemx::kernel::SetDebugMode(debugMode);
                        }
                        else
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else
                {
                    for (char o : options)
                    {
                        switch (o)
                        {
                            case 'h':
                            {
                                PrintHelp();
                                return 1;
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
            std::string filePath = cmajor::systemx::SearchBin("sh");
            args.push_back(filePath);
        }
        cmajor::systemx::machine::Machine machine;
        cmajor::systemx::kernel::Kernel::Instance().SetMachine(&machine);
        cmajor::systemx::kernel::Kernel::Instance().Start();
        cmajor::systemx::kernel::Process* process = cmajor::systemx::kernel::ProcessManager::Instance().CreateProcess();
        process->SetFilePath(args[0]);
        process->SetUMask(cmajor::systemx::machine::UMask());
        if (util::Path::GetFileName(process->FilePath()) != "sh.x")
        {
            process->SetUID(cmajor::systemx::machine::UID());
            process->SetGID(cmajor::systemx::machine::GID());
        }
        cmajor::systemx::kernel::Load(process, args, env, machine);
        if (verbose)
        {
            std::cout << "running '" << args[0] << "'..." << std::endl;;
        }
        machine.Start();
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
        if (verbose)
        {
            if (exitCode == 0)
            {
                exitCode = process->ExitCode();
            }
            std::cout << "'" << args[0] << "' exited with code " << static_cast<int>(exitCode) << std::endl;
            std::cout << std::endl;
            std::chrono::steady_clock::duration userTime = process->UserTime();
            std::chrono::steady_clock::duration sleepTime = process->SleepTime();
            std::chrono::steady_clock::duration systemTime = process->SystemTime();
            std::chrono::steady_clock::duration totalTime = userTime + sleepTime + systemTime;
            std::chrono::steady_clock::duration childUserTime = process->ChildUserTime();
            std::chrono::steady_clock::duration childSleepTime = process->ChildSleepTime();
            std::chrono::steady_clock::duration childSystemTime = process->ChildSystemTime();
            std::chrono::steady_clock::duration childTotalTime = childUserTime + childSleepTime + childSystemTime;
            double userTimePercent = 0.0;
            double sleepTimePercent = 0.0;
            double systemTimePercent = 0.0;
            if (totalTime.count() > 0)
            {
                userTimePercent = (100.0 * userTime) / totalTime;
                sleepTimePercent = (100.0 * sleepTime) / totalTime;
                systemTimePercent = (100.0 * systemTime) / totalTime;
            }
            double childUserTimePercent = 0.0;
            double childSleepTimePercent = 0.0;
            double childSystemTimePercent = 0.0;
            if (childTotalTime.count() > 0)
            {
                childUserTimePercent = (100.0 * childUserTime) / childTotalTime;
                childSleepTimePercent = (100.0 * childSleepTime) / childTotalTime;
                childSystemTimePercent = (100.0 * childSystemTime) / childTotalTime;
            }
            std::cout << "user time:         " << util::DurationStr(userTime) << " (" << userTimePercent << "%)" << std::endl;
            std::cout << "sleep time:        " << util::DurationStr(sleepTime) << " (" << sleepTimePercent << "%)" << std::endl;
            std::cout << "system time:       " << util::DurationStr(systemTime) << " (" << systemTimePercent << "%)" << std::endl;
            std::cout << "total time:        " << util::DurationStr(totalTime) << std::endl;
            std::cout << "child user time:   " << util::DurationStr(childUserTime) << " (" << childUserTimePercent << "%)" << std::endl;
            std::cout << "child sleep time:  " << util::DurationStr(childSleepTime) << " (" << childSleepTimePercent << "%)" << std::endl;
            std::cout << "child system time: " << util::DurationStr(childSystemTime) << " (" << childSystemTimePercent << "%)" << std::endl;;
            std::cout << "child total time:  " << util::DurationStr(childTotalTime) << std::endl;
        }
        cmajor::systemx::kernel::ProcessManager::Instance().DeleteProcess(process->Id());
        cmajor::systemx::kernel::Kernel::Instance().Stop();
        machine.Exit();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    DoneApplication();
    return exitCode;
}
