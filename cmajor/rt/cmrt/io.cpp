// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/io.hpp>
#include <cmrt/debug.hpp>
#include <cmrt/error.hpp>
#include <cmrt/strings.hpp>
#include <util/text_util.hpp>
#include <util/unicode.hpp>
#include <util/system.hpp>
#include <cstring>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <direct.h>
#include <errno.h>
#include <string.h>
#include <filesystem>

namespace cmajor::cpp::rt {

const int defaultStdBufferSize = 2048;

enum class FileTableFlags : int
{
    none = 0, stdInInUtf16Mode = 1 << 0, stdOutInUtf16Mode = 1 << 1, stdErrInUtf16Mode = 1 << 2
};

constexpr FileTableFlags operator|(FileTableFlags left, FileTableFlags right)
{
    return FileTableFlags(int(left) | int(right));
}

constexpr FileTableFlags operator&(FileTableFlags left, FileTableFlags right)
{
    return FileTableFlags(int(left) & int(right));
}

constexpr FileTableFlags operator~(FileTableFlags flags)
{
    return FileTableFlags(~int(flags));
}

void FlushStdOutAndStdErr();

class File
{
public:
    File();
    virtual ~File();
    virtual bool CanDispose() const { return true; }
    virtual bool Close(int32_t& errorId) = 0;
    virtual int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) = 0;
    virtual bool WriteByte(uint8_t x, int32_t& errorId) = 0;
    virtual int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) = 0;
    virtual int32_t ReadByte(int32_t& errorId) = 0;
    virtual bool Eof() const = 0;
    virtual bool GetError(int32_t& errorId) const = 0;
    virtual bool Seek(int64_t pos, Origin origin, int32_t& errorId) = 0;
    virtual int64_t Tell(int32_t& errorId) = 0;
    virtual bool Flush(int32_t& errorId) = 0;
};

File::File()
{
}

File::~File()
{
}

class StdInputFile : public File
{
public:
    StdInputFile();
    bool CanDispose() const override { return false; }
    bool Close(int32_t& errorId) override;
    int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) override;
    bool WriteByte(uint8_t x, int32_t& errorId) override;
    int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) override;
    int32_t ReadByte(int32_t& errorId) override;
    bool Eof() const override;
    bool GetError(int32_t& errorId) const override;
    bool Seek(int64_t pos, Origin origin, int32_t& errorId) override;
    int64_t Tell(int32_t& errorId) override;
    bool Flush(int32_t& errorId) override;
};

StdInputFile::StdInputFile()
{
}

bool StdInputFile::Close(int32_t& errorId)
{
    errorId = 0;
    return true;
}

int64_t StdInputFile::Write(const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = AllocateError("cannot write to STDIN");
    return -1;
}

bool StdInputFile::WriteByte(uint8_t x, int32_t& errorId)
{
    errorId = AllocateError("cannot write to STDIN");
    return false;
}

int64_t StdInputFile::Read(uint8_t* buffer, int64_t count, int32_t& errorId)
{
    try
    {
        FlushStdOutAndStdErr();
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            return cmajor::cpp::rt::ReadBytesFromCmdbSession(buffer, count);
        }
        errorId = 0;
        int64_t result = std::fread(buffer, 1, count, stdin);
        if (ferror(stdin))
        {
            errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(std::strerror(errno)));
            return -1;
        }
        return result;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

int32_t StdInputFile::ReadByte(int32_t& errorId)
{
    errorId = 0;
    try
    {
        FlushStdOutAndStdErr();
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            uint8_t buffer = '\0';
            if (cmajor::cpp::rt::ReadBytesFromCmdbSession(&buffer, 1) == 1)
            {
                return buffer;
            }
            else
            {
                return -1;
            }
        }
        int32_t result = std::fgetc(stdin);
        if (result == EOF)
        {
            return -1;
        }
        return result;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

bool StdInputFile::Eof() const
{
    return std::feof(stdin);
}

bool StdInputFile::GetError(int32_t& errorId) const
{
    errorId = 0;
    if (std::ferror(stdin))
    {
        errorId = AllocateError("STDIN: " + util::PlatformStringToUtf8(std::strerror(errno)));
        return true;
    }
    return false;
}

bool StdInputFile::Seek(int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = AllocateError("cannot seek STDIN");
    return false;
}

int64_t StdInputFile::Tell(int32_t& errorId)
{
    errorId = AllocateError("cannot tell STDIN");
    return -1;
}

bool StdInputFile::Flush(int32_t& errorId)
{
    errorId = 0;
    return true;
}

class StdOutputFile : public File
{
public:
    StdOutputFile(FILE* file_, int handle_, const std::string& name_);
    bool CanDispose() const override { return false; }
    bool Close(int32_t& errorId) override;
    int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) override;
    bool WriteByte(uint8_t x, int32_t& errorId) override;
    int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) override;
    int32_t ReadByte(int32_t& errorId) override;
    bool Eof() const override;
    bool GetError(int32_t& errorId) const override;
    bool Seek(int64_t pos, Origin origin, int32_t& errorId) override;
    int64_t Tell(int32_t& errorId) override;
    bool Flush(int32_t& errorId) override;
