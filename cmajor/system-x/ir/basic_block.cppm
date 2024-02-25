// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.basic.block;

import cmajor.systemx.ir.instruction;
import util;
import std.core;

export namespace cmajor::systemx::ir {

class BasicBlock
{
public:
    BasicBlock(int id_);
    BasicBlock(const BasicBlock&) = delete;
    BasicBlock& operator=(const BasicBlock&) = delete;
    void AddInstruction(Instruction* instruction);
    bool IsEmpty() const { return instructions.empty(); }
    int Id() const { return id; }
    void SetId(int id_) { id = id_; }
    void Write(util::CodeFormatter& formatter, Function& function, Context& context);
private:
    int id;
    std::vector<std::unique_ptr<Instruction>> instructions;
};

} // namespace cmajor::systemx::ir
