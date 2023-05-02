// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import cmajor.build;
import soul.lexer;
import cmajor.ast;
import util;

std::string Version()
{
    return "5.0.0";
}

void PrintHelp()
{
    std::cout << "Cmajor parser version " << Version() << "\n";
    std::cout << "usage: cmparser [options] { FILE.cmp | FILE.sln | FILE.cm }" << "\n";
    std::cout << "options:" << "\n";
    std::cout << "--help | -h" << "\n";
    std::cout << "  Print help and exit" << "\n";
    std::cout << "--verbose | -v" << "\n";
    std::cout << "  Be verbose" << "\n";
}

int main(int argc, const char** argv)
{
    try
    {
        std::vector<std::string> files;
        bool verbose = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
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
                if (verbose)
                {
                    std::cout << ">>> " << file << "\n";
                }
                std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::build::ParseSourceFile(file, fileMap);
            }
            else if (file.ends_with(".cmp"))
            {
                if (verbose)
                {
                    std::cout << ">> " << file << "\n";
                }
                std::unique_ptr<cmajor::ast::Project> project = cmajor::build::ParseProjectFile(file, "debug", cmajor::ast::BackEnd::llvm, "llvm");
            }
            else if (file.ends_with(".cms"))
            {
                if (verbose)
                {
                    std::cout << "> " << file << "\n";
                }
                std::unique_ptr<cmajor::ast::Solution> solution = cmajor::build::ParseSolutionFile(file);
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
