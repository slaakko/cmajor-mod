
// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================
export module cmajor.ast.writer;

import std.core;
import util;
import cmajor.ast.specifier;
import soul.ast.source.pos;
import cmajor.ast.node;

export namespace cmajor::ast {

class AstWriter
{
public:
    AstWriter(const std::string& fileName_);
    util::BinaryStreamWriter& GetBinaryStreamWriter() { return binaryStreamWriter; }
    void Write(Node* node);
    void Write(Specifiers specifiers);
    void Write(const soul::ast::SourcePos& sourcePos);
    //void SetLexers(std::vector<soulng::lexer::Lexer*>* lexers_); TODO
    void SetSourcePosConversionModuleId(const util::uuid& sourcePosConversionModuleId_); 
    const util::uuid& SourcePosConversionModuleId() const { return sourcePosConversionModuleId; } 
private:
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamWriter binaryStreamWriter;
    //std::vector<soulng::lexer::Lexer*>* lexers;
    util::uuid sourcePosConversionModuleId;
};

} // namespace cmajor::ast
