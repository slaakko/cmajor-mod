// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.visitor;

import std.core;

export namespace cmajor::fault::tolerant::ast {

class Node;
class KeywordNode;
class UsingAliasNode;
class TypedefAliasNode;
class AttributeNode;
class AttributesNode;
class ClassNode;
class InitializerListNode;
class ThisInitializerNode;
class BaseInitializerNode;
class MemberInitializerNode;
class StaticConstructorNode;
class ConstructorNode;
class DestructorNode;
class MemberFunctionNode;
class ConversionFunctionNode;
class MemberVariableNode;
class CompileUnitNode;
class ConceptIdNode;
class DisjunctiveConstraintNode;
class ConjunctiveConstraintNode;
class ParenthesizedConstraintNode;
class PredicateConstraintNode;
class IsConstraintNode;
class MultiparamConstraintNode;
class TypeNameConstraintNode;
class ConstructorConstraintNode;
class DestructorConstraintNode;
class MemberFunctionConstraintNode;
class FunctionConstraintNode;
class WhereConstraintNode;
class EmbeddedConstraintNode;
class AxiomStatementNode;
class AxiomNode;
class ConceptNode;
class ConstantNode;
class DelegateNode;
class ClassDelegateNode;
class EnumConstantNode;
class EnumTypeNode;
class EquivalenceNode;
class ImplicationNode;
class DisjunctionNode;
class BitOrNode;
class BitXorNode;
class EqualNode;
class NotEqualNode;
class LessEqualNode;
class GreaterEqualNode;
class ShiftLeftNode;
class ShiftRightNode;
class PlusNode;
class MinusNode;
class DivNode;
class RemNode;
class IncrementNode;
class DecrementNode;
class NotNode;
class ComplementNode;
class BinaryExprNode;
class PrefixExprNode;
class PostfixExprNode;
class SubscriptExprNode;
class InvokeExprNode;
class IsExprNode;
class AsExprNode;
class DotMemberNode;
class ArrowMemberNode;
class ParenthesizedExprNode;
class SizeOfExprNode;
class TypeNameExprNode;
class TypeIdExprNode;
class CastExprNode;
class ConstructExprNode;
class NewExprNode;
class FunctionNode;
class OperatorFunctionGroupIdNode;
class GlobalVariableNode;
class IdentifierNode;
class QualifiedIdNode;
class InterfaceNode;
class UsingKeywordNode;
class TypedefKeywordNode;
class NamespaceKeywordNode;
class ConstKeywordNode;
class AutoKeywordNode;
class BoolKeywordNode;
class SByteKeywordNode;
class ByteKeywordNode;
class ShortKeywordNode;
class UShortKeywordNode;
class IntKeywordNode;
class UIntKeywordNode;
class LongKeywordNode;
class ULongKeywordNode;
class FloatKeywordNode;
class DoubleKeywordNode;
class CharKeywordNode;
class WCharKeywordNode;
class UCharKeywordNode;
class VoidKeywordNode;
class ThisKeywordNode;
class BaseKeywordNode;
class SizeOfKeywordNode;
class TypeNameKeywordNode;
class TypeIdKeywordNode;
class CastKeywordNode;
class ConstructKeywordNode;
class NewKeywordNode;
class TrueKeywordNode;
class FalseKeywordNode;
class NullKeywordNode;
class ConceptKeywordNode;
class WhereKeywordNode;
class OrKeywordNode;
class AndKeywordNode;
class IsKeywordNode;
class AsKeywordNode;
class OperatorKeywordNode;
class AxiomKeywordNode;
class ReturnKeywordNode;
class IfKeywordNode;
class ElseKeywordNode;
class WhileKeywordNode;
class DoKeywordNode;
class ForKeywordNode;
class BreakKeywordNode;
class ContinueKeywordNode;
class GotoKeywordNode;
class CaseKeywordNode;
class DefaultKeywordNode;
class SwitchKeywordNode;
class DeleteKeywordNode;
class DestroyKeywordNode;
class ThrowKeywordNode;
class TryKeywordNode;
class CatchKeywordNode;
class AssertKeywordNode;
class ElifKeywordNode;
class EndifKeywordNode;
class ClassKeywordNode;
class DelegateKeywordNode;
class EnumKeywordNode;
class InterfaceKeywordNode;
class PublicKeywordNode;
class ProtectedKeywordNode;
class PrivateKeywordNode;
class InternalKeywordNode;
class StaticKeywordNode;
class VirtualKeywordNode;
class OverrideKeywordNode;
class AbstractKeywordNode;
class InlineKeywordNode;
class ExplicitKeywordNode;
class ExternKeywordNode;
class SuppressKeywordNode;
class ConstExprKeywordNode;
class CDeclKeywordNode;
class NothrowKeywordNode;
class WinApiKeywordNode;
class UnitTestKeywordNode;
class LiteralTokenNode;
class BooleanLiteralNode;
class SByteLiteralNode;
class ByteLiteralNode;
class ShortLiteralNode;
class UShortLiteralNode;
class IntLiteralNode;
class UIntLiteralNode;
class LongLiteralNode;
class ULongLiteralNode;
class FloatLiteralNode;
class DoubleLiteralNode;
class CharLiteralNode;
class WCharLiteralNode;
class UCharLiteralNode;
class StringLiteralNode;
class WStringLiteralNode;
class UStringLiteralNode;
class NullLiteralNode;
class ArrayLiteralNode;
class StructuredLiteralNode;
class NamespaceDefinitionNode;
class NamespaceImportNode;
class ParameterNode;
class ParameterListNode;
class DotNode;
class ArrowNode;
class CommaNode;
class ColonNode;
class SemicolonNode;
class LBraceNode;
class RBraceNode;
class LBracketNode;
class RBracketNode;
class LParenNode;
class RParenNode;
class AssignNode;
class LAngleNode;
class RAngleNode;
class StarNode;
class AmpNode;
class AmpAmpNode;
class HashNode;
class SpecifierNode;
class SpecifiersNode;
class CompoundStatementNode;
class ReturnStatementNode;
class IfStatementNode;
class WhileStatementNode;
class DoStatementNode;
class RangeForStatementNode;
class ForStatementNode;
class BreakStatementNode;
class ContinueStatementNode;
class GotoStatementNode;
class GotoCaseStatementNode;
class GotoDefaultStatementNode;
class CaseExprNode;
class CaseStatementNode;
class DefaultStatementNode;
class SwitchStatementNode;
class AssignmentStatementNode;
class ExpressionStatementNode;
class EmptyStatementNode;
class ConstructionStatementNode;
class DeleteStatementNode;
class DestroyStatementNode;
class ThrowStatementNode;
class CatchNode;
class TryStatementNode;
class LabeledStatementNode;
class AssertStatementNode;
class CondCompDisjunctionNode;
class CondCompConjunctionNode;
class CondCompNotNode;
class CondCompParenExprNode;
class CondCompSymbolNode;
class IfGroupNode;
class ElifGroupNode;
class ElseGroupNode;
class CondCompStatementNode;
class CompletionStatementNode;
class ConditionCompletionNode;
class ContainerCompletionNode;
class ExpressionCompletionNode;
class TemplateIdNode;
class TemplateParameterNode;
class TemplateParameterListNode;
class FullInstantiationRequest;
class PointerNode;
class LValueRefNode;
class RValueRefNode;
class ArrayNode;
class TypeExprNode;

class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(UsingAliasNode& node) {}
    virtual void Visit(TypedefAliasNode& node) {}
    virtual void Visit(AttributeNode& node) {}
    virtual void Visit(AttributesNode& node) {}
    virtual void Visit(ClassNode& node) {}
    virtual void Visit(InitializerListNode& node) {}
    virtual void Visit(ThisInitializerNode& node) {}
    virtual void Visit(BaseInitializerNode& node) {}
    virtual void Visit(MemberInitializerNode& node) {}
    virtual void Visit(StaticConstructorNode& node) {}
    virtual void Visit(ConstructorNode& node) {}
    virtual void Visit(DestructorNode& node) {}
    virtual void Visit(MemberFunctionNode& node) {}
    virtual void Visit(ConversionFunctionNode& node) {}
    virtual void Visit(MemberVariableNode& node) {}
    virtual void Visit(CompileUnitNode& node) {}
    virtual void Visit(ConceptIdNode& node) {}
    virtual void Visit(DisjunctiveConstraintNode& node) {}
    virtual void Visit(ConjunctiveConstraintNode& node) {}
    virtual void Visit(ParenthesizedConstraintNode& node) {}
    virtual void Visit(PredicateConstraintNode& node) {}
    virtual void Visit(IsConstraintNode& node) {}
    virtual void Visit(MultiparamConstraintNode& node) {}
    virtual void Visit(TypeNameConstraintNode& node) {}
    virtual void Visit(ConstructorConstraintNode& node) {}
    virtual void Visit(DestructorConstraintNode& node) {}
    virtual void Visit(MemberFunctionConstraintNode& node) {}
    virtual void Visit(FunctionConstraintNode& node) {}
    virtual void Visit(WhereConstraintNode& node) {}
    virtual void Visit(EmbeddedConstraintNode& node) {}
    virtual void Visit(AxiomStatementNode& node) {}
    virtual void Visit(AxiomNode& node) {}
    virtual void Visit(ConceptNode& node) {}
    virtual void Visit(ConstantNode& node) {}
    virtual void Visit(DelegateNode& node) {}
    virtual void Visit(ClassDelegateNode& node) {}
    virtual void Visit(EnumConstantNode& node) {}
    virtual void Visit(EnumTypeNode& node) {}
    virtual void Visit(EquivalenceNode& node) {}
    virtual void Visit(ImplicationNode& node) {}
    virtual void Visit(DisjunctionNode& node) {}
    virtual void Visit(BitOrNode& node) {}
    virtual void Visit(BitXorNode& node) {}
    virtual void Visit(EqualNode& node) {}
    virtual void Visit(NotEqualNode& node) {}
    virtual void Visit(LessEqualNode& node) {}
    virtual void Visit(GreaterEqualNode& node) {}
    virtual void Visit(ShiftLeftNode& node) {}
    virtual void Visit(ShiftRightNode& node) {}
    virtual void Visit(PlusNode& node) {}
    virtual void Visit(MinusNode& node) {}
    virtual void Visit(DivNode& node) {}
    virtual void Visit(RemNode& node) {}
    virtual void Visit(IncrementNode& node) {}
    virtual void Visit(DecrementNode& node) {}
    virtual void Visit(NotNode& node) {}
    virtual void Visit(ComplementNode& node) {}
    virtual void Visit(BinaryExprNode& node) {}
    virtual void Visit(PrefixExprNode& node) {}
    virtual void Visit(PostfixExprNode& node) {}
    virtual void Visit(SubscriptExprNode& node) {}
    virtual void Visit(InvokeExprNode& node) {}
    virtual void Visit(IsExprNode& node) {}
    virtual void Visit(AsExprNode& node) {}
    virtual void Visit(DotMemberNode& node) {}
    virtual void Visit(ArrowMemberNode& node) {}
    virtual void Visit(ParenthesizedExprNode& node) {}
    virtual void Visit(SizeOfExprNode& node) {}
    virtual void Visit(TypeNameExprNode& node) {}
    virtual void Visit(TypeIdExprNode& node) {}
    virtual void Visit(CastExprNode& node) {}
    virtual void Visit(ConstructExprNode& node) {}
    virtual void Visit(NewExprNode& node) {}
    virtual void Visit(FunctionNode& node) {}
    virtual void Visit(OperatorFunctionGroupIdNode& node) {}
    virtual void Visit(GlobalVariableNode& node) {}
    virtual void Visit(IdentifierNode& node) {}
    virtual void Visit(QualifiedIdNode& node) {}
    virtual void Visit(InterfaceNode& node) {}
    virtual void Visit(UsingKeywordNode& node) {}
    virtual void Visit(TypedefKeywordNode& node) {}
    virtual void Visit(NamespaceKeywordNode& node) {}
    virtual void Visit(ConstKeywordNode& node) {}
    virtual void Visit(AutoKeywordNode& node) {}
    virtual void Visit(BoolKeywordNode& node) {}
    virtual void Visit(SByteKeywordNode& node) {}
    virtual void Visit(ByteKeywordNode& node) {}
    virtual void Visit(ShortKeywordNode& node) {}
    virtual void Visit(UShortKeywordNode& node) {}
    virtual void Visit(IntKeywordNode& node) {}
    virtual void Visit(UIntKeywordNode& node) {}
    virtual void Visit(LongKeywordNode& node) {}
    virtual void Visit(ULongKeywordNode& node) {}
    virtual void Visit(FloatKeywordNode& node) {}
    virtual void Visit(DoubleKeywordNode& node) {}
    virtual void Visit(CharKeywordNode& node) {}
    virtual void Visit(WCharKeywordNode& node) {}
    virtual void Visit(UCharKeywordNode& node) {}
    virtual void Visit(VoidKeywordNode& node) {}
    virtual void Visit(ThisKeywordNode& node) {}
    virtual void Visit(BaseKeywordNode& node) {}
    virtual void Visit(SizeOfKeywordNode& node) {}
    virtual void Visit(TypeNameKeywordNode& node) {}
    virtual void Visit(TypeIdKeywordNode& node) {}
    virtual void Visit(CastKeywordNode& node) {}
    virtual void Visit(ConstructKeywordNode& node) {}
    virtual void Visit(NewKeywordNode& node) {}
    virtual void Visit(TrueKeywordNode& node) {}
    virtual void Visit(FalseKeywordNode& node) {}
    virtual void Visit(NullKeywordNode& node) {}
    virtual void Visit(ConceptKeywordNode& node) {}
    virtual void Visit(WhereKeywordNode& node) {}
    virtual void Visit(OrKeywordNode& node) {}
    virtual void Visit(AndKeywordNode& node) {}
    virtual void Visit(IsKeywordNode& node) {}
    virtual void Visit(AsKeywordNode& node) {}
    virtual void Visit(OperatorKeywordNode& node) {}
    virtual void Visit(AxiomKeywordNode& node) {}
    virtual void Visit(ReturnKeywordNode& node) {}
    virtual void Visit(IfKeywordNode& node) {}
    virtual void Visit(ElseKeywordNode& node) {}
    virtual void Visit(WhileKeywordNode& node) {}
    virtual void Visit(DoKeywordNode& node) {}
    virtual void Visit(ForKeywordNode& node) {}
    virtual void Visit(BreakKeywordNode& node) {}
    virtual void Visit(ContinueKeywordNode& node) {}
    virtual void Visit(GotoKeywordNode& node) {}
    virtual void Visit(CaseKeywordNode& node) {}
    virtual void Visit(DefaultKeywordNode& node) {}
    virtual void Visit(SwitchKeywordNode& node) {}
    virtual void Visit(DeleteKeywordNode& node) {}
    virtual void Visit(DestroyKeywordNode& node) {}
    virtual void Visit(ThrowKeywordNode& node) {}
    virtual void Visit(TryKeywordNode& node) {}
    virtual void Visit(CatchKeywordNode& node) {}
    virtual void Visit(AssertKeywordNode& node) {}
    virtual void Visit(ElifKeywordNode& node) {}
    virtual void Visit(EndifKeywordNode& node) {}
    virtual void Visit(ClassKeywordNode& node) {}
    virtual void Visit(DelegateKeywordNode& node) {}
    virtual void Visit(EnumKeywordNode& node) {}
    virtual void Visit(InterfaceKeywordNode& node) {}
    virtual void Visit(PublicKeywordNode& node) {}
    virtual void Visit(ProtectedKeywordNode& node) {}
    virtual void Visit(PrivateKeywordNode& node) {}
    virtual void Visit(InternalKeywordNode& node) {}
    virtual void Visit(StaticKeywordNode& node) {}
    virtual void Visit(VirtualKeywordNode& node) {}
    virtual void Visit(OverrideKeywordNode& node) {}
    virtual void Visit(AbstractKeywordNode& node) {}
    virtual void Visit(InlineKeywordNode& node) {}
    virtual void Visit(ExplicitKeywordNode& node) {}
    virtual void Visit(ExternKeywordNode& node) {}
    virtual void Visit(SuppressKeywordNode& node) {}
    virtual void Visit(ConstExprKeywordNode& node) {}
    virtual void Visit(CDeclKeywordNode& node) {}
    virtual void Visit(NothrowKeywordNode& node) {}
    virtual void Visit(WinApiKeywordNode& node) {}
    virtual void Visit(UnitTestKeywordNode& node) {}
    virtual void Visit(LiteralTokenNode& node) {}
    virtual void Visit(BooleanLiteralNode& node) {}
    virtual void Visit(SByteLiteralNode& node) {}
    virtual void Visit(ByteLiteralNode& node) {}
    virtual void Visit(ShortLiteralNode& node) {}
    virtual void Visit(UShortLiteralNode& node) {}
    virtual void Visit(IntLiteralNode& node) {}
    virtual void Visit(UIntLiteralNode& node) {}
    virtual void Visit(LongLiteralNode& node) {}
    virtual void Visit(ULongLiteralNode& node) {}
    virtual void Visit(FloatLiteralNode& node) {}
    virtual void Visit(DoubleLiteralNode& node) {}
    virtual void Visit(CharLiteralNode& node) {}
    virtual void Visit(WCharLiteralNode& node) {}
    virtual void Visit(UCharLiteralNode& node) {}
    virtual void Visit(StringLiteralNode& node) {}
    virtual void Visit(WStringLiteralNode& node) {}
    virtual void Visit(UStringLiteralNode& node) {}
    virtual void Visit(NullLiteralNode& node) {}
    virtual void Visit(ArrayLiteralNode& node) {}
    virtual void Visit(StructuredLiteralNode& node) {}
    virtual void Visit(NamespaceDefinitionNode& node) {}
    virtual void Visit(NamespaceImportNode& node) {}
    virtual void Visit(ParameterNode& node) {}
    virtual void Visit(ParameterListNode& node) {}
    virtual void Visit(DotNode& node) {}
    virtual void Visit(ArrowNode& node) {}
    virtual void Visit(CommaNode& node) {}
    virtual void Visit(ColonNode& node) {}
    virtual void Visit(SemicolonNode& node) {}
    virtual void Visit(LBraceNode& node) {}
    virtual void Visit(RBraceNode& node) {}
    virtual void Visit(LBracketNode& node) {}
    virtual void Visit(RBracketNode& node) {}
    virtual void Visit(LParenNode& node) {}
    virtual void Visit(RParenNode& node) {}
    virtual void Visit(AssignNode& node) {}
    virtual void Visit(LAngleNode& node) {}
    virtual void Visit(RAngleNode& node) {}
    virtual void Visit(StarNode& node) {}
    virtual void Visit(AmpNode& node) {}
    virtual void Visit(AmpAmpNode& node) {}
    virtual void Visit(HashNode& node) {}
    virtual void Visit(SpecifierNode& node) {}
    virtual void Visit(SpecifiersNode& node) {}
    virtual void Visit(CompoundStatementNode& node) {}
    virtual void Visit(ReturnStatementNode& node) {}
    virtual void Visit(IfStatementNode& node) {}
    virtual void Visit(WhileStatementNode& node) {}
    virtual void Visit(DoStatementNode& node) {}
    virtual void Visit(RangeForStatementNode& node) {}
    virtual void Visit(ForStatementNode& node) {}
    virtual void Visit(BreakStatementNode& node) {}
    virtual void Visit(ContinueStatementNode& node) {}
    virtual void Visit(GotoStatementNode& node) {}
    virtual void Visit(GotoCaseStatementNode& node) {}
    virtual void Visit(GotoDefaultStatementNode& node) {}
    virtual void Visit(CaseExprNode& node) {}
    virtual void Visit(CaseStatementNode& node) {}
    virtual void Visit(DefaultStatementNode& node) {}
    virtual void Visit(SwitchStatementNode& node) {}
    virtual void Visit(AssignmentStatementNode& node) {}
    virtual void Visit(ExpressionStatementNode& node) {}
    virtual void Visit(EmptyStatementNode& node) {}
    virtual void Visit(ConstructionStatementNode& node) {}
    virtual void Visit(DeleteStatementNode& node) {}
    virtual void Visit(DestroyStatementNode& node) {}
    virtual void Visit(ThrowStatementNode& node) {}
    virtual void Visit(CatchNode& node) {}
    virtual void Visit(TryStatementNode& node) {}
    virtual void Visit(LabeledStatementNode& node) {}
    virtual void Visit(AssertStatementNode& node) {}
    virtual void Visit(CondCompDisjunctionNode& node) {}
    virtual void Visit(CondCompConjunctionNode& node) {}
    virtual void Visit(CondCompNotNode& node) {}
    virtual void Visit(CondCompParenExprNode& node) {}
    virtual void Visit(CondCompSymbolNode& node) {}
    virtual void Visit(IfGroupNode& node) {}
    virtual void Visit(ElifGroupNode& node) {}
    virtual void Visit(ElseGroupNode& node) {}
    virtual void Visit(CondCompStatementNode& node) {}
    virtual void Visit(CompletionStatementNode& node) {}
    virtual void Visit(ConditionCompletionNode& node) {}
    virtual void Visit(ContainerCompletionNode& node) {}
    virtual void Visit(ExpressionCompletionNode& node) {}
    virtual void Visit(TemplateIdNode& node) {}
    virtual void Visit(TemplateParameterNode& node) {}
    virtual void Visit(TemplateParameterListNode& node) {}
    virtual void Visit(FullInstantiationRequest& node) {}
    virtual void Visit(PointerNode& node) {}
    virtual void Visit(LValueRefNode& node) {}
    virtual void Visit(RValueRefNode& node) {}
    virtual void Visit(ArrayNode& node) {}
    virtual void Visit(TypeExprNode& node) {}
};

