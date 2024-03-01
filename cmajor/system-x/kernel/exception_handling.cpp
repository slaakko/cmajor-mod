// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.exception.handling;

import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;
import cmajor.systemx.object;

namespace cmajor::systemx::kernel {

void AddStackTraceEntry(std::string& stackTrace, cmajor::systemx::object::FunctionTableEntry* functionTableEntry, int32_t lineNumber, int index)
{
    stackTrace.append(std::to_string(index)).append(": ");
    if (!functionTableEntry->FullName().empty())
    {
        stackTrace.append(functionTableEntry->FullName());
    }
    if (!functionTableEntry->MangledName().empty())
    {
        stackTrace.append("[").append(functionTableEntry->MangledName()).append("]");
    }
    if (!functionTableEntry->SourceFileName().empty())
    {
        stackTrace.append(" ").append(functionTableEntry->SourceFileName());
    }
    if (lineNumber != -1)
    {
        stackTrace.append(" line ").append(std::to_string(lineNumber));
    }
    stackTrace.append("\n");
}

std::string GetStackTrace(Process* process)
{
    std::string stackTrace = "STACK TRACE\n";
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
            int32_t lineNumber = entry->SearchLineNumber(pc);
            AddStackTraceEntry(stackTrace, entry, lineNumber, index);
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
                AddStackTraceEntry(stackTrace, entry, lineNumber, index);
                ++index;
            }
            fp = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
        }
    }
    return stackTrace;
}

bool DispatchExceptionUsingRecord(cmajor::systemx::object::ExceptionTableRecord* record, uint64_t fp,
    uint64_t exceptionAddress, uint64_t exceptionClassId, Process* process, uint64_t& setFP, uint64_t& setSP, uint64_t& setPC)
{
    process->SetCurrentTryRecord(nullptr);
    switch (record->Kind())
    {
    case cmajor::systemx::object::ExceptionTableRecordKind::tryRecord:
    {
        cmajor::systemx::object::TryRecord* tryRecord = static_cast<cmajor::systemx::object::TryRecord*>(record);
        for (cmajor::systemx::object::HandlerRecord* handlerRecord : tryRecord->HandlerRecords())
        {
            uint64_t caughtClassId = handlerRecord->CaughtClassId();
            if (exceptionClassId % caughtClassId == 0) // handle this one...
            {
                setPC = handlerRecord->HandlerAddress();
                setFP = fp;
                setSP = fp + record->Table()->GetFunctionTableEntry()->FrameSize();
                process->SetCurrentExceptionAddress(exceptionAddress);
                process->SetCurrentExceptionClassId(exceptionClassId);
                process->SetCurrentTryRecord(tryRecord);
                return true;
            }
        }
        for (cmajor::systemx::object::CleanupRecord* cleanupRecord : tryRecord->CleanupRecords())
        {
            setPC = cleanupRecord->CleanupAddress();
            setFP = fp;
            setSP = fp + record->Table()->GetFunctionTableEntry()->FrameSize();
            process->SetCurrentExceptionAddress(exceptionAddress);
            process->SetCurrentExceptionClassId(exceptionClassId);
            process->SetCurrentTryRecord(tryRecord);
            return true;
        }
        break;
    }
    case cmajor::systemx::object::ExceptionTableRecordKind::cleanupRecord:
    {
        cmajor::systemx::object::CleanupRecord* cleanupRecord = static_cast<cmajor::systemx::object::CleanupRecord*>(record);
        setPC = cleanupRecord->CleanupAddress();
        setFP = fp;
        setSP = fp + record->Table()->GetFunctionTableEntry()->FrameSize();
        process->SetCurrentExceptionAddress(exceptionAddress);
        process->SetCurrentExceptionClassId(exceptionClassId);
        process->SetCurrentTryRecord(cleanupRecord->GetTryRecord());
        return true;
    }
    }
    return false;
}

