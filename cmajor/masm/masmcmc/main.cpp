// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import util;
import cmajor.symbols;
import cmajor.backend;
import cmajor.build;
import cmajor.ir;
import std.core;
import std.filesystem;

void InitApplication()
{
    util::Init();
}

void DoneApplication()
{
    util::Done();
}

const char* version = "5.0.0";

void PrintHelp()
{
    std::cout << "Cmajor compiler version " << version << " with MASM backend for Windows x64" << std::endl;
    std::cout << "Usage: masmcmc [options] { project.cmp | solution.cms }" << std::endl;
    std::cout << "Compiles given Cmajor solutions and projects to Windows libraries or executables using Microsoft Visual Studio." << std::endl;
    std::cout << "Options:\n" <<
        "--help (-h)\n" <<
        "   print this help message\n" <<
        "--config=CONFIG (-c=CONFIG)\n" <<
        "   set configuration to CONFIG (debug | release)\n" <<
        "   default is debug\n" <<
        "--optimization-level=LEVEL (-O=LEVEL)\n" <<
        "   set optimization level to LEVEL=0-3\n" <<
        "   defaults: debug=0, release=2\n" <<
        "--disable-warnings=WARNING_LIST | -w=WARNING_LIST" << "\n" <<
        "  disable warning messages for specified warnings in WARNING_LIST that is a semicolon-separated list of warning numbers." << "\n" <<
        "--verbose (-v)\n" <<
        "   print verbose messages\n" <<
        "--quiet (-q)\n" <<
        "   print no messages\n" <<
        "--rebuild (-r)\n" <<
        "   build although sources not changed\n" <<
        "--clean (-e)\n" <<
        "   clean given solutions and projects\n" <<
        "--define SYMBOL (-D SYMBOL)\n" <<
        "   define a conditional compilation symbol SYMBOL.\n" <<
        "--link-with-debug-runtime (-d)\n" <<
        "   link with debug runtime 'cmrtmasmd.lib'\n" <<
        "--single-threaded (-s)\n" <<
        "   compile using a single thread\n" <<
        "--time (-t)\n" <<
        "   print duration of compilation\n" <<
        "--unit-test-file-path=FILEPATH (-f=FILEPATH)\n"
        "   set unit test file path to FILEPATH\n" <<
        "--unit-test-name=UNITTESTNAME (-n=UNITTESTNAME)\n"
        "   set unit test name to UNITTESTNAME\n" <<
        "--unit-test (-u)\n"
        "   compile unit test\n" <<
        std::endl;
}

