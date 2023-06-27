// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.debug;
import soul.xml.dom;
import util;
import std.core;

void InitApplication()
{
    util::Init();
}

const char* version = "5.0.0";

void PrintHelp()
{
    std::cout << "Cmajor debug information file conversion utility version " << version << std::endl;
    std::cout << "Convert FILE.cmdb to FILE.xml for inspection." << std::endl;
    std::cout << "Usage: cmdb2xml [options] FILE.cmdb" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print this help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
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
            else
            {
                if (arg.ends_with(".cmdb"))
                {
                    std::string cmdbFilePath = util::GetFullPath(arg);
                    if (verbose)
                    {
                        std::cout << "> " << cmdbFilePath << std::endl;
                    }
                    std::string xmlDebugInfoFilePath = util::Path::ChangeExtension(cmdbFilePath, ".xml");
                    std::unique_ptr<soul::xml::Document> xmlDoc = cmajor::debug::GetDebugInfoAsXml(cmdbFilePath);
                    cmajor::debug::WriteXmlDebugInfo(xmlDoc.get(), xmlDebugInfoFilePath);
                    if (verbose)
                    {
                        std::cout << "==> " << xmlDebugInfoFilePath << std::endl;
                    }
                }
                else
                {
                    throw std::runtime_error("unknown file extension (not .cmdb)");
                }
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
