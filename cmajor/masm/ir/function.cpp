// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.masm.ir.function;

import cmajor.masm.ir.context;
import util;

namespace cmajor::masm::ir {

Function::Function(const std::string& name_, Type* type_, Context& context) : Value(), name(name_), type(type_), nextResultNumber(0), nextBBNumber(0), isInline(false), mdId(-1)
{
    Type* t = type;
    if (type->IsPtrType())
    {
        PtrType* ptrType = static_cast<PtrType*>(type);
        t = ptrType->BaseType();
    }
    if (t->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(t);
        entryBlock.reset(new BasicBlock(nextBBNumber++));

        for (Type* paramType : functionType->ParamTypes())
        {
            Instruction* paramInst = new ParamInstruction(paramType);
            entryBlock->AddInstruction(paramInst);
            params.push_back(paramInst);
        }
    }
    else
    {
        throw std::runtime_error("function type or function pointer type expected");
    }
}

BasicBlock* Function::CreateBasicBlock()
{
    if (basicBlocks.empty())
    {
        BasicBlock* bb = entryBlock.get();
        basicBlocks.push_back(std::move(entryBlock));
        return bb;
    }
    BasicBlock* bb = new BasicBlock(nextBBNumber++);
    basicBlocks.push_back(std::unique_ptr<BasicBlock>(bb));
    return bb;
}

BasicBlock* Function::CreateCleanupBasicBlock()
{
    BasicBlock* cubb = new BasicBlock(-1);
    cleanupBasicBlocks.push_back(std::unique_ptr<BasicBlock>(cubb));
    return cubb;
}

void Function::Finalize()
{
    nextBBNumber = 0;
    for (std::unique_ptr<BasicBlock>& cubb : cleanupBasicBlocks)
    {
        basicBlocks.push_back(std::move(cubb));
    }
    for (auto& bb : basicBlocks)
    {
        if (bb->IsEmpty())
        {
            continue;
        }
        bb->SetId(nextBBNumber++);
    }
}

Value* Function::GetParam(int index) const
{
    Assert(index >= 0 && index < params.size(), "invalid param index");
    return params[index];
}

void Function::Write(util::CodeFormatter& formatter, Context& context)
{
    if (!comment.empty())
    {
        formatter.WriteLine("// " + comment);
        formatter.WriteLine();
    }
    if (basicBlocks.empty())
    {
        formatter.Write("extern ");
    }
    else if (isInline)
    {
        formatter.Write("inline ");
    }
    std::string mdIdStr;
    if (mdId != -1)
    {
        mdIdStr.append(" !").append(std::to_string(mdId));
    }
    formatter.WriteLine("function " + type->Name() + " " + name + mdIdStr);
    if (basicBlocks.empty())
    {
        return;
    }
    formatter.WriteLine("{");
    formatter.IncIndent();
    bool first = true;
    for (const auto& bb : basicBlocks)
    {
        if (bb->IsEmpty())
        {
            continue;
        }
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        bb->Write(formatter, *this, context);
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void Function::SetComment(const std::string& comment_)
{
    comment = comment_;
}

} // namespace cmajor::masm::ir
