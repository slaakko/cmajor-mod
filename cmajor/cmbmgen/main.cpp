// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import util;
import cmajor.binary.message.cpp.generator;
import cmajor.binary.message.cmajor.generator;
import cmajor.binary.message.protocol;

void PrintHelp()
{
    std::cout << "Cmajor Binary Message Generator version " << cmajor::bmp::Version() << std::endl;
    std::cout << "usage: cmbmgen [options] { FILE.schema }" << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--language=LANGUAGE | -l=LANGUAGE" << std::endl;
    std::cout << "  Generate message file code in LANGUAGE." << std::endl;
    std::cout << "  LANGUAGE can be 'C++' or 'Cmajor'." << std::endl;
    std::cout << "  Default language is 'C++'." << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        std::vector<std::string> fileNames;
        bool verbose = false;
        std::string language = "C++";
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg.find("=") != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--language")
                        {
                            language = util::ToUpperNarrow(components[1]);
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
            }
            else if (arg.starts_with("-"))
            {
                if (arg.find("=") != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-l")
                        {
                            language = util::ToUpperNarrow(components[1]);
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
                            return 1;
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
                fileNames.push_back(util::GetFullPath(arg));
            }
        }
        for (const auto& fileName : fileNames)
        {
            if (language == "C++")
            {
                cmajor::binary::message::cpp::GenerateBinaryMessageFiles(fileName, verbose);
            }
            else if (language == "CMAJOR")
            {
                cmajor::binary::message::cm::GenerateBinaryMessageFiles(fileName, verbose);
            }
            else
            {
                throw std::runtime_error("unknown language '" + language + "'");
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    util::Done();
    return 0;
}
