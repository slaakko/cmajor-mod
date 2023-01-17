export module cmajor.symbols.symbol.creator.visitor;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.ast;

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
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::CursorIdNode& cursorIdNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::ast::FunctionNode& functionNode) override;
    void Visit(cmajor::ast::FunctionPtrNode& functionPtrNode) override;
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
    void Visit(cmajor::ast::SameConstraintNode& sameConstraintNode) override;
    void Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode) override;
    void Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode) override;
    void Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode) override;
    void Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode) override;
    void Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;
    void Visit(cmajor::ast::LabelNode& labelNode) override;
    void Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode) override;
    void Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode) override;
    void Visit(cmajor::ast::ReturnStatementNode& returnStatementNode) override;
    void Visit(cmajor::ast::IfStatementNode& ifStatementNode) override;
    void Visit(cmajor::ast::WhileStatementNode& whileStatementNode) override;
    void Visit(cmajor::ast::DoStatementNode& doStatementNode) override;
    void Visit(cmajor::ast::ForStatementNode& forStatementNode) override;
    void Visit(cmajor::ast::BreakStatementNode& breakStatementNode) override;
    void Visit(cmajor::ast::ContinueStatementNode& continueStatementNode) override;
    void Visit(cmajor::ast::GotoStatementNode& gotoStatementNode) override;
    void Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode) override;
    void Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode) override;
    void Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode) override;
    void Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode) override;
    void Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode) override;
    void Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode) override;
    void Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode) override;
    void Visit(cmajor::ast::SwitchStatementNode& switchStatementNode) override;
    void Visit(cmajor::ast::CaseStatementNode& caseStatementNode) override;
    void Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode) override;
    void Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode) override;
    void Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode) override;
    void Visit(cmajor::ast::ThrowStatementNode& throwStatementNode) override;
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
    void Visit(cmajor::ast::ThisNode& thisNode) override;
    void Visit(cmajor::ast::BaseNode& baseNode) override;
    void Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode) override;
    void Visit(cmajor::ast::CommentNode& commentNode) override;
    void Visit(cmajor::ast::BoolNode& boolNode) override;
    void Visit(cmajor::ast::SByteNode& sbyteNode) override;
    void Visit(cmajor::ast::ByteNode& byteNode) override;
    void Visit(cmajor::ast::ShortNode& shortNode) override;
    void Visit(cmajor::ast::UShortNode& ushortNode) override;
    void Visit(cmajor::ast::IntNode& intNode) override;
    void Visit(cmajor::ast::UIntNode& uintNode) override;
    void Visit(cmajor::ast::LongNode& longNode) override;
    void Visit(cmajor::ast::ULongNode& ulongNode) override;
    void Visit(cmajor::ast::FloatNode& floatNode) override;
    void Visit(cmajor::ast::DoubleNode& doubleNode) override;
    void Visit(cmajor::ast::CharNode& charNode) override;
    void Visit(cmajor::ast::WCharNode& wcharNode) override;
    void Visit(cmajor::ast::UCharNode& ucharNode) override;
    void Visit(cmajor::ast::VoidNode& voidNode) override;

    void Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode) override;
    void Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode) override;
    void Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode) override;
    void Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode) override;
    void Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode) override;
    void Visit(cmajor::ast::IntLiteralNode& intLiteralNode) override;
    void Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode) override;
    void Visit(cmajor::ast::LongLiteralNode& longLiteralNode) override;
    void Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode) override;
    void Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode) override;
    void Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode) override;
    void Visit(cmajor::ast::CharLiteralNode& charLiteralNode) override;
    void Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode) override;
    void Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode) override;
    void Visit(cmajor::ast::StringLiteralNode& stringLiteralNode) override;
    void Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode) override;
    void Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode) override;
    void Visit(cmajor::ast::NullLiteralNode& nullLiteralNode) override;
    void Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode) override;
    void Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode) override;
    void Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode) override;

    void SetLeaveFunction() { leaveFunction = true; }
    void InsertTracer(cmajor::ast::CompoundStatementNode* body);
    void SetEditMode() { editMode = true; }
    void SetSource(Source* source_) { source = source_; }
    const std::vector<std::string>& Errors() const { return errors; }
private:
    SymbolTable& symbolTable;
    cmajor::ast::ClassNode* classInstanceNode;
    ClassTemplateSpecializationSymbol* classTemplateSpecialization;
    std::stack<bool> conditionalCompilationStack;
    Source* source;
    int32_t functionIndex;
    bool leaveFunction;
    bool editMode;
    int level;
    std::vector<std::string> errors;
};
} // namespace cmajor::symbols
