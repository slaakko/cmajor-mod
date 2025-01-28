// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.util;

namespace cmajor::masm::intermediate {

void ReplaceInstructionWithValue(Instruction* inst, Value* value)
{
    inst->ReplaceUsesWith(value);
    inst->Parent()->RemoveInstruction(inst);
}

void ReplaceInstructionWithInstruction(Instruction* oldInst, Instruction* newInst)
{
    if (oldInst->IsValueInstruction() && newInst->IsValueInstruction())
    {
        ValueInstruction* valueInst = static_cast<ValueInstruction*>(newInst);
        oldInst->ReplaceUsesWith(valueInst->Result());
    }
    BasicBlock* bb = oldInst->Parent();
    bb->InsertInstructionAfter(newInst, oldInst);
    bb->RemoveInstruction(oldInst);
}

bool IsPowerOfTwo(uint64_t n, int& shift)
{
    uint64_t p = 2;
    shift = 1;
    while (p != 0 && p < n)
    {
        p <<= 1;
        ++shift;
    }
    if (n == p)
    {
        return true;
    }
    else
    {
        return false;
    }
}

} // cmajor::masm::intermediate
