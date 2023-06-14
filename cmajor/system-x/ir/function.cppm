// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.ir.function;

import cmajor.systemx.ir.type;
import cmajor.systemx.ir.basic.block;
import cmajor.systemx.ir.value;

export namespace cmajor::systemx::ir {

class Context;

class Function : public Value
{
public:
    Function(const std::string& name_, FunctionType* type_, Context& context);
    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;
    BasicBlock* CreateBasicBlock();
    BasicBlock* CreateCleanupBasicBlock();
    void Finalize();
    uint64_t GetNextResultNumber() { return nextResultNumber++; }
    Type* GetType(Context& context) override { return type; }
    Value* GetParam(int index) const;
    std::string Name(Context& context) override { return "@" + name; }
    void Write(util::CodeFormatter& formatter, Context& context);
    void SetLinkOnce() { linkOnce = true; }
    void SetMdId(int mdId_) { mdId = mdId_; }
private:
    std::string name;
    FunctionType* type;
    std::vector<Instruction*> params;
    std::unique_ptr<BasicBlock> entryBlock;
    std::vector<std::unique_ptr<BasicBlock>> basicBlocks;
    std::vector<std::unique_ptr<BasicBlock>> cleanupBasicBlocks;
    uint64_t nextResultNumber;
    bool linkOnce;
    int mdId;
    int nextBBNumber;
};

} // namespace cmajor::systemx::ir
