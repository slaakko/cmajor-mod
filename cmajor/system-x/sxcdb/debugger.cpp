// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.debugger;

import cmajor.systemx.sxutil;
import util;

namespace cmajor::systemx::sxcdb {

Debugger::Debugger(cmajor::systemx::machine::Machine* machine_, cmajor::systemx::kernel::Process* process_) : 
    pageSize(16), file(-1), currentFile(-1), currentLineColLen(), nextBreakPointId(0), frame(-1), machine(machine_), process(process_), bp(nullptr), exit(false), cont(false), 
    mode(Mode::all), prevFrameCount(-1)
{
}

void Debugger::AddBreakPoint(const BreakPoint& bp)
{
    bpMap[bp.PC()] = bp;
}

void Debugger::Intercept(cmajor::systemx::machine::Processor* processor)
{
    cmajor::systemx::machine::UserProcess* currentProcess = processor->CurrentProcess();
    if (currentProcess == process)
    {
        uint64_t pc = processor->Regs().GetPC();
        auto it = bpMap.find(pc);
        if (it != bpMap.end())
        {
            bp = &it->second;
            GetFrames();
            HandleFrame();
        }
        else if (prevFrame.Index() != -1 && 
            !bpMap.empty() && 
            pc >= static_cast<uint64_t>(prevFrame.Entry()->FunctionStart()) && 
            pc < static_cast<uint64_t>(prevFrame.Entry()->FunctionStart() + prevFrame.Entry()->FunctionLength()))
        {
            return;
        }
        else
        {
            GetFrames();
            if (command && command->IsNext() && prevFrameCount < frames.FrameCount())
            {
                return;
            }
            else if (command && command->IsStep() && prevFrameCount < frames.FrameCount())
            {
                HandleFrame();
            }
        }
    }
    if (exit)
    {
        cmajor::systemx::kernel::WriteToTerminal("exiting...\n", process);
        machine->SetExiting();
    }
    return;
}

void Debugger::GetFrames()
{
    frames = cmajor::systemx::sxcdb::GetFrames(process);
    frame = 0;
}

void Debugger::SetPrevFrame()
{
    prevFrameCount = frames.FrameCount();
    if (prevFrameCount > 0)
    {
        prevFrame = frames.GetFrame(0);
    }
}

void Debugger::HandleFrame()
{
    try
    {
        bool canHandle = PrintLocation();
        if (canHandle)
        {
            RemoveTemporaryBreakPoints();
            transientValues.clear();
            indexedValues.clear();
            ReadCommand();
            SetPrevFrame();
        }
    }
    catch (const std::exception& ex)
    {
        cmajor::systemx::kernel::WriteToTerminal(std::string(ex.what()) + "\n", process);
    }
}

void Debugger::ProcessExit()
{
    // todo
}

bool Debugger::PrintLocation()
{
    if (frames.FrameCount() > 0)
    {
        const Frame& currentFrame = frames.GetFrame(static_cast<int>(frame));
        if (currentFrame.LineColLen().IsValid())
        {
            cmajor::systemx::object::FunctionTableEntry* entry = currentFrame.Entry();
            cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
            cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
            int32_t sourceFileNameId = entry->SourceFileNameId();
            functionTable->ReadSourceFileTable(*symbolTable, process->RV(), process->GetMachine()->Mem());
            currentFile = functionTable->GetSourceFileIndex(sourceFileNameId);
            if (currentFile != -1)
            {
                file = currentFile;
                SourceFile& sourceFile = sourceFiles.GetSourceFile(functionTable->GetSourceFileName(static_cast<int32_t>(file), *symbolTable, process->RV(), 
                    process->GetMachine()->Mem()));
                currentLineColLen = currentFrame.LineColLen();
                sourceFile.PrintCurrent(process, *this);
                return true;
            }
        }
    }
    return false;
}

void Debugger::Exit()
{
    exit = true;
}

void Debugger::PrintHelp()
{
    std::vector<std::string> commands;
    std::vector<std::string> descriptions;
    commands.push_back("ENTER");
    descriptions.push_back("repeat latest command");
    commands.push_back("ex(it)");
    descriptions.push_back("exit the debugger");
    commands.push_back("he(lp)");
    descriptions.push_back("print this help");
    commands.push_back("files");
    descriptions.push_back("print source file name list starting from index 0");
    commands.push_back("files START");
    descriptions.push_back("print source file name list starting from index START");
    commands.push_back("fi(le)");
    descriptions.push_back("print active source file index and its file path");
    commands.push_back("fi(le) FILE");
    descriptions.push_back("set active source file index to FILE and print its file path");
    commands.push_back("li(st)");
    descriptions.push_back("list a page of source lines of active source file starting from line number 1");
    commands.push_back("li(st) LINE");
    descriptions.push_back("list a page of source lines of active source file starting from line number LINE");
    commands.push_back("li(st) FILE LINE");
    descriptions.push_back("set active source file index to FILE and list source lines of it starting from line number LINE");
    commands.push_back("frames");
    descriptions.push_back("print stack frames starting from index 0");
    commands.push_back("frames START");
    descriptions.push_back("print stack frames starting from index START");
    commands.push_back("fr(ame)");
    descriptions.push_back("print active frame index and active frame");
    commands.push_back("fr(ame) FRAME");
    descriptions.push_back("set active frame index to FRAME and print it");
    commands.push_back("modes");
    descriptions.push_back("print debugger modes");
    commands.push_back("mo(de)");
    descriptions.push_back("print current debugger mode");
    commands.push_back("mo(de) MODE");
    descriptions.push_back("set current debugger mode to MODE and print it");
    commands.push_back("pr(cs)");
    descriptions.push_back("print process list starting from index 0");
    commands.push_back("pr(cs) START");
    descriptions.push_back("print process list starting from index START");
    commands.push_back("br(eak) LINE");
    descriptions.push_back("set breakpoint to line number LINE of active source file");
    commands.push_back("br(eak) FILE LINE");
    descriptions.push_back("set breakpoint to source file index FILE and line number LINE");
    commands.push_back("breakpoints");
    descriptions.push_back("print breakpoints starting from index 0");
    commands.push_back("breakpoints START");
    descriptions.push_back("print breakpoints starting from index START");
    commands.push_back("de(lete) BREAKPOINT");
    descriptions.push_back("delete breakpoint number BREAKPOINT");
    commands.push_back("cl(ear)");
    descriptions.push_back("clear all breakpoints");
    commands.push_back("co(ntinue)");
    descriptions.push_back("continue execution");
    commands.push_back("ne(xt)");
    descriptions.push_back("go to next line");
    commands.push_back("st(ep)");
    descriptions.push_back("step inside function calls");
    commands.push_back("ou(t)");
    descriptions.push_back("go to out of current function");

    int width = 0;
    for (const auto& command : commands)
    {
        width = std::max(width, static_cast<int>(command.length()));
    }
    std::string lines;
    for (int i = 0; i < static_cast<int>(commands.size()); ++i)
    {
        lines.append(util::Format(commands[i], width, util::FormatJustify::right)).append(" : ").append(descriptions[i]).append(1, '\n');
    }
    cmajor::systemx::kernel::WriteToTerminal(lines, process);
}

void Debugger::Files(int64_t start)
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    int64_t n = functionTable->SourceFileCount(*symbolTable, process->RV(), machine->Mem());
    int64_t end = std::min(start + pageSize - 1, n - 1);
    int64_t width = 0;
    for (int64_t fileIndex = start; fileIndex <= end; ++fileIndex)
    {
        width = std::max(width, static_cast<int64_t>(util::Log10(static_cast<int>(fileIndex))));
    }
    for (int64_t fileIndex = start; fileIndex <= end; ++fileIndex)
    {
        std::string sourceFileName = functionTable->GetSourceFileName(static_cast<int32_t>(fileIndex), *symbolTable, process->RV(), machine->Mem());
        std::string line;
        if (fileIndex == file)
        {
            line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
        }
        line.append(util::Format(std::to_string(fileIndex), static_cast<int32_t>(width), util::FormatJustify::right)).append(1, ' ').
            append(sourceFileName);
        if (fileIndex == file)
        {
            line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
        }
        line.append(1, '\n');
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
    if (end < n - 1)
    {
        cmajor::systemx::kernel::WriteToTerminal("ENTER=next page\n", process);
    }
}

void Debugger::PrintFile()
{
    if (file == -1)
    {
        cmajor::systemx::kernel::WriteToTerminal("file not set\n", process);
    }
    else
    {
        cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
        cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
        std::string sourceFileName = functionTable->GetSourceFileName(static_cast<int32_t>(file), *symbolTable, process->RV(), machine->Mem());
        std::string line;
        line.append("file=").append(std::to_string(file).append(1, ' ').append(sourceFileName)).append(1, '\n');
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
}

void Debugger::SetFile(int64_t file_)
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    int64_t n = functionTable->SourceFileCount(*symbolTable, process->RV(), machine->Mem());
    if (file_ >= 0 && file_ < n)
    {
        file = file_;
        PrintFile();
    }
    else
    {
        throw std::runtime_error("invalid file index " + std::to_string(file_));
    }
}

void Debugger::List(int64_t listFile, int64_t listStartLine)
{
    file = listFile;
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    SourceFile& sourceFile = sourceFiles.GetSourceFile(functionTable->GetSourceFileName(static_cast<int32_t>(listFile), *symbolTable, process->RV(), 
        process->GetMachine()->Mem()));
    sourceFile.Print(static_cast<int>(listStartLine), process, *this, static_cast<int>(listFile), true);
}

void Debugger::PrintModes()
{
    for (int i = 1; i <= static_cast<int>(Mode::last); i <<= 1)
    {
        std::string line;
        bool enabled = (static_cast<int>(mode) & i) != 0;
        if (enabled)
        {
            line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
        }
        line.append(ModeName(static_cast<Mode>(i))).append(1, '\n');
        if (enabled)
        {
            line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
        }
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
}

void Debugger::PrintMode()
{
    PrintModes();
    std::string line;
    line.append("mode=").append(std::to_string(static_cast<int>(mode))).append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
}

void Debugger::SetMode(int64_t mode_)
{
    mode = Mode(mode_);
    PrintMode();
}

void Debugger::PrintFrames(int64_t start)
{
    int64_t n = frames.FrameCount();
    int64_t end = std::min(start + pageSize - 1, n - 1);
    int64_t width = 0;
    for (int64_t frameIndex = start; frameIndex <= end; ++frameIndex)
    {
        width = std::max(width, static_cast<int64_t>(util::Log10(static_cast<int>(frameIndex))));
    }
    for (int64_t frameIndex = start; frameIndex <= end; ++frameIndex)
    {
        PrintFrame(frameIndex, width);
    }
    if (end < n - 1)
    {
        cmajor::systemx::kernel::WriteToTerminal("ENTER=next page\n", process);
    }
}

void Debugger::PrintFrame()
{
    PrintFrame(frame, static_cast<int64_t>(util::Log10(static_cast<int>(frame))));
}

void Debugger::PrintFrame(int64_t frameIndex, int64_t width)
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    std::string line;
    if (frameIndex == frame)
    {
        line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
    }
    const Frame& frm = frames.GetFrame(static_cast<int>(frameIndex));
    line.append(util::Format(std::to_string(frameIndex), static_cast<int>(width), util::FormatJustify::right)).append(1, ' ');
    if (!frm.Entry()->FullName().empty())
    {
        line.append(frm.Entry()->FullName()).append(1, ' ');
    }
    else
    {
        line.append("Main ");
    }
    int32_t sourceFileNameId = frm.Entry()->SourceFileNameId();
    if (sourceFileNameId != -1)
    {
        functionTable->ReadSourceFileTable(*symbolTable, process->RV(), process->GetMachine()->Mem());
        int32_t sourceFileIndex = functionTable->GetSourceFileIndex(sourceFileNameId);
        if (sourceFileIndex != -1)
        {
            line.append(std::to_string(sourceFileIndex)).append(1, ' ');
        }
    }
    if (!frm.Entry()->SourceFileName().empty())
    {
        line.append(frm.Entry()->SourceFileName());
    }
    if (frm.LineColLen().IsValid())
    {
        line.append(" line ").append(std::to_string(frm.LineColLen().line));
    }
    if (frameIndex == frame)
    {
        line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
    }
    line.append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
}

void Debugger::SetFrame(int64_t frame_)
{
    if (frame_ >= 0 && frame_ < frames.FrameCount())
    {
        frame = frame_;
        PrintFrame();
    }
    else
    {
        throw std::runtime_error("invalid frame index " + std::to_string(frame_));
    }
}

void Debugger::PrintProcs(int64_t start, const std::vector<cmajor::systemx::kernel::Process*>& processes)
{
    int64_t n = static_cast<int>(processes.size());
    int64_t end = std::min(start + pageSize, n - 1);
    int64_t width = 0;
    for (int64_t i = start; i <= end; ++i)
    {
        cmajor::systemx::kernel::Process* proc = processes[i];
        std::string pid = std::to_string(proc->Id());
        width = std::max(width, int64_t(pid.length()));
    }
    for (int64_t i = start; i <= end; ++i)
    {
        cmajor::systemx::kernel::Process* proc = processes[i];
        std::string line;
        if (proc == process)
        {
            line.append(util::ToUtf8(cmajor::systemx::SetColors(cmajor::systemx::ConsoleColor::green, cmajor::systemx::ConsoleColor::black)));
        }
        line.append(util::Format(std::to_string(proc->Id()), static_cast<int>(width), util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(1, ' ').append(proc->FilePath());
        cmajor::systemx::machine::ProcessState state = proc->State();
        line.append(1, ' ').append(cmajor::systemx::machine::ProcessStateStr(state));
        if (proc == process)
        {
            line.append(util::ToUtf8(cmajor::systemx::ResetColors()));
        }
        line.append(1, '\n');
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
    if (end < n - 1)
    {
        cmajor::systemx::kernel::WriteToTerminal("ENTER=next page\n", process);
    }
}

void Debugger::Break(int64_t breakFile, int64_t breakLine)
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    int n = functionTable->SourceFileCount(*symbolTable, process->RV(), machine->Mem());
    if (breakFile < 0 || breakFile >= n)
    {
        throw std::runtime_error("invalid breakpoint file " + std::to_string(breakFile));
    }
    SourceFile& sourceFile = sourceFiles.GetSourceFile(functionTable->GetSourceFileName(static_cast<int32_t>(breakFile), *symbolTable, process->RV(), 
        process->GetMachine()->Mem()));
    int64_t lineCount = sourceFile.LineCount();
    if (breakLine < 1 || breakLine >= lineCount)
    {
        throw std::runtime_error("invalid breakpoint line " + std::to_string(breakLine));
    }
    cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(static_cast<int32_t>(breakFile), static_cast<int32_t>(breakLine), *symbolTable, process->RV(), 
        process->GetMachine()->Mem());
    if (!entry)
    {
        throw std::runtime_error("invalid breakpoint line " + std::to_string(breakLine));
    }
    std::vector<int64_t> pcs = entry->SearchPCs(static_cast<int32_t>(breakLine));
    if (pcs.empty())
    {
        throw std::runtime_error("function '" + entry->FullName() + "' has no code associated with line " + std::to_string(breakLine));
    }
    for (int64_t pc : pcs)
    {
        auto it = bpMap.find(pc);
        if (it != bpMap.end() && it->second.Id() != -1)
        {
            const BreakPoint& breakPoint = it->second;
            std::string line;
            line.append("breakpoint " + std::to_string(breakPoint.Id()) + " set to function '" + entry->FullName() + "' line " + std::to_string(breakLine) +
                " address #" + util::ToHexString(static_cast<uint64_t>(pc)));
            line.append(1, '\n');
            cmajor::systemx::kernel::WriteToTerminal(line, process);
        }
        else
        {
            BreakPoint breakPoint(pc, static_cast<int>(nextBreakPointId++), entry, static_cast<int32_t>(breakLine));
            AddBreakPoint(breakPoint);
            std::string line;
            line.append("breakpoint " + std::to_string(breakPoint.Id()) + " set to function '" + entry->FullName() + "' line " + std::to_string(breakLine) +
                " address #" + util::ToHexString(static_cast<uint64_t>(pc)));
            line.append(1, '\n');
            cmajor::systemx::kernel::WriteToTerminal(line, process);
        }
    }
}

void Debugger::PrintBreakPoints(int64_t start)
{
    std::vector<BreakPoint> bps;
    for (const auto& pcBpPair : bpMap)
    {
        BreakPoint bp = pcBpPair.second;
        if (bp.Id() != -1)
        {
            bps.push_back(bp);
        }
    }
    std::sort(bps.begin(), bps.end());
    int64_t n = static_cast<int64_t>(bps.size());
    int64_t end = std::min(start + pageSize, n - 1);
    for (int64_t i = start; i <= end; ++i)
    {
        const BreakPoint& bp = bps[i];
        std::string line;
        line.append("breakpoint " + std::to_string(bp.Id()) + " set to function '" + bp.Entry()->FullName() + "' line " + std::to_string(bp.Line()) +
            " address #" + util::ToHexString(bp.PC()));
        line.append(1, '\n');
        cmajor::systemx::kernel::WriteToTerminal(line, process);
    }
    if (end < n - 1)
    {
        cmajor::systemx::kernel::WriteToTerminal("ENTER=next page\n", process);
    }
}

void Debugger::DeleteBreakPoint(int64_t bpId)
{
    bool deleted = false;
    for (const auto& pcBpPair : bpMap)
    {
        const BreakPoint& bp = pcBpPair.second;
        if (bp.Id() == bpId)
        {
            bpMap.erase(bp.PC());
            std::string line;
            line.append("breakpoint " + std::to_string(bpId) + " deleted").append(1, '\n');
            cmajor::systemx::kernel::WriteToTerminal(line, process);
            deleted = true;
            break;
        }
    }
    if (!deleted)
    {
        throw std::runtime_error("breakpoint " + std::to_string(bpId) + " not found");
    }
}

void Debugger::ClearBreakPoints()
{
    bpMap.clear();
    std::string line;
    line.append("breakpoints cleared").append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
}

void Debugger::RemoveTemporaryBreakPoints()
{
    std::vector<uint64_t> temporaryBpPCs;
    for (const auto& bp : bpMap)
    {
        if (bp.second.Id() == -1)
        {
            temporaryBpPCs.push_back(bp.second.PC());
        }
    }
    for (const uint64_t pc : temporaryBpPCs)
    {
        bpMap.erase(pc);
    }
}

void Debugger::InsertNextBreakPoints()
{
    if (frames.FrameCount() == 0) return;
    const Frame& currentFrame = frames.GetFrame(0);
    int32_t currentIndex = currentFrame.Idx();
    cmajor::systemx::object::FunctionTableEntry* entry = currentFrame.Entry();
    if (!entry) return;
    std::vector<int32_t> next = entry->Next(currentIndex);
    for (int32_t nextIndex : next)
    {
        int64_t pc = entry->SearchPC(nextIndex);
        if (pc != -1)
        {
            AddBreakPoint(BreakPoint(static_cast<uint64_t>(pc)));
        }
    }
    if (frames.FrameCount() > 1)
    {
        const Frame& prevFrame = frames.GetFrame(1);
        int32_t prevFrameIndex = prevFrame.Idx();
        cmajor::systemx::object::FunctionTableEntry* entry = prevFrame.Entry();
        if (!entry) return;
        int64_t pc = entry->SearchPC(prevFrameIndex);
        if (pc != -1)
        {
            AddBreakPoint(BreakPoint(static_cast<uint64_t>(pc)));
        }
    }
}

void Debugger::InsertOutBreakPoints()
{
    if (frames.FrameCount() > 1)
    {
        const Frame& prevFrame = frames.GetFrame(1);
        int32_t prevFrameIndex = prevFrame.Idx();
        cmajor::systemx::object::FunctionTableEntry* entry = prevFrame.Entry();
        if (!entry) return;
        int64_t pc = entry->SearchPC(prevFrameIndex);
        if (pc != -1)
        {
            AddBreakPoint(BreakPoint(static_cast<uint64_t>(pc)));
        }
    }
}

void Debugger::Continue()
{
    std::string line;
    line.append("continue...").append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
    cont = true;
}

void Debugger::Next()
{
    InsertNextBreakPoints();
    std::string line;
    line.append("next...").append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
    cont = true;
}

void Debugger::Step()
{
    InsertNextBreakPoints();
    std::string line;
    line.append("step...").append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
    cont = true;
}

void Debugger::Out()
{
    InsertOutBreakPoints();
    std::string line;
    line.append("out...").append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(line, process);
    cont = true;
}

void Debugger::PrintLocals()
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    cmajor::systemx::object::StringTable* stringTable = functionTable->GetStringTable();
    cmajor::systemx::object::TypeTable* typeTable = process->GetTypeTable();
    std::string lines;
    lines.append("locals:");
    if (frames.FrameCount() > 0)
    {
        const Frame& frm = frames.GetFrame(static_cast<int>(frame));
        cmajor::systemx::object::FunctionTableEntry* entry = frm.Entry();
        if (entry)
        {
            const std::vector<cmajor::systemx::object::LocalEntry>& localEntries = entry->LocalEntries();
            int64_t count = std::min(static_cast<int64_t>(localEntries.size()), pageSize);
            for (int64_t i = 0; i < count; ++i)
            {
                const cmajor::systemx::object::LocalEntry& localEntry = localEntries[i];
                cmajor::systemx::object::Type* type = typeTable->GetType(localEntry.typeId, process->RV(), process->GetMachine()->Mem(), stringTable, symbolTable);
                uint64_t address = frm.FP() + localEntry.offset;
                std::vector<std::unique_ptr<cmajor::systemx::object::TypedValue>> stack;
                cmajor::systemx::object::EvaluationContext context(address, process->RV(), process->GetMachine()->Mem(), stringTable, symbolTable, typeTable, 
                    &indexedValues, &transientValues, &stack, pageSize);
                std::unique_ptr<cmajor::systemx::object::TypedValue> value = type->Evaluate(context);
                value->SetAddress(address);
                int32_t index = static_cast<int32_t>(indexedValues.size());
                lines.append("\n").append("$").append(std::to_string(index)).append(" = ").append(localEntry.name).
                    append(" = ").append(value->ToString(false));
                indexedValues.push_back(std::move(value));
            }
        }
    }
    lines.append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(lines, process);
}

void Debugger::Print(const std::string& expr, int64_t start)
{
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    cmajor::systemx::object::StringTable* stringTable = functionTable->GetStringTable();
    cmajor::systemx::object::TypeTable* typeTable = process->GetTypeTable();
    std::vector<std::unique_ptr<cmajor::systemx::object::TypedValue>> stack;
    cmajor::systemx::object::EvaluationContext context(0, process->RV(), process->GetMachine()->Mem(), stringTable, symbolTable, typeTable,
        &indexedValues, &transientValues, &stack, pageSize);
    context.start = start;
    context.content = true;
    std::string lines;
    lines.append("print...");
    cmajor::systemx::object::TypedValue* value = cmajor::systemx::object::Evaluate(expr, context);
    if (value->NextStart() != -1)
    {
        std::vector<int64_t> args(1, value->NextStart());
        command->SetArgs(args);
    }
    lines.append(1, '\n').append(value->ToString(false));
    lines.append(1, '\n');
    cmajor::systemx::kernel::WriteToTerminal(lines, process);
}

std::string Debugger::ReadLine()
{
    std::string prompt = "> ";
    cmajor::systemx::kernel::WriteToTerminal(prompt, process);
    std::string line = cmajor::systemx::kernel::ReadLineFromTerminal();
    return line;
}

std::vector<std::string> Tokenize(const std::string& line)
{
    std::vector<std::string> tokens;
    std::string token;
    int state = 0;
    for (char c : line)
    {
        switch (state)
        {
            case 0:
            {
                if (c == ' ' || c == '\t')
                {
                    state = 1;
                }
                else
                {
                    token.append(1, c);
                }
                break;
            }
            case 1:
            {
                if (c != ' ' && c != '\t')
                {
                    if (!token.empty())
                    {
                        tokens.push_back(token);
                    }
                    token.clear();
                    token.append(1, c);
                    state = 0;
                }
                break;
            }
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

void Debugger::ReadCommand()
{
    cont = false;
    while (!cont && !exit)
    {
        try
        {
            std::string line = ReadLine();
            if (line.empty())
            {
                if (!command)
                {
                    throw std::runtime_error("no previous command to repeat, try 'help'");
                }
                command->Execute(*this);
            }
            else
            {
                std::vector<std::string> tokens = Tokenize(line);
                if (tokens.empty())
                {
                    throw std::runtime_error("invalid command, try 'help'");
                }
                else
                {
                    std::string commandName = tokens.front();
                    command.reset(GetCommand(commandName));
                    if (command->IsPrint())
                    {
                        std::string expr;
                        int n = static_cast<int>(tokens.size());
                        for (int i = 1; i < n; ++i)
                        {
                            const std::string& token = tokens[i];
                            if (i > 1)
                            {
                                expr.append(1, ' ');
                            }
                            expr.append(token);
                        }
                        PrintCommand* printCommand = static_cast<PrintCommand*>(command.get());
                        printCommand->SetExpr(expr);
                    }
                    else
                    {
                        std::vector<int64_t> args;
                        for (int i = 1; i < tokens.size(); ++i)
                        {
                            args.push_back(std::stoll(tokens[i]));
                        }
                        command->SetArgs(args);
                    }
                    command->Execute(*this);
                }
            }
        }
        catch (const std::exception& ex)
        {
            cmajor::systemx::kernel::WriteToTerminal(std::string(ex.what()) + "\n", process);
        }
    }
}

} // namespace cmajor::systemx::sxcdb
