// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.type.binder;

import std.core;
import cmajor.binder.type.resolver;
import cmajor.symbols;
import cmajor.ast;

export namespace cmajor::binder {

class BoundCompileUnit;
class AttributeBinder;
class BoundGlobalVariable;

class ConstraintSymbolBinder
{
public:
    virtual ~ConstraintSymbolBinder();
    virtual void BindConstraintSymbols(cmajor::ast::Node* node, cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit) = 0;
};

void SetConstraintSymbolBinder(ConstraintSymbolBinder* constraintSymbolBinder_);

class TypeBinder : public cmajor::ast::Visitor
{
public:
    TypeBinder(BoundCompileUnit& boundCompileUnit_);
    void AddUsingNodesToCurrentCompileUnit(cmajor::ast::Node* node);
    void Visit(cmajor::ast::CompileUnitNode& compileUnitNode) override;
    void Visit(cmajor::ast::NamespaceNode& namespaceNode) override;
    void Visit(cmajor::ast::AliasNode& aliasNode) override;
    void Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::ClassNode& classNode) override;
    void BindClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::ast::ClassNode* classNode, bool fromOwnCompileUnit);
    void BindClassTemplate(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::ast::ClassNode* classNode);
    void Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode) override;
    void Visit(cmajor::ast::ConstructorNode& constructorNode) override;
    void Visit(cmajor::ast::DestructorNode& destructorNode) override;
    void Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode) override;
    void Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode) override;
    void Visit(cmajor::ast::MemberVariableNode& memberVariableNode) override;
    void Visit(cmajor::ast::InterfaceNode& interfaceNode) override;
    void BindInterface(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::ast::InterfaceNode* interfaceNode, bool fromOwnCompileUnit);
    void Visit(cmajor::ast::DelegateNode& delegateNode) override;
    void Visit(cmajor::ast::ClassDelegateNode& classDelegateNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;
    void BindConcept(cmajor::symbols::ConceptSymbol* conceptSymbol, cmajor::ast::ConceptNode* conceptNode);

    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::TryStatementNode& tryStatementNode) override;
    void Visit(cmajor::ast::CatchNode& catchNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode) override;
    void Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode) override;
    void Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode) override;

    void Visit(cmajor::ast::TypedefNode& typedefNode) override;
    void BindTypedef(cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol, cmajor::ast::TypedefNode* typedefNode, bool fromOwnCompileUnit);
    void BindAlias(cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol, cmajor::ast::AliasNode* aliasNode, bool fromOwnCompileUnit);
    void Visit(cmajor::ast::ConstantNode& constantNode) override;
    void Visit(cmajor::ast::EnumTypeNode& enumTypeNode) override;
    void Visit(cmajor::ast::EnumConstantNode& enumConstantNode) override;
    void Visit(cmajor::ast::GlobalVariableNode& globalVariableNode) override;
    void Visit(cmajor::ast::FullInstantiationRequestNode& fullInstantiationRequestNode) override;

    void SetContainerScope(cmajor::symbols::ContainerScope* containerScope_) { containerScope = containerScope_; }
    void SetCurrentFunctionSymbol(cmajor::symbols::FunctionSymbol* currentFunctionSymbol_) { currentFunctionSymbol = currentFunctionSymbol_; }
    void CreateMemberSymbols();
    BoundGlobalVariable* GetBoundGlobalVariable() const { return boundGlobalVariable; }

private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::Module* module;
    cmajor::symbols::ContainerScope* containerScope;
    std::vector<cmajor::ast::Node*> usingNodes;
    cmajor::symbols::EnumTypeSymbol* enumType;
    cmajor::symbols::FunctionSymbol* currentFunctionSymbol;
    cmajor::symbols::ClassTypeSymbol* currentClassTypeSymbol;
    std::stack<bool> conditionalCompilationStack;
    TypeResolverFlags typeResolverFlags;
    BoundGlobalVariable* boundGlobalVariable;
};

void BindClass(cmajor::symbols::ClassTypeSymbol* classType, void* boundCompileUnit);

} // namespace cmajor::binder
