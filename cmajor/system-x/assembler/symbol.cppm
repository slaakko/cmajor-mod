// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.symbol;

import cmajor.systemx.assembler.node;

export namespace cmajor::systemx::assembler {

export namespace symbol {}

class Symbol : public Node
{
public:
    Symbol(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    const std::string& Name() const { return name; }
    void Write(util::CodeFormatter& formatter);
private:
    std::string name;
};

class LocalSymbol : public Symbol
{
public:
    LocalSymbol(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    void Accept(Visitor& visitor) override;
};

class GlobalSymbol : public Symbol
{
public:
    GlobalSymbol(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    void Accept(Visitor& visitor) override;
};

class At : public Node
{
public:
    At(const soul::ast::SourcePos& sourcePos_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter);
};

LocalSymbol* MakeLocalSymbol(int id);
GlobalSymbol* MakeGlobalSymbol(const std::string& name);

} // namespace cmajor::systemx::assembler
