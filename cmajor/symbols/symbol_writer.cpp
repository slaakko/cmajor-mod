// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.writer;

import std.core;
import cmajor.symbols.symbol;
/*
#include <cmajor/symbols/SymbolWriter.hpp>
#include <cmajor/symbols/ModuleCache.hpp>
#include <cmajor/symbols/Symbol.hpp>
*/
namespace cmajor::symbols {

SymbolWriter::SymbolWriter(const std::string& fileName_) : astWriter(fileName_), spanConversionModuleId(util::nil_uuid())//, lexers(nullptr) TODO
{
}

void SymbolWriter::Write(Symbol* symbol)
{
    astWriter.GetBinaryStreamWriter().Write(static_cast<uint8_t>(symbol->GetSymbolType()));
    /* TODO
    Module* sourceModule = GetModuleById(symbol->SourceModuleId());
    
    if (sourceModule && !spanConversionModuleId.is_nil() && sourceModule->Id() == spanConversionModuleId && sourceModule->GetFlag(ModuleFlags::compiling))
    {
        astWriter.Write(symbol->GetSpan(), true);
    }
    else
    {
        astWriter.Write(symbol->GetSpan(), false);
    }
    */
    astWriter.GetBinaryStreamWriter().Write(symbol->SourceModuleId());
    astWriter.GetBinaryStreamWriter().Write(symbol->Name());
    symbol->Write(*this);
}
/* TODO
void SymbolWriter::SetLexers(std::vector<soulng::lexer::Lexer*>* lexers_)
{
    lexers = lexers_;
    astWriter.SetLexers(lexers);
}

void SymbolWriter::SetSpanConversionModuleId(const boost::uuids::uuid& spanConversionModuleId_)
{
    spanConversionModuleId = spanConversionModuleId_;
    astWriter.SetSpanConversionModuleId(spanConversionModuleId);
}
*/
} // namespace cmajor::symbols
