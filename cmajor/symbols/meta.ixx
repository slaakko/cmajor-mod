module;
#include <boost/uuid/uuid.hpp>
export module cmajor.symbols.meta;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import soul.ast.source.pos;
import std.core;

export namespace cmajor::symbols {

class TypeSymbol;
class Value;
class SymbolTable;

class IntrinsicFunction
{
public:
    IntrinsicFunction();
    virtual ~IntrinsicFunction();
    virtual int Arity() const = 0;
    virtual const char* GroupName() const = 0;
    virtual int NumberOfTypeParameters() const = 0;
    virtual TypeSymbol* ReturnType(SymbolTable& symbolTable) const = 0;
    virtual std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId) = 0;
    std::unique_ptr<Value> Evaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId);
};

void MetaInit(SymbolTable& symbolTable);
} // namespace cmajor::symbols
