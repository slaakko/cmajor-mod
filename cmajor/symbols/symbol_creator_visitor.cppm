// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.symbol.creator.visitor;

import cmajor.ast;
import std.core;

export namespace cmajor::symbols {

class SymbolTable;
class ClassTemplateSpecializationSymbol;
class Source;

class SymbolCreatorVisitor : public cmajor::ast::Visitor
{
public:
    SymbolCreatorVisitor(SymbolTable& symbolTable_);
    void SetClassInstanceNode(cmajor::ast::ClassNode* classInstanceNode_);
    void SetClassTemplateSpecialization(ClassTemplateSpecializationSymbol* classTemplateSpecialization_);
    void Visit(cmajor::ast::CompileUnitNode& compileUnitNode) override;
    void Visit(cmajor::ast::NamespaceNode& namespaceNode) override;
    void Visit(cmajor::ast::AliasNode& aliasNode) override;
    void Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::ClassNode& classNode) override;
    void Visit(cmajor::ast::ThisInitializerNode& thisInitializerNode) override;
    void Visit(cmajor::ast::BaseInitializerNode& baseInitializerNode) override;
    void Visit(cmajor::ast::MemberInitializerNode& memberInitializerNode) override;
    void Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode) override;
    void Visit(cmajor::ast::ConstructorNode& constructorNode) override;
    void Visit(cmajor::ast::DestructorNode& destructorNode) override;
    void Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode) override;
    void Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode) override;
    void Visit(cmajor::ast::MemberVariableNode& memberVariableNode) override;
    void Visit(cmajor::ast::InterfaceNode& interfaceNode) override;
    void Visit(cmajor::ast::DelegateNode& delegateNode) override;
    void Visit(cmajor::ast::ClassDelegateNode& classDelegateNode) override;
    void Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode) override;
    void Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode) override;
    void Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode) override;
    void Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode) override;
    void Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode) override;
    void Visit(cmajor::ast::IsConstraintNode& isConstraintNode) override;
    void Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode) override;
    void Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode) override;
    void Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode) override;
    void Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode) override;
    void Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode) override;
    void Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode) override;
    void Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode) override;
    void Visit(cmajor::ast::AxiomNode& axiomNode) override;
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::ReturnStatementNode& returnStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode) override;
    void Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode) override;
    void Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode) override;
    void Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::TryStatementNode& tryStatementNode) override;
    void Visit(cmajor::ast::CatchNode& catchNode) override;
    void Visit(cmajor::ast::AssertStatementNode& assertStatementNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode) override;
    void Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode) override;
    void Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode) override;
    void Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode) override;
    void Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode) override;
    void Visit(cmajor::ast::TypedefNode& typedefNode) override;
    void Visit(cmajor::ast::ConstantNode& constantNode) override;
    void Visit(cmajor::ast::EnumTypeNode& enumTypeNode) override;
    void Visit(cmajor::ast::EnumConstantNode& enumConstantNode) override;
    void Visit(cmajor::ast::GlobalVariableNode& globalVariableNode) override;
    void Visit(cmajor::ast::ParameterNode& parameterNode) override;
    void Visit(cmajor::ast::TemplateParameterNode& templateParameterNode) override;
    void Visit(cmajor::ast::ConstNode& constNode) override;
    void Visit(cmajor::ast::LValueRefNode& lvalueRefNode) override;
    void Visit(cmajor::ast::RValueRefNode& rvalueRefNode) override;
    void Visit(cmajor::ast::PointerNode& pointerNode) override;
    void Visit(cmajor::ast::ArrayNode& arrayNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;
    void Visit(cmajor::ast::ArrowNode& arrowNode) override;
    void Visit(cmajor::ast::EquivalenceNode& equivalenceNode) override;
    void Visit(cmajor::ast::ImplicationNode& implicationNode) override;
    void Visit(cmajor::ast::DisjunctionNode& disjunctionNode) override;
    void Visit(cmajor::ast::ConjunctionNode& conjunctionNode) override;
    void Visit(cmajor::ast::BitOrNode& bitOrNode) override;
    void Visit(cmajor::ast::BitXorNode& bitXorNode) override;
    void Visit(cmajor::ast::BitAndNode& bitAndNode) override;
    void Visit(cmajor::ast::EqualNode& equalNode) override;
    void Visit(cmajor::ast::NotEqualNode& notEqualNode) override;
    void Visit(cmajor::ast::LessNode& lessNode) override;
    void Visit(cmajor::ast::GreaterNode& greaterNode) override;
    void Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode) override;
    void Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode) override;
    void Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode) override;
    void Visit(cmajor::ast::ShiftRightNode& shiftRightNode) override;
    void Visit(cmajor::ast::AddNode& addNode) override;
    void Visit(cmajor::ast::SubNode& subNode) override;
    void Visit(cmajor::ast::MulNode& mulNode) override;
    void Visit(cmajor::ast::DivNode& divNode) override;
    void Visit(cmajor::ast::RemNode& remNode) override;
    void Visit(cmajor::ast::NotNode& notNode) override;
    void Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode) override;
    void Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode) override;
    void Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode) override;
    void Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode) override;
    void Visit(cmajor::ast::DerefNode& derefNode) override;
    void Visit(cmajor::ast::AddrOfNode& addrOfNode) override;
    void Visit(cmajor::ast::ComplementNode& complementNode) override;
    void Visit(cmajor::ast::IsNode& isNode) override;
    void Visit(cmajor::ast::AsNode& asNode) override;
    void Visit(cmajor::ast::IndexingNode& indexingNode) override;
    void Visit(cmajor::ast::InvokeNode& invokeNode) override;
    void Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode) override;
    void Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode) override;
    void Visit(cmajor::ast::SizeOfNode& sizeOfNode) override;
    void Visit(cmajor::ast::TypeNameNode& typeNameNode) override;
    void Visit(cmajor::ast::TypeIdNode& typeIdNode) override;
    void Visit(cmajor::ast::CastNode& castNode) override;
    void Visit(cmajor::ast::ConstructNode& constructNode) override;
    void Visit(cmajor::ast::NewNode& newNode) override;
    void Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode) override;
    void SetLeaveFunction() { leaveFunction = true; }
private:
    SymbolTable& symbolTable;
    cmajor::ast::ClassNode* classInstanceNode;
    ClassTemplateSpecializationSymbol* classTemplateSpecialization;
    std::stack<bool> conditionalCompilationStack;
    int32_t functionIndex;
    int level;
    bool leaveFunction;
};
} // namespace cmajor::symbols
