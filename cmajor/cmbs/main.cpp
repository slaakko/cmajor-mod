// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmbs.keep.alive.server;
import cmbs.build.server;
import cmajor.symbols;
import cmbs.logging;
import util;
import std.core;

void InitApplication()
{
    util::Init();
}

std::string Version() 
{
    return "5.0.0";
}

void PrintHelp()
{
    std::cout << "Cmajor Build Server version " << Version() << std::endl;

}

std::mutex mtx;

struct KeepAliveServerRun
{
    KeepAliveServerRun(int keepAliveServerPort, std::condition_variable* exitVar, bool* exiting, bool logging)
    {
        cmbs::StartKeepAliveServer(keepAliveServerPort, exitVar, exiting, logging);
    }
    ~KeepAliveServerRun()
    {
        cmbs::StopKeepAliveServer();
    }
};

struct BuildServerRun
{
    BuildServerRun(int port, std::condition_variable* exitVar, bool* exiting, bool logging, bool progress)
    {
        cmbs::StartBuildServer(port, exitVar, exiting, logging, progress);
    }
    ~BuildServerRun()
    {
        cmbs::StopBuildServer();
    }
};

int main(int argc, const char** argv)
{
    std::condition_variable exitVar;
    bool exiting = false;
    try
    {
        cmajor::symbols::SetCompilerVersion(Version());
        int portMapServerPort = 54421;
        int port = 55001;
        int keepAliveServerPort = 55002;
        bool logging = false;
        bool wait = false;
        bool progress = false;
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
                else if (arg == "--log")
                {
                    logging = true;
                }
                else if (arg == "--wait")
                {
                    wait = true;
                }
                else if (arg == "--progress")
                {
                    progress = true;
                }
                else if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "--port")
                        {
                            port = std::stoi(components[1]);
                        }
                        else if (components[0] == "--keepAliveServerPort")
                        {
                            keepAliveServerPort = std::stoi(components[1]);
                        }
                        else if (components[0] == "--portMapServicePort")
                        {
                            portMapServerPort = std::stoi(components[1]);
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
                if (arg.find('=') != std::string::npos)
                {
                    std::vector<std::string> components = util::Split(arg, '=');
                    if (components.size() == 2)
                    {
                        if (components[0] == "-p")
                        {
                            port = std::stoi(components[1]);
                        }
                        else if (components[0] == "-k")
                        {
                            keepAliveServerPort = std::stoi(components[1]);
                        }
                        else if (components[0] == "-m")
                        {
                            portMapServerPort = std::stoi(components[1]);
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
                            case 'h':
                            {
                                PrintHelp();
                                return 1;
                            }
                            case 'l':
                            {
                                logging = true;
                                break;
                            }
                            case 'w':
                            {
                                wait = true;
                                break;
                            }
                            case 'g':
                            {
                                progress = true;
                                break;
                            }
                            default:
                            {
                                throw std::runtime_error("unknown option '" + arg + "'");
                            }
                        }
                    }
                }
            }
            else
            {
                throw std::runtime_error("unknown option");
            }
        }
        if (wait)
        {
            std::this_thread::sleep_for(std::chrono::seconds{ 45 });
        }
        KeepAliveServerRun keepAliveServerRun(keepAliveServerPort, &exitVar, &exiting, logging);
        BuildServerRun runBuildServer(port, &exitVar, &exiting, logging, progress);
        while (!cmbs::BuildServerStopRequested() && !cmbs::Timeout())
        {
            std::unique_lock<std::mutex> lock(mtx);
            if (exitVar.wait_for(lock, std::chrono::seconds{ 3 }, [exiting] { return exiting; }))
            {
                if (cmbs::BuildServerStopRequested())
                {
                    util::LogFileWriter writer(cmbs::LogFilePath());
                    writer.WriteCurrentDateTime();
                    writer << "Build server stop request received" << std::endl;
                }
                else if (cmbs::Timeout())
                {
                    util::LogFileWriter writer(cmbs::LogFilePath());
                    writer.WriteCurrentDateTime();
                    writer << "Build server timeout, exiting..." << std::endl;
                }
                break;
            }
        }
        util::LogFileWriter writer(cmbs::LogFilePath());
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer << "Build server stopped." << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "build-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        util::LogFileWriter writer(cmbs::LogFilePath());
        writer.WriteLine("================================================================================");
        writer.WriteCurrentDateTime();
        writer << "main got exception: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