int main(int argc, const char** argv)
{
    cmajor::symbols::SetBackEnd(cmajor::symbols::BackEnd::masm);
    cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::masmBackEnd);
    std::set<std::string> builtProjects;
    std::unique_ptr<cmajor::symbols::Module> rootModule;
    std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
    std::unique_ptr<cmajor::ir::EmittingContext> emittingContext;
    try
    {
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        InitApplication();
        std::string projectName;
        std::string projectDirectory;
        std::string target = "program";
        std::vector<std::string> files;
        std::vector<std::string> sourceFiles;
        std::vector<std::string> referenceFiles;
        if (argc < 2)
        {
            PrintHelp();
        }
        else
        {
            cmajor::symbols::SetCompilerVersion(version);
            bool prevWasDefine = false;
            bool noDebugInfo = false;
            bool useModuleCache = true;
            for (int i = 1; i < argc; ++i)
            {
                std::string arg = argv[i];
                if (arg.starts_with("--"))
                {
                    if (arg == "--help")
                    {
                        PrintHelp();
                        return 1;
                    }
                    else if (arg == "--verbose")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                    }
                    else if (arg == "--quiet")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
                    }
                    else if (arg == "--clean")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
                    }
                    else if (arg == "--rebuild")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
                    }
                    else if (arg == "--define")
                    {
                        prevWasDefine = true;
                    }
                    else if (arg == "--single-threaded")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                    }
                    else if (arg == "--time")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::time);
                    }
                    else if (arg == "--unit-test")
                    {
                        cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest);
                    }
                    else if (arg.find('=') != std::string::npos)
                    {
                        std::vector<std::string> components = util::Split(arg, '=');
                        if (components.size() == 2)
                        {
                            if (components[0] == "--config")
                            {
                                if (components[1] == "release")
                                {
                                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
                                }
                                else if (components[1] != "debug")
                                {
                                    throw std::runtime_error("unknown configuration '" + components[1] + "'");
                                }
                            }
                            else if (components[0] == "--optimization-level")
                            {
                                int optimizationLevel = std::stoi(components[1]);
                                if (optimizationLevel >= 0 && optimizationLevel <= 3)
                                {
                                    cmajor::symbols::SetOptimizationLevel(optimizationLevel);
                                }
                                else
                                {
                                    throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                                }
                            }
                            else if (components[0] == "--disable-warnings")
                            {
                                std::string warningList = components[1];
                                std::vector<std::string> warnings = util::Split(warningList, ';');
                                for (const auto& warning : warnings)
                                {
                                    int warningNumber = std::stoi(warning);
                                    cmajor::symbols::DisableWarning(warningNumber);
                                }
                            }
                            else if (components[0] == "--pass")
                            {
                                cmajor::symbols::SetPass(components[1]);
                            }
                            else if (components[0] == "--unit-test-file-path")
                            {
                                cmajor::symbols::SetUnitTestFilePath(components[1]);
                            }
                            else if (components[0] == "--unit-test-name")
                            {
                                cmajor::symbols::SetUnitTestName(components[1]);
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
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
                else if (arg.starts_with("-"))
                {
                    if (arg.find('=') != std::string::npos)
                    {
                        std::vector<std::string> components = util::Split(arg, '=');
                        if (components[0] == "-c")
                        {
                            if (components[1] == "release")
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
                            }
                            else if (components[1] != "debug")
                            {
                                throw std::runtime_error("unknown configuration '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "-O")
                        {
                            int optimizationLevel = std::stoi(components[1]);
                            if (optimizationLevel >= 0 && optimizationLevel <= 3)
                            {
                                cmajor::symbols::SetOptimizationLevel(optimizationLevel);
                            }
                            else
                            {
                                throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "-w")
                        {
                            std::string warningList = components[1];
                            std::vector<std::string> warnings = util::Split(warningList, ';');
                            for (const auto& warning : warnings)
                            {
                                int warningNumber = std::stoi(warning);
                                cmajor::symbols::DisableWarning(warningNumber);
                            }
                        }
                        else if (components[0] == "-p")
                        {
                            cmajor::symbols::SetPass(components[1]);
                        }
                        else if (components[0] == "-f")
                        {
                            cmajor::symbols::SetUnitTestFilePath(components[1]);
                        }
                        else if (components[0] == "-n")
                        {
                            cmajor::symbols::SetUnitTestName(components[1]);
                        }
                        else
                        {
                            throw std::runtime_error("unknown option '" + arg + "'");
                        }
                    }
                    else
                    {
                        std::string options = arg.substr(1);
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
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                                break;
                            }
                            case 'q':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
                                break;
                            }
                            case 'e':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
                                break;
                            }
                            case 'r':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
                                break;
                            }
                            case 'D':
                            {
                                prevWasDefine = true;
                                break;
                            }
                            case 'g':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo);
                                break;
                            }
                            case 's':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                                break;
                            }
                            case 't':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::time);
                                break;
                            }
                            default:
                            {
                                throw std::runtime_error("unknown option -" + std::string(1, o) + "'");
                            }
                            }
                        }
                    }
                }
                else if (prevWasDefine)
                {
                    prevWasDefine = false;
                    cmajor::symbols::DefineCommandLineConditionalSymbol(util::ToUtf32(arg));
                }
                else
                {
                    files.push_back(arg);
                }
            }
            if (files.empty())
            {
                PrintHelp();
                return 0;
            }
            if (GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                std::cout << "Cmajor compiler version " << version << " with MASM backend for Windows x64" << std::endl;
            }
            emittingContext = cmajor::backend::GetCurrentBackEnd()->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
            cmajor::symbols::SetUseModuleCache(useModuleCache);
            if (!GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && !noDebugInfo)
            {
                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo);
            }
            for (const std::string& file : files)
            {
                std::filesystem::path fp(file);
                bool prevUnitTest = false;
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
                {
                    prevUnitTest = cmajor::symbols::BeginUnitTest();
                }
                if (fp.extension() == ".cms")
                {
                    if (GetGlobalFlag(cmajor::symbols::GlobalFlags::msbuild))
                    {
                        throw std::runtime_error("solution file '" + fp.generic_string() + "'  cannot be given in --msbuild mode");
                    }
                    else if (!std::filesystem::exists(fp))
                    {
                        throw std::runtime_error("solution file '" + fp.generic_string() + "' not found");
                    }
                    else
                    {
                        cmajor::build::BuildSolution(util::GetFullPath(fp.generic_string()), rootModules);
                    }
                }
                else if (fp.extension() == ".cmp")
                {
                    if (GetGlobalFlag(cmajor::symbols::GlobalFlags::msbuild))
                    {
                        throw std::runtime_error("project file '" + fp.generic_string() + "'  cannot be given in --msbuild mode");
                    }
                    else if (!std::filesystem::exists(fp))
                    {
                        throw std::runtime_error("project file '" + fp.generic_string() + "' not found");
                    }
                    else
                    {
                        cmajor::build::BuildProject(util::GetFullPath(fp.generic_string()), rootModule, builtProjects);
                    }
                }
                else
                {
                    throw std::runtime_error("file '" + fp.generic_string() + "' has unknown extension (not .cmp or .cms)");
                }
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
                {
                    cmajor::symbols::EndUnitTest(prevUnitTest);
                }
            }
            if (GetGlobalFlag(cmajor::symbols::GlobalFlags::time))
            {
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                auto dur = end - start;
                long long totalSecs = std::chrono::duration_cast<std::chrono::seconds>(dur).count() + 1;
                int hours = static_cast<int>(totalSecs / 3600);
                int mins = static_cast<int>((totalSecs / 60) % 60);
                int secs = static_cast<int>(totalSecs % 60);
                std::cout <<
                    (hours > 0 ? std::to_string(hours) + " hour" + ((hours != 1) ? "s " : " ") : "") <<
                    (mins > 0 ? std::to_string(mins) + " minute" + ((mins != 1) ? "s " : " ") : "") <<
                    secs << " second" << ((secs != 1) ? "s" : "") << std::endl;
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::quiet))
        {
            std::cerr << ex.What() << std::endl;
        }
        return 1;
    }
    catch (const std::exception& ex)
    {
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::quiet))
        {
            std::cerr << ex.what() << std::endl;
        }
        return 1;
    }
    DoneApplication();
}
