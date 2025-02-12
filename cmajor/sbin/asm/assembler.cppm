// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.assembler;

import cmajor.sbin.assembly.visitor;
import cmajor.sbin.assembly.asm_file;
import cmajor.sbin.coff;
import soul.ast.span;
import soul.lexer;
import std.core;

export namespace cmajor::sbin::assembly {

class Assembler : public Visitor
{
public:
    Assembler(const std::string& asmFilePath_, bool verbose_);
    void Assemble();
    void ThrowError(const std::string& errorMessage, const soul::ast::Span& span);
    void Visit(SymbolNode& symbolNode) override;
    void Visit(LabelNode& labelNode) override;
    void Visit(DeclarationNode& declarationNode) override;
    void Visit(DataDefinitionNode& dataDefinitionNode) override;
    void Visit(BinaryExprNode& binaryExprNode) override;
    void Visit(UnaryExprNode& unaryExprNode) override;
    void Visit(RegisterNode& registerNode) override;
    void Visit(ContentExprNode& contentExprNode) override;
    void Visit(SizeExprNode& sizeExprNode) override;
    void Visit(ParenthesizedExprNode& parenthesizedExprNode) override;
    void Visit(HexNumberNode& hexNumberNode) override;
    void Visit(RealNode& realNode) override;
    void Visit(IntegerNode& integerNode) override;
    void Visit(StringNode& stringNode) override;
    void Visit(InstructionNode& instructionNode) override;
    void Visit(FunctionDefinitionNode& functionDefinitionNode) override;
    void Visit(MacroDefinitionNode& macroDefinitionNode) override;
    void Visit(AsmFileNode& asmFileNode) override;
private:
    bool verbose;
    soul::lexer::FileMap fileMap;
    std::string asmFilePath;
    std::string objFilePath;
    std::unique_ptr<AsmFileNode> asmFile;
    std::unique_ptr<cmajor::sbin::coff::CoffObjectFile> objectFile;
};

} // namespace cmajor::sbin::assembly
