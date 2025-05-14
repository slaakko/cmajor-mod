// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.source_file;

import cmajor.systemx.kernel;
import util;
import std.core;

export namespace cmajor::systemx::sxcdb {

class Debugger;

class SourceFile
{
public:
    SourceFile();
    SourceFile(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
    int64_t LineCount() const { return static_cast<int64_t>(lines.size()); }
    const std::string& GetLine(int lineNumber) const { return lines[lineNumber]; }
    void Print(int64_t start, cmajor::systemx::kernel::Process* process, Debugger& debugger, int64_t file, bool printNext);
    void PrintCurrent(cmajor::systemx::kernel::Process* process, Debugger& debugger);
private:
    std::string filePath;
    std::vector<std::string> lines;
};

class SourceFiles
{
public:
    SourceFiles();
    SourceFile& GetSourceFile(const std::string& filePath);
private:
    std::map<std::string, SourceFile> sourceFileMap;
};

} // namespace cmajor::systemx::sxcdb
