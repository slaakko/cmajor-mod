// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.meta;

import soul.ast.source.pos;
import std.core;
import util.uuid;

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
    virtual std::unique_ptr<Value> DoEvaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) = 0;
    std::unique_ptr<Value> Evaluate(const std::vector<std::unique_ptr<Value>>& arguments, const std::vector<TypeSymbol*>& templateArguments, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
};

} // namespace cmajor::symbols
