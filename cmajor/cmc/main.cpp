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
}

int main(int argc, const char** argv)
{
    std::unique_ptr<cmajor::symbols::Module> rootModule;
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
                // todo
            }
            else
            {
                throw std::runtime_error("unknown file extension '" + std::filesystem::path(file).extension().generic_string() + "': not .cmp or .cms");
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}