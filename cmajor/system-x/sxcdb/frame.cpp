// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.frame;

namespace cmajor::systemx::sxcdb {

Frame::Frame() : index(-1), pc(0), entry(nullptr), lineNumber(0)
{
}

Frame::Frame(int index_, uint64_t pc_, cmajor::systemx::object::FunctionTableEntry* entry_, int lineNumber_) : index(index_), pc(pc_), entry(entry_), lineNumber(lineNumber_)
{
}

Frames::Frames()
{
}

void Frames::AddFrame(const Frame& frame)
{
    frames.push_back(frame);
}

Frames GetFrames(cmajor::systemx::kernel::Process* process)
{
    Frames frames;
    int index = 0;
    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    if (processor && symbolTable)
    {
        uint64_t pc = processor->Regs().GetPC();
        cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
        cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(pc, *symbolTable, process->RV(), processor->GetMachine()->Mem());
        if (entry)
        {
            int lineNumber = entry->SearchLineNumber(pc);
            frames.AddFrame(Frame(index, pc, entry, lineNumber));
            ++index;
        }
        uint64_t fp = processor->Regs().Get(cmajor::systemx::machine::regFP);
        while (fp != 0 && fp != cmajor::systemx::machine::stackSegmentBaseAddress)
        {
            uint64_t ret = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp - 8, cmajor::systemx::machine::Protection::read);
            uint64_t pc = ret - 4;
            cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(pc, *symbolTable, process->RV(), processor->GetMachine()->Mem());
            if (entry)
            {
                int32_t lineNumber = entry->SearchLineNumber(pc);
                frames.AddFrame(Frame(index, pc, entry, lineNumber));
                ++index;
            }
            fp = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
        }
    }
    return frames;
}

} // namespace cmajor::systemx::sxcdb
