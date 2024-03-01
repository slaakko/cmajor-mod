// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.meta;

import cmajor.ast;
import soul.ast.span;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class TypeSymbol;
class Value;
class SymbolTable;

class IntrinsicFunction
{
public:
    IntrinsicFunction(const soul::ast::Span& span_);
    const soul::ast::Span& GetSpan() const { return span; }
    virtual ~IntrinsicFunction();
    virtual int Arity() const = 0;
    virtual const char* GroupName() const = 0;
    virtual int NumberOfTypeParameters() const = 0;
    virtual TypeSymbol* ReturnType(SymbolTable& symbolTable) const = 0;
    virtual std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node) = 0;
    std::unique_ptr<Value> Evaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, cmajor::ast::Node* node);
private:
    soul::ast::Span span;
};

void MetaInit(SymbolTable& symbolTable, const soul::ast::Span& rootSpan);

} // namespace cmajor::symbols
