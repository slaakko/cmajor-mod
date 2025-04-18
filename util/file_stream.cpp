// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <stdio.h>
#include <errno.h>
#include <string.h>

module util.file.stream;

import std.core;
import std.filesystem;
import util.text.util;

namespace util {

FileStream::FileStream(int handle) : filePath(), file(nullptr), needToClose(false)
{
    switch (handle)
    {
        case 0:
        {
            file = stdin;
            filePath = "STDIN";
            break;
        }
        case 1:
        {
            file = stdout;
            filePath = "STDOUT";
            break;
        }
        case 2:
        {
            file = stderr;
            filePath = "STDERR";
            break;
        }
        default:
        {
            throw std::runtime_error("invalid handle value");
        }
    }
}

FileStream::FileStream(const std::string& filePath_, OpenMode openMode) : filePath(filePath_), file()
{
    std::string mode;
    if ((openMode & OpenMode::read) != OpenMode::none && (openMode & OpenMode::write) == OpenMode::none)
    {
        mode += "r";
    }
    if ((openMode & OpenMode::write) != OpenMode::none)
    {
        mode += "w";
        if ((openMode & OpenMode::read) != OpenMode::none)
        {
            mode += "+";
        }
    }
    if ((openMode & OpenMode::append) != OpenMode::none)
    {
        if ((openMode & (OpenMode::read | OpenMode::write)) != OpenMode::none)
        {
            throw std::runtime_error("could not open file '" + std::string(filePath) + "': open mode not supported");
        }
        else
        {
            mode += "a";
        }
    }
    if ((openMode & OpenMode::binary) != OpenMode::none)
    {
        mode += "b";
    }
    std::string nativeFilePath = Utf8StringToPlatformString(filePath);
    errno_t result = fopen_s(&file, nativeFilePath.c_str(), mode.c_str());
    if (result != 0)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), result);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not open file '" + std::string(filePath) + "': " + PlatformStringToUtf8(s));
    }
    needToClose = true;
}

FileStream::~FileStream()
{
    if (file && needToClose)
    {
        std::fclose(file);
    }
}

int FileStream::ReadByte()
{
    int result = std::fgetc(file);
    if (result == EOF)
    {
        if (std::feof(file))
        {
            return -1;
        }
        else
        {
            char buf[1024];
            errno_t res = strerror_s(buf, sizeof(buf), errno);
            std::string s;
            if (res == 0)
            {
                s = buf;
            }
            throw std::runtime_error("could not read from file '" + filePath + "': " + PlatformStringToUtf8(s));
        }
    }
    SetPosition(Position() + 1);
    return result;
}

int64_t FileStream::Read(uint8_t* buf, int64_t count)
{
    int64_t result = std::fread(buf, 1, count, file);
    if (std::ferror(file))
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not read from file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
    SetPosition(Position() + result);
    return result;
}

void FileStream::Write(uint8_t x)
{
    int result = std::fputc(x, file);
    if (result == EOF)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not write to file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
    SetPosition(Position() + 1);
}

void FileStream::Write(uint8_t* buf, int64_t count)
{
    int64_t result = std::fwrite(buf, 1, count, file);
    if (result != count)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not write to file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
    SetPosition(Position() + result);
}

void FileStream::Flush()
{
    int result = std::fflush(file);
    if (result != 0)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not flush file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
}

void FileStream::Seek(int64_t pos, Origin origin)
{
    int seekOrigin = SEEK_SET;
    switch (origin)
    {
        case Origin::seekCur:
        {
            seekOrigin = SEEK_CUR;
            break;
        }
        case Origin::seekEnd:
        {
            seekOrigin = SEEK_END;
            break;
        }
        case Origin::seekSet:
        {
            seekOrigin = SEEK_SET;
            break;
        }
    }
#ifdef _WIN32
    int result = _fseeki64(file, pos, seekOrigin);
#else
    int result = std::fseek(file, pos, seekOrigin);
#endif
    if (result != 0)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not seek file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
    switch (origin)
    {
        case Origin::seekCur:
        {
            SetPosition(Position() + pos);
            break;
        }
        case Origin::seekEnd:
        {
            SetPosition(Size() + pos);
            break;
        }
        case Origin::seekSet:
        {
            SetPosition(pos);
            break;
        }
    }
}

int64_t FileStream::Tell()
{
    int64_t result = std::ftell(file);
    if (result == -1)
    {
        char buf[1024];
        errno_t res = strerror_s(buf, sizeof(buf), errno);
        std::string s;
        if (res == 0)
        {
            s = buf;
        }
        throw std::runtime_error("could not tell file '" + filePath + "': " + PlatformStringToUtf8(s));
    }
    return result;
}

int64_t FileStream::Size() const
{
    return std::filesystem::file_size(filePath);
}

std::string ReadFile(const std::string& filePath)
{
    return ReadFile(filePath, false);
}

std::string ReadFile(const std::string& filePath, bool skipBOM)
{
    std::string s;
    FileStream fs(filePath, OpenMode::read | OpenMode::binary);
    int64_t n = fs.Size();
    for (int64_t i = 0; i < n; ++i)
    {
        int c = fs.ReadByte();
        if (c != -1)
        {
            s.append(1, c);
        }
        else
        {
            throw std::runtime_error("unexpected end of '" + filePath + "'");
        }
    }
    if (skipBOM && s.length() >= 3 && static_cast<uint8_t>(s[0]) == 0xEF && static_cast<uint8_t>(s[1]) == 0xBB && static_cast<uint8_t>(s[2]) == 0xBF)
    {
        return s.substr(3);
    }
    return s;
}

std::vector<uint8_t> ReadBinaryFile(const std::string& filePath)
{
    std::vector<uint8_t> content;
    FileStream fs(filePath, OpenMode::read | OpenMode::binary);
    int64_t n = fs.Size();
    for (int64_t i = 0; i < n; ++i)
    {
        int c = fs.ReadByte();
        if (c != -1)
        {
            content.push_back(static_cast<uint8_t>(c));
        }
        else
        {
            throw std::runtime_error("unexpected end of '" + filePath + "'");
        }
    }
    return content;
}

} // util
