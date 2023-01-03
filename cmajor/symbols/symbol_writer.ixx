// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.symbol.writer;

import cmajor.ast.writer;
import util;
import std.core;

export namespace cmajor::symbols {

class Symbol;

class SymbolWriter
{
public:
    SymbolWriter(const std::string& fileName_);
    cmajor::ast::AstWriter& GetAstWriter() { return astWriter; }
    util::BinaryStreamWriter& GetBinaryStreamWriter() { return astWriter.GetBinaryStreamWriter(); }
    void Write(Symbol* symbol);
    //void SetLexers(std::vector<soulng::lexer::Lexer*>* lexers_); TODO
    void SetSpanConversionModuleId(const util::uuid& spanConversionModuleId_);
private:
    cmajor::ast::AstWriter astWriter;
    util::uuid spanConversionModuleId;
    //std::vector<soulng::lexer::Lexer*>* lexers;
};

} // namespace cmajor::symbols
