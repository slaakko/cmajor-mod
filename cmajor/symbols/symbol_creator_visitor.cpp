// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.creator.visitor;

import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import cmajor.symbols.sources;
import cmajor.symbols.modules;
import cmajor.symbols.global.flags;
import soul.ast.span;

namespace cmajor::symbols {

SymbolCreatorVisitor::SymbolCreatorVisitor(SymbolTable& symbolTable_) :
    symbolTable(symbolTable_), classInstanceNode(nullptr), classTemplateSpecialization(nullptr), functionIndex(0), level(0)
{
    symbolTable.ResetAxiomNumber();
    symbolTable.ResetAliasNodesAndNamespaceImports();
}

void SymbolCreatorVisitor::SetClassInstanceNode(cmajor::ast::ClassNode* classInstanceNode_)
{
    classInstanceNode = classInstanceNode_;
}

void SymbolCreatorVisitor::SetClassTemplateSpecialization(ClassTemplateSpecializationSymbol* classTemplateSpecialization_)
{
    classTemplateSpecialization = classTemplateSpecialization_;
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CompileUnitNode& compileUnitNode)
{
    compileUnitNode.GlobalNs()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    symbolTable.BeginNamespace(namespaceNode);
    if (namespaceNode.Id())
    {
        namespaceNode.Id()->Accept(*this);
    }
    cmajor::ast::NodeList<cmajor::ast::Node>& members = namespaceNode.Members();
    int n = members.Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = members[i];
        member->Accept(*this);
    }
    symbolTable.EndNamespace();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AliasNode& aliasNode)
{
    aliasNode.Id()->Accept(*this);
    aliasNode.TypeExpr()->Accept(*this);
    AliasTypeSymbol* symbol = symbolTable.AddAliasType(aliasNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    namespaceImportNode.Ns()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    templateIdNode.Primary()->Accept(*this);
    const cmajor::ast::NodeList<cmajor::ast::Node>& args = templateIdNode.TemplateArguments();
    int n = args.Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* arg = args[i];
        arg->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FunctionNode& functionNode)
{
    symbolTable.BeginFunction(functionNode, functionIndex++);
    ++level;
    if (functionNode.ReturnTypeExpr())
    {
        functionNode.ReturnTypeExpr()->Accept(*this);
    }
    int nt = functionNode.TemplateParameters().Count();
    for (int i = 0; i < nt; ++i)
    {
        symbolTable.AddTemplateParameter(*functionNode.TemplateParameters()[i]);
    }
    int n = functionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = functionNode.Parameters()[i];
        parameterNode->Accept(*this);
    }
    if (functionNode.WhereConstraint())
    {
        functionNode.WhereConstraint()->Accept(*this);
    }
    if (nt == 0)
    {
        if (functionNode.Body())
        {
            functionNode.Body()->Accept(*this);
        }
    }
    --level;
    symbolTable.EndFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ClassNode& classNode)
{
    if (&classNode == classInstanceNode)
    {
        symbolTable.BeginClassTemplateSpecialization(*classInstanceNode, classTemplateSpecialization);
    }
    else
    {
        symbolTable.BeginClass(classNode);
    }
    ++level;
    if (classNode.Id()->Str() == U"Bitset")
    {
        int x = 0;
    }
    classNode.Id()->Accept(*this);
    int nt = classNode.TemplateParameters().Count();
    for (int i = 0; i < nt; ++i)
    {
        symbolTable.AddTemplateParameter(*classNode.TemplateParameters()[i]);
    }
    int nb = classNode.BaseClassOrInterfaces().Count();
    for (int i = 0; i < nb; ++i)
    {
        classNode.BaseClassOrInterfaces()[i]->Accept(*this);
    }
    if (nt == 0)
    {
        int n = classNode.Members().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* member = classNode.Members()[i];
            member->Accept(*this);
        }
    }
    --level;
    if (&classNode == classInstanceNode)
    {
        symbolTable.EndClassTemplateSpecialization();
    }
    else
    {
        symbolTable.EndClass();
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ThisInitializerNode& thisInitializerNode)
{
    int n = thisInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        thisInitializerNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BaseInitializerNode& baseInitializerNode)
{
    int n = baseInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        baseInitializerNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberInitializerNode& memberInitializerNode)
{
    memberInitializerNode.MemberId()->Accept(*this);
    int n = memberInitializerNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        memberInitializerNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    symbolTable.BeginStaticConstructor(staticConstructorNode, functionIndex++);
    ++level;
    int ni = staticConstructorNode.Initializers().Count();
    for (int i = 0; i < ni; ++i)
    {
        staticConstructorNode.Initializers()[i]->Accept(*this);
    }
    if (staticConstructorNode.WhereConstraint())
    {
        staticConstructorNode.WhereConstraint()->Accept(*this);
    }
    if (staticConstructorNode.Body())
    {
        staticConstructorNode.Body()->Accept(*this);
    }
    --level;
    symbolTable.EndStaticConstructor();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    symbolTable.BeginConstructor(constructorNode, functionIndex++);
    ++level;
    int ni = constructorNode.Initializers().Count();
    for (int i = 0; i < ni; ++i)
    {
        constructorNode.Initializers()[i]->Accept(*this);
    }
    if (constructorNode.WhereConstraint())
    {
        constructorNode.WhereConstraint()->Accept(*this);
    }
    int n = constructorNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = constructorNode.Parameters()[i];
        parameterNode->Accept(*this);
    }
    if (constructorNode.Body())
    {
        constructorNode.Body()->Accept(*this);
    }
    --level;
    symbolTable.EndConstructor();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    symbolTable.BeginDestructor(destructorNode, functionIndex++);
    ++level;
    if (destructorNode.Body())
    {
        destructorNode.Body()->Accept(*this);
    }
    --level;
    symbolTable.EndDestructor();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    symbolTable.BeginMemberFunction(memberFunctionNode, functionIndex++);
    ++level;
    if (memberFunctionNode.WhereConstraint())
    {
        memberFunctionNode.WhereConstraint()->Accept(*this);
    }
    if (memberFunctionNode.ReturnTypeExpr())
    {
        memberFunctionNode.ReturnTypeExpr()->Accept(*this);
    }
    int n = memberFunctionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = memberFunctionNode.Parameters()[i];
        parameterNode->Accept(*this);
    }
    if (memberFunctionNode.Body())
    {
        memberFunctionNode.Body()->Accept(*this);
    }
    --level;
    symbolTable.EndMemberFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    symbolTable.BeginConversionFunction(conversionFunctionNode, functionIndex++);
    ++level;
    if (conversionFunctionNode.WhereConstraint())
    {
        conversionFunctionNode.WhereConstraint()->Accept(*this);
    }
    if (conversionFunctionNode.ReturnTypeExpr())
    {
        conversionFunctionNode.ReturnTypeExpr()->Accept(*this);
    }
    if (conversionFunctionNode.Body())
    {
        conversionFunctionNode.Body()->Accept(*this);
    }
    --level;
    symbolTable.EndConversionFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    symbolTable.AddMemberVariable(memberVariableNode);
    memberVariableNode.TypeExpr()->Accept(*this);
    memberVariableNode.Id()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::InterfaceNode& interfaceNode)
{
    symbolTable.BeginInterface(interfaceNode);
    ++level;
    interfaceNode.Id()->Accept(*this);
    int n = interfaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = interfaceNode.Members()[i];
        member->Accept(*this);
    }
    --level;
    symbolTable.EndInterface();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DelegateNode& delegateNode)
{
    symbolTable.BeginDelegate(delegateNode);
    ++level;
    delegateNode.ReturnTypeExpr()->Accept(*this);
    delegateNode.Id()->Accept(*this);
    int n = delegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = delegateNode.Parameters()[i];
        parameterNode->Accept(*this);
    }
    --level;
    symbolTable.EndDelegate();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ClassDelegateNode& classDelegateNode)
{
    symbolTable.BeginClassDelegate(classDelegateNode);
    ++level;
    classDelegateNode.ReturnTypeExpr()->Accept(*this);
    classDelegateNode.Id()->Accept(*this);
    int n = classDelegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = classDelegateNode.Parameters()[i];
        parameterNode->Accept(*this);
    }
    --level;
    symbolTable.EndClassDelegate();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    parenthesizedConstraintNode.Constraint()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    disjunctiveConstraintNode.Left()->Accept(*this);
    disjunctiveConstraintNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    conjunctiveConstraintNode.Left()->Accept(*this);
    conjunctiveConstraintNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    whereConstraintNode.Constraint()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode)
{
    predicateConstraintNode.InvokeExpr()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    isConstraintNode.TypeExpr()->Accept(*this);
    isConstraintNode.ConceptOrTypeName()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    multiParamConstraintNode.ConceptId()->Accept(*this);
    int n = multiParamConstraintNode.TypeExprs().Count();
    for (int i = 0; i < n; ++i)
    {
        multiParamConstraintNode.TypeExprs()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode)
{
    typeNameConstraintNode.TypeId()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode)
{
    constructorConstraintNode.TypeParamId()->Accept(*this);
    int n = constructorConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        constructorConstraintNode.Parameters()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode)
{
    destructorConstraintNode.TypeParamId()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode)
{
    memberFunctionConstraintNode.ReturnTypeExpr()->Accept(*this);
    memberFunctionConstraintNode.TypeParamId()->Accept(*this);
    int n = memberFunctionConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        memberFunctionConstraintNode.Parameters()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode)
{
    functionConstraintNode.ReturnTypeExpr()->Accept(*this);
    int n = functionConstraintNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        functionConstraintNode.Parameters()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode)
{
    axiomStatementNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AxiomNode& axiomNode)
{
    symbolTable.BeginAxiom(axiomNode);
    ++level;
    if (axiomNode.Id())
    {
        axiomNode.Id()->Accept(*this);
    }
    int n = axiomNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        axiomNode.Parameters()[i]->Accept(*this);
    }
    int ns = axiomNode.Statements().Count();
    for (int i = 0; i < ns; ++i)
    {
        axiomNode.Statements()[i]->Accept(*this);
    }
    --level;
    symbolTable.EndAxiom();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    conceptIdNode.Id()->Accept(*this);
    int n = conceptIdNode.TypeParameters().Count();
    for (int i = 0; i < n; ++i)
    {
        conceptIdNode.TypeParameters()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    symbolTable.BeginConcept(conceptNode, true);
    ++level;
    conceptNode.Id()->Accept(*this);
    int n = conceptNode.TypeParameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::IdentifierNode* identifierNode = conceptNode.TypeParameters()[i];
        symbolTable.AddTemplateParameter(*identifierNode);
    }
    if (conceptNode.Refinement())
    {
        conceptNode.Refinement()->Accept(*this);
    }
    int nc = conceptNode.Constraints().Count();
    for (int i = 0; i < nc; ++i)
    {
        conceptNode.Constraints()[i]->Accept(*this);
    }
    int na = conceptNode.Axioms().Count();
    for (int i = 0; i < na; ++i)
    {
        conceptNode.Axioms()[i]->Accept(*this);
    }
    --level;
    symbolTable.EndConcept();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Label()->Accept(*this);
    labeledStatementNode.Stmt()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    symbolTable.BeginDeclarationBlock(compoundStatementNode);
    ++level;
    int n = compoundStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statement = compoundStatementNode.Statements()[i];
        statement->Accept(*this);
    }
    --level;
    symbolTable.EndDeclarationBlock();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ReturnStatementNode& returnStatementNode)
{
    if (returnStatementNode.Expression())
    {
        returnStatementNode.Expression()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    ifStatementNode.Condition()->Accept(*this);
    ifStatementNode.ThenS()->Accept(*this);
    if (ifStatementNode.ElseS())
    {
        ifStatementNode.ElseS()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    whileStatementNode.Condition()->Accept(*this);
    whileStatementNode.Statement()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    doStatementNode.Statement()->Accept(*this);
    if (doStatementNode.Condition())
    {
        doStatementNode.Condition()->Accept(*this);
    }
    else
    {
        throw Exception("condition expected", doStatementNode.GetFullSpan());
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    symbolTable.BeginDeclarationBlock(forStatementNode);
    ++level;
    forStatementNode.InitS()->Accept(*this);
    if (forStatementNode.Condition())
    {
        forStatementNode.Condition()->Accept(*this);
    }
    else
    {
        throw Exception("condition expected", forStatementNode.GetFullSpan());
    }
    if (forStatementNode.LoopS())
    {
        forStatementNode.LoopS()->Accept(*this);
    }
    else
    {
        throw Exception("loop expression expected", forStatementNode.GetFullSpan());
    }
    if (forStatementNode.ActionS())
    {
        forStatementNode.ActionS()->Accept(*this);
    }
    else
    {
        throw Exception("action expected", forStatementNode.GetFullSpan());
    }
    --level;
    symbolTable.EndDeclarationBlock();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    symbolTable.AddLocalVariable(constructionStatementNode);
    constructionStatementNode.TypeExpr()->Accept(*this);
    if (constructionStatementNode.Id())
    {
        constructionStatementNode.Id()->Accept(*this);
    }
    else
    {
        throw Exception("identifier expected", constructionStatementNode.GetFullSpan());
    }
    int n = constructionStatementNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        constructionStatementNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    deleteStatementNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode)
{
    destroyStatementNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode)
{
    assignmentStatementNode.TargetExpr()->Accept(*this);
    assignmentStatementNode.SourceExpr()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode)
{
    expressionStatementNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    switchStatementNode.Condition()->Accept(*this);
    int n = switchStatementNode.Cases().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::CaseStatementNode* caseStatementNode = switchStatementNode.Cases()[i];
        caseStatementNode->Accept(*this);
    }
    if (switchStatementNode.Default())
    {
        switchStatementNode.Default()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    int nc = caseStatementNode.CaseExprs().Count();
    for (int i = 0; i < nc; ++i)
    {
        caseStatementNode.CaseExprs()[i]->Accept(*this);
    }
    int n = caseStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = caseStatementNode.Statements()[i];
        statementNode->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    int n = defaultStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = defaultStatementNode.Statements()[i];
        statementNode->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    tryStatementNode.TryBlock()->Accept(*this);
    int n = tryStatementNode.Catches().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::CatchNode* catchNode = tryStatementNode.Catches()[i];
        catchNode->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CatchNode& catchNode)
{
    symbolTable.BeginDeclarationBlock(catchNode);
    if (catchNode.Id())
    {
        symbolTable.AddLocalVariable(*catchNode.Id());
    }
    catchNode.CatchBlock()->Accept(*this);
    symbolTable.EndDeclarationBlock();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AssertStatementNode& assertStatementNode)
{
    assertStatementNode.AssertExpr()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode)
{
    conditionalCompilationPartNode.Expr()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    conditionalCompilationDisjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationDisjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left || right);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    conditionalCompilationConjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationConjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left && right);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    conditionalCompilationNotNode.Expr()->Accept(*this);
    bool operand = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(!operand);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    bool defined = symbolTable.GetModule()->IsSymbolDefined(conditionalCompilationPrimaryNode.Symbol());
    conditionalCompilationStack.push(defined);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    conditionalCompilationStatementNode.IfPart()->Accept(*this);
    bool defined = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    if (defined)
    {
        int n = conditionalCompilationStatementNode.IfPart()->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statement = conditionalCompilationStatementNode.IfPart()->Statements()[i];
            statement->Accept(*this);
        }
    }
    else
    {
        bool executed = false;
        int n = conditionalCompilationStatementNode.ElifParts().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::ConditionalCompilationPartNode* elifPart = conditionalCompilationStatementNode.ElifParts()[i];
            elifPart->Accept(*this);
            bool defined = conditionalCompilationStack.top();
            conditionalCompilationStack.pop();
            if (defined)
            {
                int n = elifPart->Statements().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::StatementNode* statement = elifPart->Statements()[i];
                    statement->Accept(*this);
                }
                executed = true;
                break;
            }
        }
        if (!executed)
        {
            cmajor::ast::ConditionalCompilationPartNode* elsePart = conditionalCompilationStatementNode.ElsePart();
            if (elsePart)
            {
                int n = elsePart->Statements().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::StatementNode* statement = elsePart->Statements()[i];
                    statement->Accept(*this);
                }
            }
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypedefNode& typedefNode)
{
    AliasTypeSymbol* symbol = symbolTable.AddAliasType(typedefNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstantNode& constantNode)
{
    ConstantSymbol* symbol = symbolTable.AddConstant(constantNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EnumTypeNode& enumTypeNode)
{
    symbolTable.BeginEnumType(enumTypeNode);
    ++level;
    enumTypeNode.Id()->Accept(*this);
    if (enumTypeNode.GetUnderlyingType())
    {
        enumTypeNode.GetUnderlyingType()->Accept(*this);
    }
    int n = enumTypeNode.Constants().Count();
    for (int i = 0; i < n; ++i)
    {
        enumTypeNode.Constants()[i]->Accept(*this);
    }
    --level;
    symbolTable.EndEnumType();
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EnumConstantNode& enumConstantNode)
{
    symbolTable.AddEnumConstant(enumConstantNode);
    enumConstantNode.Id()->Accept(*this);
    if (enumConstantNode.GetValue())
    {
        enumConstantNode.GetValue()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GlobalVariableNode& globalVariableNode)
{
    GlobalVariableSymbol* symbol = symbolTable.AddGlobalVariable(globalVariableNode);
    globalVariableNode.Id()->Accept(*this);
    if (globalVariableNode.Initializer())
    {
        globalVariableNode.Initializer()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParameterNode& parameterNode)
{
    switch (parameterNode.Parent()->GetNodeType())
    {
    case cmajor::ast::NodeType::functionNode:
    case cmajor::ast::NodeType::constructorNode:
    case cmajor::ast::NodeType::memberFunctionNode:
    case cmajor::ast::NodeType::delegateNode:
    case cmajor::ast::NodeType::classDelegateNode:
    {
        symbolTable.AddParameter(parameterNode);
        break;
    }
    }
    parameterNode.TypeExpr()->Accept(*this);
    if (parameterNode.Id())
    {
        parameterNode.Id()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TemplateParameterNode& templateParameterNode)
{
    templateParameterNode.Id()->Accept(*this);
    if (templateParameterNode.DefaultTemplateArgument())
    {
        templateParameterNode.DefaultTemplateArgument()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstNode& constNode)
{
    constNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LValueRefNode& lvalueRefNode)
{
    lvalueRefNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::RValueRefNode& rvalueRefNode)
{
    rvalueRefNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PointerNode& pointerNode)
{
    pointerNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ArrayNode& arrayNode)
{
    arrayNode.Subject()->Accept(*this);
    if (arrayNode.Size())
    {
        arrayNode.Size()->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DotNode& dotNode)
{
    dotNode.Subject()->Accept(*this);
    dotNode.MemberId()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ArrowNode& arrowNode)
{
    arrowNode.Subject()->Accept(*this);
    arrowNode.MemberId()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EquivalenceNode& equivalenceNode)
{
    equivalenceNode.Left()->Accept(*this);
    equivalenceNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ImplicationNode& implicationNode)
{
    implicationNode.Left()->Accept(*this);
    implicationNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DisjunctionNode& disjunctionNode)
{
    disjunctionNode.Left()->Accept(*this);
    disjunctionNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConjunctionNode& conjunctionNode)
{
    conjunctionNode.Left()->Accept(*this);
    conjunctionNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitOrNode& bitOrNode)
{
    bitOrNode.Left()->Accept(*this);
    bitOrNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitXorNode& bitXorNode)
{
    bitXorNode.Left()->Accept(*this);
    bitXorNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitAndNode& bitAndNode)
{
    bitAndNode.Left()->Accept(*this);
    bitAndNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EqualNode& equalNode)
{
    equalNode.Left()->Accept(*this);
    equalNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NotEqualNode& notEqualNode)
{
    notEqualNode.Left()->Accept(*this);
    notEqualNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LessNode& lessNode)
{
    lessNode.Left()->Accept(*this);
    lessNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GreaterNode& greaterNode)
{
    greaterNode.Left()->Accept(*this);
    greaterNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode)
{
    lessOrEqualNode.Left()->Accept(*this);
    lessOrEqualNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode)
{
    greaterOrEqualNode.Left()->Accept(*this);
    greaterOrEqualNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode)
{
    shiftLeftNode.Left()->Accept(*this);
    shiftLeftNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShiftRightNode& shiftRightNode)
{
    shiftRightNode.Left()->Accept(*this);
    shiftRightNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AddNode& addNode)
{
    addNode.Left()->Accept(*this);
    addNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SubNode& subNode)
{
    subNode.Left()->Accept(*this);
    subNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MulNode& mulNode)
{
    mulNode.Left()->Accept(*this);
    mulNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DivNode& divNode)
{
    divNode.Left()->Accept(*this);
    divNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::RemNode& remNode)
{
    remNode.Left()->Accept(*this);
    remNode.Right()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NotNode& notNode)
{
    notNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode)
{
    unaryPlusNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode)
{
    unaryMinusNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode)
{
    prefixIncrementNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode)
{
    prefixDecrementNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DerefNode& derefNode)
{
    derefNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AddrOfNode& addrOfNode)
{
    addrOfNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ComplementNode& complementNode)
{
    complementNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IsNode& isNode)
{
    isNode.Expr()->Accept(*this);
    if (isNode.TargetTypeExpr())
    {
        isNode.TargetTypeExpr()->Accept(*this);
    }
    else
    {
        throw Exception("target type expected", isNode.GetFullSpan());
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AsNode& asNode)
{
    asNode.Expr()->Accept(*this);
    if (asNode.TargetTypeExpr())
    {
        asNode.TargetTypeExpr()->Accept(*this);
    }
    else
    {
        throw Exception("target type expected", asNode.GetFullSpan());
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IndexingNode& indexingNode)
{
    indexingNode.Subject()->Accept(*this);
    if (indexingNode.Index())
    {
        indexingNode.Index()->Accept(*this);
    }
    else
    {
        throw Exception("index expected", indexingNode.GetFullSpan());
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::InvokeNode& invokeNode)
{
    invokeNode.Subject()->Accept(*this);
    int n = invokeNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        invokeNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode)
{
    postfixIncrementNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode)
{
    postfixDecrementNode.Subject()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SizeOfNode& sizeOfNode)
{
    sizeOfNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeNameNode& typeNameNode)
{
    typeNameNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeIdNode& typeIdNode)
{
    typeIdNode.Expression()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CastNode& castNode)
{
    castNode.TargetTypeExpr()->Accept(*this);
    castNode.SourceExpr()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructNode& constructNode)
{
    constructNode.TypeExpr()->Accept(*this);
    int n = constructNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        constructNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NewNode& newNode)
{
    newNode.TypeExpr()->Accept(*this);
    int n = newNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        newNode.Arguments()[i]->Accept(*this);
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode)
{
    parenthesizedExpressionNode.Subject()->Accept(*this);
}

} // namespace cmajor::symbols
