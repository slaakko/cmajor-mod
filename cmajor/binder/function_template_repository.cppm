// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.function_template_repository;

import std.core;
import cmajor.symbols;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;

struct FunctionTemplateKey
{
    FunctionTemplateKey(cmajor::symbols::FunctionSymbol* functionTemplate_, const std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes_) : 
        functionTemplate(functionTemplate_), templateArgumentTypes(templateArgumentTypes_)
    {
    }
    cmajor::symbols::FunctionSymbol* functionTemplate;
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
};

bool operator==(const FunctionTemplateKey& left, const FunctionTemplateKey& right);
bool operator!=(const FunctionTemplateKey& left, const FunctionTemplateKey& right);

struct FunctionTemplateKeyHash
{
    size_t operator()(const FunctionTemplateKey& key) const
    {
        size_t x = std::hash<cmajor::symbols::FunctionSymbol*>()(key.functionTemplate);
        int n = key.templateArgumentTypes.size();
        for (int i = 0; i < n; ++i)
        {
            x = x ^ std::hash<cmajor::symbols::TypeSymbol*>()(key.templateArgumentTypes[i]);
        }
        return x;
    }
};

class FunctionTemplateRepository
{
public:
    FunctionTemplateRepository(BoundCompileUnit& boundCompileUnit_);
    cmajor::symbols::FunctionSymbol* Instantiate(cmajor::symbols::FunctionSymbol* functionTemplate, 
        const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMapping, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_map<FunctionTemplateKey, cmajor::symbols::FunctionSymbol*, FunctionTemplateKeyHash> functionTemplateMap;
};

} // namespace cmajor::binder
