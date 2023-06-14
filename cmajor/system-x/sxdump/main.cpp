// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.systemx.assembler;
import cmajor.systemx.object;
import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import soul.xml.xpath;
import util;
import std.core;

void InitApplication()
{
    util::Init();
    cmajor::systemx::kernel::Init(true);
}

void DoneApplication()
{
    cmajor::systemx::kernel::Done();
    util::Done();
}

void TrapAdder(cmajor::systemx::object::SymbolTable& symbolTable)
{
    cmajor::systemx::kernel::AddTrapsToSymbolTable(symbolTable);
}

void PrintHelp()
{
    std::cout << "System X Dump" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage: sxdump [options] { FILE.o | FILE.x }" << std::endl;
    std::cout << std::endl;
    std::cout << "Print the contents of FILE.o or FILE.x as text to FILE.o.txt or FILE.x.txt" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        cmajor::systemx::machine::Machine machine;
        bool verbose = false;
        std::vector<std::string> fileNames;
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
                    verbose = true;
                }
                else
                {
                    throw std::runtime_error("unknown argument '" + arg + "'");
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
                        throw std::runtime_error("unknown argument '-" + std::string(1, o) + "'");
                    }
                    }
                }
            }
            else
            {
                fileNames.push_back(util::GetFullPath(arg));
            }
        }
        cmajor::systemx::object::SetTrapAdderFunc(TrapAdder);
        for (const auto& fileName : fileNames)
        {
            if (verbose)
            {
                std::cout << "> " << fileName << std::endl;
            }
            std::string textFileName;
            cmajor::systemx::object::WriteBinaryFileAsText(fileName, machine, textFileName);
            if (verbose)
            {
                std::cout << "==> " << textFileName << std::endl;
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    DoneApplication();
    return 0;
}
