// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.cpp.ir.basic.block;

import cmajor.cpp.ir.context;
import cmajor.debug;
import util;

namespace cmajor::cpp::ir {

BasicBlock::BasicBlock(int id_, const std::string& name_) : 
    id(id_), name(name_), included(false), isCleanupBlock(false), parent(nullptr), handlerBlock(nullptr), referenced(false), cleanupBlock(nullptr)
{
}

void BasicBlock::AddInstruction(Instruction* instruction)
{
    instructions.push_back(std::unique_ptr<Instruction>(instruction));
}

void BasicBlock::AddChild(BasicBlock* child)
{
    children.push_back(child);
}

void BasicBlock::SetParent(BasicBlock* parent_)
{
    if (parent_ != nullptr)
    {
        parent = parent_;
        parent->AddChild(this);
    }
}

void BasicBlock::Write(util::CodeFormatter& formatter, Function& function, Context& context, util::BinaryStreamWriter& writer, int32_t& numInsts, bool writeDebugInfoRecords)
{
    context.SetSourceSpan(cmajor::debug::SourceSpan()); 
    if (!Referenced()) return;
    BasicBlock* prevBB = context.GetCurrentBasicBlock();
    context.SetCurrentBasicBlock(this);
    int indent = formatter.IndentSize();
    bool indentDecremented = false;
    if (formatter.CurrentIndent() > 0)
    {
        formatter.DecIndent();
        indentDecremented = true;
    }
    if (!IsCleanupBlock())
    {
        formatter.Write(util::Format("__bb" + std::to_string(id) + ":", indent, util::FormatWidth::min));
    }
    bool first = true;
    std::string n = "[" + name + "] ";
    int ni = instructions.size();
    if (ni == 0)
    {
        formatter.WriteLine("// " + n);
        if (indentDecremented)
        {
            formatter.IncIndent();
        }
        formatter.WriteLine(";");
    }
    for (int i = 0; i < ni; ++i)
    {
        Instruction* inst = instructions[i].get();
        if (writeDebugInfoRecords)
        {
            inst->SetLineNumbers(formatter, context);
            inst->WriteDebugInfoRecord(writer, numInsts);
        }
        std::string flagsStr;
        if (inst->Flags() != 0)
        {
            flagsStr.append(", flags=[").append(cmajor::debug::InstructionFlagsStr(static_cast<cmajor::debug::InstructionFlags>(inst->Flags()))).append("]"); 
        }
        formatter.WriteLine("// " + n + inst->IrName() + " : source span=" + inst->GetSourceSpan().ToString() +
            ", line index=" + std::to_string(inst->CppLineIndex()) + ", scope=" + std::to_string(inst->ScopeId()) + flagsStr + ":"); 
        if (first)
        {
            if (indentDecremented)
            {
                formatter.IncIndent();
            }
            first = false;
        }
        inst->Write(formatter, function, context, writer, numInsts, writeDebugInfoRecords);
        if (!inst->NoSemicolon())
        {
            formatter.WriteLine(";");
        }
        n.clear();
    }
    if (cleanupBlock != nullptr)
    {
        if (!cleanupBlock->Included())
        {
            cleanupBlock->SetIncluded();
            cleanupBlock->Write(formatter, function, context, writer, numInsts, writeDebugInfoRecords);
        }
    }
    for (BasicBlock* child : children)
    {
        if (child == handlerBlock)
        {
            continue;
        }
        if (child == cleanupBlock)
        {
            continue;
        }
        if (!child->Included())
        {
            child->SetIncluded();
            formatter.WriteLine();
            child->Write(formatter, function, context, writer, numInsts, writeDebugInfoRecords);
        }
    }
    if (handlerBlock != nullptr)
    {
        if (!handlerBlock->Included())
        {
            handlerBlock->SetIncluded();
            formatter.WriteLine();
            handlerBlock->Write(formatter, function, context, writer, numInsts, writeDebugInfoRecords);
        }
    }
    context.SetCurrentBasicBlock(prevBB);
}

void BasicBlock::CollectReferencedBasicBlocks(std::set<BasicBlock*>& basicBlocks)
{
    for (const auto& inst : instructions)
    {
        inst->CollectReferencedBasicBlocks(this, basicBlocks);
    }
}

} // namespace cmajor::cpp::ir
