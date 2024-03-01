// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.visitor;

import cmajor.fault.tolerant.ast.alias;
import cmajor.fault.tolerant.ast.attribute;
import cmajor.fault.tolerant.ast.classes;
import cmajor.fault.tolerant.ast.compile.unit;
import cmajor.fault.tolerant.ast.concepts;
import cmajor.fault.tolerant.ast.constant;
import cmajor.fault.tolerant.ast.delegate;
import cmajor.fault.tolerant.ast.enumeration;
import cmajor.fault.tolerant.ast.expression;
import cmajor.fault.tolerant.ast.function;
import cmajor.fault.tolerant.ast.global.variable;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.interface;
import cmajor.fault.tolerant.ast.literal;
import cmajor.fault.tolerant.ast.namespaces;
import cmajor.fault.tolerant.ast.statement;
import cmajor.fault.tolerant.ast.templates;
import cmajor.fault.tolerant.ast.type_expr;

namespace cmajor::fault::tolerant::ast {

Visitor::~Visitor()
{
}

void DefaultVisitor::Visit(UsingAliasNode& node)
{
    node.GetSpecifiers()->Accept(*this);
    node.UsingKeyword()->Accept(*this);
    node.Identifier()->Accept(*this);
    node.Assign()->Accept(*this);
    node.TypeExpr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(TypedefAliasNode& node)
{
    node.GetSpecifiers()->Accept(*this);
    node.Typedef()->Accept(*this);
    node.TypeExpr()->Accept(*this);
    node.Identifier()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(AttributeNode& node)
{
    node.Name()->Accept(*this);
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
    }
    node.Value()->Accept(*this);
}

void DefaultVisitor::Visit(AttributesNode& node)
{
    node.LBracket()->Accept(*this);
    node.AttributeList()->Accept(*this);
    node.RBracket()->Accept(*this);
}

void DefaultVisitor::Visit(ClassNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.ClassKeyword()->Accept(*this);
    node.Identifier()->Accept(*this);
    node.TemplateParameterList()->Accept(*this);
    if (node.Colon())
    {
        node.Colon()->Accept(*this);
    }
    node.BaseClassAndInterfaceList()->Accept(*this);
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    node.LBrace()->Accept(*this);
    node.Members().Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(InitializerListNode& node)
{
    for (const auto& n : node.Nodes())
    {
        n->Accept(*this);
    }
}

void DefaultVisitor::Visit(ThisInitializerNode& node)
{
    node.ThisKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.ArgumentList().Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(BaseInitializerNode& node)
{
    node.BaseKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.ArgumentList().Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(MemberInitializerNode& node)
{
    node.Id()->Accept(*this);
    node.LParen()->Accept(*this);
    node.ArgumentList().Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(StaticConstructorNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.ClassName()->Accept(*this);
    node.LParen()->Accept(*this);
    node.RParen()->Accept(*this);
    if (node.Colon())
    {
        node.Colon()->Accept(*this);
    }
    node.InitializerList()->Accept(*this);
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ConstructorNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.ClassName()->Accept(*this);
    node.ParameterList()->Accept(*this);
    if (node.Colon())
    {
        node.Colon()->Accept(*this);
    }
    node.InitializerList()->Accept(*this);
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(DestructorNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.Complement()->Accept(*this);
    node.ClassName()->Accept(*this);
    node.LParen()->Accept(*this);
    node.RParen()->Accept(*this);
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(MemberFunctionNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    if (node.Specifiers())
    {
        node.Specifiers()->Accept(*this);
    }
    node.ReturnType()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.ParameterList()->Accept(*this);
    if (node.ConstKeyword())
    {
        node.ConstKeyword()->Accept(*this);
    }
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ConversionFunctionNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.OperatorKeyword()->Accept(*this);
    node.Type()->Accept(*this);
    node.LParen()->Accept(*this);
    node.RParen()->Accept(*this);
    if (node.ConstKeyword())
    {
        node.ConstKeyword()->Accept(*this);
    }
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(MemberVariableNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.Type()->Accept(*this);
    node.Id()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(CompileUnitNode& node)
{
    node.GlobalNs()->Accept(*this);
}

void DefaultVisitor::Visit(ConceptIdNode& node)
{
    if (node.RefinedConcept())
    {
        node.RefinedConcept()->Accept(*this);
    }
    node.LAngle()->Accept(*this);
    node.TypeParameterList()->Accept(*this);
    node.RAngle()->Accept(*this);
}

void DefaultVisitor::Visit(DisjunctiveConstraintNode& node)
{
    node.Left()->Accept(*this);
    node.Op()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ConjunctiveConstraintNode& node)
{
    node.Left()->Accept(*this);
    node.Op()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ParenthesizedConstraintNode& node)
{
    node.LParen()->Accept(*this);
    node.ConstraintExpr()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(PredicateConstraintNode& node)
{
    node.InvokeExpr()->Accept(*this);
}

void DefaultVisitor::Visit(IsConstraintNode& node)
{
    node.TypeExpr()->Accept(*this);
    node.IsKeyword()->Accept(*this);
    node.ConceptOrTypeName()->Accept(*this);
}

void DefaultVisitor::Visit(MultiparamConstraintNode& node)
{
    node.ConceptName()->Accept(*this);
    node.LAngle()->Accept(*this);
    node.TypeExprList()->Accept(*this);
    node.RAngle()->Accept(*this);
}

void DefaultVisitor::Visit(TypeNameConstraintNode& node)
{
    node.TypeNameKeyword()->Accept(*this);
    node.TypeExpr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(ConstructorConstraintNode& node)
{
    node.TypeParamId()->Accept(*this);
    node.ParameterList()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(DestructorConstraintNode& node)
{
    node.Complement()->Accept(*this);
    node.TypeParamId()->Accept(*this);
    node.LParen()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(MemberFunctionConstraintNode& node)
{
    node.ReturnType()->Accept(*this);
    node.TypeParamId()->Accept(*this);
    node.Dot()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.ParameterList()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(FunctionConstraintNode& node)
{
    node.ReturnType()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.ParameterList()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(WhereConstraintNode& node)
{
    node.WhereKeyword()->Accept(*this);
    node.ConstraintExpr()->Accept(*this);
}

void DefaultVisitor::Visit(EmbeddedConstraintNode& node)
{
    node.WhereConstraint()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(AxiomStatementNode& node)
{
    node.Expression()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(AxiomNode& node)
{
    if (node.AxiomId())
    {
        node.AxiomId()->Accept(*this);
    }
    node.ParameterList()->Accept(*this);
    node.LBrace()->Accept(*this);
    node.AxiomStatements()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(ConceptNode& node)
{
    node.Specifiers()->Accept(*this);
    node.ConceptKeyword()->Accept(*this);
    node.ConceptId()->Accept(*this);
    node.LAngle()->Accept(*this);
    node.TypeParameterList()->Accept(*this);
    node.RAngle()->Accept(*this);
    if (node.Refinement())
    {
        node.Refinement()->Accept(*this);
    }
    if (node.WhereConstraint())
    {
        node.WhereConstraint()->Accept(*this);
    }
    node.LBrace()->Accept(*this);
    node.BodyConstraints()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(ConstantNode& node)
{
    node.Specifiers()->Accept(*this);
    node.ConstKeyword()->Accept(*this);
    node.Type()->Accept(*this);
    node.Id()->Accept(*this);
    node.Assign()->Accept(*this);
    node.Expr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(DelegateNode& node)
{
    node.Specifiers()->Accept(*this);
    node.DelegateKeyword()->Accept(*this);
    node.ReturnType()->Accept(*this);
    node.Id()->Accept(*this);
    node.ParameterList()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(ClassDelegateNode& node)
{
    node.Specifiers()->Accept(*this);
    node.ClassKeyword()->Accept(*this);
    node.DelegateKeyword()->Accept(*this);
    node.ReturnType()->Accept(*this);
    node.Id()->Accept(*this);
    node.ParameterList()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(EnumConstantNode& node)
{
    node.Id()->Accept(*this);
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
        node.Value()->Accept(*this);
    }
}

void DefaultVisitor::Visit(EnumTypeNode& node)
{
    node.Specifiers()->Accept(*this);
    node.EnumKeyword()->Accept(*this);
    node.Id()->Accept(*this);
    if (node.Colon())
    {
        node.Colon()->Accept(*this);
        node.UnderlyingType()->Accept(*this);
    }
    node.LBrace()->Accept(*this);
    node.EnumConstantList()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(EquivalenceNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ImplicationNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(DisjunctionNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(BitOrNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(BitXorNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(EqualNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(NotEqualNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(LessEqualNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(GreaterEqualNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ShiftLeftNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ShiftRightNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(PlusNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(MinusNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(DivNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(RemNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(IncrementNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(DecrementNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(NotNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ComplementNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(BinaryExprNode& node)
{
    node.Left()->Accept(*this);
    node.Operator()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(PrefixExprNode& node)
{
    node.Operator()->Accept(*this);
    node.Operand()->Accept(*this);
}

void DefaultVisitor::Visit(PostfixExprNode& node)
{
    node.Operand()->Accept(*this);
    node.Operator()->Accept(*this);
}

void DefaultVisitor::Visit(SubscriptExprNode& node)
{
    node.Operand()->Accept(*this);
    node.LBracket()->Accept(*this);
    if (node.Index())
    {
        node.Index()->Accept(*this);
    }
    node.RBracket()->Accept(*this);
}

void DefaultVisitor::Visit(InvokeExprNode& node)
{
    node.Operand()->Accept(*this);
    node.LParen()->Accept(*this);
    node.ArgumentList()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(IsExprNode& node)
{
    node.Left()->Accept(*this);
    node.Operator()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(AsExprNode& node)
{
    node.Left()->Accept(*this);
    node.Operator()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(DotMemberNode& node)
{
    node.Left()->Accept(*this);
    node.Dot()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ArrowMemberNode& node)
{
    node.Left()->Accept(*this);
    node.Arrow()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ParenthesizedExprNode& node)
{
    node.LParen()->Accept(*this);
    node.Operand()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(SizeOfExprNode& node)
{
    node.SizeOf()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Operand()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(TypeNameExprNode& node)
{
    node.TypeName()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Operand()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(TypeIdExprNode& node)
{
    node.TypeId()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Operand()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(CastExprNode& node)
{
    node.Cast()->Accept(*this);
    node.LAngle()->Accept(*this);
    node.Left()->Accept(*this);
    node.RAngle()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Right()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(ConstructExprNode& node)
{
    node.Construct()->Accept(*this);
    node.LAngle()->Accept(*this);
    node.Operand()->Accept(*this);
    node.RAngle()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Args().Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(NewExprNode& node)
{
    node.NewKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Args().Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(FunctionNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.ReturnType()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.TemplateParameterList()->Accept(*this);
    node.ParameterList()->Accept(*this);
    if (node.Constraint())
    {
        node.Constraint()->Accept(*this);
    }
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(OperatorFunctionGroupIdNode& node)
{
    node.OperatorKeyword()->Accept(*this);
    node.OperatorNodes()->Accept(*this);
}

void DefaultVisitor::Visit(GlobalVariableNode& node)
{
    node.Specifiers()->Accept(*this);
    node.Type()->Accept(*this);
    node.Id()->Accept(*this);
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
        node.Initializer()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(QualifiedIdNode& node)
{
    for (const auto& node : node.Nodes())
    {
        node->Accept(*this);
    }
}

void DefaultVisitor::Visit(IdentifierNode& node)
{
    VisitIdentifier(&node);
}

void DefaultVisitor::Visit(InterfaceNode& node)
{
    if (node.Attributes())
    {
        node.Attributes()->Accept(*this);
    }
    node.Specifiers()->Accept(*this);
    node.InterfaceKeyword()->Accept(*this);
    node.Id()->Accept(*this);
    node.LBrace()->Accept(*this);
    node.MemberFunctions()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(UsingKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(TypedefKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(NamespaceKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ConstKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AutoKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(BoolKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(SByteKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ByteKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ShortKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(UShortKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(IntKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(UIntKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(LongKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ULongKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(FloatKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DoubleKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(CharKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(WCharKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(UCharKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(VoidKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ThisKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(BaseKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(SizeOfKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(TypeNameKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(TypeIdKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(CastKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ConstructKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(NewKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(TrueKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(FalseKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(NullKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ConceptKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(WhereKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(OrKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AndKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(IsKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AsKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(OperatorKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AxiomKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ReturnKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(IfKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ElseKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(WhileKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DoKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ForKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(BreakKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ContinueKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(GotoKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(CaseKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DefaultKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(SwitchKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DeleteKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DestroyKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ThrowKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(TryKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(CatchKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AssertKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ElifKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(EndifKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ClassKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(DelegateKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(EnumKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(InterfaceKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(PublicKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ProtectedKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(PrivateKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(InternalKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(StaticKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(VirtualKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(OverrideKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(AbstractKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(InlineKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ExplicitKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ExternKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(SuppressKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(ConstExprKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(CDeclKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(NothrowKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(WinApiKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(UnitTestKeywordNode& node)
{
    VisitKeyword(&node);
}

void DefaultVisitor::Visit(BooleanLiteralNode& node)
{
    VisitKeyword(node.Rep());
}

void DefaultVisitor::Visit(SByteLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(ByteLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(ShortLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(UShortLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(IntLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(UIntLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(LongLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(ULongLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(FloatLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(DoubleLiteralNode& node)
{
    VisitNumber(node.Rep());
}

void DefaultVisitor::Visit(CharLiteralNode& node)
{
    VisitChar(node.Rep());
}

void DefaultVisitor::Visit(WCharLiteralNode& node)
{
    VisitChar(node.Rep());
}

void DefaultVisitor::Visit(UCharLiteralNode& node)
{
    VisitChar(node.Rep());
}

void DefaultVisitor::Visit(StringLiteralNode& node)
{
    VisitString(node.Rep());
}

void DefaultVisitor::Visit(WStringLiteralNode& node)
{
    VisitString(node.Rep());
}

void DefaultVisitor::Visit(UStringLiteralNode& node)
{
    VisitString(node.Rep());
}

void DefaultVisitor::Visit(NullLiteralNode& node)
{
    VisitKeyword(node.Rep());
}

void DefaultVisitor::Visit(ArrayLiteralNode& node)
{
    node.LBracket()->Accept(*this);
    node.ValueList().Accept(*this);
    node.RBracket()->Accept(*this);
}

void DefaultVisitor::Visit(StructuredLiteralNode& node)
{
    node.LBrace()->Accept(*this);
    node.ValueList().Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(NamespaceDefinitionNode& node)
{
    if (node.NamespaceKeyword())
    {
        node.NamespaceKeyword()->Accept(*this);
    }
    if (node.NamespaceName())
    {
        node.NamespaceName()->Accept(*this);
    }
    if (node.LBrace())
    {
        node.LBrace()->Accept(*this);
    }
    node.Content().Accept(*this);
    if (node.RBrace())
    {
        node.RBrace()->Accept(*this);
    }
}

void DefaultVisitor::Visit(NamespaceImportNode& node)
{
    if (node.UsingKeyword())
    {
        node.UsingKeyword()->Accept(*this);
    }
    node.QualifiedId()->Accept(*this);
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ParameterNode& node)
{
    node.TypeExpr()->Accept(*this);
    if (node.Name())
    {
        node.Name()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ParameterListNode& node)
{
    if (node.LParen())
    {
        node.LParen()->Accept(*this);
    }
    for (const auto& node : node.Nodes())
    {
        node->Accept(*this);
    }
    if (node.RParen())
    {
        node.RParen()->Accept(*this);
    }
}

void DefaultVisitor::Visit(DotNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ArrowNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(CommaNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(ColonNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(SemicolonNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(LBraceNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(RBraceNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(LBracketNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(RBracketNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(LParenNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(RParenNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(AssignNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(LAngleNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(RAngleNode& node)
{
    VisitParen(&node);
}

void DefaultVisitor::Visit(StarNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(AmpNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(AmpAmpNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(HashNode& node)
{
    VisitOperator(&node);
}

void DefaultVisitor::Visit(SpecifierNode& node)
{
    node.Keyword()->Accept(*this);
}

void DefaultVisitor::Visit(SpecifiersNode& node)
{
    for (const auto& specifierNode : node.SpecifierNodes())
    {
        specifierNode->Accept(*this);
    }
}

void DefaultVisitor::Visit(CompoundStatementNode& node)
{
    node.LBrace()->Accept(*this);
    node.Statements()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(ReturnStatementNode& node)
{
    node.ReturnKeyword()->Accept(*this);
    if (node.ReturnValue())
    {
        node.ReturnValue()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(IfStatementNode& node)
{
    node.IfKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Condition()->Accept(*this);
    node.RParen()->Accept(*this);
    node.ThenStatement()->Accept(*this);
    if (node.ElseKeyword())
    {
        node.ElseKeyword()->Accept(*this);
    }
    if (node.ElseStatement())
    {
        node.ElseStatement()->Accept(*this);
    }
}

void DefaultVisitor::Visit(WhileStatementNode& node)
{
    node.WhileKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Condition()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Statement()->Accept(*this);
}

void DefaultVisitor::Visit(DoStatementNode& node)
{
    node.DoKeyword()->Accept(*this);
    node.Statement()->Accept(*this);
    node.WhileKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Condition()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(RangeForStatementNode& node)
{
    node.ForKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.TypeExpr()->Accept(*this);
    node.Id()->Accept(*this);
    node.Colon()->Accept(*this);
    node.Container()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Statement()->Accept(*this);
}

void DefaultVisitor::Visit(ForStatementNode& node)
{
    node.ForKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.InitStatement()->Accept(*this);
    node.Condition()->Accept(*this);
    node.Semicolon()->Accept(*this);
    node.LoopStatement()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Statement()->Accept(*this);
}

void DefaultVisitor::Visit(BreakStatementNode& node)
{
    node.BreakKeyword()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(ContinueStatementNode& node)
{
    node.ContinueKeyword()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(GotoStatementNode& node)
{
    node.GotoKeyword()->Accept(*this);
    node.Target()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(GotoCaseStatementNode& node)
{
    node.GotoKeyword()->Accept(*this);
    node.CaseKeyword()->Accept(*this);
    node.CaseExpr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(GotoDefaultStatementNode& node)
{
    node.GotoKeyword()->Accept(*this);
    node.DefaultKeyword()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(CaseExprNode& node)
{
    node.CaseKeyword()->Accept(*this);
    node.CaseExpr()->Accept(*this);
    node.Colon()->Accept(*this);
}

void DefaultVisitor::Visit(CaseStatementNode& node)
{
    node.Cases()->Accept(*this);
    node.LBrace()->Accept(*this);
    node.Statements()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(DefaultStatementNode& node)
{
    node.DefaultKeyword()->Accept(*this);
    node.Colon()->Accept(*this);
    node.LBrace()->Accept(*this);
    node.Statements()->Accept(*this);
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(SwitchStatementNode& node)
{
    node.SwitchKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Condition()->Accept(*this);
    node.RParen()->Accept(*this);
    node.LBrace()->Accept(*this);
    node.CaseStatements()->Accept(*this);
    if (node.DefaultStatement())
    {
        node.DefaultStatement()->Accept(*this);
    }
    node.RBrace()->Accept(*this);
}

void DefaultVisitor::Visit(AssignmentStatementNode& node)
{
    node.Left()->Accept(*this);
    node.Assign()->Accept(*this);
    node.Right()->Accept(*this);
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ExpressionStatementNode& node)
{
    node.Expression()->Accept(*this);
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(EmptyStatementNode& node)
{
    if (node.Semicolon())
    {
        node.Semicolon()->Accept(*this);
    }
}

void DefaultVisitor::Visit(ConstructionStatementNode& node)
{
    node.TypeExpr()->Accept(*this);
    node.Id()->Accept(*this);
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
    }
    if (node.LParen())
    {
        node.LParen()->Accept(*this);
    }
    node.ArgumentList()->Accept(*this);
    if (node.RParen())
    {
        node.RParen()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(DeleteStatementNode& node)
{
    node.DeleteKeyword()->Accept(*this);
    node.Ptr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(DestroyStatementNode& node)
{
    node.DestroyKeyword()->Accept(*this);
    node.Ptr()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(ThrowStatementNode& node)
{
    node.ThrowKeyword()->Accept(*this);
    if (node.Exception())
    {
        node.Exception()->Accept(*this);
    }
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(CatchNode& node)
{
    node.CatchKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.TypeExpr()->Accept(*this);
    if (node.Id())
    {
        node.Id()->Accept(*this);
    }
    node.RParen()->Accept(*this);
    node.CatchBlock()->Accept(*this);
}

void DefaultVisitor::Visit(TryStatementNode& node)
{
    node.TryKeyword()->Accept(*this);
    node.TryBlock()->Accept(*this);
    node.Catches()->Accept(*this);
}

void DefaultVisitor::Visit(LabeledStatementNode& node)
{
    node.Id()->Accept(*this);
    node.Colon()->Accept(*this);
    node.Statement()->Accept(*this);
}

void DefaultVisitor::Visit(AssertStatementNode& node)
{
    node.Hash()->Accept(*this);
    node.AssertKeyword()->Accept(*this);
    node.Expression()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompDisjunctionNode& node)
{
    node.Left()->Accept(*this);
    node.Op()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompConjunctionNode& node)
{
    node.Left()->Accept(*this);
    node.Op()->Accept(*this);
    node.Right()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompNotNode& node)
{
    node.Not()->Accept(*this);
    node.Prefix()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompParenExprNode& node)
{
    node.LParen()->Accept(*this);
    node.Expr()->Accept(*this);
    node.RParen()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompSymbolNode& node)
{
    node.Symbol()->Accept(*this);
}

void DefaultVisitor::Visit(IfGroupNode& node)
{
    node.Hash()->Accept(*this);
    node.IfKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Expr()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Statements()->Accept(*this);
}

void DefaultVisitor::Visit(ElifGroupNode& node)
{
    node.Hash()->Accept(*this);
    node.ElifKeyword()->Accept(*this);
    node.LParen()->Accept(*this);
    node.Expr()->Accept(*this);
    node.RParen()->Accept(*this);
    node.Statements()->Accept(*this);
}

void DefaultVisitor::Visit(ElseGroupNode& node)
{
    node.Hash()->Accept(*this);
    node.ElseKeyword()->Accept(*this);
    node.Statements()->Accept(*this);
}

void DefaultVisitor::Visit(CondCompStatementNode& node)
{
    node.IfGroup()->Accept(*this);
    node.ElifGroups()->Accept(*this);
    if (node.ElseGroup())
    {
        node.ElseGroup()->Accept(*this);
    }
    node.Hash()->Accept(*this);
    node.EndifKeyword()->Accept(*this);
}

void DefaultVisitor::Visit(TemplateIdNode& node)
{
    node.Primary()->Accept(*this);
    node.LAngle()->Accept(*this);
    node.TemplateArgumentList()->Accept(*this);
    node.RAngle()->Accept(*this);
}

void DefaultVisitor::Visit(TemplateParameterNode& node)
{
    node.Identifier()->Accept(*this);
    if (node.Assign())
    {
        node.Assign()->Accept(*this);
    }
    if (node.DefaultValue())
    {
        node.DefaultValue()->Accept(*this);
    }
}

void DefaultVisitor::Visit(TemplateParameterListNode& node)
{
    if (node.LAngle())
    {
        node.LAngle()->Accept(*this);
    }
    for (const auto& node : node.Nodes())
    {
        node->Accept(*this);
    }
    if (node.RAngle())
    {
        node.RAngle()->Accept(*this);
    }
}

void DefaultVisitor::Visit(FullInstantiationRequest& node)
{
    node.NewKeyword()->Accept(*this);
    node.ClassKeyword()->Accept(*this);
    node.TemplateId()->Accept(*this);
    node.Semicolon()->Accept(*this);
}

void DefaultVisitor::Visit(PointerNode& node)
{
    node.Subject()->Accept(*this);
    node.Star()->Accept(*this);
}

void DefaultVisitor::Visit(LValueRefNode& node)
{
    node.Subject()->Accept(*this);
    node.Amp()->Accept(*this);
}

void DefaultVisitor::Visit(RValueRefNode& node)
{
    node.Subject()->Accept(*this);
    node.AmpAmp()->Accept(*this);
}

void DefaultVisitor::Visit(ArrayNode& node)
{
    node.Subject()->Accept(*this);
    node.LBracket()->Accept(*this);
    if (node.Size())
    {
        node.Size()->Accept(*this);
    }
    node.RBracket()->Accept(*this);
}

void DefaultVisitor::Visit(TypeExprNode& node)
{
    for (const auto& node : node.Nodes())
    {
        node->Accept(*this);
    }
}

} // namespace cmajor::fault::tolerant::ast
