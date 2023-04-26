// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.writer;

namespace cmajor::ast {

    AstWriter::AstWriter(const std::string& fileName_) : fileStream(fileName_,util::OpenMode::binary| util::OpenMode::write), bufferedStream(fileStream), binaryStreamWriter(bufferedStream)
{
}

void AstWriter::Write(Node* node)
{
    binaryStreamWriter.Write(static_cast<uint8_t>(node->GetNodeType()));
    binaryStreamWriter.Write(node->ModuleId());
    node->Write(*this);
}

void AstWriter::Write(Specifiers specifiers)
{
    binaryStreamWriter.Write(static_cast<uint32_t>(specifiers));
}

void AstWriter::Write(const soul::ast::SourcePos& sourcePos)
{
    binaryStreamWriter.WriteULEB128UInt(static_cast<uint32_t>(sourcePos.file));
    binaryStreamWriter.WriteULEB128UInt(static_cast<uint32_t>(sourcePos.line));
    binaryStreamWriter.WriteULEB128UInt(static_cast<uint32_t>(sourcePos.col));
}
/*
void AstWriter::SetLexers(std::vector<soulng::lexer::Lexer*>* lexers_)
{
    lexers = lexers_;
}

void AstWriter::SetSpanConversionModuleId(const util::uuid& spanConversionModuleId_)
{
    spanConversionModuleId = spanConversionModuleId_;
}
*/
} // namespace cmajor::ast
