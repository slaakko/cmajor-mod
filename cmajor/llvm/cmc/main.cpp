// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import std.filesystem;
import cmajor.backend;
import cmajor.build;
import cmajor.symbols;
import util;

void InitApplication()
{
    util::Init();
}

void DoneApplication()
{
    util::Done();
}

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
    std::cout << "--quiet | -q" << "\n";
    std::cout << "  Print no messages.\n";
    std::cout << "--single-threaded | -s" << "\n";
    std::cout << "  Single-threaded compilation." << "\n";
    std::cout << "--rebuild | -r" << "\n";
    std::cout << "  Rebuild." << "\n";
    std::cout << "--clean | -e" << "\n";
    std::cout << "  Clean." << "\n";
    std::cout << "--disable-module-cache | -m\n";
    std::cout << "  Do not cache recently built modules.\n";
    std::cout << "--link-with-debug-runtime | -d\n";
    std::cout << "  Link with debug runtime." << "\n";
    std::cout << "--config=CONFIG | -c=CONFIG" << "\n";
    std::cout << "  Compile using CONFIG configuration. CONFIG=('debug'|'release'), default is 'debug'" << "\n";
    std::cout << "--opt-level=LEVEL| -O=LEVEL" << "\n";
    std::cout << "  Set optimization level to LEVEL (0-3). Defaults: LEVEL=0 for 'debug' configuration and LEVEL=2 for 'release' configuration." << "\n";
    std::cout << "--emit-llvm | -l" << "\n";
    std::cout << "  Emit intermediate LLVM code to FILE.ll files" << "\n";
    std::cout << "--gen-debug-info | -g" << "\n";
    std::cout << "  Generate debug info (automatically enabled for 'debug' configuration)." << "\n";
    std::cout << "--no-debug-info | -i" << "\n";
    std::cout << "  Do not generate debug info." << "\n";
    std::cout << "--print | -p" << "\n";
    std::cout << "  Print each LLVM module to stderr." << "\n";
}

int main(int argc, const char** argv)
{
    std::unique_ptr<cmajor::symbols::Module> rootModule;
    std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
    try
    {
        InitApplication();
        std::vector<std::string> files;
        bool genDebugInfo = false;
        bool noDebugInfo = false;
        bool useModuleCache = true;
        cmajor::symbols::SetCompilerVersion(Version());
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
                else if (arg == "--disable-module-cache")
                {
                    useModuleCache = false;
                }
                else if (arg == "--single-threaded")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
                }
                else if (arg == "--rebuild")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
                }
                else if (arg == "--clean")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
                }
                else if (arg == "--emit-llvm")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm);
                }
                else if (arg == "--link-with-debug-runtime")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
                }
                else if (arg == "--gen-debug-info")
                {
                    genDebugInfo = true;
                }
                else if (arg == "--no-debug-info")
                {
                    noDebugInfo = true;
                }
                else if (arg == "--print")
                {
                    cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::print);
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
                            case 'q':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
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
                            case 'e':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
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
                            case 'm':
                            {
                                useModuleCache = false;
                                break;
                            }
                            case 'g':
                            {
                                genDebugInfo = true;
                                break;
                            }
                            case 'i':
                            {
                                noDebugInfo = true;
                                break;
                            }
                            case 'p':
                            {
                                cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::print);
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
        cmajor::symbols::SetUseModuleCache(useModuleCache);
        cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::llvmBackEnd);
        cmajor::backend::BackEnd* backend = cmajor::backend::GetCurrentBackEnd();
        std::set<std::string> builtProjects;
        cmajor::symbols::SetUseModuleCache(useModuleCache);
        if ((!GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && !noDebugInfo) || genDebugInfo)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo);
        }
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
        return 1;
    }
    DoneApplication();
    return 0;
}