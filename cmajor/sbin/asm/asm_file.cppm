// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.asm_file;

import cmajor.sbin.machine_x64;
import soul.ast.span;
import std.core;

export namespace cmajor::sbin::assembly {

namespace asm_file {}

class Visitor;

enum class SymbolKind : uint8_t
{
    public_, external
};

enum class Type : uint8_t
{
    none = 0u, byte_, word, dword, qword, oword, proc
};

enum class DataInstKind : uint8_t
{
    db, dw, dd, dq, real4, real8
};

enum class Operator : uint8_t
{
    plus, minus, times
};

enum class NodeKind : int
{
    symbolNode, labelNode, declarationNode, dataDefinitionNode, binaryExprNode, unaryExprNode, registerNode, contentExprNode, sizeExprNode, parenthesizedExprNode,
    hexNumberNode, realNode, integerNode, stringNode, instructionNode, functionDefinitionNode, macroDefinitionNode, asmFileNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::Span& span_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const soul::ast::Span& Span() const { return span; }
    virtual void Accept(Visitor& visitor) = 0;
private:
    NodeKind kind;
    soul::ast::Span span;
};

class SymbolNode : public Node
{
public:
    SymbolNode(const soul::ast::Span& span_, const std::string& name_);
    const std::string& Name() const { return name; }
    void Accept(Visitor& visitor) override;
private:
    std::string name;
};

class LabelNode : public Node
{
public:
    LabelNode(const soul::ast::Span& span_, const std::string& label_);
    const std::string& Label() const { return label; }
    void Accept(Visitor& visitor) override;
private:
    std::string label;
};

class DeclarationNode : public Node
{
public:
    DeclarationNode(const soul::ast::Span& span_, SymbolKind kind_, SymbolNode* symbol_, Type type_);
    SymbolKind GetSymbolKind() const { return kind; }
    SymbolNode* Symbol() const { return symbol.get(); }
    Type GetType() const { return type; }
    void Accept(Visitor& visitor) override;
private:
    SymbolKind kind;
    std::unique_ptr<SymbolNode> symbol;
    Type type;
};

class InstructionBaseNode : public Node
{
public:
    InstructionBaseNode(NodeKind kind_, const soul::ast::Span& span_);
    void SetLabel(LabelNode* label_) { label.reset(label_); }
    LabelNode* Label() const { return label.get(); }
    void AddOperand(Node* operand);
    const std::vector<std::unique_ptr<Node>>& Operands() const { return operands; }
private:
    std::unique_ptr<LabelNode> label;
    std::vector<std::unique_ptr<Node>> operands;
};

class DataDefinitionNode : public InstructionBaseNode
{
public:
    DataDefinitionNode(const soul::ast::Span& span_, LabelNode* label_, DataInstKind dataInst_);
    DataInstKind GetDataInstKind() const { return kind; }
    void Accept(Visitor& visitor) override;
private:
    DataInstKind kind;
};

class BinaryExprNode : public Node
{
public:
    BinaryExprNode(const soul::ast::Span& span_, Node* left_, Operator op_, Node* right_);
    Node* Left() const { return left.get(); }
    Operator Op() const { return op; }
    Node* Right() const { return right.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> left;
    Operator op;
    std::unique_ptr<Node> right;
};

class UnaryExprNode : public Node
{
public:
    UnaryExprNode(const soul::ast::Span& span_, Operator op_, Node* operand_);
    Operator Op() const { return op; }
    Node* Operand() const { return operand.get(); }
    void Accept(Visitor& visitor) override;
private:
    Operator op;
    std::unique_ptr<Node> operand;
};

class RegisterNode : public Node
{
public:
    RegisterNode(const soul::ast::Span& span_, cmajor::sbin::machine_x64::Register reg_);
    cmajor::sbin::machine_x64::Register Reg() const { return reg; }
    void Accept(Visitor& visitor) override;
private:
    cmajor::sbin::machine_x64::Register reg;
};

class ContentExprNode : public Node
{
public:
    ContentExprNode(const soul::ast::Span& span_, Node* operand_);
    Node* Operand() const { return operand.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> operand;
};

class SizeExprNode : public Node
{
public:
    SizeExprNode(const soul::ast::Span& span_, Type sizePrefix_, Node* operand_);
    Type SizePrefix() const { return sizePrefix; }
    Node* Operand() const { return operand.get(); }
    void Accept(Visitor& visitor) override;
private:
    Type sizePrefix;
    std::unique_ptr<Node> operand;
};

class ParenthesizedExprNode : public Node
{
public:
    ParenthesizedExprNode(const soul::ast::Span& span_, Node* operand_);
    Node* Operand() const { return operand.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> operand;
};

class HexNumberNode : public Node
{
public:
    HexNumberNode(const soul::ast::Span& span_, uint64_t value_);
    uint64_t Value() const { return value; }
    void Accept(Visitor& visitor) override;
private:
    uint64_t value;
};

class RealNode : public Node
{
public:
    RealNode(const soul::ast::Span& span_, double value_);
    double Value() const { return value; }
    void Accept(Visitor& visitor) override;
private:
    double value;
};

class IntegerNode : public Node
{
public:
    IntegerNode(const soul::ast::Span& span_, uint64_t value_);
    uint64_t Value() const { return value; }
    void Accept(Visitor& visitor) override;
private:
    uint64_t value;
};

class StringNode : public Node
{
public:
    StringNode(const soul::ast::Span& span_, const std::string& value_);
    const std::string& Value() const { return value; }
    void Accept(Visitor& visitor) override;
private:
    std::string value;
};

class InstructionNode : public InstructionBaseNode
{
public:
    InstructionNode(const soul::ast::Span& span_, cmajor::sbin::machine_x64::OpCode opCode_);
    cmajor::sbin::machine_x64::OpCode OpCode() const { return opCode; }
    void Accept(Visitor& visitor) override;
private:
    cmajor::sbin::machine_x64::OpCode opCode;
};

class DefinitionNode : public Node
{
public:
    DefinitionNode(NodeKind kind_, const soul::ast::Span& span_);
};

class FunctionDefinitionNode : public DefinitionNode
{
public:    
    FunctionDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_);
    SymbolNode* Symbol() const { return symbol.get(); }
    SymbolNode* EndpSymbol() const { return endpSymbol.get(); }
    void SetEndpSymbol(SymbolNode* endpSymbol_);
    void AddInstruction(InstructionBaseNode* inst);
    const std::vector<std::unique_ptr<InstructionBaseNode>>& Instructions() const { return instructions; }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SymbolNode> symbol;
    std::unique_ptr<SymbolNode> endpSymbol;
    std::vector<std::unique_ptr<InstructionBaseNode>> instructions;
};

class MacroDefinitionNode : public DefinitionNode
{
public: 
    MacroDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_, Node* expr_);
    SymbolNode* Symbol() const { return symbol.get(); }
    Node* Expr() const { return expr.get(); }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SymbolNode> symbol;
    std::unique_ptr<Node> expr;
};

class AsmFileNode : public Node
{
public:
    AsmFileNode(const soul::ast::Span& span_);
    void SetFileIndex(int32_t fileIndex_) { fileIndex = fileIndex_; }
    int32_t FileIndex() const { return fileIndex; }
    void AddDeclaration(DeclarationNode* declaration);
    const std::vector<std::unique_ptr<DeclarationNode>>& Declarations() const { return declarations; }
    void AddDataDefinition(DataDefinitionNode* dataDefinition);
    const std::vector<std::unique_ptr<DataDefinitionNode>>& DataDefinitions() const { return dataDefinitions; }
    void AddDefinition(DefinitionNode* definition);
    const std::vector<std::unique_ptr<DefinitionNode>>& Definitions() const { return definitions; }
    void Accept(Visitor& visitor) override;
private:
    int32_t fileIndex;
    std::vector<std::unique_ptr<DeclarationNode>> declarations;
    std::vector<std::unique_ptr<DataDefinitionNode>> dataDefinitions;
    std::vector<std::unique_ptr<DefinitionNode>> definitions;
};

} // namespace cmajor::sbin::assembly
