// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.symbol;

import cmajor.systemx.assembler.visitor;

namespace cmajor::systemx::assembler {

Symbol::Symbol(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_) : Node(kind_, sourcePos_), name(name_)
{
}

void Symbol::Write(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
}

LocalSymbol::LocalSymbol(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : Symbol(NodeKind::localSymbolNode, sourcePos_, name_)
{
}

void LocalSymbol::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GlobalSymbol::GlobalSymbol(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : Symbol(NodeKind::globalSymbolNode, sourcePos_, name_)
{
}

void GlobalSymbol::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

At::At(const soul::ast::SourcePos& sourcePos_) : Node(NodeKind::atNode, sourcePos_)
{
}

void At::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void At::Write(util::CodeFormatter& formatter)
{
    formatter.Write("@");
}

LocalSymbol* MakeLocalSymbol(int id)
{
    return new LocalSymbol(soul::ast::SourcePos(), "@" + std::to_string(id));
}

GlobalSymbol* MakeGlobalSymbol(const std::string& name)
{
    return new GlobalSymbol(soul::ast::SourcePos(), name);
}

} // namespace cmajor::systemx::assembler
