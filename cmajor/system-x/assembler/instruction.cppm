// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.instruction;

import cmajor.systemx.assembler.node;
import cmajor.systemx.assembler.symbol;
import cmajor.systemx.assembler.opcode;
import cmajor.systemx.object;
import std.core;

export namespace cmajor::systemx::assembler {

class OperandList : public Node
{
public:
    OperandList(const soul::ast::SourcePos& sourcePos_);
    OperandList(const OperandList&) = delete;
    OperandList& operator=(const OperandList&) = delete;
    int OperandCount() const { return operands.size(); }
    Node* GetOperand(int index) const { return operands[index].get(); }
    void AddOperand(Node* operand);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<Node>> operands;
};

class Instruction : public Node
{
public:
    Instruction(const soul::ast::SourcePos& sourcePos_, Symbol* label_, OpCode* opCode_);
    Instruction(int opCode_);
    Instruction(const Instruction&) = delete;
    Instruction& operator=(const Instruction&) = delete;
    Symbol* Label() const { return label.get(); }
    void SetLabel(Symbol* label_);
    OpCode* GetOpCode() const { return opCode.get(); }
    void MakeImmediate();
    OperandList* GetOperandList() const { return operandList.get(); }
    void AddOperand(Node* operand);
    void SetOperands(std::vector<cmajor::systemx::object::Value>&& operands_);
    const std::vector<cmajor::systemx::object::Value>& Operands() const { return operands; }
    void SetComment(const std::string& comment_);
    const std::string& Comment() const { return comment; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
private:
    std::unique_ptr<Symbol> label;
    std::unique_ptr<OpCode> opCode;
    std::unique_ptr<OperandList> operandList;
    std::string comment;
    std::vector<cmajor::systemx::object::Value> operands;
};

Node* MakeLocalRegOperand(uint8_t localRegNumber);
Node* MakeGlobalRegOperand(uint8_t globalRegNumber);

} // namespace cmsx::assembler
