// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.function_template_repository;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.binder;
import cmajor.binder.statement.binder;
import cmajor.binder.bound.statement;
import util;

namespace cmajor::binder{

bool operator==(const FunctionTemplateKey& left, const FunctionTemplateKey& right)
{
    if (left.functionTemplate != right.functionTemplate) return false;
    if (left.templateArgumentTypes.size() != right.templateArgumentTypes.size()) return false;
    int n = left.templateArgumentTypes.size();
    for (int i = 0; i < n; ++i)
    {
        if (!TypesEqual(left.templateArgumentTypes[i], right.templateArgumentTypes[i])) return false;
    }
    return true;
}

bool operator!=(const FunctionTemplateKey& left, const FunctionTemplateKey& right)
{
    return !(left == right);
}

FunctionTemplateRepository::FunctionTemplateRepository(BoundCompileUnit& boundCompileUnit_) : boundCompileUnit(boundCompileUnit_)
{
}

cmajor::symbols::FunctionSymbol* FunctionTemplateRepository::Instantiate(cmajor::symbols::FunctionSymbol* functionTemplate, const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMapping,
    const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    for (cmajor::symbols::TemplateParameterSymbol* templateParameter : functionTemplate->TemplateParameters())
    {
        auto it = templateParameterMapping.find(templateParameter);
        if (it != templateParameterMapping.cend())
        {
            cmajor::symbols::TypeSymbol* templateArgumentType = it->second;
            templateArgumentTypes.push_back(templateArgumentType);
        }
        else
        {
            throw cmajor::symbols::Exception("template parameter type not found", span, moduleId);
        }
    }
    FunctionTemplateKey key(functionTemplate, templateArgumentTypes);
    auto it = functionTemplateMap.find(key);
    if (it != functionTemplateMap.cend())
    {
        return it->second;
    }
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(functionTemplate);
    if (!node)
    {
        node = functionTemplate->GetFunctionNode();
        symbolTable.MapNode(node, functionTemplate);
        Assert(node, "function node not read"); 
    }
    Assert(node->GetNodeType() == cmajor::ast::NodeType::functionNode, "function node expected");
    cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
    std::unique_ptr<cmajor::ast::NamespaceNode> globalNs(new cmajor::ast::NamespaceNode(functionNode->GetSourcePos(), functionNode->ModuleId(), 
        new cmajor::ast::IdentifierNode(functionNode->GetSourcePos(), functionNode->ModuleId(), U"")));
    cmajor::ast::NamespaceNode* currentNs = globalNs.get();
    cmajor::ast::CloneContext cloneContext;
    cloneContext.SetInstantiateFunctionNode();
    int n = functionTemplate->UsingNodes().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* usingNode = functionTemplate->UsingNodes()[i];
        globalNs->AddMember(usingNode->Clone(cloneContext));
    }
    bool fileScopeAdded = false;
    if (!functionTemplate->Ns()->IsGlobalNamespace())
    {
        cmajor::symbols::FileScope* primaryFileScope = new cmajor::symbols::FileScope();
        primaryFileScope->AddContainerScope(functionTemplate->Ns()->GetContainerScope());
        boundCompileUnit.AddFileScope(primaryFileScope);
        fileScopeAdded = true;
        std::u32string fullNsName = functionTemplate->Ns()->FullName();
        std::vector<std::u32string> nsComponents = util::Split(fullNsName, '.');
        for (const std::u32string& nsComponent : nsComponents)
        {
            cmajor::ast::NamespaceNode* nsNode = new cmajor::ast::NamespaceNode(functionNode->GetSourcePos(), functionNode->ModuleId(), 
                new cmajor::ast::IdentifierNode(functionNode->GetSourcePos(), functionNode->ModuleId(), nsComponent));
            currentNs->AddMember(nsNode);
            currentNs = nsNode;
        }
    }
    cmajor::ast::FunctionNode* functionInstanceNode = static_cast<cmajor::ast::FunctionNode*>(functionNode->Clone(cloneContext));
    currentNs->AddMember(functionInstanceNode);
    symbolTable.SetCurrentCompileUnit(boundCompileUnit.GetCompileUnitNode());
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
    symbolCreatorVisitor.InsertTracer(functionInstanceNode->Body());
    globalNs->Accept(symbolCreatorVisitor);
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(functionInstanceNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionSymbol, "function symbol expected");
    cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
    functionSymbol->SetLinkOnceOdrLinkage();
    functionSymbol->SetTemplateSpecialization();
    functionSymbol->SetFunctionTemplate(functionTemplate);
    functionSymbol->SetTemplateArgumentTypes(templateArgumentTypes);
    functionTemplateMap[key] = functionSymbol;
    for (cmajor::symbols::TemplateParameterSymbol* templateParameter : functionTemplate->TemplateParameters())
    {
        auto it = templateParameterMapping.find(templateParameter);
        if (it != templateParameterMapping.cend())
        {
            cmajor::symbols::TypeSymbol* boundType = it->second;
            cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameter = new cmajor::symbols::BoundTemplateParameterSymbol(span, moduleId, templateParameter->Name());
            boundTemplateParameter->SetType(boundType);
            functionSymbol->AddMember(boundTemplateParameter);
        }
        else
        {
            throw cmajor::symbols::Exception("template parameter type not found", span, moduleId);
        }
    }
    TypeBinder typeBinder(boundCompileUnit);
    globalNs->Accept(typeBinder);
    StatementBinder statementBinder(boundCompileUnit);
    globalNs->Accept(statementBinder);
    if (fileScopeAdded)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
    boundCompileUnit.AddGlobalNs(std::move(globalNs));
    functionSymbol->SetFlag(cmajor::symbols::FunctionSymbolFlags::dontReuse);
    if (functionTemplate->IsSystemDefault())
    {
        functionSymbol->SetSystemDefault();
    }
    boundCompileUnit.SetCanReuse(functionSymbol);
    return functionSymbol;
}

} // namespace cmajor::binder

