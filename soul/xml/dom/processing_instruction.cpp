// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.processing.instruction;

import soul.xml.visitor;

namespace soul::xml {

ProcessingInstruction::ProcessingInstruction(const soul::ast::SourcePos& sourcePos_) : 
    Node(NodeKind::processingInstructionNode, sourcePos_, "processing_instruction")
{
}

ProcessingInstruction::ProcessingInstruction(const soul::ast::SourcePos& sourcePos_, const std::string& target_, const std::string& data_) : 
    Node(NodeKind::processingInstructionNode, sourcePos_, "processing_instruction"), target(target_), data(data_)
{
}

void ProcessingInstruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ProcessingInstruction::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine("<?" + target + " " + data + "?>");
}

ProcessingInstruction* MakeProcessingInstruction(const std::string& target, const std::string& data)
{
    return new ProcessingInstruction(soul::ast::SourcePos(), target, data);
}

} // namespace soul::xml
