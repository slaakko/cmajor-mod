import cmajor.sbin.assembly;
import soul.lexer;
import util;
import std.filesystem;
import std.core;

void GetAsmFiles(const std::vector<std::string>& paths, bool all, bool recursive, std::vector<std::string>& asmFiles)
{
    for (const std::string& path : paths)
    {
        if (all)
        {
            std::filesystem::directory_iterator it{ path };
            while (it != std::filesystem::directory_iterator{})
            {
                std::filesystem::directory_entry entry = *it;
                if (entry.is_directory() && recursive)
                {
                    std::filesystem::path p = entry.path();
                    if (p.filename() != "." && p.filename() != "..")
                    {
                        std::vector<std::string> subDirPaths;
                        subDirPaths.push_back(p.generic_string());
                        GetAsmFiles(subDirPaths, all, recursive, asmFiles);
                    }
                }
                else if (entry.is_regular_file())
                {
                    std::filesystem::path p = entry.path();
                    if (p.extension() == ".asm")
                    {
                        asmFiles.push_back(p.generic_string());
                    }
                }
                ++it;
            }
        }
        else
        {
            asmFiles.push_back(path);
        }
    }
}

void ParseAsmFiles(const std::vector<std::string>& asmFiles, bool verbose)
{
    for (const auto& asmFile : asmFiles)
    {
        soul::lexer::FileMap fileMap;
        std::unique_ptr<cmajor::sbin::assembly::AsmFileNode> asmFileNode = cmajor::sbin::assembly::ParseAsmFile(asmFile, verbose, fileMap);
    }
}

int main(int argc, const char** argv)
{
    try
    {
        bool verbose = false;
        bool all = false;
        bool recursive = false;
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
                else if (arg == "--all")
                {
                    all = true;
                }
                else if (arg == "--recursive")
                {
                    recursive = true;
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char c : options)
                {
                    switch (c)
                    {
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        case 'a':
                        {
                            all = true;
                            break;
                        }
                        case 'r':
                        {
                            recursive = true;
                            break;
                        }
                    }
                }
            }
            else
            {
                paths.push_back(util::GetFullPath(arg));
            }
        }
        if (recursive)
        {
            all = true;
        }
        if (all && paths.empty())
        {
            paths.push_back(".");
        }
        std::vector<std::string> asmFiles;
        GetAsmFiles(paths, all, recursive, asmFiles);
        std::sort(asmFiles.begin(), asmFiles.end());
        ParseAsmFiles(asmFiles, verbose);
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
