// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.file.util;

import std.filesystem;
import util.file.stream;
import util.buffered.stream;
import util.binary.stream.reader;
import util.binary.stream.writer;
import util.text.util;

namespace util {

void CopyFile(const std::string& source, const std::string& dest, bool force, bool verbose)
{
    if (!std::filesystem::exists(source))
    {
        if (verbose)
        {
            std::cout << "source file '" + source + "' does not exist" << std::endl;
        }
        return;
    }
    if (force || !std::filesystem::exists(dest) || std::filesystem::last_write_time(source) > std::filesystem::last_write_time(dest))
    {
        int64_t size = std::filesystem::file_size(source);
        {
            FileStream sourceFile(source, OpenMode::read | OpenMode::binary);
            BufferedStream bufferedSource(sourceFile);
            BinaryStreamReader reader(bufferedSource);
            FileStream destFile(dest, OpenMode::write | OpenMode::binary);
            BufferedStream bufferedDest(destFile);
            BinaryStreamWriter writer(bufferedDest);
            for (int64_t i = 0; i < size; ++i)
            {
                uint8_t x = reader.ReadByte();
                writer.Write(x);
            }
        }
        std::error_code ec;
        std::filesystem::last_write_time(dest, std::filesystem::last_write_time(source), ec);
        if (ec)
        {
            throw std::runtime_error("could not set write time of file '" + dest + "': " + PlatformStringToUtf8(ec.message()));
        }
        if (verbose)
        {
            std::cout << source << " -> " << dest << std::endl;
        }
    }
}

} // namespace util

