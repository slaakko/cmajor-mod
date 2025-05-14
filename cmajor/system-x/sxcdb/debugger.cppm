// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.debugger;

import cmajor.systemx.sxcdb.command;
import cmajor.systemx.sxcdb.mode;
import cmajor.systemx.sxcdb.frame;
import cmajor.systemx.sxcdb.source_file;
import cmajor.systemx.sxcdb.break_point;
import cmajor.systemx.object;
import cmajor.systemx.machine;
import cmajor.systemx.kernel;
import soul.ast.span;
import util;
import std.core;

export namespace cmajor::systemx::sxcdb {

class BreakPoint;

class Debugger : public cmajor::systemx::machine::Debugger
{
public:
    Debugger(cmajor::systemx::machine::Machine* machine_, cmajor::systemx::kernel::Process* process_);
    void AddBreakPoint(const BreakPoint& bp);
    void Intercept(cmajor::systemx::machine::Processor* processor) override;
    void GetFrames();
    void HandleFrame();
    void ProcessExit() override;
    bool PrintLocation();
    void Exit();
    void PrintHelp();
    void Files(int64_t start);
    void PrintFrames(int64_t start);
    void PrintFrame();
    void PrintFrame(int64_t frameIndex, int64_t width);
    void SetFrame(int64_t frame_);
    void PrintFile();
    void SetFile(int64_t file_);
    void List(int64_t listFile, int64_t listStartLine);
    void PrintModes();
    void PrintMode();
    void SetMode(int64_t mode_);
    void PrintProcs(int64_t start, const std::vector<cmajor::systemx::kernel::Process*>& processes);
    void Break(int64_t breakFile, int64_t breakLine);
    void PrintBreakPoints(int64_t start);
    void DeleteBreakPoint(int64_t bpId);
    void ClearBreakPoints();
    void RemoveTemporaryBreakPoints();
    void InsertNextBreakPoints();
    void InsertOutBreakPoints();
    void SetPrevFrame();
    void Continue();
    void Next();
    void Step();
    void Out();
    void PrintLocals();
    void Print(const std::string& expr, int64_t start);
    int64_t PageSize() const { return pageSize; }
    int64_t File() const { return file; }
    int64_t CurrentFile() const { return currentFile; }
    const soul::ast::LineColLen& CurrentLineColLen() const { return currentLineColLen; }
    void ReadCommand();
    std::string ReadLine();
private:
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
    int64_t pageSize;
    int64_t file;
    int64_t currentFile;
    soul::ast::LineColLen currentLineColLen;
    int64_t frame;
    int64_t nextBreakPointId;
    Mode mode;
    std::map<uint64_t, BreakPoint> bpMap;
    BreakPoint* bp;
    Frames frames;
    Frame prevFrame;
    int64_t prevFrameCount;
    SourceFiles sourceFiles;
    std::unique_ptr<Command> command;
    bool exit;
    bool cont;
    std::vector<std::unique_ptr<cmajor::systemx::object::TypedValue>> indexedValues;
    std::vector<std::unique_ptr<cmajor::systemx::object::TypedValue>> transientValues;;
};

} // namespace cmajor::systemx::sxcdb
