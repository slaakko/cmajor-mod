// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.writer;

namespace cmajor::ast {

AstWriter::AstWriter(const std::string& fileName_) : 
    fileStream(fileName_,util::OpenMode::binary | util::OpenMode::write), bufferedStream(fileStream), binaryStreamWriter(bufferedStream)
{
}

void AstWriter::Write(Node* node)
{
    binaryStreamWriter.Write(static_cast<uint8_t>(node->GetNodeType()));
    Write(node->GetSpan());
    node->Write(*this);
}

void AstWriter::Write(Specifiers specifiers)
{
    binaryStreamWriter.Write(static_cast<uint32_t>(specifiers));
}

void AstWriter::Write(const soul::ast::Span& span)
{
    binaryStreamWriter.Write(static_cast<int32_t>(span.pos));
    binaryStreamWriter.Write(static_cast<int32_t>(span.len));
}

void AstWriter::Write(const soul::ast::LineColLen& lineColLen)
{
    binaryStreamWriter.Write(static_cast<int32_t>(lineColLen.line));
    binaryStreamWriter.Write(static_cast<int32_t>(lineColLen.col));
    binaryStreamWriter.Write(static_cast<int32_t>(lineColLen.len));
}

} // namespace cmajor::ast
