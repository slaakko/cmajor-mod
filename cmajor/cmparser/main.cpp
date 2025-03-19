// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import cmajor.build;
import soul.lexer;
import cmajor.ast;
import util;

std::string Version()
{
    return "5.1.0";
}

void PrintHelp()
{
    std::cout << "Cmajor parser version " << Version() << "\n";
    std::cout << "usage: cmparser [options] { FILE.cmp | FILE.sln | FILE.cm }" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit." << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose." << "\n";
    std::cout << "--single-threaded | -s" << "\n";
    std::cout << "  Parse single-threaded." << "\n";
    std::cout << "--ast | -a" << "\n";
    std::cout << "  Write FILE.ast for each FILE.cm." << "\n";
    std::cout << "--print | -p" << "\n";
    std::cout << "  Print source code from FILE.ast." << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        cmajor::build::Flags flags = cmajor::build::Flags::none;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    flags = flags | cmajor::build::Flags::verbose;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else if (arg == "--single-threaded")
                {
                    flags = flags | cmajor::build::Flags::singleThreadedParse;
                }
                else if (arg == "--ast")
                {
                    flags = flags | cmajor::build::Flags::ast;
                }
                else if (arg == "--print")
                {
                    flags = flags | cmajor::build::Flags::print;
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
                            flags = flags | cmajor::build::Flags::verbose;
                            break;
                        }
                        case 's':
                        {
                            flags = flags | cmajor::build::Flags::singleThreadedParse;
                            break;
                        }
                        case 'a':
                        {
                            flags = flags | cmajor::build::Flags::ast;
                            break;
                        }
                        case 'p':
                        {
                            flags = flags | cmajor::build::Flags::print;
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
        soul::lexer::FileMap fileMap;
        for (const auto& file : files)
        {
            if (file.ends_with(".cm"))
            {
                if ((flags & cmajor::build::Flags::verbose) != cmajor::build::Flags::none)
                {
                    std::cout << ">>> " << file << "\n";
                }
                int fileIndex = fileMap.MapFile(file);
                std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::build::ParseSourceFile(fileIndex, fileMap, flags, nullptr);
            }
            else if (file.ends_with(".cmp"))
            {
                std::unique_ptr<cmajor::ast::Project> project = cmajor::build::ParseProject(file, "debug", cmajor::ast::BackEnd::llvm, 0, fileMap, flags);
            }
            else if (file.ends_with(".cms"))
            {
                std::unique_ptr<cmajor::ast::Solution> solution = cmajor::build::ParseSolution(file, "debug", cmajor::ast::BackEnd::llvm, 0, flags);
            }
            else
            {
                throw std::runtime_error("unknown file extension '" + util::Path::GetExtension(file) + "'");
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
