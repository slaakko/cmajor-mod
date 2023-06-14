// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.ir.basic.block;

import util;

namespace cmajor::systemx::ir {

BasicBlock::BasicBlock(int id_) : id(id_)
{
}

void BasicBlock::AddInstruction(Instruction* instruction)
{
    instructions.push_back(std::unique_ptr<Instruction>(instruction));
}

void BasicBlock::Write(util::CodeFormatter& formatter, Function& function, Context& context)
{
    int indent = formatter.IndentSize();
    bool indentDecremented = false;
    if (formatter.CurrentIndent() > 0)
    {
        formatter.DecIndent();
        indentDecremented = true;
    }
    formatter.Write(util::Format("@" + std::to_string(id), indent, util::FormatWidth::min));
    bool first = true;
    for (const auto& inst : instructions)
    {
        inst->Write(formatter, function, context);
        formatter.WriteLine();
        if (first)
        {
            if (indentDecremented)
            {
                formatter.IncIndent();
            }
            first = false;
        }
    }
}

} // namespace cmajor::systemx::ir
