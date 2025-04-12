// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.instruction;

import cmajor.systemx.assembler.constant;
import cmajor.systemx.assembler.expression;
import cmajor.systemx.assembler.visitor;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::assembler {

OperandList::OperandList(const soul::ast::SourcePos& sourcePos_) :
    Node(NodeKind::operandListNode, sourcePos_)
{
}

void OperandList::AddOperand(Node* operand)
{
#ifdef THREAD_ID_CHECK
    if (operand->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    operand->SetOwner(this);
    operands.push_back(std::unique_ptr<Node>(operand));
}

void OperandList::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void OperandList::Write(util::CodeFormatter& formatter)
{
    int n = OperandCount();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            formatter.Write(",");
        }
        Node* operand = GetOperand(i);
#ifdef THREAD_ID_CHECK
        if (operand->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        operand->Write(formatter);
    }
}

Instruction::Instruction(const soul::ast::SourcePos& sourcePos_, Symbol* label_, OpCode* opCode_) :
    Node(NodeKind::instructionNode, sourcePos_), label(label_), opCode(opCode_), operandList(new OperandList(sourcePos_))
{
}

Instruction::Instruction(int opCode_) : 
    Node(NodeKind::instructionNode, soul::ast::SourcePos()), label(), opCode(new OpCode(opCode_)), operandList(new OperandList(soul::ast::SourcePos()))
{
}

void Instruction::SetLabel(Symbol* label_)
{
#ifdef THREAD_ID_CHECK
    if (label_->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    label.reset(label_);
}

Symbol* Instruction::Label() const
{
    if (label)
    {
#ifdef THREAD_ID_CHECK
        if (label->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
    }
    return label.get();
}

void Instruction::MakeImmediate()
{
    opCode->MakeImmediate();
}

void Instruction::AddOperand(Node* operand)
{
#ifdef THREAD_ID_CHECK
    if (operand->CreatorThreadId() != CreatorThreadId() || operand->CreatorThreadId() != operandList->CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    operandList->AddOperand(operand);
}

OperandList* Instruction::GetOperandList() const
{
#ifdef THREAD_ID_CHECK
    if (CreatorThreadId() != operandList->CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return operandList.get();
}

void Instruction::SetOperands(std::vector<cmajor::systemx::object::Value>&& operands_)
{
    operands = std::move(operands_);
}

void Instruction::SetComment(const std::string& comment_)
{
    comment = comment_;
}

void Instruction::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void Instruction::Write(util::CodeFormatter& formatter)
{
    bool indentDecremented = false;
    if (label)
    {
        formatter.DecIndent();
        formatter.Write(util::Format(label->Name(), formatter.IndentSize() - 1, util::FormatWidth::min) + " ");
        indentDecremented = true;
    }
    opCode->Write(formatter);
    if (operandList->OperandCount() > 0)
    {
        formatter.Write(" ");
        operandList->Write(formatter);
    }
    if (!comment.empty())
    {
        formatter.Write(" // ");
        formatter.Write(comment);
    }
    formatter.WriteLine();
    if (indentDecremented)
    {
        formatter.IncIndent();
    }
}

Node* MakeLocalRegOperand(uint8_t localRegNumber)
{
    return new UnaryExpression(soul::ast::SourcePos(), Operator::reg, new DecimalConstant(soul::ast::SourcePos(), localRegNumber));
}

Node* MakeGlobalRegOperand(uint8_t globalRegNumber)
{
    return new GlobalSymbol(soul::ast::SourcePos(), cmajor::systemx::machine::GetGlobalRegName(globalRegNumber));
}

} // namespace cmajor::systemx::assembler
