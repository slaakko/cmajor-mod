// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import cmajor.build;
import cmajor.symbols;
import cmdoclib;
import util;

void InitApplication()
{
    cmdoclib::SetConstraintSymbolBinder();
    util::Init();
}

void DoneApplication()
{
    util::Done();
}

void PrintHelp()
{
    // todo
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc);
        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 0;
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
                        case 'v':
                        {
                            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                            break;
                        }
                        case 'h':
                        {
                            PrintHelp();
                            return 0;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        for (const std::string& file : files)
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                std::cout << "> " << file << "\n";
            }
            cmdoclib::Input input = cmdoclib::ReadInput(file);
            cmdoclib::SetInputPtr(&input);
            cmdoclib::GenerateStyleFile();
            cmdoclib::GenerateCodeStyleFile();
            rootModules.clear();
            std::string solutionName;
            std::vector<std::string> moduleNames;
            cmajor::build::BuildSolution(input.solutionFilePath, rootModules, solutionName, moduleNames);
            cmdoclib::WriteGlobals(moduleNames);
            cmdoclib::WriteModuleXml(input.targetDirPath, moduleNames);
            cmdoclib::ReadExternalModuleDocuments(&input);
            cmdoclib::BuildDocs(solutionName, moduleNames);
            std::cout << input.errors << " errors" << "\n";
            cmdoclib::SetInputPtr(nullptr);
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::cerr << ex.What() << std::endl;
        return 1;
    }

    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    DoneApplication();
    return 0;
}
