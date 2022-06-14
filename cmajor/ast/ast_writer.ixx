
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================
export module cmajor.ast.writer;

/*
#include <sngcm/ast/AstApi.hpp>
#include <sngcm/ast/Specifier.hpp>
#include <soulng/util/BinaryWriter.hpp>
#include <soulng/lexer/Span.hpp>
#include <soulng/lexer/Lexer.hpp>
*/
import std.core;
import util;
import cmajor.ast.specifier;
import soul.ast.source.pos;
import cmajor.ast.node;

export namespace cmajor::ast {

//class Node;

class AstWriter
{
public:
    AstWriter(const std::string& fileName_);
    util::BinaryStreamWriter& GetBinaryStreamWriter() { return binaryStreamWriter; }
    void Write(Node* node);
    void Write(Specifiers specifiers);
    void Write(const soul::ast::SourcePos& sourcePos);
    //void SetLexers(std::vector<soulng::lexer::Lexer*>* lexers_); TODO
    //void SetSpanConversionModuleId(const boost::uuids::uuid& spanConversionModuleId_); TODO
    //const boost::uuids::uuid& SpanConversionModuleId() const { return spanConversionModuleId; } TODO
private:
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamWriter binaryStreamWriter;
    //std::vector<soulng::lexer::Lexer*>* lexers;
    //boost::uuids::uuid spanConversionModuleId;
};

} // namespace cmajor::ast

