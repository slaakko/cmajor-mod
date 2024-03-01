// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import soul.json.rpc;
import cmajor.language.server;
import cmajor.fault.tolerant.build;
import util;
import std.core;
import std.filesystem;

int main(int argc, const char** argv)
{
    try
    {
        util::Init();
        cmajor::fault::tolerant::build::ReadSystem();
        cmajor::language::server::Initialize();
        soul::json::rpc::Config::Instance().SetLogDir(util::GetFullPath(util::Path::Combine(util::CmajorRoot(), "log")));
        std::filesystem::create_directories(soul::json::rpc::Config::Instance().LogDir());
        bool verbose = false;
        int port = 57000;
        bool wait = false;
        bool stdio = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--port")
                        {
                            port = std::stoi(components[1]);
                        }
                        else if (components[0] == "--wait")
                        {
                            wait = components[1] == "true";
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
                    else if (arg == "--log")
                    {
                        soul::json::rpc::Config::Instance().SetFlag(soul::json::rpc::Flags::log);
                    }
                    else if (arg == "--stdio")
                    {
                        stdio = true;
                    }
                    else
                    {
                        throw std::runtime_error("unknown option '" + arg + "'");
                    }
                }
            }
            else if (arg.starts_with("-"))
            {
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-p")
                        {
                            port = std::stoi(components[1]);
                        }
                        else if (components[0] == "-w")
                        {
                            wait = components[1] == "true";
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
                            case 'l':
                            {
                                soul::json::rpc::Config::Instance().SetFlag(soul::json::rpc::Flags::log);
                                break;
                            }
                            case 's':
                            {
                                stdio = true;
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
        }
        if (verbose)
        {
            std::cout << "Running server on port " << port << "..." << "\n";
        }
        if (stdio)
        {
            soul::json::rpc::RunStdIOServer(wait);
        }
        else
        {
            soul::json::rpc::RunServer(port, wait);
        }
        if (verbose)
        {
            std::cout << "Server exiting." << "\n";
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    util::Done();
    return 0;
}
