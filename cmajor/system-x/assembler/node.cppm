// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.node;

import soul.ast.source.pos;
import util;

export namespace cmajor::systemx::assembler {

class Visitor;

enum class NodeKind : int
{
    decimalConstantNode, hexConstantNode, byteConstantNode, wydeConstantNode, tetraConstantNode, characterConstantNode, stringConstantNode, clsIdConstantNode,
    localSymbolNode, globalSymbolNode, atNode,
    unaryExprNode, binaryExprNode, parenExprNode, opCodeNode, operandListNode, instructionNode
};

class Node
{
public:
    Node(NodeKind kind_, const soul::ast::SourcePos& sourcePos_);
    virtual ~Node();
    NodeKind Kind() const { return kind; }
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Write(util::CodeFormatter& formatter);
    void SetOwner(void* owner_);
private:
    NodeKind kind;
    soul::ast::SourcePos sourcePos;
    void* owner;
};

} // namespace cmajor::systemx::assembler
