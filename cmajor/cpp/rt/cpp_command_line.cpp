// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/cpp_command_line.hpp>
#include <util/unicode.hpp>
#include <util/text_util.hpp>
#ifdef _WIN32
#include <Windows.h>
#undef min
#undef max
#endif
#include <string>
#include <memory>
#include <vector>

namespace cmajor::rt {

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