void DispatchException(uint64_t exceptionAddress, uint64_t exceptionClassId, Process* process, uint64_t& setFP, uint64_t& setSP, uint64_t& setPC)
{
    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    if (processor && symbolTable && functionTable)
    {
        uint64_t pc = processor->Regs().GetPC();
        uint64_t fp = processor->Regs().Get(cmajor::systemx::machine::regFP);
        cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(pc, *symbolTable, process->RV(), processor->GetMachine()->Mem());
        if (entry)
        {
            cmajor::systemx::object::ExceptionTableRecord* record = entry->SearchExceptionTableRecord(pc);
            while (record)
            {
                uint64_t parentFP = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
                if (DispatchExceptionUsingRecord(record, parentFP, exceptionAddress, exceptionClassId, process, setFP, setSP, setPC))
                {
                    return;
                }
                if (process->CurrentTryRecord())
                {
                    record = process->CurrentTryRecord()->ParentTryRecord();
                }
                else
                {
                    record = nullptr;
                }
            }
        }
        while (fp != 0 && fp != cmajor::systemx::machine::stackSegmentBaseAddress)
        {
            uint64_t ret = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp - 8, cmajor::systemx::machine::Protection::read);
            uint64_t pc = ret - 4;
            cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(pc, *symbolTable, process->RV(), processor->GetMachine()->Mem());
            if (entry)
            {
                cmajor::systemx::object::ExceptionTableRecord* record = entry->SearchExceptionTableRecord(pc);
                while (record)
                {
                    uint64_t parentFP = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
                    if (DispatchExceptionUsingRecord(record, parentFP, exceptionAddress, exceptionClassId, process, setFP, setSP, setPC))
                    {
                        return;
                    }
                    if (process->CurrentTryRecord())
                    {
                        record = process->CurrentTryRecord()->ParentTryRecord();
                    }
                    else
                    {
                        record = nullptr;
                    }
                }
            }
            fp = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
        }
    }
    throw SystemError(EFAIL, "exception dispatch failed: exception handler not found", __FUNCTION__);
}

void ContinueExceptionDispatch(Process* process, uint64_t& setFP, uint64_t& setSP, uint64_t& setPC)
{
    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    cmajor::systemx::object::SymbolTable* symbolTable = process->GetSymbolTable();
    cmajor::systemx::object::FunctionTable* functionTable = process->GetFunctionTable();
    if (processor && symbolTable && functionTable)
    {
        uint64_t fp = processor->Regs().Get(cmajor::systemx::machine::regFP);
        if (process->CurrentTryRecord())
        {
            cmajor::systemx::object::ExceptionTableRecord* record = process->CurrentTryRecord()->ParentTryRecord();
            while (record)
            {
                uint64_t parentFP = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
                if (DispatchExceptionUsingRecord(record, parentFP, process->CurrentExceptionAddress(), process->CurrentExceptionClassId(), process, setFP, setSP, setPC))
                {
                    return;
                }
                if (process->CurrentTryRecord())
                {
                    record = process->CurrentTryRecord()->ParentTryRecord();
                }
                else
                {
                    record = nullptr;
                }
            }
        }
        fp = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
        while (fp != 0 && fp != cmajor::systemx::machine::stackSegmentBaseAddress)
        {
            uint64_t ret = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp - 8, cmajor::systemx::machine::Protection::read);
            uint64_t pc = ret - 4;
            cmajor::systemx::object::FunctionTableEntry* entry = functionTable->GetEntry(pc, *symbolTable, process->RV(), processor->GetMachine()->Mem());
            if (entry)
            {
                cmajor::systemx::object::ExceptionTableRecord* record = entry->SearchExceptionTableRecord(pc);
                while (record)
                {
                    uint64_t parentFP = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
                    if (DispatchExceptionUsingRecord(record, parentFP, process->CurrentExceptionAddress(), process->CurrentExceptionClassId(), process, setFP, setSP, setPC))
                    {
                        return;
                    }
                    if (process->CurrentTryRecord())
                    {
                        record = process->CurrentTryRecord()->ParentTryRecord();
                    }
                    else
                    {
                        record = nullptr;
                    }
                }
            }
            fp = processor->GetMachine()->Mem().ReadOcta(process->RV(), fp, cmajor::systemx::machine::Protection::read);
        }
    }
    throw SystemError(EFAIL, "exception dispatch failed: exception handler not found", __FUNCTION__);
}

} // namespace cmajor::systemx::kernel
