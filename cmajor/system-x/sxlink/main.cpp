// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.systemx.object;
import cmajor.systemx.machine;
import util;
import std.core;

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
    return "5.0.0";
}

void PrintHelp()
{
    std::cout << "System X Linker version " << Version() << std::endl;
    std::cout << "Usage: sxlink [options] { FILE.o | FILE.a }" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
    std::cout << "--clsid CLASSID_FILE.clsid | -c CLASSID_FILE.clsid" << std::endl;
    std::cout << "  Read class identifiers from CLASSID_FILE.clsid" << std::endl;
    std::cout << "--executable FILE.x | -x FILE.x" << std::endl;
    std::cout << "  Set executable output file name to FILE.x." << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bool verbose = false;
        bool prevWasClsId = false;
        bool prevWasExecutable = false;
        std::string executableFilePath;
        std::string clsIdFileName;
        std::vector<std::string> binaryFileNames;
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
                else if (arg == "--clsid")
                {
                    prevWasClsId = true;
                }
                else if (arg == "--executable")
                {
                    prevWasExecutable = true;
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
                    case 'c':
                    {
                        prevWasClsId = true;
                        break;
                    }
                    case 'x':
                    {
                        prevWasExecutable = true;
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                    }
                    }
                }
            }
            else if (prevWasClsId)
            {
                prevWasClsId = false;
                clsIdFileName = util::GetFullPath(arg);
            }
            else if (prevWasExecutable)
            {
                prevWasExecutable = false;
                executableFilePath = util::GetFullPath(arg);
            }
            else
            {
                binaryFileNames.push_back(util::GetFullPath(arg));
            }
        }
        if (executableFilePath.empty())
        {
            throw std::runtime_error("executable file not set");
        }
        cmajor::systemx::object::Link(0, executableFilePath, binaryFileNames, clsIdFileName, verbose);
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    DoneApplication();
    return 0;
}
