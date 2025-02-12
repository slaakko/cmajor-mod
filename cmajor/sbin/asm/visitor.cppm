// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.visitor;

import std.core;

export namespace cmajor::sbin::assembly {

class SymbolNode;
class LabelNode;
class DeclarationNode;
class DataDefinitionNode;
class BinaryExprNode;
class UnaryExprNode;
class RegisterNode;
class ContentExprNode;
class SizeExprNode;
class ParenthesizedExprNode;
class HexNumberNode;
class RealNode;
class IntegerNode;
class StringNode;
class InstructionNode;
class FunctionDefinitionNode;
class MacroDefinitionNode;
class AsmFileNode;

class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(SymbolNode& symbolNode) {}
    virtual void Visit(LabelNode& labelNode) {}
    virtual void Visit(DeclarationNode& declarationNode) {}
    virtual void Visit(DataDefinitionNode& dataDefinitionNode) {}
    virtual void Visit(BinaryExprNode& binaryExprNode) {}
    virtual void Visit(UnaryExprNode& unaryExprNode) {}
    virtual void Visit(RegisterNode& registerNode) {}
    virtual void Visit(ContentExprNode& contentExprNode) {}
    virtual void Visit(SizeExprNode& sizeExprNode) {}
    virtual void Visit(ParenthesizedExprNode& parenthesizedExprNode) {}
    virtual void Visit(HexNumberNode& hexNumberNode) {}
    virtual void Visit(RealNode& realNode) {}
    virtual void Visit(IntegerNode& integerNode) {}
    virtual void Visit(StringNode& stringNode) {}
    virtual void Visit(InstructionNode& instructionNode) {}
    virtual void Visit(FunctionDefinitionNode& functionDefinitionNode) {}
    virtual void Visit(MacroDefinitionNode& macroDefinitionNode) {}
    virtual void Visit(AsmFileNode& asmFileNode) {}
};

} // namespace cmajor::sbin::assembly
