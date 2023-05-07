// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.creator.visitor;

import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import cmajor.symbols.sources;
import cmajor.symbols.modules;
import cmajor.symbols.global.flags;
import soul.ast.source.pos;

namespace cmajor::symbols {

SymbolCreatorVisitor::SymbolCreatorVisitor(SymbolTable& symbolTable_) :
    symbolTable(symbolTable_), classInstanceNode(nullptr), classTemplateSpecialization(nullptr), functionIndex(0), leaveFunction(false), editMode(false), level(0), source(nullptr)
{
    symbolTable.ResetCursorContainer();
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
    try
    {
        compileUnitNode.GlobalNs()->Accept(*this);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    bool namespaceAdded = false;
    try
    {
        symbolTable.BeginNamespace(namespaceNode);
        namespaceAdded = true;
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
        symbolTable.SetCursorContainer(namespaceNode);
        symbolTable.EndNamespace();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (namespaceAdded)
            {
                symbolTable.EndNamespace();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (namespaceAdded)
            {
                symbolTable.EndNamespace();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AliasNode& aliasNode)
{
    try
    {
        aliasNode.Id()->Accept(*this);
        aliasNode.TypeExpr()->Accept(*this);
        if (editMode)
        {
            symbolTable.AddAliasNode(&aliasNode);
        }
        symbolTable.SetCursorContainer(aliasNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    try
    {
        namespaceImportNode.Ns()->Accept(*this);
        if (editMode)
        {
            symbolTable.AddNamespaceImport(&namespaceImportNode);
        }
        symbolTable.SetCursorContainer(namespaceImportNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    symbolTable.SetCursorContainer(identifierNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CursorIdNode& cursorIdNode)
{
    symbolTable.SetCursorContainer(cursorIdNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    try
    {
        templateIdNode.Primary()->Accept(*this);
        const cmajor::ast::NodeList<cmajor::ast::Node>& args = templateIdNode.TemplateArguments();
        int n = args.Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* arg = args[i];
            arg->Accept(*this);
        }
        symbolTable.SetCursorContainer(templateIdNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FunctionNode& functionNode)
{
    bool functionAdded = false;
    try
    {
        symbolTable.BeginFunction(functionNode, functionIndex++);
        ++level;
        functionAdded = true;
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
        if (nt == 0 || editMode)
        {
            if (functionNode.Body())
            {
                functionNode.Body()->Accept(*this);
            }
        }
        --level;
        symbolTable.SetCursorContainer(functionNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.Container());
        }
*/
        symbolTable.EndFunction(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (functionAdded)
            {
                --level;
                symbolTable.EndFunction(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (functionAdded)
            {
                --level;
                symbolTable.EndFunction(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FunctionPtrNode& functionPtrNode)
{
    symbolTable.SetCursorContainer(functionPtrNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ClassNode& classNode)
{
    bool classAdded = false;
    try
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
        classAdded = true;
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
        if (nt == 0 || editMode)
        {
            int n = classNode.Members().Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::Node* member = classNode.Members()[i];
                member->Accept(*this);
            }
        }
        --level;
        symbolTable.SetCursorContainer(classNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.CurrentClass());
        }
*/
        if (&classNode == classInstanceNode)
        {
            symbolTable.EndClassTemplateSpecialization();
        }
        else
        {
            symbolTable.EndClass();
        }
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (classAdded)
            {
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
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (classAdded)
            {
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
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ThisInitializerNode& thisInitializerNode)
{
    try
    {
        int n = thisInitializerNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            thisInitializerNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(thisInitializerNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BaseInitializerNode& baseInitializerNode)
{
    try
    {
        int n = baseInitializerNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            baseInitializerNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(baseInitializerNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberInitializerNode& memberInitializerNode)
{
    try
    {
        memberInitializerNode.MemberId()->Accept(*this);
        int n = memberInitializerNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            memberInitializerNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(memberInitializerNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    bool staticConstructorAdded = false;
    try
    {
        symbolTable.BeginStaticConstructor(staticConstructorNode, functionIndex++);
        ++level;
        staticConstructorAdded = true;
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
            InsertTracer(staticConstructorNode.Body());
            staticConstructorNode.Body()->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(staticConstructorNode);
        symbolTable.EndStaticConstructor(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (staticConstructorAdded)
            {
                --level;
                symbolTable.EndStaticConstructor(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (staticConstructorAdded)
            {
                --level;
                symbolTable.EndStaticConstructor(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    bool constructorAdded = false;
    try
    {
        symbolTable.BeginConstructor(constructorNode, functionIndex++);
        ++level;
        constructorAdded = true;
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
            InsertTracer(constructorNode.Body());
            constructorNode.Body()->Accept(*this);
        }
        symbolTable.SetCursorContainer(constructorNode);
        --level;
        symbolTable.EndConstructor(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (constructorAdded)
            {
                --level;
                symbolTable.EndConstructor(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (constructorAdded)
            {
                --level;
                symbolTable.EndConstructor(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    bool destructorAdded = false;
    try
    {
        symbolTable.BeginDestructor(destructorNode, functionIndex++);
        ++level;
        destructorAdded = true;
        if (destructorNode.Body())
        {
            InsertTracer(destructorNode.Body());
            destructorNode.Body()->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(destructorNode);
        symbolTable.EndDestructor(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (destructorAdded)
            {
                --level;
                symbolTable.EndDestructor(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (destructorAdded)
            {
                --level;
                symbolTable.EndDestructor(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    bool memberFunctionAdded = false;
    try
    {
        symbolTable.BeginMemberFunction(memberFunctionNode, functionIndex++);
        ++level;
        memberFunctionAdded = true;
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
            InsertTracer(memberFunctionNode.Body());
            memberFunctionNode.Body()->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(memberFunctionNode);
        symbolTable.EndMemberFunction(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (memberFunctionAdded)
            {
                --level;
                symbolTable.EndMemberFunction(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (memberFunctionAdded)
            {
                --level;
                symbolTable.EndMemberFunction(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    bool conversionFunctionAdded = false;
    try
    {
        symbolTable.BeginConversionFunction(conversionFunctionNode, functionIndex++);
        ++level;
        conversionFunctionAdded = true;
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
            InsertTracer(conversionFunctionNode.Body());
            conversionFunctionNode.Body()->Accept(*this);
        }
        symbolTable.SetCursorContainer(conversionFunctionNode);
        --level;
        symbolTable.EndConversionFunction(!leaveFunction);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (conversionFunctionAdded)
            {
                --level;
                symbolTable.EndConversionFunction(!leaveFunction);
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (conversionFunctionAdded)
            {
                --level;
                symbolTable.EndConversionFunction(!leaveFunction);
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    try
    {
        symbolTable.AddMemberVariable(memberVariableNode);
        memberVariableNode.TypeExpr()->Accept(*this);
        memberVariableNode.Id()->Accept(*this);
        symbolTable.SetCursorContainer(memberVariableNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::InterfaceNode& interfaceNode)
{
    bool interfaceAdded = false;
    try
    {
        symbolTable.BeginInterface(interfaceNode);
        ++level;
        interfaceAdded = true;
        interfaceNode.Id()->Accept(*this);
        int n = interfaceNode.Members().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* member = interfaceNode.Members()[i];
            member->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(interfaceNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.CurrentInterface());
        }
*/
        symbolTable.EndInterface();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (interfaceAdded)
            {
                --level;
                symbolTable.EndInterface();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (interfaceAdded)
            {
                --level;
                symbolTable.EndInterface();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DelegateNode& delegateNode)
{
    bool delegateAdded = false;
    try
    {
        symbolTable.BeginDelegate(delegateNode);
        ++level;
        delegateAdded = true;
        delegateNode.ReturnTypeExpr()->Accept(*this);
        delegateNode.Id()->Accept(*this);
        int n = delegateNode.Parameters().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::ParameterNode* parameterNode = delegateNode.Parameters()[i];
            parameterNode->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(delegateNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.Container());
        }
*/
        symbolTable.EndDelegate();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (delegateAdded)
            {
                --level;
                symbolTable.EndDelegate();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (delegateAdded)
            {
                --level;
                symbolTable.EndDelegate();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ClassDelegateNode& classDelegateNode)
{
    bool classDelegateAdded = false;
    try
    {
        symbolTable.BeginClassDelegate(classDelegateNode);
        ++level;
        classDelegateAdded = true;
        classDelegateNode.ReturnTypeExpr()->Accept(*this);
        classDelegateNode.Id()->Accept(*this);
        int n = classDelegateNode.Parameters().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::ParameterNode* parameterNode = classDelegateNode.Parameters()[i];
            parameterNode->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(classDelegateNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.Container());
        }
*/
        symbolTable.EndClassDelegate();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (classDelegateAdded)
            {
                --level;
                symbolTable.EndClassDelegate();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (classDelegateAdded)
            {
                --level;
                symbolTable.EndClassDelegate();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    try
    {
        parenthesizedConstraintNode.Constraint()->Accept(*this);
        symbolTable.SetCursorContainer(parenthesizedConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    try
    {
        disjunctiveConstraintNode.Left()->Accept(*this);
        disjunctiveConstraintNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(disjunctiveConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    try
    {
        conjunctiveConstraintNode.Left()->Accept(*this);
        conjunctiveConstraintNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(conjunctiveConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    try
    {
        whereConstraintNode.Constraint()->Accept(*this);
        symbolTable.SetCursorContainer(whereConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PredicateConstraintNode& predicateConstraintNode)
{
    try
    {
        predicateConstraintNode.InvokeExpr()->Accept(*this);
        symbolTable.SetCursorContainer(predicateConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    try
    {
        isConstraintNode.TypeExpr()->Accept(*this);
        isConstraintNode.ConceptOrTypeName()->Accept(*this);
        symbolTable.SetCursorContainer(isConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    try
    {
        multiParamConstraintNode.ConceptId()->Accept(*this);
        int n = multiParamConstraintNode.TypeExprs().Count();
        for (int i = 0; i < n; ++i)
        {
            multiParamConstraintNode.TypeExprs()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(multiParamConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeNameConstraintNode& typeNameConstraintNode)
{
    try
    {
        typeNameConstraintNode.TypeId()->Accept(*this);
        symbolTable.SetCursorContainer(typeNameConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructorConstraintNode& constructorConstraintNode)
{
    try
    {
        constructorConstraintNode.TypeParamId()->Accept(*this);
        int n = constructorConstraintNode.Parameters().Count();
        for (int i = 0; i < n; ++i)
        {
            constructorConstraintNode.Parameters()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(constructorConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestructorConstraintNode& destructorConstraintNode)
{
    try
    {
        destructorConstraintNode.TypeParamId()->Accept(*this);
        symbolTable.SetCursorContainer(destructorConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MemberFunctionConstraintNode& memberFunctionConstraintNode)
{
    try
    {
        memberFunctionConstraintNode.ReturnTypeExpr()->Accept(*this);
        memberFunctionConstraintNode.TypeParamId()->Accept(*this);
        int n = memberFunctionConstraintNode.Parameters().Count();
        for (int i = 0; i < n; ++i)
        {
            memberFunctionConstraintNode.Parameters()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(memberFunctionConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FunctionConstraintNode& functionConstraintNode)
{
    try
    {
        functionConstraintNode.ReturnTypeExpr()->Accept(*this);
        int n = functionConstraintNode.Parameters().Count();
        for (int i = 0; i < n; ++i)
        {
            functionConstraintNode.Parameters()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(functionConstraintNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AxiomStatementNode& axiomStatementNode)
{
    try
    {
        axiomStatementNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(axiomStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AxiomNode& axiomNode)
{
    bool axiomAdded = false;
    try
    {
        symbolTable.BeginAxiom(axiomNode);
        ++level;
        axiomAdded = true;
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
        symbolTable.SetCursorContainer(axiomNode);
        symbolTable.EndAxiom();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (axiomAdded)
            {
                --level;
                symbolTable.EndAxiom();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (axiomAdded)
            {
                --level;
                symbolTable.EndAxiom();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    try
    {
        conceptIdNode.Id()->Accept(*this);
        int n = conceptIdNode.TypeParameters().Count();
        for (int i = 0; i < n; ++i)
        {
            conceptIdNode.TypeParameters()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(conceptIdNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SameConstraintNode& sameConstraintNode)
{
    symbolTable.SetCursorContainer(sameConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DerivedConstraintNode& derivedConstraintNode)
{
    symbolTable.SetCursorContainer(derivedConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConvertibleConstraintNode& convertibleConstraintNode)
{
    symbolTable.SetCursorContainer(convertibleConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ExplicitlyConvertibleConstraintNode& explicitlyConvertibleConstraintNode)
{
    symbolTable.SetCursorContainer(explicitlyConvertibleConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CommonConstraintNode& commonConstraintNode)
{
    symbolTable.SetCursorContainer(commonConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NonreferenceTypeConstraintNode& nonreferenceTypeConstraintNode)
{
    symbolTable.SetCursorContainer(nonreferenceTypeConstraintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    bool conceptAdded = false;
    try
    {
        symbolTable.BeginConcept(conceptNode, true);
        ++level;
        conceptAdded = true;
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
        symbolTable.SetCursorContainer(conceptNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.Container());
        }
*/
        symbolTable.EndConcept();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (conceptAdded)
            {
                --level;
                symbolTable.EndConcept();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (conceptAdded)
            {
                --level;
                symbolTable.EndConcept();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LabelNode& labelNode)
{
    symbolTable.SetCursorContainer(labelNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Label()->Accept(*this);
    symbolTable.SetCursorContainer(labeledStatementNode);
    labeledStatementNode.Stmt()->Accept(*this);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    bool declarationBlockAdded = false;
    try
    {
        symbolTable.BeginDeclarationBlock(compoundStatementNode);
        ++level;
        declarationBlockAdded = true;
        int n = compoundStatementNode.Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statement = compoundStatementNode.Statements()[i];
            statement->Accept(*this);
        }
        --level;
        symbolTable.SetCursorContainer(compoundStatementNode);
        symbolTable.EndDeclarationBlock();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (declarationBlockAdded)
            {
                --level;
                symbolTable.EndDeclarationBlock();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (declarationBlockAdded)
            {
                --level;
                symbolTable.EndDeclarationBlock();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ReturnStatementNode& returnStatementNode)
{
    try
    {
        if (returnStatementNode.Expression())
        {
            returnStatementNode.Expression()->Accept(*this);
        }
        symbolTable.SetCursorContainer(returnStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    try
    {
        ifStatementNode.Condition()->Accept(*this);
        ifStatementNode.ThenS()->Accept(*this);
        if (ifStatementNode.ElseS())
        {
            ifStatementNode.ElseS()->Accept(*this);
        }
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    try
    {
        whileStatementNode.Condition()->Accept(*this);
        whileStatementNode.Statement()->Accept(*this);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    try
    {
        doStatementNode.Statement()->Accept(*this);
        if (doStatementNode.Condition())
        {
            doStatementNode.Condition()->Accept(*this);
        }
        else
        {
            throw Exception("condition expected", doStatementNode.GetSourcePos(), doStatementNode.ModuleId());
        }
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    bool declarationBlockAdded = false;
    try
    {
        symbolTable.BeginDeclarationBlock(forStatementNode);
        ++level;
        declarationBlockAdded = true;
        forStatementNode.InitS()->Accept(*this);
        if (forStatementNode.Condition())
        {
            forStatementNode.Condition()->Accept(*this);
        }
        else
        {
            throw Exception("condition expected", forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
        }
        if (forStatementNode.LoopS())
        {
            forStatementNode.LoopS()->Accept(*this);
        }
        else
        {
            throw Exception("loop expression expected", forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
        }
        if (forStatementNode.ActionS())
        {
            forStatementNode.ActionS()->Accept(*this);
        }
        else
        {
            throw Exception("action expected", forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
        }
        --level;
        symbolTable.EndDeclarationBlock();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (declarationBlockAdded)
            {
                --level;
                symbolTable.EndDeclarationBlock();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (declarationBlockAdded)
            {
                --level;
                symbolTable.EndDeclarationBlock();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BreakStatementNode& breakStatementNode)
{
    symbolTable.SetCursorContainer(breakStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ContinueStatementNode& continueStatementNode)
{
    symbolTable.SetCursorContainer(continueStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GotoStatementNode& gotoStatementNode)
{
    symbolTable.SetCursorContainer(gotoStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    try
    {
        symbolTable.AddLocalVariable(constructionStatementNode);
        constructionStatementNode.TypeExpr()->Accept(*this);
        if (constructionStatementNode.Id())
        {
            constructionStatementNode.Id()->Accept(*this);
        }
        else
        {
            throw Exception("identifier expected", constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
        }
        int n = constructionStatementNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            constructionStatementNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(constructionStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }

}

void SymbolCreatorVisitor::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    try
    {
        deleteStatementNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(deleteStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode)
{
    try
    {
        destroyStatementNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(destroyStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode)
{
    try
    {
        assignmentStatementNode.TargetExpr()->Accept(*this);
        assignmentStatementNode.SourceExpr()->Accept(*this);
        symbolTable.SetCursorContainer(assignmentStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode)
{
    try
    {
        expressionStatementNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(expressionStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode)
{
    symbolTable.SetCursorContainer(emptyStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode)
{
    try
    {
        if (editMode)
        {
            rangeForStatementNode.TypeExpr()->Accept(*this);
            rangeForStatementNode.Id()->Accept(*this);
            rangeForStatementNode.Container()->Accept(*this);
            rangeForStatementNode.Action()->Accept(*this);
            symbolTable.SetCursorContainer(rangeForStatementNode);
        }
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    try
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
        symbolTable.SetCursorContainer(switchStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    try
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
        symbolTable.SetCursorContainer(caseStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    try
    {
        int n = defaultStatementNode.Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statementNode = defaultStatementNode.Statements()[i];
            statementNode->Accept(*this);
        }
        symbolTable.SetCursorContainer(defaultStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode)
{
    symbolTable.SetCursorContainer(gotoCaseStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode)
{
    symbolTable.SetCursorContainer(gotoDefaultStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ThrowStatementNode& throwStatementNode)
{
    symbolTable.SetCursorContainer(throwStatementNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    try
    {
        tryStatementNode.TryBlock()->Accept(*this);
        int n = tryStatementNode.Catches().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::CatchNode* catchNode = tryStatementNode.Catches()[i];
            catchNode->Accept(*this);
        }
        symbolTable.SetCursorContainer(tryStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CatchNode& catchNode)
{
    try
    {
        symbolTable.BeginDeclarationBlock(catchNode);
        if (catchNode.Id())
        {
            symbolTable.AddLocalVariable(*catchNode.Id());
        }
        catchNode.CatchBlock()->Accept(*this);
        symbolTable.EndDeclarationBlock();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            symbolTable.EndDeclarationBlock();
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AssertStatementNode& assertStatementNode)
{
    try
    {
        assertStatementNode.AssertExpr()->Accept(*this);
        symbolTable.SetCursorContainer(assertStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode)
{
    try
    {
        conditionalCompilationPartNode.Expr()->Accept(*this);
        symbolTable.SetCursorContainer(conditionalCompilationPartNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    try
    {
        conditionalCompilationDisjunctionNode.Left()->Accept(*this);
        bool left = conditionalCompilationStack.top();
        conditionalCompilationStack.pop();
        conditionalCompilationDisjunctionNode.Right()->Accept(*this);
        bool right = conditionalCompilationStack.top();
        conditionalCompilationStack.pop();
        conditionalCompilationStack.push(left || right);
        symbolTable.SetCursorContainer(conditionalCompilationDisjunctionNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    try
    {
        conditionalCompilationConjunctionNode.Left()->Accept(*this);
        bool left = conditionalCompilationStack.top();
        conditionalCompilationStack.pop();
        conditionalCompilationConjunctionNode.Right()->Accept(*this);
        bool right = conditionalCompilationStack.top();
        conditionalCompilationStack.pop();
        conditionalCompilationStack.push(left && right);
        symbolTable.SetCursorContainer(conditionalCompilationConjunctionNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    try
    {
        conditionalCompilationNotNode.Expr()->Accept(*this);
        bool operand = conditionalCompilationStack.top();
        conditionalCompilationStack.pop();
        conditionalCompilationStack.push(!operand);
        symbolTable.SetCursorContainer(conditionalCompilationNotNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    try
    {
        bool defined = symbolTable.GetModule()->IsSymbolDefined(conditionalCompilationPrimaryNode.Symbol());
        conditionalCompilationStack.push(defined);
        symbolTable.SetCursorContainer(conditionalCompilationPrimaryNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    try
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
        symbolTable.SetCursorContainer(conditionalCompilationStatementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypedefNode& typedefNode)
{
    TypedefSymbol* symbol = symbolTable.AddTypedef(typedefNode);
    symbolTable.SetCursorContainer(typedefNode);
/*  TODO 
    if (level == 0 && source)
    {
        source->AddSymbol(symbol);
    }
*/
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstantNode& constantNode)
{
    ConstantSymbol* symbol = symbolTable.AddConstant(constantNode);
    /*  TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbol);
        }
    */
    symbolTable.SetCursorContainer(constantNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EnumTypeNode& enumTypeNode)
{
    bool enumTypeAdded = false;
    try
    {
        symbolTable.BeginEnumType(enumTypeNode);
        ++level;
        enumTypeAdded = true;
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
        symbolTable.SetCursorContainer(enumTypeNode);
/*      TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbolTable.Container());
        }
*/
        symbolTable.EndEnumType();
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            if (enumTypeAdded)
            {
                --level;
                symbolTable.EndEnumType();
            }
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            if (enumTypeAdded)
            {
                --level;
                symbolTable.EndEnumType();
            }
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EnumConstantNode& enumConstantNode)
{
    try
    {
        symbolTable.AddEnumConstant(enumConstantNode);
        enumConstantNode.Id()->Accept(*this);
        if (enumConstantNode.GetValue())
        {
            enumConstantNode.GetValue()->Accept(*this);
        }
        symbolTable.SetCursorContainer(enumConstantNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GlobalVariableNode& globalVariableNode)
{
    try
    {
        GlobalVariableSymbol* symbol = symbolTable.AddGlobalVariable(globalVariableNode);
/* TODO
        if (level == 0 && source)
        {
            source->AddSymbol(symbol);
        }
*/
        globalVariableNode.TypeExpr()->Accept(*this);
        globalVariableNode.Id()->Accept(*this);
        if (globalVariableNode.Initializer())
        {
            globalVariableNode.Initializer()->Accept(*this);
        }
        symbolTable.SetCursorContainer(globalVariableNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParameterNode& parameterNode)
{
    try
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
        symbolTable.SetCursorContainer(parameterNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TemplateParameterNode& templateParameterNode)
{
    try
    {
        templateParameterNode.Id()->Accept(*this);
        if (templateParameterNode.DefaultTemplateArgument())
        {
            templateParameterNode.DefaultTemplateArgument()->Accept(*this);
        }
        symbolTable.SetCursorContainer(templateParameterNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstNode& constNode)
{
    try
    {
        constNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(constNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LValueRefNode& lvalueRefNode)
{
    try
    {
        lvalueRefNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(lvalueRefNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::RValueRefNode& rvalueRefNode)
{
    try
    {
        rvalueRefNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(rvalueRefNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PointerNode& pointerNode)
{
    try
    {
        pointerNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(pointerNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ArrayNode& arrayNode)
{
    try
    {
        arrayNode.Subject()->Accept(*this);
        if (arrayNode.Size())
        {
            arrayNode.Size()->Accept(*this);
        }
        symbolTable.SetCursorContainer(arrayNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DotNode& dotNode)
{
    try
    {
        dotNode.Subject()->Accept(*this);
        dotNode.MemberId()->Accept(*this);
        symbolTable.SetCursorContainer(dotNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ArrowNode& arrowNode)
{
    try
    {
        arrowNode.Subject()->Accept(*this);
        arrowNode.MemberId()->Accept(*this);
        symbolTable.SetCursorContainer(arrowNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EquivalenceNode& equivalenceNode)
{
    try
    {
        equivalenceNode.Left()->Accept(*this);
        equivalenceNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(equivalenceNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ImplicationNode& implicationNode)
{
    try
    {
        implicationNode.Left()->Accept(*this);
        implicationNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(implicationNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DisjunctionNode& disjunctionNode)
{
    try
    {
        disjunctionNode.Left()->Accept(*this);
        disjunctionNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(disjunctionNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConjunctionNode& conjunctionNode)
{
    try
    {
        conjunctionNode.Left()->Accept(*this);
        conjunctionNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(conjunctionNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitOrNode& bitOrNode)
{
    try
    {
        bitOrNode.Left()->Accept(*this);
        bitOrNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(bitOrNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitXorNode& bitXorNode)
{
    try
    {
        bitXorNode.Left()->Accept(*this);
        bitXorNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(bitXorNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BitAndNode& bitAndNode)
{
    try
    {
        bitAndNode.Left()->Accept(*this);
        bitAndNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(bitAndNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::EqualNode& equalNode)
{
    try
    {
        equalNode.Left()->Accept(*this);
        equalNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(equalNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NotEqualNode& notEqualNode)
{
    try
    {
        notEqualNode.Left()->Accept(*this);
        notEqualNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(notEqualNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LessNode& lessNode)
{
    try
    {
        lessNode.Left()->Accept(*this);
        lessNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(lessNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GreaterNode& greaterNode)
{
    try
    {
        greaterNode.Left()->Accept(*this);
        greaterNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(greaterNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode)
{
    try
    {
        lessOrEqualNode.Left()->Accept(*this);
        lessOrEqualNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(lessOrEqualNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode)
{
    try
    {
        greaterOrEqualNode.Left()->Accept(*this);
        greaterOrEqualNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(greaterOrEqualNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode)
{
    try
    {
        shiftLeftNode.Left()->Accept(*this);
        shiftLeftNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(shiftLeftNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShiftRightNode& shiftRightNode)
{
    try
    {
        shiftRightNode.Left()->Accept(*this);
        shiftRightNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(shiftRightNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AddNode& addNode)
{
    try
    {
        addNode.Left()->Accept(*this);
        addNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(addNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SubNode& subNode)
{
    try
    {
        subNode.Left()->Accept(*this);
        subNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(subNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::MulNode& mulNode)
{
    try
    {
        mulNode.Left()->Accept(*this);
        mulNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(mulNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DivNode& divNode)
{
    try
    {
        divNode.Left()->Accept(*this);
        divNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(divNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::RemNode& remNode)
{
    try
    {
        remNode.Left()->Accept(*this);
        remNode.Right()->Accept(*this);
        symbolTable.SetCursorContainer(remNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NotNode& notNode)
{
    try
    {
        notNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(notNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode)
{
    try
    {
        unaryPlusNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(unaryPlusNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode)
{
    try
    {
        unaryMinusNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(unaryMinusNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode)
{
    try
    {
        prefixIncrementNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(prefixIncrementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode)
{
    try
    {
        prefixDecrementNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(prefixDecrementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DerefNode& derefNode)
{
    try
    {
        derefNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(derefNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AddrOfNode& addrOfNode)
{
    try
    {
        addrOfNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(addrOfNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ComplementNode& complementNode)
{
    try
    {
        complementNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(complementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IsNode& isNode)
{
    try
    {
        isNode.Expr()->Accept(*this);
        if (isNode.TargetTypeExpr())
        {
            isNode.TargetTypeExpr()->Accept(*this);
        }
        else
        {
            throw Exception("target type expected", isNode.GetSourcePos(), isNode.ModuleId());
        }
        symbolTable.SetCursorContainer(isNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::AsNode& asNode)
{
    try
    {
        asNode.Expr()->Accept(*this);
        if (asNode.TargetTypeExpr())
        {
            asNode.TargetTypeExpr()->Accept(*this);
        }
        else
        {
            throw Exception("target type expected", asNode.GetSourcePos(), asNode.ModuleId());
        }
        symbolTable.SetCursorContainer(asNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IndexingNode& indexingNode)
{
    try
    {
        indexingNode.Subject()->Accept(*this);
        if (indexingNode.Index())
        {
            indexingNode.Index()->Accept(*this);
        }
        else
        {
            throw Exception("index expected", indexingNode.GetSourcePos(), indexingNode.ModuleId());
        }
        symbolTable.SetCursorContainer(indexingNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::InvokeNode& invokeNode)
{
    try
    {
        invokeNode.Subject()->Accept(*this);
        int n = invokeNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            invokeNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(invokeNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode)
{
    try
    {
        postfixIncrementNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(postfixIncrementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode)
{
    try
    {
        postfixDecrementNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(postfixDecrementNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SizeOfNode& sizeOfNode)
{
    try
    {
        sizeOfNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(sizeOfNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeNameNode& typeNameNode)
{
    try
    {
        typeNameNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(typeNameNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::TypeIdNode& typeIdNode)
{
    try
    {
        typeIdNode.Expression()->Accept(*this);
        symbolTable.SetCursorContainer(typeIdNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CastNode& castNode)
{
    try
    {
        castNode.TargetTypeExpr()->Accept(*this);
        castNode.SourceExpr()->Accept(*this);
        symbolTable.SetCursorContainer(castNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ConstructNode& constructNode)
{
    try
    {
        constructNode.TypeExpr()->Accept(*this);
        int n = constructNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            constructNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(constructNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NewNode& newNode)
{
    try
    {
        newNode.TypeExpr()->Accept(*this);
        int n = newNode.Arguments().Count();
        for (int i = 0; i < n; ++i)
        {
            newNode.Arguments()[i]->Accept(*this);
        }
        symbolTable.SetCursorContainer(newNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ThisNode& thisNode)
{
    symbolTable.SetCursorContainer(thisNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BaseNode& baseNode)
{
    symbolTable.SetCursorContainer(baseNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode)
{
    try
    {
        parenthesizedExpressionNode.Subject()->Accept(*this);
        symbolTable.SetCursorContainer(parenthesizedExpressionNode);
    }
    catch (const Exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.Message());
        }
        else
        {
            throw;
        }
    }
    catch (const std::exception& ex)
    {
        if (editMode)
        {
            errors.push_back(ex.what());
        }
        else
        {
            throw;
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CommentNode& commentNode)
{
    symbolTable.SetCursorContainer(commentNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BoolNode& boolNode)
{
    symbolTable.SetCursorContainer(boolNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    symbolTable.SetCursorContainer(sbyteNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ByteNode& byteNode)
{
    symbolTable.SetCursorContainer(byteNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShortNode& shortNode)
{
    symbolTable.SetCursorContainer(shortNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UShortNode& ushortNode)
{
    symbolTable.SetCursorContainer(ushortNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IntNode& intNode)
{
    symbolTable.SetCursorContainer(intNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UIntNode& uintNode)
{
    symbolTable.SetCursorContainer(uintNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LongNode& longNode)
{
    symbolTable.SetCursorContainer(longNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ULongNode& ulongNode)
{
    symbolTable.SetCursorContainer(ulongNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FloatNode& floatNode)
{
    symbolTable.SetCursorContainer(floatNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    symbolTable.SetCursorContainer(doubleNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CharNode& charNode)
{
    symbolTable.SetCursorContainer(charNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WCharNode& wcharNode)
{
    symbolTable.SetCursorContainer(wcharNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UCharNode& ucharNode)
{
    symbolTable.SetCursorContainer(ucharNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::VoidNode& voidNode)
{
    symbolTable.SetCursorContainer(voidNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode)
{
    symbolTable.SetCursorContainer(booleanLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode)
{
    symbolTable.SetCursorContainer(sbyteLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode)
{
    symbolTable.SetCursorContainer(byteLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode)
{
    symbolTable.SetCursorContainer(shortLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode)
{
    symbolTable.SetCursorContainer(ushortLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::IntLiteralNode& intLiteralNode)
{
    symbolTable.SetCursorContainer(intLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode)
{
    symbolTable.SetCursorContainer(uintLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::LongLiteralNode& longLiteralNode)
{
    symbolTable.SetCursorContainer(longLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode)
{
    symbolTable.SetCursorContainer(ulongLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode)
{
    symbolTable.SetCursorContainer(floatLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode)
{
    symbolTable.SetCursorContainer(doubleLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::CharLiteralNode& charLiteralNode)
{
    symbolTable.SetCursorContainer(charLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode)
{
    symbolTable.SetCursorContainer(wcharLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode)
{
    symbolTable.SetCursorContainer(ucharLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::StringLiteralNode& stringLiteralNode)
{
    symbolTable.SetCursorContainer(stringLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode)
{
    symbolTable.SetCursorContainer(wstringLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode)
{
    symbolTable.SetCursorContainer(ustringLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::NullLiteralNode& nullLiteralNode)
{
    symbolTable.SetCursorContainer(nullLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::ArrayLiteralNode& arrayLiteralNode)
{
    symbolTable.SetCursorContainer(arrayLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::StructuredLiteralNode& structuredLiteralNode)
{
    symbolTable.SetCursorContainer(structuredLiteralNode);
}

void SymbolCreatorVisitor::Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode)
{
    symbolTable.SetCursorContainer(uuidLiteralNode);
}

void SymbolCreatorVisitor::InsertTracer(cmajor::ast::CompoundStatementNode* body)
{
    if (!GetGlobalFlag(GlobalFlags::trace)) return;
    /* TODO
        Module* rootModule = GetRootModuleForCurrentThread();
    if (!rootModule) return;
    if (rootModule->IsCore()) return;
    if (rootModule->Name() == U"System.Runtime") return;
    if (rootModule->Name() == U"System.Parsing") return;
    if (body->TracerInserted()) return;
    body->SetTracerInserted();
    soul:ast::SourcePos sourcePos = body->GetSourcePos();
    util::uuid moduleId = body->ModuleId();
    DotNode* typeExprNode = new DotNode(sourcePos, moduleId, new DotNode(sourcePos, moduleId, new IdentifierNode(sourcePos, moduleId, U"System"), 
        new IdentifierNode(sourcePos, moduleId, U"Runtime")),
        new IdentifierNode(sourcePos, moduleId, U"Tracer"));
    ConstructionStatementNode* constructTracer(new ConstructionStatementNode(sourcePos, moduleId, typeExprNode, new IdentifierNode(sourcePos, moduleId, U"@tracer")));
    constructTracer->AddArgument(new IntLiteralNode(sourcePos, moduleId, -1));
    body->Statements().Insert(0, constructTracer);
*/
}

} // namespace cmajor::symbols
