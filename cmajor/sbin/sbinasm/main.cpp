// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.sbin.assembly;
import util;
import std.core;

void PrintHelp()
{
    // todo
}

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        bool verbose = false;
        std::vector<std::string> files;
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
                            verbose = true;
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
        for (const auto& file : files)
        {
            cmajor::sbin::assembly::Assembler assembler(file, -1, verbose, false);
            assembler.Assemble();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}
