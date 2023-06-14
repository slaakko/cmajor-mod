// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import std.filesystem;
import util;
import soul.rex;

std::string version()
{
    return "5.0.0";
}

void PrintHelp()
{
    std::cout << "cmajor file installation utility version " << version() << std::endl;
    std::cout << std::endl;
    std::cout << "usage: cminst [options] (SOURCEFILE DESTFILE | SOURCEFILES... DIR)" << std::endl;
    std::cout << std::endl;
    std::cout << "copies SOURCEFILE to DESTFILE or SOURCEFILES to DIR for each file if:" << std::endl;
    std::cout << "- destination file does not exist or" << std::endl;
    std::cout << "- destination file is older than source file or" << std::endl;
    std::cout << "- forced" << std::endl;
    std::cout << std::endl;
    std::cout << "wild cards ok for SOURCEFILES" << std::endl;
    std::cout << std::endl;
    std::cout << "options:" << std::endl;
    std::cout << std::endl;
    std::cout << "--verbose | -v:" << std::endl;
    std::cout << "  be verbose" << std::endl;
    std::cout << std::endl;
    std::cout << "--help | -h:" << std::endl;
    std::cout << "  print help and exit" << std::endl;
    std::cout << std::endl;
    std::cout << "--force | -f:" << std::endl;
    std::cout << "  force copy although source file is older than destination file" << std::endl;
    std::cout << std::endl;
}

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
            util::FileStream sourceFile(source, util::OpenMode::read | util::OpenMode::binary);
            util::BufferedStream bufferedSource(sourceFile);
            util::BinaryStreamReader reader(bufferedSource);
            util::FileStream destFile(dest, util::OpenMode::write | util::OpenMode::binary);
            util::BufferedStream bufferedDest(destFile);
            util::BinaryStreamWriter writer(bufferedDest);
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
            throw std::runtime_error("could not set write time of file '" + dest + "': " + ec.message());
        }
        if (verbose)
        {
            std::cout << source << " -> " << dest << std::endl;
        }
    }
}

int main(int argc, const char** argv)
{
    try
    {
        bool verbose = false;
        bool force = false;
        std::vector<std::string> paths;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--force")
                {
                    force = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
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
                    case 'h':
                    {
                        PrintHelp();
                        return 1;
                    }
                    case 'f':
                    {
                        force = true;
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                    }
                    }
                }
            }
            else
            {
                std::string path = util::GetFullPath(arg);
                if (std::filesystem::exists(path))
                {
                    paths.push_back(path);
                }
                else
                {
                    std::string dir = util::Path::GetDirectoryName(path);
                    if (std::filesystem::exists(dir))
                    {
                        std::string fileMask = util::Path::GetFileName(path);
                        std::filesystem::directory_iterator it(dir);
                        if (it != std::filesystem::directory_iterator())
                        {
                            while (it != std::filesystem::directory_iterator())
                            {
                                std::filesystem::directory_entry entry(*it);
                                if (std::filesystem::is_regular_file(entry.path()))
                                {
                                    std::string fileName = util::Path::GetFileName(entry.path().generic_string());
                                    if (soul::rex::FilePatternMatch(fileName, fileMask))
                                    {
                                        std::string path = util::Path::Combine(dir, fileName);
                                        paths.push_back(path);
                                    }
                                    else
                                    {
                                        paths.push_back(path);
                                    }
                                }
                                ++it;
                            }
                        }
                        else
                        {
                            paths.push_back(dir);
                        }
                    }
                    else
                    {
                        if (verbose)
                        {
                            std::cout << "source directory '" + dir + "' does not exist" << std::endl;
                        }
                    }
                }
            }
        }
        if (paths.size() < 2)
        {
            PrintHelp();
            return 1;
        }
        if (paths.size() == 2 && std::filesystem::is_regular_file(paths.front()) && std::filesystem::is_regular_file(paths.back()) || !std::filesystem::exists(paths.back()))
        {
            std::string source = util::GetFullPath(paths.front());
            std::string dest = util::GetFullPath(paths.back());
            CopyFile(source, dest, force, verbose);
        }
        else if (std::filesystem::is_directory(paths.back()))
        {
            std::string destDir = paths.back();
            for (int i = 0; i < paths.size() - 1; ++i)
            {
                std::string source = util::GetFullPath(paths[i]);
                std::string dest = util::GetFullPath(util::Path::Combine(destDir, util::Path::GetFileName(source)));
                CopyFile(source, dest, force, verbose);
            }
        }
        else
        {
            return 1;
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
