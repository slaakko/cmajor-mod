// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.source_file;

import cmajor.systemx.sxcdb.debugger;
import cmajor.systemx.sxcdb.break_point;
import cmajor.systemx.sxutil;
import util;

namespace cmajor::systemx::sxcdb {

void AddLines(const std::string& fileContent, std::vector<std::string>& lines)
{
    lines.push_back(std::string());
    std::string line;
    int state = 0;
    for (char c : fileContent)
    {
        switch (state)
        {
            case 0:
            {
                if (c == '\r')
                {
                    state = 1;
                }
                else if (c == '\n')
                {
                    lines.push_back(line);
                    line.clear();
                }
                else
                {
                    line.append(1, c);
                }
                break;
            }
            case 1:
            {
                if (c == '\n')
                {
                    lines.push_back(line);
                    line.clear();
                    state = 0;
                }
                break;
            }
        }
    }
    if (!line.empty())
    {
        lines.push_back(line);
    }
}

SourceFile::SourceFile()
{
}

SourceFile::SourceFile(const std::string& filePath_) : filePath(filePath_)
{
    std::string content = util::ReadFile(filePath);
    AddLines(content, lines);
}

void SourceFile::Print(int64_t start, cmajor::systemx::kernel::Process* process, Debugger& debugger, int64_t file, bool printNext)
{
    int64_t end = std::min(start + debugger.PageSize(), LineCount() - 1);
    int64_t width = 0;
    for (int64_t lineNumber = start; lineNumber <= end; ++lineNumber)
    {
        width = std::max(width, static_cast<int64_t>(util::Log10(static_cast<int>(lineNumber))));
    }
    soul::ast::LineColLen currentLineColLen = debugger.CurrentLineColLen();
    for (int64_t lineNumber = start; lineNumber <= end; ++lineNumber)
    {
        std::string line;
        if (file == debugger.CurrentFile() && lineNumber == currentLineColLen.line)
        {
            line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
            line.append(util::Format(std::to_string(lineNumber), static_cast<int>(width), util::FormatJustify::right));
            line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
            line.append(1, ' ');
            std::string ln = GetLine(static_cast<int>(lineNumber));
            line.append(ln.substr(0, currentLineColLen.col - 1));
            line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
            line.append(ln.substr(currentLineColLen.col - 1, currentLineColLen.len));
            line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
            line.append(ln.substr(currentLineColLen.col - 1 + currentLineColLen.len));
            line.append(1, '\n');
        }
        else
        {
            line.append(util::Format(std::to_string(lineNumber), static_cast<int>(width), util::FormatJustify::right)).append(1, ' ');
            line.append(GetLine(static_cast<int>(lineNumber))).append(1, '\n');
        }
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
    if (printNext && end < LineCount() - 1)
    {
        cmajor::systemx::kernel::WriteToTerminal("ENTER=next page\n", process);
    }
}

void SourceFile::PrintCurrent(cmajor::systemx::kernel::Process* process, Debugger& debugger)
{
    int64_t start = std::max(static_cast<int64_t>(1), debugger.CurrentLineColLen().line - debugger.PageSize() / 2);
    Print(start, process, debugger, debugger.CurrentFile(), false);
}

SourceFiles::SourceFiles()
{
}

SourceFile& SourceFiles::GetSourceFile(const std::string& filePath)
{
    auto it = sourceFileMap.find(filePath);
    if (it != sourceFileMap.end())
    {
        return it->second;
    }
    sourceFileMap[filePath] = SourceFile(filePath);
    return sourceFileMap[filePath];
}

} // namespace cmajor::systemx::sxcdb
