// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.systemx.intermediate;
import cmajor.systemx.assembler;
import cmajor.systemx.machine;
import util;
import std.core;

void InitApplication()
{
    util::Init();
    cmajor::systemx::intermediate::Init();
}

void DoneApplication()
{
    cmajor::systemx::intermediate::Done();
    util::Done();
}

std::string Version()
{
    return "5.1.0";
}

void PrintHelp()
{
    std::cout << "System X Intermediate Compiler version " << Version() << std::endl;
    std::cout << "Usage: sxic [options] { FILE.i }" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose" << std::endl;
    std::cout << "--config=CONFIG | -c=CONFIG" << std::endl;
    std::cout << "  set configuration to CONFIG (debug | release)" << std::endl;
    std::cout << "  default is debug" << std::endl;
    std::cout << "--optimization-level=LEVEL | -O=LEVEL" << std::endl;
    std::cout << "  set optimization level to LEVEL=0-3" << std::endl;
    std::cout << "  defaults: debug=0, release=2" << std::endl;
    std::cout << "--pass=PASS | -p=PASS" << std::endl;
    std::cout << "  process intermediate code by running PASS\n" << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bool verbose = false;
        bool release = false;
        int optimizationLevel = -1;
        std::string pass;
        std::vector<std::string> fileNames;
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
                else if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--config")
                        {
                            if (components[1] == "release")
                            {
                                release = true;
                            }
                            else if (components[1] != "debug")
                            {
                                throw std::runtime_error("unknown configuration '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "--optimization-level")
                        {
                            optimizationLevel = std::stoi(components[1]);
                            if (optimizationLevel < 0 || optimizationLevel > 3)
                            {
                                throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "--pass")
                        {
                            pass = components[1];
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
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                if (options.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-c")
                        {
                            if (components[1] == "release")
                            {
                                release = true;
                            }
                            else if (components[1] != "debug")
                            {
                                throw std::runtime_error("unknown configuration '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "-O")
                        {
                            optimizationLevel = std::stoi(components[1]);
                            if (optimizationLevel < 0 || optimizationLevel > 3)
                            {
                                throw std::runtime_error("unknown optimization level '" + components[1] + "'");
                            }
                        }
                        else if (components[0] == "-p")
                        {
                            pass = components[1];
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
        if (optimizationLevel == -1)
        {
            if (release)
            {
                optimizationLevel = 2;
            }
            else
            {
                optimizationLevel = 0;
            }
        }
        for (const auto& fileName : fileNames)
        {
            if (verbose)
            {
                std::cout << "> " << fileName << std::endl;
            }
            cmajor::systemx::intermediate::Context intermediateContext;
            cmajor::systemx::intermediate::Parse(0, fileName, intermediateContext, verbose);
            cmajor::systemx::intermediate::Verify(intermediateContext);
            if (pass.empty())
            {
                pass = "opt-" + std::to_string(optimizationLevel);
            }
            cmajor::systemx::intermediate::PassManager::Instance().Run(-1, &intermediateContext, pass, verbose);
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
