// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.asm_file;

import cmajor.sbin.coff;
import cmajor.sbin.machine_x64;
import soul.ast.span;
import std.core;

export namespace cmajor::sbin::assembly {

namespace asm_file {}

class Visitor;
class Node;

enum class SymbolKind : uint8_t
{
    none = 0u, public_ = 1u << 0u, external = 1u << 1u, linkOnce = 1u << 2u, code = 1u << 3u, data = 1u << 4u, macro = 1u << 5u, label = 1u << 6u
};

constexpr SymbolKind operator|(SymbolKind left, SymbolKind right)
{
    return SymbolKind(uint8_t(left) | uint8_t(right));
}

constexpr SymbolKind operator&(SymbolKind left, SymbolKind right)
{
    return SymbolKind(uint8_t(left) & uint8_t(right));
}

constexpr SymbolKind operator~(SymbolKind kind)
{
    return SymbolKind(~uint8_t(kind));
}

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

class Symbol
{
public:
    Symbol(SymbolKind kind_, Type type_, const std::string& name_);
    SymbolKind Kind() const { return kind; }
    Type GetType() const { return type; }
    void SetType(Type type_) { type = type_; }
    const std::string& Name() const { return name; }
    bool IsKind(SymbolKind kindFlag) const { return (kind & kindFlag) != SymbolKind::none; }
    void SetKindFlag(SymbolKind kindFlag) { kind = kind | kindFlag; }
    void ResetKindFlag(SymbolKind kindFlag) { kind = kind & ~kindFlag; }
    cmajor::sbin::coff::SymbolTableEntry* Entry() const { return entry; }
    void SetEntry(cmajor::sbin::coff::SymbolTableEntry* entry_) { entry = entry_; }
    cmajor::sbin::coff::SymbolTableEntry* SectionDefinitionEntry() const { return sectionDefinitionEntry; }
    void SetSectionDefinitionEntry(cmajor::sbin::coff::SymbolTableEntry* sectionDefinitionEntry_) { sectionDefinitionEntry = sectionDefinitionEntry_; }
    cmajor::sbin::coff::Section* Section() const { return section; }
    void SetSection(cmajor::sbin::coff::Section* section_) { section = section_; }
    Node* GetNode() const { return node; }
    void SetNode(Node* node_) { node = node_; }
    bool HasValue() const { return hasValue; }
    uint64_t Value() const { return value; }
    void SetValue(uint64_t value_);
    Symbol* GetSubSymbol(const std::string& name) const;
    void AddSubSymbol(Symbol* subSymbol);
    const std::vector<std::unique_ptr<Symbol>>& SubSymbols() const { return subSymbols; }
private:
    SymbolKind kind;
    Type type;
    std::string name;
    cmajor::sbin::coff::SymbolTableEntry* entry;
    cmajor::sbin::coff::SymbolTableEntry* sectionDefinitionEntry;
    cmajor::sbin::coff::Section* section;
    Node* node;
    bool hasValue;
    uint64_t value;
    std::map<std::string, Symbol*> subSymbolMap;
    std::vector<std::unique_ptr<Symbol>> subSymbols;
};

struct JmpPos
{
    JmpPos(int64_t pos_, Symbol* symbol_, const soul::ast::Span& span_) : pos(pos_), symbol(symbol_), span(span_) {}
    int64_t pos;
    Symbol* symbol;
    soul::ast::Span span;
};

struct SymbolDifference
{
    SymbolDifference(int64_t pos_, Symbol* symbol0_, Symbol* symbol1_, const soul::ast::Span& span_) : pos(pos_), symbol0(symbol0_), symbol1(symbol1_), span(span_) {}
    int64_t pos;
    Symbol* symbol0;
    Symbol* symbol1;
    soul::ast::Span span;
};

enum class NodeKind : int
{
    none, symbolNode, labelNode, declarationNode, dataDefinitionNode, binaryExprNode, unaryExprNode, registerNode, contentExprNode, sizeExprNode, parenthesizedExprNode,
    hexNumberNode, realNode, integerNode, stringNode, instructionNode, functionDefinitionNode, macroDefinitionNode, asmFileNode, immediateNode, displacementNode, sibNode,
    symbolDifferenceNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::Span& span_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    bool IsFunctionDefinitionNode() const { return kind == NodeKind::functionDefinitionNode; }
    bool IsMacroDefinitionNode() const { return kind == NodeKind::macroDefinitionNode; }
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
    DefinitionNode(NodeKind kind_, const soul::ast::Span& span_, SymbolNode* symbol_);
    SymbolNode* Symbol() const { return symbol.get(); }
private:
    std::unique_ptr<SymbolNode> symbol;
};

class FunctionDefinitionNode : public DefinitionNode
{
public:    
    FunctionDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_);
    SymbolNode* EndpSymbol() const { return endpSymbol.get(); }
    void SetEndpSymbol(SymbolNode* endpSymbol_);
    void AddInstruction(InstructionBaseNode* inst);
    const std::vector<std::unique_ptr<InstructionBaseNode>>& Instructions() const { return instructions; }
    void Accept(Visitor& visitor) override;
private:
    
    std::unique_ptr<SymbolNode> endpSymbol;
    std::vector<std::unique_ptr<InstructionBaseNode>> instructions;
};

class MacroDefinitionNode : public DefinitionNode
{
public: 
    MacroDefinitionNode(const soul::ast::Span& span_, SymbolNode* symbol_, Node* expr_);
    Node* Expr() const { return expr.get(); }
    void Accept(Visitor& visitor) override;
private:
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
