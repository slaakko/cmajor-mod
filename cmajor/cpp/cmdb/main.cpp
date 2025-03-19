#ifdef _WIN32
#include <Windows.h>
#undef min
#undef max
#endif

import std.core;
import cmajor.debug;
import cmajor.msg;
import cmajor.portmap;
import soul.xml.serialization;
import soul.xml.xpath;
import util;

void InitApplication()
{
    util::Init();
}

const char* version = "5.1.0";

void PrintHelp()
{
    std::cout << "Usage: cmdb [options] EXECUTABLE[.exe] [ ARGS... ]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
    std::cout << "--debug | -d" << std::endl;
    std::cout << "  Debug the debugger." << std::endl;
    std::cout << "--dontBreakOnThrow | -t" << std::endl;
    std::cout << "  Do not break on throw instructions." << std::endl;
    std::cout << "--server | -e" << std::endl;
    std::cout << "  Start debugger in server mode. Default is console mode." << std::endl;
    std::cout << "--client | -c" << std::endl;
    std::cout << "  Used in combination with --server. Starts test client along with server." << std::endl;
    std::cout << "--port=PORT_NUMBER | -p=PORT_NUMBER" << std::endl;
    std::cout << "  Set debug server port number. Default port is 54326." << std::endl;
    std::cout << "--idle | -i" << std::endl;
    std::cout << "  Send idle messages." << std::endl;
    std::cout << "--log | -l" << std::endl;
    std::cout << "  Write log to %CMAJOR_ROOT%/log/cmdb.log (by default C:\\cmajor\\log\\cmdb.log)." << std::endl;
    std::cout << "--sessionPort=PORT_NUMBER | -s=PORT_NUMBER" << std::endl;
    std::cout << "  Set the port number of the CMDB session that cmdb and the program being debugged will use for exchanging console I/O messages. Default port is 54322." << std::endl;
    std::cout << "--portMapServicePort=PORT_NUMBER | -m=PORT_NUMBER" << std::endl;
    std::cout << "  Set port map service port number to PORT_NUMBER." << std::endl;
    std::cout << "  Optional. When set revises main port number and session port number using port map service every minute." << std::endl;
    std::cout << "--killPort=PORT_NUMBER | -k=PORT_NUMBER" << std::endl;
    std::cout << "  Optional. Set kill channel port number to PORT_NUMBER." << std::endl;
}

int main(int argc, const char** argv)
{
    bool server = false;
    bool client = false;
    int portMapServicePort = -1;
    try
    {
        InitApplication();
        bool verbose = false;
        std::string executable;
        std::vector<std::string> args;
        bool executableSeen = false;
        bool breakOnThrow = true;
        bool log = false;
        bool idle = false;
        int port = 54326;
        int sessionPort = 54322;
        int killPort = -1;
        bool wait = false;
        std::vector<int> portNumbers;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (!executableSeen)
            {
                if (arg.starts_with("--"))
                {
                    if (arg == "--verbose")
                    {
                        verbose = true;
                    }
                    else if (arg == "--wait")
                    {
                        wait = true;
                    }
                    else if (arg == "--debug")
                    {
                        cmajor::debug::SetDebugFlag();
                    }
                    else if (arg == "--help")
                    {
                        PrintHelp();
                        return 1;
                    }
                    else if (arg == "--dontBreakOnThrow")
                    {
                        breakOnThrow = false;
                    }
                    else if (arg == "--server")
                    {
                        server = true;
                    }
                    else if (arg == "--client")
                    {
                        client = true;
                    }
                    else if (arg == "--log")
                    {
                        log = true;
                    }
                    else if (arg == "--idle")
                    {
                        idle = true;
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
                            else if (components[0] == "--sessionPort")
                            {
                                sessionPort = std::stoi(components[1]);
                                cmajor::debug::SetCmdbSessionPort(sessionPort);
                            }
                            else if (components[0] == "--portMapServicePort")
                            {
                                portMapServicePort = std::stoi(components[1]);
                            }
                            else if (components[0] == "--killPort")
                            {
                                killPort = std::stoi(components[1]);
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
                            if (components[0] == "p")
                            {
                                port = std::stoi(components[1]);
                            }
                            else if (components[0] == "s")
                            {
                                sessionPort = std::stoi(components[1]);
                                cmajor::debug::SetCmdbSessionPort(sessionPort);
                            }
                            else if (components[0] == "m")
                            {
                                portMapServicePort = std::stoi(components[1]);
                            }
                            else if (components[0] == "k")
                            {
                                killPort = std::stoi(components[1]);
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
                            case 'd':
                            {
                                cmajor::debug::SetDebugFlag();
                                break;
                            }
                            case 'h':
                            {
                                PrintHelp();
                                return 1;
                            }
                            case 't':
                            {
                                breakOnThrow = false;
                                break;
                            }
                            case 'e':
                            {
                                server = true;
                                break;
                            }
                            case 'c':
                            {
                                client = true;
                                break;
                            }
                            case 'l':
                            {
                                log = true;
                                break;
                            }
                            case 'i':
                            {
                                idle = true;
                                break;
                            }
                            case 'w':
                            {
                                wait = true;
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
                else
                {
                    executable = arg;
                    executableSeen = true;
                }
            }
            else
            {
                args.push_back(arg);
            }
        }
        if (executable.empty())
        {
            throw std::runtime_error("no executable specified");
        }
        if (wait)
        {
            std::this_thread::sleep_for(std::chrono::seconds{ 45 });
        }
        if (server)
        {
            if (killPort != -1)
            {
                cmajor::debug::StartKillChannel(killPort);
            }
#ifdef _WIN32
            if (portMapServicePort != -1)
            {
                std::vector<int> portNumbers;
                portNumbers.push_back(port);
                portNumbers.push_back(sessionPort);
                cmajor::portmap::StartPortMapClient(portMapServicePort, portNumbers, "cmdb", util::GetPid());
            }
#endif
            if (client)
            {
                cmajor::debug::StartDebuggerServer(executable, args, verbose, breakOnThrow, version, port, log, idle);
                cmajor::debug::RunClient(port);
                cmajor::debug::StopDebuggerServer();
            }
            else
            {
                cmajor::debug::RunDebuggerServer(executable, args, verbose, breakOnThrow, version, port, log, idle);
            }
#ifdef _WIN32
            if (portMapServicePort != -1)
            {
                cmajor::portmap::StopPortMapClient();
                portMapServicePort = -1;
            }
#endif
            if (killPort != -1)
            {
                cmajor::debug::StopKillChannel();
            }
        }
        else
        {
            cmajor::debug::RunDebuggerInteractive(executable, args, verbose, breakOnThrow, version);
        }
    }
    catch (const std::exception& ex)
    {
#ifdef _WIN32
        if (portMapServicePort != -1)
        {
            cmajor::portmap::StopPortMapClient();
        }
#endif
        if (server)
        {
            std::cout << "debug-server-error" << std::endl;
            std::cout << ex.what() << std::endl;
        }
        else
        {
            std::cerr << ex.what() << std::endl;
        }
        return 1;
    }
    return 0;
}

