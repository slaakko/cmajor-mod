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
    void Files(int start);
    void PrintFrames(int start);
    void PrintFrame();
    void PrintFrame(int frameIndex, int width);
    void SetFrame(int frame_);
    void PrintFile();
    void SetFile(int file_);
    void List(int listFile, int listStartLine);
    void PrintModes();
    void PrintMode();
    void SetMode(int mode_);
    void PrintProcs(int start, const std::vector<cmajor::systemx::kernel::Process*>& processes);
    void Break(int breakFile, int breakLine);
    void PrintBreakPoints(int start);
    void DeleteBreakPoint(int bpId);
    void ClearBreakPoints();
    void RemoveTemporaryBreakPoints();
    void InsertNextBreakPoints();
    void InsertOutBreakPoints();
    void SetPrevFrame();
    void Continue();
    void Next();
    void Step();
    void Out();
    int PageSize() const { return pageSize; }
    int File() const { return file; }
    int CurrentFile() const { return currentFile; }
    const soul::ast::LineColLen& CurrentLineColLen() const { return currentLineColLen; }
    void ReadCommand();
    std::string ReadLine();
private:
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
    int pageSize;
    int file;
    int currentFile;
    soul::ast::LineColLen currentLineColLen;
    int frame;
    int nextBreakPointId;
    Mode mode;
    std::map<uint64_t, BreakPoint> bpMap;
    BreakPoint* bp;
    Frames frames;
    Frame prevFrame;
    int prevFrameCount;
    SourceFiles sourceFiles;
    std::unique_ptr<Command> command;
    bool exit;
    bool cont;
};

} // namespace cmajor::systemx::sxcdb