private:
    FILE* file;
    int handle;
    std::string name;
};

StdOutputFile::StdOutputFile(FILE* file_, int handle_, const std::string& name_) : file(file_), handle(handle_), name(name_)
{
}

bool StdOutputFile::Close(int32_t& errorId)
{
    errorId = 0;
    return true;
}

int64_t StdOutputFile::Write(const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    try
    {
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            cmajor::cpp::rt::WriteBytesToCmdbSession(handle, buffer, count);
            return count;
        }
        errorId = 0;
        int64_t result = std::fwrite(buffer, 1, count, file);
        if (result != count)
        {
            errorId = AllocateError("could not write to " + name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
            return -1;
        }
        return result;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not write to " + name + ": " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

bool StdOutputFile::WriteByte(uint8_t x, int32_t& errorId)
{
    try
    {
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            uint8_t buffer = x;
            cmajor::cpp::rt::WriteBytesToCmdbSession(handle, &buffer, 1);
            return 1;
        }
        errorId = 0;
        int result = std::fputc(x, file);
        if (result == EOF)
        {
            errorId = AllocateError("could not write to '" + name + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
            return false;
        }
        return true;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not write to ': " + name + "': " + util::PlatformStringToUtf8(ex.what()));
        return false;
    }
}

int64_t StdOutputFile::Read(uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = AllocateError("cannot read from " + name);
    return -1;
}

int32_t StdOutputFile::ReadByte(int32_t& errorId)
{
    errorId = AllocateError("cannot read from " + name);
    return -1;
}

bool StdOutputFile::Eof() const
{
    return std::feof(file);
}

bool StdOutputFile::GetError(int32_t& errorId) const
{
    errorId = 0;
    if (std::ferror(file))
    {
        errorId = AllocateError(name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
        return true;
    }
    return false;
}

bool StdOutputFile::Seek(int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = AllocateError("cannot seek " + name);
    return false;
}

int64_t StdOutputFile::Tell(int32_t& errorId)
{
    errorId = AllocateError("cannot tell " + name);
    return -1;
}

bool StdOutputFile::Flush(int32_t& errorId)
{
    errorId = 0;
    int result = fflush(file);
    if (result != 0)
    {
        errorId = AllocateError("could not flush " + name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

class StdUnicodeInputFile : public File
{
public:
    StdUnicodeInputFile();
    bool CanDispose() const override { return false; }
    bool Close(int32_t& errorId) override;
    int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) override;
    bool WriteByte(uint8_t x, int32_t& errorId) override;
    int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) override;
    int32_t ReadByte(int32_t& errorId) override;
    bool Eof() const override;
    bool GetError(int32_t& errorId) const override;
    bool Seek(int64_t pos, Origin origin, int32_t& errorId) override;
    int64_t Tell(int32_t& errorId) override;
    bool Flush(int32_t& errorId) override;
private:
    int bufferSize;
    void AllocateBuffer();
    std::string stdInBuf;
    std::unique_ptr<char16_t[]> utf16buffer;
};

StdUnicodeInputFile::StdUnicodeInputFile() : bufferSize(defaultStdBufferSize)
{
}

void StdUnicodeInputFile::AllocateBuffer()
{
    if (!utf16buffer)
    {
        utf16buffer.reset(new char16_t[bufferSize]);
    }
}

bool StdUnicodeInputFile::Close(int32_t& errorId)
{
    errorId = 0;
    return true;
}

int64_t StdUnicodeInputFile::Write(const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = AllocateError("cannot write to STDIN");
    return -1;
}

bool StdUnicodeInputFile::WriteByte(uint8_t x, int32_t& errorId)
{
    errorId = AllocateError("cannot write to STDIN");
    return false;
}

int64_t StdUnicodeInputFile::Read(uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = 0;
    try
    {
        FlushStdOutAndStdErr();
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            return cmajor::cpp::rt::ReadBytesFromCmdbSession(buffer, count);
        }
        AllocateBuffer();
        int64_t result = 0;
        if (stdInBuf.empty())
        {
            int64_t utf16result = std::fread(utf16buffer.get(), sizeof(char16_t), bufferSize, stdin);
            std::u16string utf16Str;
            for (int i = 0; i < utf16result; ++i)
            {
                utf16Str.append(1, utf16buffer[i]);
            }
            stdInBuf = util::ToUtf8(utf16Str);
        }
        if (!stdInBuf.empty())
        {
            result = 0;
            uint8_t* p = buffer;
            while (result < count && !stdInBuf.empty())
            {
                uint8_t x = static_cast<uint8_t>(stdInBuf[0]);
                *p++ = x;
                stdInBuf.erase(stdInBuf.begin());
                ++result;
            }
        }
        if (result < count)
        {
            if (ferror(stdin))
            {
                errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(std::strerror(errno)));
                return -1;
            }
        }
        return result;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

int32_t StdUnicodeInputFile::ReadByte(int32_t& errorId)
{
    errorId = 0;
    try
    {
        FlushStdOutAndStdErr();
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            uint8_t buffer = '\0';
            if (cmajor::cpp::rt::ReadBytesFromCmdbSession(&buffer, 1) == 1)
            {
                return buffer;
            }
            else
            {
                return -1;
            }
        }
        if (stdInBuf.empty())
        {
            uint8_t buffer = '\0';
            int errorStringHandle = -1;
            int result = std::fgetwc(stdin);
            if (result == WEOF)
            {
                return -1;
            }
            else
            {
                std::u16string utf16Str(1, result);
                stdInBuf = util::ToUtf8(utf16Str);
            }
        }
        if (!stdInBuf.empty())
        {
            uint8_t x = static_cast<uint8_t>(stdInBuf[0]);
            stdInBuf.erase(stdInBuf.begin());
            return x;
        }
        return -1;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not read from STDIN: " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

bool StdUnicodeInputFile::Eof() const
{
    return std::feof(stdin);
}

bool StdUnicodeInputFile::GetError(int32_t& errorId) const
{
    errorId = 0;
    if (std::ferror(stdin))
    {
        errorId = AllocateError("STDIN: " + util::PlatformStringToUtf8(std::strerror(errno)));
        return true;
    }
    return false;
}

bool StdUnicodeInputFile::Seek(int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = AllocateError("cannot seek STDIN");
    return false;
}

int64_t StdUnicodeInputFile::Tell(int32_t& errorId)
{
    errorId = AllocateError("cannot tell STDIN");
    return -1;
}

bool StdUnicodeInputFile::Flush(int32_t& errorId)
{
    errorId = 0;
    return true;
}

class StdUnicodeOutputFile : public File
{
public:
    StdUnicodeOutputFile(FILE* file_, int handle_, const std::string& name_);
    bool CanDispose() const override { return false; }
    bool Close(int32_t& errorId) override;
    int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) override;
    bool WriteByte(uint8_t x, int32_t& errorId) override;
    int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) override;
    int32_t ReadByte(int32_t& errorId) override;
    bool Eof() const override;
    bool GetError(int32_t& errorId) const override;
    bool Seek(int64_t pos, Origin origin, int32_t& errorId) override;
    int64_t Tell(int32_t& errorId) override;
    bool Flush(int32_t& errorId) override;
private:
    FILE* file;
    int handle;
    std::string name;
    util::Utf8ToUtf32Engine engine;
};

StdUnicodeOutputFile::StdUnicodeOutputFile(FILE* file_, int handle_, const std::string& name_) : file(file_), handle(handle_), name(name_)
{
}

bool StdUnicodeOutputFile::Close(int32_t& errorId)
{
    errorId = 0;
    return true;
}

int64_t StdUnicodeOutputFile::Write(const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    try
    {
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            cmajor::cpp::rt::WriteBytesToCmdbSession(handle, buffer, count);
            return count;
        }
        errorId = 0;
        std::u32string utf32Chars;
        const uint8_t* e = buffer + count;
        for (const uint8_t* p = buffer; p != e; ++p)
        {
            engine.Put(*p);
            if (engine.ResultReady())
            {
                utf32Chars.append(1, engine.Result());
            }
        }
        std::u16string utf16Chars(util::ToUtf16(utf32Chars));
        if (!utf16Chars.empty())
        {
            int64_t utf16result = std::fwrite(utf16Chars.c_str(), sizeof(char16_t), utf16Chars.length(), file);
            if (utf16result != utf16Chars.length())
            {
                errorId = AllocateError("could not write to " + name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
                return -1;
            }
        }
        int64_t result = count;
        return result;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not write to " + name + ": " + util::PlatformStringToUtf8(ex.what()));
        return -1;
    }
}

bool StdUnicodeOutputFile::WriteByte(uint8_t x, int32_t& errorId)
{
    try
    {
        if (cmajor::cpp::rt::IsCmdbSessionOpen())
        {
            uint8_t buffer = x;
            cmajor::cpp::rt::WriteBytesToCmdbSession(handle, &buffer, 1);
            return 1;
        }
        return Write(&x, 1, errorId) == 1;
    }
    catch (const std::exception& ex)
    {
        errorId = AllocateError("could not write to " + name + ": " + util::PlatformStringToUtf8(ex.what()));
        return false;
    }
}

int64_t StdUnicodeOutputFile::Read(uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = AllocateError("cannot read from " + name);
    return -1;
}

int32_t StdUnicodeOutputFile::ReadByte(int32_t& errorId)
{
    errorId = AllocateError("cannot read from " + name);
    return -1;
}

bool StdUnicodeOutputFile::Eof() const
{
    return std::feof(file);
}

bool StdUnicodeOutputFile::GetError(int32_t& errorId) const
{
    errorId = 0;
    if (std::ferror(file))
    {
        errorId = AllocateError(name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
        return true;
    }
    return false;
}

bool StdUnicodeOutputFile::Seek(int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = AllocateError("cannot seek " + name);
    return false;
}

int64_t StdUnicodeOutputFile::Tell(int32_t& errorId)
{
    errorId = AllocateError("cannot tell " + name);
    return -1;
}

bool StdUnicodeOutputFile::Flush(int32_t& errorId)
{
    errorId = 0;
    int result = fflush(file);
    if (result != 0)
    {
        errorId = AllocateError("could not flush " + name + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

class FileTable
{
public:
    static FileTable& Instance();
    void Init();
    void Done();
    bool GetFlag(FileTableFlags flag) const { return (flags & flag) != FileTableFlags::none; }
    void SetFlag(FileTableFlags flag) { flags = flags | flag; }
    File* StdInput() const { return stdInput.get(); }
    File* StdOutput() const { return stdOutput.get(); }
    File* StdError() const { return stdError.get(); }
    void FlushStdOutAndStdErr();
private:
    FileTable();
    FileTableFlags flags;
    std::unique_ptr<File> stdInput;
    std::unique_ptr<File> stdOutput;
    std::unique_ptr<File> stdError;
};

void FileTable::Init()
{
}

void FileTable::Done()
{
    FlushStdOutAndStdErr();
    if (GetFlag(FileTableFlags::stdInInUtf16Mode))
    {
        _setmode(0, _O_TEXT);
    }
    if (GetFlag(FileTableFlags::stdOutInUtf16Mode))
    {
        _setmode(1, _O_TEXT);
    }
    if (GetFlag(FileTableFlags::stdErrInUtf16Mode))
    {
        _setmode(2, _O_TEXT);
    }
    stdInput.reset();
    stdOutput.reset();
    stdError.reset();
}

FileTable& FileTable::Instance()
{
    static FileTable instance;
    return instance;
}

FileTable::FileTable() : flags(FileTableFlags::none)
{
    FlushStdOutAndStdErr();
    if (_isatty(0))
    {
        _setmode(0, _O_U16TEXT);
        SetFlag(FileTableFlags::stdInInUtf16Mode);
        stdInput.reset(new StdUnicodeInputFile());
    }
    else
    {
        stdInput.reset(new StdInputFile());
    }
    if (_isatty(1))
    {
        _setmode(1, _O_U16TEXT);
        SetFlag(FileTableFlags::stdOutInUtf16Mode);
        stdOutput.reset(new StdUnicodeOutputFile(stdout, 1, "STDOUT"));
    }
    else
    {
        stdOutput.reset(new StdOutputFile(stdout, 1, "STDOUT"));
    }
    if (_isatty(2))
    {
        _setmode(2, _O_U16TEXT);
        SetFlag(FileTableFlags::stdErrInUtf16Mode);
        stdError.reset(new StdUnicodeOutputFile(stderr, 2, "STDERR"));
    }
    else
    {
        stdError.reset(new StdOutputFile(stderr, 2, "STDERR"));
    }
}

void FileTable::FlushStdOutAndStdErr()
{
    std::fflush(stdout);
    std::fflush(stderr);
}

void FlushStdOutAndStdErr()
{
    FileTable::Instance().FlushStdOutAndStdErr();
}

class RegularFile : public File
{
public:
    RegularFile(FILE* file_, const std::string& filePath_);
    ~RegularFile();
    bool Close(int32_t& errorId) override;
    int64_t Write(const uint8_t* buffer, int64_t count, int32_t& errorId) override;
    bool WriteByte(uint8_t x, int32_t& errorId) override;
    int64_t Read(uint8_t* buffer, int64_t count, int32_t& errorId) override;
    int32_t ReadByte(int32_t& errorId) override;
    bool Eof() const override;
    bool GetError(int32_t& errorId) const override;
    bool Seek(int64_t pos, Origin origin, int32_t& errorId) override;
    int64_t Tell(int32_t& errorId) override;
    bool Flush(int32_t& errorId) override;
private:
    std::string filePath;
    FILE* file;
};

RegularFile::RegularFile(FILE* file_, const std::string& filePath_) : file(file_), filePath(filePath_)
{
}

RegularFile::~RegularFile()
{
    if (file)
    {
        int32_t errorId = 0;
        Close(errorId);
    }
}

bool RegularFile::Close(int32_t& errorId)
{
    errorId = 0;
    int result = std::fclose(file);
    file = nullptr;
    if (result != 0)
    {
        errorId = AllocateError("could not close file '" + std::string(filePath) + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

int64_t RegularFile::Write(const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = 0;
    int64_t result = std::fwrite(buffer, 1, count, file);
    if (result != count)
    {
        errorId = AllocateError("could not write to file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return -1;
    }
    return result;
}

bool RegularFile::WriteByte(uint8_t x, int32_t& errorId)
{
    errorId = 0;
    int result = std::fputc(x, file);
    if (result == EOF)
    {
        errorId = AllocateError("could not write to file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

int64_t RegularFile::Read(uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = 0;
    int64_t result = std::fread(buffer, 1, count, file);
    if (ferror(file))
    {
        errorId = AllocateError("could not read from file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return -1;
    }
    return result;
}

int32_t RegularFile::ReadByte(int32_t& errorId)
{
    errorId = 0;
    int result = std::fgetc(file);
    if (result == EOF)
    {
        return -1;
    }
    return result;
}

bool RegularFile::Eof() const
{
    return std::feof(file);
}

bool RegularFile::GetError(int32_t& errorId) const
{
    errorId = 0;
    if (std::ferror(file))
    {
        errorId = AllocateError(filePath + ": " + util::PlatformStringToUtf8(std::strerror(errno)));
        return true;
    }
    return false;
}

bool RegularFile::Seek(int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = 0;
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
    int result = fseek(file, pos, seekOrigin);
    if (result != 0)
    {
        errorId = AllocateError("could not seek file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

int64_t RegularFile::Tell(int32_t& errorId)
{
    errorId = 0;
    int64_t result = ftell(file);
    if (result == -1)
    {
        errorId = AllocateError("could not tell file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return -1;
    }
    return result;
}

bool RegularFile::Flush(int32_t& errorId)
{
    errorId = 0;
    int result = fflush(file);
    if (result != 0)
    {
        errorId = AllocateError("could not flush file '" + filePath + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return false;
    }
    return true;
}

void* OpenFile(const char* filePath, OpenMode openMode, int32_t& errorId)
{
    errorId = 0;
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
            errorId = AllocateError("could not open file '" + std::string(filePath) + "': open mode not supported");
            return nullptr;
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
    std::string nativeFilePath = util::Utf8StringToPlatformString(filePath);
    FILE* file = std::fopen(nativeFilePath.c_str(), mode.c_str());
    if (!file)
    {
        errorId = AllocateError("could not open file '" + std::string(filePath) + "': " + util::PlatformStringToUtf8(std::strerror(errno)));
        return nullptr;
    }
    return new RegularFile(file, filePath);
}

void* OpenStdFile(int handle, int32_t& errorId)
{
    errorId = 0;
    switch (handle)
    {
    case 0: return FileTable::Instance().StdInput();
    case 1: return FileTable::Instance().StdOutput();
    case 2: return FileTable::Instance().StdError();
    default:
    {
        errorId = AllocateError("invalid standard file handle " + std::to_string(handle));
        break;
    }
    }
    return nullptr;
}

bool CloseFile(void* fileHandle, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not close file: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Close(errorId);
}

bool DisposeFile(void* fileHandle, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not dispose file: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    if (file->CanDispose())
    {
        delete file;
    }
    return true;
}

int64_t WriteFile(void* fileHandle, const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not write to file: invalid file handle");
        return -1;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Write(buffer, count, errorId);
}

bool WriteByte(void* fileHandle, int8_t x, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not write to file: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->WriteByte(x, errorId);
}

int64_t ReadFile(void* fileHandle, uint8_t* buffer, int64_t bufferSize, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not read from file: invalid file handle");
        return -1;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Read(buffer, bufferSize, errorId);
}

int32_t ReadByte(void* fileHandle, int32_t& errorId)
{
    if (!fileHandle)
    {
        return -1;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->ReadByte(errorId);
}

bool Eof(void* fileHandle)
{
    if (!fileHandle) return true;
    File* file = static_cast<File*>(fileHandle);
    return file->Eof();
}

bool GetFileError(void* fileHandle, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could get file error: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->GetError(errorId);
}

bool SeekFile(void* fileHandle, int64_t pos, Origin origin, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not seek file: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Seek(pos, origin, errorId);
}

int64_t TellFile(void* fileHandle, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not tell file: invalid file handle");
        return -1;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Tell(errorId);
}

bool FlushFile(void* fileHandle, int32_t& errorId)
{
    errorId = 0;
    if (!fileHandle)
    {
        errorId = AllocateError("could not flush file: invalid file handle");
        return false;
    }
    File* file = static_cast<File*>(fileHandle);
    return file->Flush(errorId);
}

void InitIO()
{
    FileTable::Instance().Init();
}

void DoneIO()
{
    FileTable::Instance().Done();
}

} // cmajor::cpp::rt

void* RtmOpenFile(const char* filePath, cmajor::cpp::rt::OpenMode openMode, int32_t& errorId)
{
    return cmajor::cpp::rt::OpenFile(filePath, openMode, errorId);
}

void* RtmOpenStdFile(int handle, int32_t& errorId)
{
    return cmajor::cpp::rt::OpenStdFile(handle, errorId);
}

void RtmPrintToFile(void* fileHandle, const char* s)
{
    int32_t errorId = 0;
    std::string str(s);
    int64_t bufferSize = str.length() + 1;
    std::unique_ptr<uint8_t> buf(static_cast<uint8_t*>(std::malloc(bufferSize)));
    std::strcpy((char*)buf.get(), str.c_str());
    RtmWrite(fileHandle, buf.get(), bufferSize, errorId);
}

bool RtmClose(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::CloseFile(fileHandle, errorId);
}

bool RtmDisposeFile(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::DisposeFile(fileHandle, errorId);
}

int64_t RtmWrite(void* fileHandle, const uint8_t* buffer, int64_t count, int32_t& errorId)
{
    return cmajor::cpp::rt::WriteFile(fileHandle, buffer, count, errorId);
}

bool RtmWriteByte(void* fileHandle, uint8_t x, int32_t& errorId)
{
    return cmajor::cpp::rt::WriteByte(fileHandle, x, errorId);
}

int64_t RtmRead(void* fileHandle, uint8_t* buffer, int64_t bufferSize, int32_t& errorId)
{
    return cmajor::cpp::rt::ReadFile(fileHandle, buffer, bufferSize, errorId);
}

int32_t RtmReadByte(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::ReadByte(fileHandle, errorId);
}

bool RtmEof(void* fileHandle)
{
    return cmajor::cpp::rt::Eof(fileHandle);
}

bool RtmGetFileError(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::GetFileError(fileHandle, errorId);
}

bool RtmSeek(void* fileHandle, int64_t pos, cmajor::cpp::rt::Origin origin, int32_t& errorId)
{
    return cmajor::cpp::rt::SeekFile(fileHandle, pos, origin, errorId);
}

int64_t RtmTell(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::TellFile(fileHandle, errorId);
}

bool RtmFlush(void* fileHandle, int32_t& errorId)
{
    return cmajor::cpp::rt::FlushFile(fileHandle, errorId);
}

bool RtmFileExists(const char* filePath, int32_t& errorId)
{
    std::error_code ec;
    bool exists = std::filesystem::exists(filePath, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("error checking if file '" + std::string(filePath) + "' exists: " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    return exists;
}

bool RtmLastWriteTimeLess(const char* filePath1, const char* filePath2, int& errorId)
{
    errorId = 0;
    std::filesystem::path f1 = filePath1;
    std::filesystem::path f2 = filePath2;
    std::error_code ec;
    std::filesystem::file_time_type t1 = std::filesystem::last_write_time(f1, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not get last write time of file '" + std::string(filePath1) + "': " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    std::filesystem::file_time_type t2 = std::filesystem::last_write_time(f2, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not get last write time of file '" + std::string(filePath2) + "': " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    if (t1 < t2)
    {
        return true;
    }
    return false;
}

int64_t RtmGetFileSize(const char* filePath, int32_t& errorId)
{
    errorId = 0;
    std::error_code ec;
    int64_t fileSize = std::filesystem::file_size(filePath, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not get size of file '" + std::string(filePath) + "': " + util::PlatformStringToUtf8(ec.message()));
        return -1;
    }
    return fileSize;
}

bool RtmRemoveFile(const char* filePath, int32_t& errorId)
{
    errorId = 0;
    std::error_code ec;
    std::filesystem::remove(filePath, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not remove file '" + std::string(filePath) + "': " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    return true;
}

bool RtmCopyFile(const char* sourceFilePath, const char* targetFilePath, int32_t& errorId)
{
    errorId = 0;
    std::error_code ec;
    std::filesystem::copy(sourceFilePath, targetFilePath, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not copy file '" + std::string(sourceFilePath) + "': " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    return true;
}

bool RtmMoveFile(const char* sourceFilePath, const char* targetFilePath, int32_t& errorId)
{
    errorId = 0;
    std::error_code ec;
    std::filesystem::rename(sourceFilePath, targetFilePath, ec);
    if (ec)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not move file '" + std::string(sourceFilePath) + "': " + util::PlatformStringToUtf8(ec.message()));
        return false;
    }
    return true;
}

bool RtmIsConsoleHandle(int handle)
{
    return _isatty(handle);
}

int RtmGetCurrentWorkingDirectoryHandle(int32_t& errorId)
{
    errorId = 0;
    std::unique_ptr<char[]> buffer(new char[8192]);
    if (getcwd(buffer.get(), 8192))
    {
        return RtmAllocateString(buffer.get());
    }
    else
    {
        std::string errorMessage = std::string("could not get current working directory: ") + util::PlatformStringToUtf8(std::strerror(errno));
        errorId = RtmAllocateError(errorMessage.c_str());;
        return -1;
    }
}

const char* RtmGetCurrentWorkingDirectory(int handle)
{
    return RtmGetString(handle);

}
void RtmFreeCurrentWorkingDirectoryHandle(int handle)
{
    RtmFreeString(handle);
}

int RtmBeginGetPathToExecutable()
{
    std::string pathToExecutable = util::GetPathToExecutable();
    return RtmAllocateString(pathToExecutable.c_str());
}

const char* RtmGetPathToExecutable(int pathHandle)
{
    return RtmGetString(pathHandle);
}

void RtmEndGetPathToExecutable(int pathHandle)
{
    RtmFreeString(pathHandle);
}
