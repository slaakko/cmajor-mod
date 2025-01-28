
// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================
export module cmajor.ast.writer;

import std.core;
import util;
import cmajor.ast.specifier;
import soul.ast.span;
import cmajor.ast.node;

export namespace cmajor::ast {

class AstWriter
{
public:
    AstWriter(const std::string& fileName_);
    util::BinaryStreamWriter& GetBinaryStreamWriter() { return binaryStreamWriter; }
    void Write(Node* node);
    void Write(Specifiers specifiers);
    void Write(const soul::ast::Span& span);
    void Write(const soul::ast::LineColLen& lineColLen);
private:
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamWriter binaryStreamWriter;
};

} // namespace cmajor::ast

