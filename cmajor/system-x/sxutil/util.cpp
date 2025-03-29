// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxutil.util;

import std.filesystem;
import util;

namespace cmajor::systemx {

std::vector<std::string> ParseArgs(const std::string& commandLine)
{
    std::vector<std::string> args;
    std::string arg;
    int state = 0;
    for (char c : commandLine)
    {
        switch (state)
        {
            case 0:
            {
                if (c == ' ' || c == '\t')
                {
                    state = 1;
                }
                else if (c == '\\')
                {
                    state = 2;
                }
                else if (c == '"')
                {
                    state = 3;
                }
                else
                {
                    arg.append(1, c);
                }
                break;
            }
            case 1:
            {
                if (c != ' ' && c != '\t')
                {
                    args.push_back(arg);
                    arg.clear();
                    arg.append(1, c);
                    state = 0;
                }
                break;
            }
            case 2:
            {
                if (c != '"')
                {
                    arg.append(1, '\\');
                }
                arg.append(1, c);
                state = 0;
                break;
            }
            case 3:
            {
                if (c == '"')
                {
                    args.push_back(arg);
                    arg.clear();
                    state = 0;
                }
                else if (c == '\\')
                {
                    state = 4;
                }
                else
                {
                    arg.append(1, c);
                }
                break;
            }
            case 4:
            {
                if (c != '"')
                {
                    arg.append(1, '\\');
                }
                arg.append(1, c);
                state = 3;
                break;
            }
        }
    }
    if (!arg.empty())
    {
        args.push_back(arg);
    }
    return args;
}

std::string SearchBin(const std::string& fileName)
{
    if (std::filesystem::exists(fileName))
    {
        return fileName;
    }
    std::string executableFileName = fileName;
    if (!util::Path::HasExtension(fileName))
    {
        executableFileName.append(".x");
    }
    std::string fullPath = util::GetFullPath(executableFileName);
    if (std::filesystem::exists(fullPath))
    {
        return fullPath;
    }
    std::string binDirectory = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "system-x"), "bin"));
    fullPath = util::GetFullPath(util::Path::Combine(binDirectory, executableFileName));
    if (std::filesystem::exists(fullPath))
    {
        return fullPath;
    }
    else
    {
        throw std::runtime_error("executable '" + fileName + "' not found");
    }
}

} // namespace cmajor::systemx
