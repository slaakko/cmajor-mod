// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

module cmajor.systemx.executor.console;

import cmajor.systemx.executor.color;
import cmajor.systemx.kernel;
import util;

namespace cmajor::systemx::executor {

const int defaultStdBufferSize = 2048;

Console::Console() : cmajor::systemx::kernel::File("CONSOLE"), stdInInUtf16Mode(false), stdOutInUtf16Mode(false), stdErrInUtf16Mode(false), bufferSize(defaultStdBufferSize)
{
}

void Console::SetToUtf16Mode()
{
    if (_isatty(0))
    {
        _setmode(0, _O_U16TEXT);
        stdInInUtf16Mode = true;
    }
    if (_isatty(1))
    {
        _setmode(1, _O_U16TEXT);
        stdOutInUtf16Mode = true;
    }
    if (_isatty(2))
    {
        _setmode(2, _O_U16TEXT);
        stdErrInUtf16Mode = true;
    }
}

void Console::SetToTextMode()
{
    _setmode(0, _O_TEXT);
    _setmode(1, _O_TEXT);
    _setmode(2, _O_TEXT);
}

void Console::AllocateBuffer()
{
    if (!utf16buffer)
    {
        utf16buffer.reset(new char16_t[bufferSize]);
    }
}

int Console::Columns() const
{
    return cmajor::systemx::kernel::OsGetConsoleNumberOfColumns();
}

int Console::Rows() const
{
    return cmajor::systemx::kernel::OsGetConsoleNumberOfRows();
}

void Console::Close(cmajor::systemx::kernel::Process* process)
{
}

std::vector<uint8_t> Console::Read(int64_t count, cmajor::systemx::machine::Process* process)
{
    std::vector<uint8_t> buffer;
    int64_t result = 0;
    if (stdInInUtf16Mode)
    {
        AllocateBuffer();
        if (stdInBuf.empty())
        {
            int errorStringHandle = -1;
            int result = std::fgetwc(stdin);
            if (result == WEOF)
            {
                return buffer;
            }
            else
            {
                std::u16string utf16Str(1, result);
                stdInBuf = util::ToUtf8(utf16Str);
            }
        }
    }
    else
    {
        if (stdInBuf.empty())
        {
            std::getline(std::cin, stdInBuf);
        }
    }
    if (!stdInBuf.empty())
    {
        result = 0;
        while (result < count && !stdInBuf.empty())
        {
            uint8_t x = static_cast<uint8_t>(stdInBuf[0]);
            buffer.push_back(x);
            stdInBuf.erase(stdInBuf.begin());
            ++result;
        }
    }
    return buffer;
}

int64_t Console::Write(const std::vector<uint8_t>& buffer, cmajor::systemx::machine::Process* process)
{
    std::u32string utf32Chars;
    for (uint8_t x : buffer)
    {
        utf8Engine.Put(x);
        if (utf8Engine.ResultReady())
        {
            char32_t c = utf8Engine.Result();
            if (c < static_cast<char32_t>(0x100000u))
            {
                utf32Chars.push_back(c);
            }
            else if (c < 0x10FFFD)
            {
                uint8_t fgColor = static_cast<uint32_t>(c) & 0xFF;
                uint8_t bgColor = (static_cast<uint32_t>(c) >> 8) & 0xFF;
                utf32Chars.append(SetColors(static_cast<ConsoleColor>(fgColor), static_cast<ConsoleColor>(bgColor)));
            }
            else if (c == 0x10FFFD)
            {
                utf32Chars.append(ResetColors());
            }
        }
    }
    if (stdOutInUtf16Mode)
    {
        std::u16string utf16Chars(util::ToUtf16(utf32Chars));
        if (!utf16Chars.empty())
        {
            int64_t utf16result = std::fwrite(utf16Chars.c_str(), sizeof(char16_t), utf16Chars.length(), stdout);
        }
    }
    else
    {
        std::string utf8Chars = util::ToUtf8(utf32Chars);
        std::cout << utf8Chars;
    }
    return buffer.size();
}

} // namespace cmajor::systemx::executor