class DefaultVisitor : public Visitor
{
public:
    virtual void VisitIdentifier(IdentifierNode* identifier) {}
    virtual void VisitOperator(Node* operatorNode) {}
    virtual void VisitParen(Node* paren) {}
    virtual void VisitKeyword(KeywordNode* keyword) {}
    virtual void VisitNumber(LiteralTokenNode* number) {}
    virtual void VisitChar(LiteralTokenNode* chr) {}
    virtual void VisitString(LiteralTokenNode* str) {}
    void Visit(UsingAliasNode& node) override;
    void Visit(TypedefAliasNode& node) override;
    void Visit(AttributeNode& node) override;
    void Visit(AttributesNode& node) override;
    void Visit(ClassNode& node) override;
    void Visit(InitializerListNode& node) override;
    void Visit(ThisInitializerNode& node) override;
    void Visit(BaseInitializerNode& node) override;
    void Visit(MemberInitializerNode& node) override;
    void Visit(StaticConstructorNode& node) override;
    void Visit(ConstructorNode& node) override;
    void Visit(DestructorNode& node) override;
    void Visit(MemberFunctionNode& node) override;
    void Visit(ConversionFunctionNode& node) override;
    void Visit(MemberVariableNode& node) override;
    void Visit(CompileUnitNode& node) override;
    void Visit(ConceptIdNode& node) override;
    void Visit(DisjunctiveConstraintNode& node) override;
    void Visit(ConjunctiveConstraintNode& node) override;
    void Visit(ParenthesizedConstraintNode& node) override;
    void Visit(PredicateConstraintNode& node) override;
    void Visit(IsConstraintNode& node) override;
    void Visit(MultiparamConstraintNode& node) override;
    void Visit(TypeNameConstraintNode& node) override;
    void Visit(ConstructorConstraintNode& node) override;
    void Visit(DestructorConstraintNode& node) override;
    void Visit(MemberFunctionConstraintNode& node) override;
    void Visit(FunctionConstraintNode& node) override;
    void Visit(WhereConstraintNode& node) override;
    void Visit(EmbeddedConstraintNode& node) override;
    void Visit(AxiomStatementNode& node) override;
    void Visit(AxiomNode& node) override;
    void Visit(ConceptNode& node) override;
    void Visit(ConstantNode& node) override;
    void Visit(DelegateNode& node) override;
    void Visit(ClassDelegateNode& node) override;
    void Visit(EnumConstantNode& node) override;
    void Visit(EnumTypeNode& node) override;
    void Visit(EquivalenceNode& node) override;
    void Visit(ImplicationNode& node) override;
    void Visit(DisjunctionNode& node) override;
    void Visit(BitOrNode& node) override;
    void Visit(BitXorNode& node) override;
    void Visit(EqualNode& node) override;
    void Visit(NotEqualNode& node) override;
    void Visit(LessEqualNode& node) override;
    void Visit(GreaterEqualNode& node) override;
    void Visit(ShiftLeftNode& node) override;
    void Visit(ShiftRightNode& node) override;
    void Visit(PlusNode& node) override;
    void Visit(MinusNode& node) override;
    void Visit(DivNode& node) override;
    void Visit(RemNode& node) override;
    void Visit(IncrementNode& node) override;
    void Visit(DecrementNode& node) override;
    void Visit(NotNode& node) override;
    void Visit(ComplementNode& node) override;
    void Visit(BinaryExprNode& node) override;
    void Visit(PrefixExprNode& node) override;
    void Visit(PostfixExprNode& node) override;
    void Visit(SubscriptExprNode& node) override;
    void Visit(InvokeExprNode& node) override;
    void Visit(IsExprNode& node) override;
    void Visit(AsExprNode& node) override;
    void Visit(DotMemberNode& node) override;
    void Visit(ArrowMemberNode& node) override;
    void Visit(ParenthesizedExprNode& node) override;
    void Visit(SizeOfExprNode& node) override;
    void Visit(TypeNameExprNode& node) override;
    void Visit(TypeIdExprNode& node) override;
    void Visit(CastExprNode& node) override;
    void Visit(ConstructExprNode& node) override;
    void Visit(NewExprNode& node) override;
    void Visit(FunctionNode& node) override;
    void Visit(OperatorFunctionGroupIdNode& node) override;
    void Visit(GlobalVariableNode& node) override;
    void Visit(QualifiedIdNode& node) override;
    void Visit(IdentifierNode& node) override;
    void Visit(InterfaceNode& node) override;
    void Visit(UsingKeywordNode& node) override;
    void Visit(TypedefKeywordNode& node) override;
    void Visit(NamespaceKeywordNode& node) override;
    void Visit(ConstKeywordNode& node) override;
    void Visit(AutoKeywordNode& node) override;
    void Visit(BoolKeywordNode& node) override;
    void Visit(SByteKeywordNode& node) override;
    void Visit(ByteKeywordNode& node) override;
    void Visit(ShortKeywordNode& node) override;
    void Visit(UShortKeywordNode& node) override;
    void Visit(IntKeywordNode& node) override;
    void Visit(UIntKeywordNode& node) override;
    void Visit(LongKeywordNode& node) override;
    void Visit(ULongKeywordNode& node) override;
    void Visit(FloatKeywordNode& node) override;
    void Visit(DoubleKeywordNode& node) override;
    void Visit(CharKeywordNode& node) override;
    void Visit(WCharKeywordNode& node) override;
    void Visit(UCharKeywordNode& node) override;
    void Visit(VoidKeywordNode& node) override;
    void Visit(ThisKeywordNode& node) override;
    void Visit(BaseKeywordNode& node) override;
    void Visit(SizeOfKeywordNode& node) override;
    void Visit(TypeNameKeywordNode& node) override;
    void Visit(TypeIdKeywordNode& node) override;
    void Visit(CastKeywordNode& node) override;
    void Visit(ConstructKeywordNode& node) override;
    void Visit(NewKeywordNode& node) override;
    void Visit(TrueKeywordNode& node) override;
    void Visit(FalseKeywordNode& node) override;
    void Visit(NullKeywordNode& node) override;
    void Visit(ConceptKeywordNode& node) override;
    void Visit(WhereKeywordNode& node) override;
    void Visit(OrKeywordNode& node) override;
    void Visit(AndKeywordNode& node) override;
    void Visit(IsKeywordNode& node) override;
    void Visit(AsKeywordNode& node) override;
    void Visit(OperatorKeywordNode& node) override;
    void Visit(AxiomKeywordNode& node) override;
    void Visit(ReturnKeywordNode& node) override;
    void Visit(IfKeywordNode& node) override;
    void Visit(ElseKeywordNode& node) override;
    void Visit(WhileKeywordNode& node) override;
    void Visit(DoKeywordNode& node) override;
    void Visit(ForKeywordNode& node) override;
    void Visit(BreakKeywordNode& node) override;
    void Visit(ContinueKeywordNode& node) override;
    void Visit(GotoKeywordNode& node) override;
    void Visit(CaseKeywordNode& node) override;
    void Visit(DefaultKeywordNode& node) override;
    void Visit(SwitchKeywordNode& node) override;
    void Visit(DeleteKeywordNode& node) override;
    void Visit(DestroyKeywordNode& node) override;
    void Visit(ThrowKeywordNode& node) override;
    void Visit(TryKeywordNode& node) override;
    void Visit(CatchKeywordNode& node) override;
    void Visit(AssertKeywordNode& node) override;
    void Visit(ElifKeywordNode& node) override;
    void Visit(EndifKeywordNode& node) override;
    void Visit(ClassKeywordNode& node) override;
    void Visit(DelegateKeywordNode& node) override;
    void Visit(EnumKeywordNode& node) override;
    void Visit(InterfaceKeywordNode& node) override;
    void Visit(PublicKeywordNode& node) override;
    void Visit(ProtectedKeywordNode& node) override;
    void Visit(PrivateKeywordNode& node) override;
    void Visit(InternalKeywordNode& node) override;
    void Visit(StaticKeywordNode& node) override;
    void Visit(VirtualKeywordNode& node) override;
    void Visit(OverrideKeywordNode& node) override;
    void Visit(AbstractKeywordNode& node) override;
    void Visit(InlineKeywordNode& node) override;
    void Visit(ExplicitKeywordNode& node) override;
    void Visit(ExternKeywordNode& node) override;
    void Visit(SuppressKeywordNode& node) override;
    void Visit(ConstExprKeywordNode& node) override;
    void Visit(CDeclKeywordNode& node) override;
    void Visit(NothrowKeywordNode& node) override;
    void Visit(WinApiKeywordNode& node) override;
    void Visit(UnitTestKeywordNode& node) override;
    void Visit(BooleanLiteralNode& node) override;
    void Visit(SByteLiteralNode& node) override;
    void Visit(ByteLiteralNode& node) override;
    void Visit(ShortLiteralNode& node) override;
    void Visit(UShortLiteralNode& node) override;
    void Visit(IntLiteralNode& node) override;
    void Visit(UIntLiteralNode& node) override;
    void Visit(LongLiteralNode& node) override;
    void Visit(ULongLiteralNode& node) override;
    void Visit(FloatLiteralNode& node) override;
    void Visit(DoubleLiteralNode& node) override;
    void Visit(CharLiteralNode& node) override;
    void Visit(WCharLiteralNode& node) override;
    void Visit(UCharLiteralNode& node) override;
    void Visit(StringLiteralNode& node) override;
    void Visit(WStringLiteralNode& node) override;
    void Visit(UStringLiteralNode& node) override;
    void Visit(NullLiteralNode& node) override;
    void Visit(ArrayLiteralNode& node) override;
    void Visit(StructuredLiteralNode& node) override;
    void Visit(NamespaceDefinitionNode& node) override;
    void Visit(NamespaceImportNode& node) override;
    void Visit(ParameterNode& node) override;
    void Visit(ParameterListNode& node) override;
    void Visit(DotNode& node) override;
    void Visit(ArrowNode& node) override;
    void Visit(CommaNode& node) override;
    void Visit(ColonNode& node) override;
    void Visit(SemicolonNode& node) override;
    void Visit(LBraceNode& node) override;
    void Visit(RBraceNode& node) override;
    void Visit(LBracketNode& node) override;
    void Visit(RBracketNode& node) override;
    void Visit(LParenNode& node) override;
    void Visit(RParenNode& node) override;
    void Visit(AssignNode& node) override;
    void Visit(LAngleNode& node) override;
    void Visit(RAngleNode& node) override;
    void Visit(StarNode& node) override;
    void Visit(AmpNode& node) override;
    void Visit(AmpAmpNode& node) override;
    void Visit(HashNode& node) override;
    void Visit(SpecifierNode& node) override;
    void Visit(SpecifiersNode& node) override;
    void Visit(CompoundStatementNode& node) override;
    void Visit(ReturnStatementNode& node) override;
    void Visit(IfStatementNode& node) override;
    void Visit(WhileStatementNode& node) override;
    void Visit(DoStatementNode& node) override;
    void Visit(RangeForStatementNode& node) override;
    void Visit(ForStatementNode& node) override;
    void Visit(BreakStatementNode& node) override;
    void Visit(ContinueStatementNode& node) override;
    void Visit(GotoStatementNode& node) override;
    void Visit(GotoCaseStatementNode& node) override;
    void Visit(GotoDefaultStatementNode& node) override;
    void Visit(CaseExprNode& node) override;
    void Visit(CaseStatementNode& node) override;
    void Visit(DefaultStatementNode& node) override;
    void Visit(SwitchStatementNode& node) override;
    void Visit(AssignmentStatementNode& node) override;
    void Visit(ExpressionStatementNode& node) override;
    void Visit(EmptyStatementNode& node) override;
    void Visit(ConstructionStatementNode& node) override;
    void Visit(DeleteStatementNode& node) override;
    void Visit(DestroyStatementNode& node) override;
    void Visit(ThrowStatementNode& node) override;
    void Visit(CatchNode& node) override;
    void Visit(TryStatementNode& node) override;
    void Visit(LabeledStatementNode& node) override;
    void Visit(AssertStatementNode& node) override;
    void Visit(CondCompDisjunctionNode& node) override;
    void Visit(CondCompConjunctionNode& node) override;
    void Visit(CondCompNotNode& node) override;
    void Visit(CondCompParenExprNode& node) override;
    void Visit(CondCompSymbolNode& node) override;
    void Visit(IfGroupNode& node) override;
    void Visit(ElifGroupNode& node) override;
    void Visit(ElseGroupNode& node) override;
    void Visit(CondCompStatementNode& node) override;
    void Visit(TemplateIdNode& node) override;
    void Visit(TemplateParameterNode& node) override;
    void Visit(TemplateParameterListNode& node) override;
    void Visit(FullInstantiationRequest& node) override;
    void Visit(PointerNode& node) override;
    void Visit(LValueRefNode& node) override;
    void Visit(RValueRefNode& node) override;
    void Visit(ArrayNode& node) override;
    void Visit(TypeExprNode& node) override;
};

} // namespace cmajor::fault::tolerant::ast
