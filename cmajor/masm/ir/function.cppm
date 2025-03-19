// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.function;

import cmajor.masm.ir.type;
import cmajor.masm.ir.basic_block;
import cmajor.masm.ir.value;
import cmajor.masm.ir.instruction;
import util;

export namespace cmajor::masm::ir {

class Context;

class Function : public Value
{
public:
    Function(const std::string& name_, Type* type_, Context& context);
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    BasicBlock* CreateBasicBlock();
    BasicBlock* CreateCleanupBasicBlock();
    void Finalize();
    int32_t GetNextResultNumber() { return nextResultNumber++; }
    int GetNextInstructionIndex() { return nextInstructionIndex++; }
    Type* GetType(Context& context) override { return type; }
    Value* GetParam(int index) const;
    std::string Name(Context& context) override { return "@" + name; }
    void SetComment(const std::string& comment_);
    void SetInline() { isInline = true; }
    bool Inline() const { return isInline; }
    void SetLinkOnce() { linkOnce = true; }
    bool LinkOnce() const { return linkOnce; }
    void SetMdId(int mdId_) { mdId = mdId_; }
    int MdId() const { return mdId; }
    void Write(util::CodeFormatter& formatter, Context& context, util::CodeFormatter& linesFormatter);
private:
    std::string name;
    Type* type;
    std::vector<Instruction*> params;
    std::unique_ptr<BasicBlock> entryBlock;
    std::vector<std::unique_ptr<BasicBlock>> basicBlocks;
    std::vector<std::unique_ptr<BasicBlock>> cleanupBasicBlocks;
    int32_t nextResultNumber;
    int nextInstructionIndex;
    int nextBBNumber;
    std::string comment;
    bool isInline;
    bool linkOnce;
    int mdId;
};

} // namespace cmajor::masm::ir
