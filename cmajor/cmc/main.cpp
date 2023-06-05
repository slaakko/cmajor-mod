// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import std.filesystem;
import cmajor.build;
import cmajor.symbols;
import util;

std::string Version()
{
    return util::CmajorVersionStr();
}

void PrintHelp()
{
    std::cout << "Cmajor compiler version " << Version() << "\n";
    std::cout << "Usage: cmc [options] { PROJECT.cmp | SOLUTION.cms } " << "\n";
    std::cout << "Options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--single-threaded | -s" << "\n";
    std::cout << "  Single-threaded compile." << "\n";
    std::cout << "--rebuild | -r" << "\n";
    std::cout << "  Rebuild." << "\n";
    std::cout << "--config=CONFIG | -c=CONFIG" << "\n";
    std::cout << "  Compile using CONFIG configuration. CONFIG=('debug'|'release'), default is 'debug'" << "\n";
    std::cout << "--opt-level=LEVEL| -O=LEVEL" << "\n";
    std::cout << "  Set optimization level to LEVEL (0-3). Defaults: LEVEL=0 for 'debug' configuration and LEVEL=2 for 'release' configuration." << "\n";
    std::cout << "--emit-llvm | -l" << "\n";
    std::cout << "  Emit intermediate LLVM code to FILE.ll files\n" << "\n";
}

int main(int argc, const char** argv)
{
    std::unique_ptr<cmajor::symbols::Module> rootModule;
    std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
    try
    {
        std::vector<std::string> files;
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
                else if (arg == "--single-threaded")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                }
                else if (arg == "--rebuild")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
                }
                else if (arg == "--emit-llvm")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm);
                }
                else if (arg == "--link-with-debug-runtime")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
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
                        else if (components[0] == "--opt-level")
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
                    }
                    else
                    {
                        throw std::runtime_error("invalid argument '" + arg + "'");
                    }
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                if (options.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(options, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "c")
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
                        else if (components[0] == "O")
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
                    }
                    else
                    {
                        throw std::runtime_error("invalid argument '" + arg + "'");
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
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
                                break;
                            }
                            case 's':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                                break;
                            }
                            case 'r':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
                                break;
                            }
                            case 'l':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm);
                                break;
                            }
                            case 'd':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
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
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        cmajor::symbols::SetUseModuleCache(true);
        std::set<std::string> builtProjects;
        for (const auto& file : files)
        {
            if (file.ends_with(".cmp"))
            {
                cmajor::build::BuildProject(file, rootModule, builtProjects);
            }
            else if (file.ends_with(".cms"))
            {
                cmajor::build::BuildSolution(file, rootModules);
            }
            else
            {
                throw std::runtime_error("unknown file extension '" + std::filesystem::path(file).extension().generic_string() + "': not .cmp or .cms");
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::cerr << ex.What() << std::endl;
        return 1;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "unknown exception" << std::endl;
    }
    return 0;
}