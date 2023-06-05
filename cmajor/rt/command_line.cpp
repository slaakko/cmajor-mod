// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <Windows.h>
#undef min
#undef max
#endif

module cmajor.rt.command.line;

import util;
import std.filesystem;
import soul.rex;
import cmajor.rt.mod;

namespace cmajor::rt {

bool ContainsWildCards(const std::string& s)
{
    for (char c : s)
    {
        switch (c)
        {
            case '*': case '?':
            {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::string> Expand(const std::string& arg, soul::rex::context::Context& context)
{
    std::vector<std::string> result;
    std::string path = util::GetFullPath(arg);
    std::string dir = util::Path::GetDirectoryName(path);
    std::string filePattern = util::Path::GetFileName(path);
    std::string lexerModuleFileName = util::GetPathToModule(GetRTModuleHandle());
    soul::rex::nfa::Nfa nfa = soul::rex::CompileFilePattern(lexerModuleFileName, context, filePattern);
    for (const auto& dirEntry : std::filesystem::directory_iterator(std::filesystem::path(dir), std::filesystem::directory_options::skip_permission_denied))
    {
        if (soul::rex::PatternMatch(dirEntry.path().filename().generic_string(), nfa))
        {
            result.push_back(dirEntry.path().generic_string());
        }
    }
    return result;
}

std::vector<std::string> ExpandArgs(const std::vector<std::string>& args)
{
    soul::rex::context::Context context;
    std::vector<std::string> result;
    for (const auto& arg : args)
    {
        if (ContainsWildCards(arg))
        {
            std::vector<std::string> files = Expand(arg, context);
            for (const auto& file : files)
            {
                result.push_back(file);
            }
        }
        else
        {
            result.push_back(arg);
        }
    }
    return result;
}

class CommandLine
{
public:    
    static CommandLine& Instance();
    int ArgC() const { return argc; }
    const char** ArgV() const { return argv; }
private:
    std::u32string commandLine;
    int argc;
    const char** argv;
    std::unique_ptr<char> mem;
    CommandLine();
    void ProcessCommandLine();
    void MakeArgv(const std::vector<std::string>& args);
};

CommandLine& CommandLine::Instance()
{
    static CommandLine instance;
    return instance;
}

CommandLine::CommandLine() : commandLine(), argc(0), argv()
{
    std::u16string cmdLine = reinterpret_cast<char16_t*>(GetCommandLineW());
    std::string commandLineUtf8 = util::ToUtf8(cmdLine);
    commandLine = util::ToUtf32(util::Trim(commandLineUtf8));
    ProcessCommandLine();
}

void CommandLine::MakeArgv(const std::vector<std::string>& args)
{
    int size = 0;
    for (const auto& arg : args)
    {
        size += 8 + arg.length() + 1;
    }
    size += 8;
    mem.reset(static_cast<char*>(malloc(size)));
    argv = reinterpret_cast<const char**>(mem.get());
    char* start = mem.get() + ((args.size() + 1) * 8);
    int i = 0;
    for (const auto& arg : args)
    {
        strcpy(start, arg.c_str());
        argv[i] = start;
        start += arg.length() + 1;
        ++i;
    }
    argv[i] = nullptr;
    argc = i;
}

void CommandLine::ProcessCommandLine()
{
    int state = 0;
    std::u32string arg;
    std::vector<std::string> args;
    for (char32_t c : commandLine)
    {
        switch (state)
        {
            case 0:
            {
                switch (c)
                {
                    case '"':
                    {
                        state = 1;
                        break;
                    }
                    case ' ':
                    {
                        state = 2;
                        break;
                    }
                    case '\\':
                    {
                        state = 3;
                        break;
                    }
                    default:
                    {
                        arg.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 1:
            {
                switch (c)
                {
                    case '"':
                    {
                        args.push_back(util::ToUtf8(arg));
                        arg.clear();
                        state = 0;
                        break;
                    }
                    case '\\':
                    {
                        state = 4;
                        break;
                    }
                    default:
                    {
                        arg.append(1, c);
                        break;
                    }
                }
                break;
            }
            case 2:
            {
                switch (c)
                {
                    case ' ':
                    {
                        break;
                    }
                    case '"':
                    {
                        if (!arg.empty())
                        {
                            args.push_back(util::ToUtf8(arg));
                            arg.clear();
                        }
                        state = 1;
                        break;
                    }
                    case '\\':
                    {
                        if (!arg.empty())
                        {
                            args.push_back(util::ToUtf8(arg));
                            arg.clear();
                        }
                        state = 3;
                        break;
                    }
                    default:
                    {
                        if (!arg.empty())
                        {
                            args.push_back(util::ToUtf8(arg));
                            arg.clear();
                        }
                        arg.append(1, c);
                        state = 0;
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch (c)
                {
                    case '"':
                    {
                        arg.append(1, c);
                        state = 0;
                        break;
                    }
                    default:
                    {
                        arg.append(1, '\\').append(1, c);
                        state = 0;
                        break;
                    }
                }
                break;
            }
            case 4:
            {
                switch (c)
                {
                    case '"':
                    {
                        arg.append(1, c);
                        state = 1;
                        break;
                    }
                    default:
                    {
                        arg.append(1, '\\').append(1, c);
                        state = 1;
                        break;
                    }
                }
                break;
            }
        }
    }
    if (!arg.empty())
    {
        args.push_back(util::ToUtf8(arg));
    }
    args = ExpandArgs(args);
    MakeArgv(args);
}

} // namespace cmajor.rt

int RtArgc()
{
    return cmajor::rt::CommandLine::Instance().ArgC();
}

const char** RtArgv()
{
    return cmajor::rt::CommandLine::Instance().ArgV();
}
