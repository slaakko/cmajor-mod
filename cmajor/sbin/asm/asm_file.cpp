// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.asm_file;

import cmajor.sbin.assembly.visitor;

namespace cmajor::sbin::assembly {

Node::Node(NodeKind kind_, const soul::ast::Span& span_) : kind(kind_), span(span_)
{
}

Node::~Node()
{
}

SymbolNode::SymbolNode(const soul::ast::Span& span_, const std::string& name_) : Node(NodeKind::symbolNode, span_), name(name_)
{
}

void SymbolNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LabelNode::LabelNode(const soul::ast::Span& span_, const std::string& label_) : Node(NodeKind::labelNode, span_), label(label_)
{
}

void LabelNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DeclarationNode::DeclarationNode(const soul::ast::Span& span_, SymbolKind kind_, SymbolNode* symbol_, Type type_) :
    Node(NodeKind::declarationNode, span_), kind(kind_), symbol(symbol_), type(type_)
{
}

void DeclarationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InstructionBaseNode::InstructionBaseNode(NodeKind kind_, const soul::ast::Span& span_) : Node(kind_, span_)
{
}

void InstructionBaseNode::AddOperand(Node* operand)
{
    operands.push_back(std::unique_ptr<Node>(operand));
}

DataDefinitionNode::DataDefinitionNode(const soul::ast::Span& span_, LabelNode* label_, DataInstKind kind_) : 
    InstructionBaseNode(NodeKind::dataDefinitionNode, span_), kind(kind_)
{
    SetLabel(label_);
}

void DataDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BinaryExprNode::BinaryExprNode(const soul::ast::Span& span_, Node* left_, Operator op_, Node* right_) : 
    Node(NodeKind::binaryExprNode, span_), left(left_), op(op_), right(right_)
{
}

void BinaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UnaryExprNode::UnaryExprNode(const soul::ast::Span& span_, Operator op_, Node* operand_) :
    Node(NodeKind::unaryExprNode, span_), op(op_), operand(operand_)
{
}

void UnaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RegisterNode::RegisterNode(const soul::ast::Span& span_, cmajor::sbin::machine_x64::Register reg_) : Node(NodeKind::registerNode, span_), reg(reg_)
{
}

void RegisterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ContentExprNode::ContentExprNode(const soul::ast::Span& span_, Node* operand_) : Node(NodeKind::contentExprNode, span_), operand(operand_)
{
}

void ContentExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SizeExprNode::SizeExprNode(const soul::ast::Span& span_, Type sizePrefix_, Node* operand_) : Node(NodeKind::sizeExprNode, span_), sizePrefix(sizePrefix_), operand(operand_)
{
}

void SizeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParenthesizedExprNode::ParenthesizedExprNode(const soul::ast::Span& span_, Node* operand_) : Node(NodeKind::parenthesizedExprNode, span_), operand(operand_)
{
}

void ParenthesizedExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

HexNumberNode::HexNumberNode(const soul::ast::Span& span_, uint64_t value_) : Node(NodeKind::hexNumberNode, span_), value(value_)
{
}

void HexNumberNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RealNode::RealNode(const soul::ast::Span& span_, double value_) : Node(NodeKind::realNode, span_), value(value_)
{
}

void RealNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntegerNode::IntegerNode(const soul::ast::Span& span_, uint64_t value_) : Node(NodeKind::integerNode, span_), value(value_)
{
}

void IntegerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StringNode::StringNode(const soul::ast::Span& span_, const std::string& value_) : Node(NodeKind::stringNode, span_), value(value_)
{
}

void StringNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InstructionNode::InstructionNode(const soul::ast::Span& span_, cmajor::sbin::machine_x64::OpCode opCode_) :
    InstructionBaseNode(NodeKind::instructionNode, span_), opCode(opCode_)
{
}

void InstructionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DefinitionNode::DefinitionNode(NodeKind kind_, const soul::ast::Span& span_) : Node(kind_, span_)
{
}

FunctionDefinitionNode::FunctionDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_) : DefinitionNode(NodeKind::functionDefinitionNode, span_), symbol(symbol_)
{
}

void FunctionDefinitionNode::SetEndpSymbol(SymbolNode* endpSymbol_)
{
    endpSymbol.reset(endpSymbol_);
}

void FunctionDefinitionNode::AddInstruction(InstructionBaseNode* inst)
{
    instructions.push_back(std::unique_ptr<InstructionBaseNode>(inst));
}

void FunctionDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MacroDefinitionNode::MacroDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_, Node* expr_) :
    DefinitionNode(NodeKind::macroDefinitionNode, span_), symbol(symbol_), expr(expr_)
{
}

void MacroDefinitionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AsmFileNode::AsmFileNode(const soul::ast::Span& span_) : Node(NodeKind::asmFileNode, span_), fileIndex(0)
{
}

void AsmFileNode::AddDeclaration(DeclarationNode* declaration)
{
    declarations.push_back(std::unique_ptr<DeclarationNode>(declaration));
}

void AsmFileNode::AddDataDefinition(DataDefinitionNode* dataDefinition)
{
    dataDefinitions.push_back(std::unique_ptr<DataDefinitionNode>(dataDefinition));
}

void AsmFileNode::AddDefinition(DefinitionNode* definition)
{
    definitions.push_back(std::unique_ptr<DefinitionNode>(definition));
}

void AsmFileNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::sbin::assembly
