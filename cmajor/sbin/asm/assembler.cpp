// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.assembly.assembler;

import cmajor.sbin.assembly.asm_file_parser;
import util;

namespace cmajor::sbin::assembly {

Assembler::Assembler(const std::string& asmFilePath_, bool verbose_) : 
    verbose(verbose), fileMap(), asmFilePath(asmFilePath_), objFilePath(util::Path::ChangeExtension(asmFilePath, ".obj")), 
    objectFile(new cmajor::sbin::coff::CoffObjectFile())
{
}

void Assembler::Assemble()
{
    asmFile = ParseAsmFile(asmFilePath, verbose, fileMap);
}

void Assembler::ThrowError(const std::string& errorMessage, const soul::ast::Span& span)
{
    std::string msg;
    msg.append(errorMessage);
    if (asmFile)
    {
        const std::vector<int>* lineStartIndeces = fileMap.LineStartIndeces(asmFile->FileIndex());
        if (lineStartIndeces)
        {
            soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
            std::u32string line = fileMap.GetFileLine(asmFile->FileIndex(), lineColLen.line);
            msg.append("\n").append(util::ToUtf8(line)).append("\n");
            msg.append(lineColLen.col - 1, ' ').append(lineColLen.len, '^').append("\n");
        }
    }
    throw std::runtime_error(msg);
}

void Assembler::Visit(SymbolNode& symbolNode)
{
}

void Assembler::Visit(LabelNode& labelNode) 
{
}

void Assembler::Visit(DeclarationNode& declarationNode)
{
}

void Assembler::Visit(DataDefinitionNode& dataDefinitionNode)
{
}

void Assembler::Visit(BinaryExprNode& binaryExprNode)
{
}

void Assembler::Visit(UnaryExprNode& unaryExprNode)
{
}

void Assembler::Visit(RegisterNode& registerNode)
{
}

void Assembler::Visit(ContentExprNode& contentExprNode)
{
}

void Assembler::Visit(SizeExprNode& sizeExprNode)
{
}

void Assembler::Visit(ParenthesizedExprNode& parenthesizedExprNode)
{
}

void Assembler::Visit(HexNumberNode& hexNumberNode)
{
}

void Assembler::Visit(RealNode& realNode)
{
}

void Assembler::Visit(IntegerNode& integerNode)
{
}

void Assembler::Visit(StringNode& stringNode)
{
}

void Assembler::Visit(InstructionNode& instructionNode)
{
}

void Assembler::Visit(FunctionDefinitionNode& functionDefinitionNode)
{
}

void Assembler::Visit(MacroDefinitionNode& macroDefinitionNode)
{
}

void Assembler::Visit(AsmFileNode& asmFileNode)
{
}

} // namespace cmajor::sbin::assembly
