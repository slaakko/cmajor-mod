// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.expression.binder;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.bound.function;
import cmajor.binder.bound.expression;
import cmajor.binder.overload.resolution;
import cmajor.binder.statement.binder;
import cmajor.binder.type.resolver;
import cmajor.binder.evaluator;
import cmajor.binder.access;
import util;

namespace cmajor::binder {

class ExpressionBinder : public cmajor::ast::Visitor
{
public:
    ExpressionBinder(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundCompileUnit& boundCompileUnit_, BoundFunction* boundFunction_, 
        cmajor::symbols::ContainerScope* containerScope_, StatementBinder* statementBinder_, bool lvalue_);
    std::unique_ptr<BoundExpression> GetExpression() { return std::move(expression); }

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
    void Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode) override;

    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::ast::ParameterNode& parameterNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;
    void Visit(cmajor::ast::ArrowNode& arrowNode) override;
    void BindArrow(cmajor::ast::Node& node, const std::u32string& name);
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
    void Visit(cmajor::ast::TypeIdNode& typeNameNode) override;
    void Visit(cmajor::ast::CastNode& castNode) override;
    void Visit(cmajor::ast::ConstructNode& constructNode) override;
    void Visit(cmajor::ast::NewNode& newNode) override;
    void Visit(cmajor::ast::ThisNode& thisNode) override;
    void Visit(cmajor::ast::BaseNode& baseNode) override;
    void Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode) override;
    void Visit(cmajor::ast::FunctionPtrNode& functionPtrNode) override;
    void BindUnaryOp(BoundExpression* operand, cmajor::ast::Node& node, const std::u32string& groupName);
private:
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::Module* module;
    BoundFunction* boundFunction;
    cmajor::symbols::ContainerScope* containerScope;
    StatementBinder* statementBinder;
    std::unique_ptr<BoundExpression> expression;
    bool lvalue;
    bool inhibitCompile;
    void BindUnaryOp(cmajor::ast::UnaryNode& unaryNode, const std::u32string& groupName);
    void BindBinaryOp(cmajor::ast::BinaryNode& binaryNode, const std::u32string& groupName);
    void BindBinaryOp(BoundExpression* left, BoundExpression* right, cmajor::ast::Node& node, const std::u32string& groupName);
    void BindDerefExpr(cmajor::ast::Node& node);
    void BindSymbol(cmajor::symbols::Symbol* symbol, cmajor::ast::IdentifierNode* idNode);
};

ExpressionBinder::ExpressionBinder(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, BoundCompileUnit& boundCompileUnit_, BoundFunction* boundFunction_, 
    cmajor::symbols::ContainerScope* containerScope_,
    StatementBinder* statementBinder_, bool lvalue_) :
    sourcePos(sourcePos_), moduleId(moduleId_), boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()), module(&boundCompileUnit.GetModule()),
    boundFunction(boundFunction_), containerScope(containerScope_), statementBinder(statementBinder_), lvalue(lvalue_), inhibitCompile(false)
{
}

void ExpressionBinder::BindUnaryOp(BoundExpression* operand, cmajor::ast::Node& node, const std::u32string& groupName)
{
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpression>(operand));
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, operand->GetType()->BaseType()->ClassOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<BoundFunctionCall> operatorFunCall = ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, node.GetSourcePos(), node.ModuleId(),
        OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
    if (!operatorFunCall)
    {
        if (arguments[0]->GetType()->PlainType(node.GetSourcePos(), node.ModuleId())->IsClassTypeSymbol())
        {
            if (arguments[0]->GetType()->IsReferenceType())
            {
                cmajor::symbols::TypeSymbol* type = arguments[0]->GetType()->RemoveReference(node.GetSourcePos(), node.ModuleId())->AddPointer(node.GetSourcePos(), node.ModuleId());
                arguments[0].reset(new BoundReferenceToPointerExpression(std::move(arguments[0]), type));
            }
            else
            {
                cmajor::symbols::TypeSymbol* type = arguments[0]->GetType()->PlainType(node.GetSourcePos(), node.ModuleId())->AddPointer(node.GetSourcePos(), node.ModuleId());
                arguments[0].reset(new BoundAddressOfExpression(std::move(arguments[0]), type));
            }
            operatorFunCall = std::move(ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, node.GetSourcePos(), node.ModuleId()));
        }
        else
        {
            throw* exception;
        }
    }
    CheckAccess(boundFunction->GetFunctionSymbol(), operatorFunCall->GetFunctionSymbol());
    cmajor::symbols::LocalVariableSymbol* temporary = nullptr;
    if (operatorFunCall->GetFunctionSymbol()->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::TypeSymbol* type = operatorFunCall->GetFunctionSymbol()->ReturnType();
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsBound())
            {
                boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
            }
        }
        temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, node.GetSourcePos(), node.ModuleId());
        operatorFunCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(node.GetSourcePos(), node.ModuleId(), temporary)),
            type->AddPointer(node.GetSourcePos(), node.ModuleId()))));
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
            if (classType->Destructor())
            {
                std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                destructorCall->AddArgument(std::unique_ptr<BoundExpression>(operatorFunCall->Arguments().back()->Clone()));
                boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
            }
        }
    }
    expression.reset(operatorFunCall.release());
    if (temporary)
    {
        expression.reset(new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(node.GetSourcePos(), node.ModuleId(), temporary))));
        expression->SetFlag(BoundExpressionFlags::bindToRvalueReference);
    }
}

void ExpressionBinder::BindUnaryOp(cmajor::ast::UnaryNode& unaryNode, const std::u32string& groupName)
{
    unaryNode.Subject()->Accept(*this);
    BoundExpression* operand = expression.release();
    BindUnaryOp(operand, unaryNode, groupName);
}

void ExpressionBinder::BindBinaryOp(cmajor::ast::BinaryNode& binaryNode, const std::u32string& groupName)
{
    binaryNode.Left()->Accept(*this);
    BoundExpression* left = expression.release();
    binaryNode.Right()->Accept(*this);
    BoundExpression* right = expression.release();
    BindBinaryOp(left, right, binaryNode, groupName);
}

void ExpressionBinder::BindBinaryOp(BoundExpression* left, BoundExpression* right, cmajor::ast::Node& node, const std::u32string& groupName)
{
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::unique_ptr<BoundExpression>(left));
    arguments.push_back(std::unique_ptr<BoundExpression>(right));
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, left->GetType()->BaseType()->ClassOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, right->GetType()->BaseType()->ClassOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<BoundFunctionCall> operatorFunCall = ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, node.GetSourcePos(), node.ModuleId(),
        OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
    if (!operatorFunCall)
    {
        if (arguments[0]->GetType()->PlainType(node.GetSourcePos(), node.ModuleId())->IsClassTypeSymbol())
        {
            if (arguments[0]->GetType()->IsReferenceType())
            {
                cmajor::symbols::TypeSymbol* type = arguments[0]->GetType()->RemoveReference(node.GetSourcePos(), node.ModuleId())->AddPointer(node.GetSourcePos(), node.ModuleId());
                arguments[0].reset(new BoundReferenceToPointerExpression(std::move(arguments[0]), type));
            }
            else
            {
                cmajor::symbols::TypeSymbol* type = arguments[0]->GetType()->PlainType(node.GetSourcePos(), node.ModuleId())->AddPointer(node.GetSourcePos(), node.ModuleId());
                arguments[0].reset(new BoundAddressOfExpression(std::move(arguments[0]), type));
            }
            operatorFunCall = std::move(ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, node.GetSourcePos(), node.ModuleId()));
        }
        else
        {
            throw* exception;
        }
    }
    CheckAccess(boundFunction->GetFunctionSymbol(), operatorFunCall->GetFunctionSymbol());
    cmajor::symbols::LocalVariableSymbol* temporary = nullptr;
    if (operatorFunCall->GetFunctionSymbol()->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::TypeSymbol* type = operatorFunCall->GetFunctionSymbol()->ReturnType();
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsBound())
            {
                boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
            }
        }
        temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, node.GetSourcePos(), node.ModuleId());
        operatorFunCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(node.GetSourcePos(), node.ModuleId(), temporary)),
            type->AddPointer(node.GetSourcePos(), node.ModuleId()))));
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
            if (classType->Destructor())
            {
                std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                destructorCall->AddArgument(std::unique_ptr<BoundExpression>(operatorFunCall->Arguments().back()->Clone()));
                boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
            }
        }
    }
    expression.reset(operatorFunCall.release());
    if (temporary)
    {
        expression.reset(new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(node.GetSourcePos(), node.ModuleId(), temporary))));
        expression->SetFlag(BoundExpressionFlags::bindToRvalueReference);
    }
}

void ExpressionBinder::BindSymbol(cmajor::symbols::Symbol* symbol, cmajor::ast::IdentifierNode* idNode)
{
    soul::ast::SourcePos spn = sourcePos;
    util::uuid mid = moduleId;
    if (idNode)
    {
        spn = idNode->GetSourcePos();
        mid = idNode->ModuleId();
    }
    switch (symbol->GetSymbolType())
    {
    case cmajor::symbols::SymbolType::functionGroupSymbol:
    {
        cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol = static_cast<cmajor::symbols::FunctionGroupSymbol*>(symbol);
        BoundFunctionGroupExpression* boundFunctionGroupExpression = new BoundFunctionGroupExpression(sourcePos, moduleId, functionGroupSymbol);
        cmajor::symbols::ParameterSymbol* thisParam = boundFunction->GetFunctionSymbol()->GetThisParam();
        if (thisParam)
        {
            boundFunctionGroupExpression->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(spn, mid, thisParam)));
        }
        expression.reset(boundFunctionGroupExpression);
        break;
    }
    case cmajor::symbols::SymbolType::classTypeSymbol: case cmajor::symbols::SymbolType::classTemplateSpecializationSymbol:
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), classTypeSymbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, classTypeSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, classTypeSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, classTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::classGroupTypeSymbol:
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroupTypeSymbol = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(symbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, classGroupTypeSymbol));
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = classGroupTypeSymbol->GetClass(0);
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && classTypeSymbol)
        {
            symbolTable.MapSymbol(idNode, classTypeSymbol);
        }
        if (idNode && classTypeSymbol)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, classTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::interfaceTypeSymbol:
    {
        cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(symbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, interfaceTypeSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, interfaceTypeSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, interfaceTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::delegateTypeSymbol:
    {
        cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol = static_cast<cmajor::symbols::DelegateTypeSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), delegateTypeSymbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, delegateTypeSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, delegateTypeSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, delegateTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::classDelegateTypeSymbol:
    {
        cmajor::symbols::ClassDelegateTypeSymbol* classDelegateTypeSymbol = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), classDelegateTypeSymbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, classDelegateTypeSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, classDelegateTypeSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, classDelegateTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::typedefSymbol:
    {
        cmajor::symbols::TypedefSymbol* typedefSymbol = static_cast<cmajor::symbols::TypedefSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), typedefSymbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, typedefSymbol->GetType()));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, typedefSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, typedefSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::boundTemplateParameterSymbol:
    {
        cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameterSymbol = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(symbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, boundTemplateParameterSymbol->GetType()));
        break;
    }
    case cmajor::symbols::SymbolType::parameterSymbol:
    {
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), parameterSymbol);
        expression.reset(new BoundParameter(spn, mid, parameterSymbol));
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, parameterSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::localVariableSymbol:
    {
        cmajor::symbols::LocalVariableSymbol* localVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), localVariableSymbol);
        expression.reset(new BoundLocalVariable(spn, mid, localVariableSymbol));
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, localVariableSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::memberVariableSymbol:
    {
        cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = static_cast<cmajor::symbols::MemberVariableSymbol*>(symbol);
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, memberVariableSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, memberVariableSymbol);
        }
        cmajor::symbols::FunctionSymbol* currentFuctionSymbol = boundFunction->GetFunctionSymbol();
        CheckAccess(currentFuctionSymbol, memberVariableSymbol);
        BoundMemberVariable* bmv = new BoundMemberVariable(spn, mid, memberVariableSymbol);
        bool accessFromOwnScope = false;
        cmajor::symbols::ClassTypeSymbol* currentClass = currentFuctionSymbol->ContainingClassNoThrow();
        if (currentClass)
        {
            cmajor::symbols::ClassTypeSymbol* cp = memberVariableSymbol->ContainingClassNoThrow();
            Assert(cp, "class type symbol expected");
            if (cp == currentClass)
            {
                accessFromOwnScope = true;
            }
        }
        if (memberVariableSymbol->IsStatic())
        {
            if (!accessFromOwnScope)
            {
                bmv->SetStaticInitNeeded();
            }
        }
        else
        {
            cmajor::symbols::ParameterSymbol* thisParam = currentFuctionSymbol->GetThisParam();
            if (accessFromOwnScope && !currentFuctionSymbol->IsStatic())
            {
                if (thisParam)
                {
                    cmajor::symbols::TypeSymbol* thisPointerType = thisParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId);
                    if (thisParam->GetType()->IsConstType())
                    {
                        thisPointerType = thisPointerType->AddConst(sourcePos, moduleId);
                    }
                    bmv->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundParameter(spn, mid, thisParam)));
                }
                else
                {
                    Assert(false, "this param expected");
                }
            }
            else if (thisParam)
            {
                cmajor::symbols::ClassTypeSymbol* containingClass = memberVariableSymbol->ContainingClassNoThrow();
                cmajor::symbols::TypeSymbol* containingClassPointerType = containingClass->AddPointer(sourcePos, moduleId);
                cmajor::symbols::TypeSymbol* thisPointerType = thisParam->GetType()->BaseType()->AddPointer(sourcePos, moduleId);
                if (thisParam->GetType()->IsConstType())
                {
                    thisPointerType = thisPointerType->AddConst(sourcePos, moduleId);
                    containingClassPointerType->AddConst(sourcePos, moduleId);
                }
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(thisPointerType, containingClassPointerType, containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                if (conversionFun)
                {
                    bmv->SetClassPtr(std::unique_ptr<BoundExpression>(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(
                        spn, mid, thisParam)), conversionFun)));
                }
            }
        }
        expression.reset(bmv);
        break;
    }
    case cmajor::symbols::SymbolType::constantSymbol:
    {
        cmajor::symbols::ConstantSymbol* constantSymbol = static_cast<cmajor::symbols::ConstantSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), constantSymbol);
        expression.reset(new BoundConstant(spn, mid, constantSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, constantSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, constantSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::enumTypeSymbol:
    {
        cmajor::symbols::EnumTypeSymbol* enumTypeSymbol = static_cast<cmajor::symbols::EnumTypeSymbol*>(symbol);
        CheckAccess(boundFunction->GetFunctionSymbol(), enumTypeSymbol);
        expression.reset(new BoundTypeExpression(sourcePos, moduleId, enumTypeSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, enumTypeSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, enumTypeSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::enumConstantSymbol:
    {
        cmajor::symbols::EnumConstantSymbol* enumConstantSymbol = static_cast<cmajor::symbols::EnumConstantSymbol*>(symbol);
        expression.reset(new BoundEnumConstant(spn, mid, enumConstantSymbol));
        if (idNode && GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            symbolTable.MapSymbol(idNode, enumConstantSymbol);
        }
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, enumConstantSymbol);
        }
        break;
    }
    case cmajor::symbols::SymbolType::namespaceSymbol:
    {
        cmajor::symbols::NamespaceSymbol* ns = static_cast<cmajor::symbols::NamespaceSymbol*>(symbol);
        expression.reset(new BoundNamespaceExpression(sourcePos, moduleId, ns));
        if (idNode)
        {
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, ns);
        }
        break;
    }
    case cmajor::symbols::SymbolType::globalVariableGroupSymbol:
    {
        cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = nullptr;
        cmajor::symbols::GlobalVariableGroupSymbol* globalVariableGroup = static_cast<cmajor::symbols::GlobalVariableGroupSymbol*>(symbol);
        std::vector<cmajor::symbols::GlobalVariableSymbol*> globalVariables;
        globalVariableGroup->CollectGlobalVariables(boundCompileUnit.GetCompileUnitNode()->FilePath(), globalVariables);
        if (globalVariables.empty())
        {
            throw cmajor::symbols::Exception("global variable group '" + util::ToUtf8(globalVariableGroup->Name()) + "' contains no relevant public or internal global variables", sourcePos, moduleId);
        }
        else if (globalVariables.size() == 1)
        {
            globalVariableSymbol = globalVariables.front();
            if (globalVariableSymbol->Access() == cmajor::symbols::SymbolAccess::private_ && globalVariableSymbol->CompileUnitFilePath() != boundCompileUnit.GetCompileUnitNode()->FilePath())
            {
                throw cmajor::symbols::Exception("global variable group '" + util::ToUtf8(globalVariableGroup->Name()) +
                    "' does not contain a public or internal global variable with the given name but do contain a private global variable defined in the compile unit " +
                    globalVariableSymbol->CompileUnitFilePath(), sourcePos, moduleId, globalVariableSymbol->GetSourcePos(), globalVariableSymbol->SourceModuleId());
            }
            else
            {
                expression.reset(new BoundGlobalVariable(spn, mid, globalVariableSymbol));
            }
        }
        else
        {
            std::string compileUnits;
            bool first = true;
            for (cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol : globalVariables)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    compileUnits.append(", ");
                }
                compileUnits.append(globalVariableSymbol->CompileUnitFilePath());
            }
            throw cmajor::symbols::Exception("global variable group '" + util::ToUtf8(globalVariableGroup->Name()) +
                "' does not contain a public or internal global variable with the given name but do contain private global variables defined in the following compile units: " + compileUnits,
                sourcePos, moduleId);
        }
        if (idNode)
        {
            if (globalVariableSymbol)
            {
                cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, globalVariableSymbol);
            }
        }
        break;
    }
    default:
    {
        throw cmajor::symbols::Exception("could not bind '" + util::ToUtf8(symbol->FullName()) + "'", sourcePos, moduleId, symbol->GetSourcePos(), symbol->SourceModuleId());
    }
    }
}

void ExpressionBinder::Visit(cmajor::ast::BoolNode& boolNode)
{
    expression.reset(new BoundTypeExpression(boolNode.GetSourcePos(), boolNode.ModuleId(), symbolTable.GetTypeByName(U"bool")));
}

void ExpressionBinder::Visit(cmajor::ast::SByteNode& sbyteNode)
{
    expression.reset(new BoundTypeExpression(sbyteNode.GetSourcePos(), sbyteNode.ModuleId(), symbolTable.GetTypeByName(U"sbyte")));
}

void ExpressionBinder::Visit(cmajor::ast::ByteNode& byteNode)
{
    expression.reset(new BoundTypeExpression(byteNode.GetSourcePos(), byteNode.ModuleId(), symbolTable.GetTypeByName(U"byte")));
}

void ExpressionBinder::Visit(cmajor::ast::ShortNode& shortNode)
{
    expression.reset(new BoundTypeExpression(shortNode.GetSourcePos(), shortNode.ModuleId(), symbolTable.GetTypeByName(U"short")));
}

void ExpressionBinder::Visit(cmajor::ast::UShortNode& ushortNode)
{
    expression.reset(new BoundTypeExpression(ushortNode.GetSourcePos(), ushortNode.ModuleId(), symbolTable.GetTypeByName(U"ushort")));
}

void ExpressionBinder::Visit(cmajor::ast::IntNode& intNode)
{
    expression.reset(new BoundTypeExpression(intNode.GetSourcePos(), intNode.ModuleId(), symbolTable.GetTypeByName(U"int")));
}

void ExpressionBinder::Visit(cmajor::ast::UIntNode& uintNode)
{
    expression.reset(new BoundTypeExpression(uintNode.GetSourcePos(), uintNode.ModuleId(), symbolTable.GetTypeByName(U"uint")));
}

void ExpressionBinder::Visit(cmajor::ast::LongNode& longNode)
{
    expression.reset(new BoundTypeExpression(longNode.GetSourcePos(), longNode.ModuleId(), symbolTable.GetTypeByName(U"long")));
}

void ExpressionBinder::Visit(cmajor::ast::ULongNode& ulongNode)
{
    expression.reset(new BoundTypeExpression(ulongNode.GetSourcePos(), ulongNode.ModuleId(), symbolTable.GetTypeByName(U"ulong")));
}

void ExpressionBinder::Visit(cmajor::ast::FloatNode& floatNode)
{
    expression.reset(new BoundTypeExpression(floatNode.GetSourcePos(), floatNode.ModuleId(), symbolTable.GetTypeByName(U"float")));
}

void ExpressionBinder::Visit(cmajor::ast::DoubleNode& doubleNode)
{
    expression.reset(new BoundTypeExpression(doubleNode.GetSourcePos(), doubleNode.ModuleId(), symbolTable.GetTypeByName(U"double")));
}

void ExpressionBinder::Visit(cmajor::ast::CharNode& charNode)
{
    expression.reset(new BoundTypeExpression(charNode.GetSourcePos(), charNode.ModuleId(), symbolTable.GetTypeByName(U"char")));
}

void ExpressionBinder::Visit(cmajor::ast::WCharNode& wcharNode)
{
    expression.reset(new BoundTypeExpression(wcharNode.GetSourcePos(), wcharNode.ModuleId(), symbolTable.GetTypeByName(U"wchar")));
}

void ExpressionBinder::Visit(cmajor::ast::UCharNode& ucharNode)
{
    expression.reset(new BoundTypeExpression(ucharNode.GetSourcePos(), ucharNode.ModuleId(), symbolTable.GetTypeByName(U"uchar")));
}

void ExpressionBinder::Visit(cmajor::ast::VoidNode& voidNode)
{
    expression.reset(new BoundTypeExpression(voidNode.GetSourcePos(), voidNode.ModuleId(), symbolTable.GetTypeByName(U"void")));
}

void ExpressionBinder::Visit(cmajor::ast::BooleanLiteralNode& booleanLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::BoolValue(booleanLiteralNode.GetSourcePos(), booleanLiteralNode.ModuleId(), 
        booleanLiteralNode.Value())), symbolTable.GetTypeByName(U"bool")));
}

void ExpressionBinder::Visit(cmajor::ast::SByteLiteralNode& sbyteLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::SByteValue(sbyteLiteralNode.GetSourcePos(), sbyteLiteralNode.ModuleId(), 
        sbyteLiteralNode.Value())), symbolTable.GetTypeByName(U"sbyte")));
}

void ExpressionBinder::Visit(cmajor::ast::ByteLiteralNode& byteLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::ByteValue(byteLiteralNode.GetSourcePos(), byteLiteralNode.ModuleId(), 
        byteLiteralNode.Value())), symbolTable.GetTypeByName(U"byte")));
}

void ExpressionBinder::Visit(cmajor::ast::ShortLiteralNode& shortLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::ShortValue(shortLiteralNode.GetSourcePos(), shortLiteralNode.ModuleId(), 
        shortLiteralNode.Value())), symbolTable.GetTypeByName(U"short")));
}

void ExpressionBinder::Visit(cmajor::ast::UShortLiteralNode& ushortLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UShortValue(ushortLiteralNode.GetSourcePos(), ushortLiteralNode.ModuleId(), 
        ushortLiteralNode.Value())), symbolTable.GetTypeByName(U"ushort")));
}

void ExpressionBinder::Visit(cmajor::ast::IntLiteralNode& intLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::IntValue(intLiteralNode.GetSourcePos(), intLiteralNode.ModuleId(), 
        intLiteralNode.Value())), symbolTable.GetTypeByName(U"int")));
}

void ExpressionBinder::Visit(cmajor::ast::UIntLiteralNode& uintLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UIntValue(uintLiteralNode.GetSourcePos(), uintLiteralNode.ModuleId(), 
        uintLiteralNode.Value())), symbolTable.GetTypeByName(U"uint")));
}

void ExpressionBinder::Visit(cmajor::ast::LongLiteralNode& longLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::LongValue(longLiteralNode.GetSourcePos(), longLiteralNode.ModuleId(), 
        longLiteralNode.Value())), symbolTable.GetTypeByName(U"long")));
}

void ExpressionBinder::Visit(cmajor::ast::ULongLiteralNode& ulongLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::ULongValue(ulongLiteralNode.GetSourcePos(), ulongLiteralNode.ModuleId(), 
        ulongLiteralNode.Value())), symbolTable.GetTypeByName(U"ulong")));
}

void ExpressionBinder::Visit(cmajor::ast::FloatLiteralNode& floatLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::FloatValue(floatLiteralNode.GetSourcePos(), floatLiteralNode.ModuleId(), 
        floatLiteralNode.Value())), symbolTable.GetTypeByName(U"float")));
}

void ExpressionBinder::Visit(cmajor::ast::DoubleLiteralNode& doubleLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::DoubleValue(doubleLiteralNode.GetSourcePos(), doubleLiteralNode.ModuleId(), 
        doubleLiteralNode.Value())), symbolTable.GetTypeByName(U"double")));
}

void ExpressionBinder::Visit(cmajor::ast::CharLiteralNode& charLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::CharValue(charLiteralNode.GetSourcePos(), charLiteralNode.ModuleId(), 
        charLiteralNode.Value())), symbolTable.GetTypeByName(U"char")));
}

void ExpressionBinder::Visit(cmajor::ast::WCharLiteralNode& wcharLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::WCharValue(wcharLiteralNode.GetSourcePos(), wcharLiteralNode.ModuleId(), 
        wcharLiteralNode.Value())), symbolTable.GetTypeByName(U"wchar")));
}

void ExpressionBinder::Visit(cmajor::ast::UCharLiteralNode& ucharLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UCharValue(ucharLiteralNode.GetSourcePos(), ucharLiteralNode.ModuleId(), 
        ucharLiteralNode.Value())), symbolTable.GetTypeByName(U"uchar")));
}

void ExpressionBinder::Visit(cmajor::ast::StringLiteralNode& stringLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(stringLiteralNode.GetSourcePos(), stringLiteralNode.ModuleId(), 
        boundCompileUnit.Install(stringLiteralNode.Value()), stringLiteralNode.Value())),
        symbolTable.GetTypeByName(U"char")->AddConst(stringLiteralNode.GetSourcePos(), stringLiteralNode.ModuleId())->AddPointer(stringLiteralNode.GetSourcePos(), 
            stringLiteralNode.ModuleId())));
}

void ExpressionBinder::Visit(cmajor::ast::WStringLiteralNode& wstringLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::WStringValue(wstringLiteralNode.GetSourcePos(), wstringLiteralNode.ModuleId(), 
        boundCompileUnit.Install(wstringLiteralNode.Value()), wstringLiteralNode.Value())),
        symbolTable.GetTypeByName(U"wchar")->AddConst(wstringLiteralNode.GetSourcePos(), wstringLiteralNode.ModuleId())->AddPointer(wstringLiteralNode.GetSourcePos(), 
            wstringLiteralNode.ModuleId())));
}

void ExpressionBinder::Visit(cmajor::ast::UStringLiteralNode& ustringLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UStringValue(ustringLiteralNode.GetSourcePos(), ustringLiteralNode.ModuleId(), 
        boundCompileUnit.Install(ustringLiteralNode.Value()), ustringLiteralNode.Value())),
        symbolTable.GetTypeByName(U"uchar")->AddConst(ustringLiteralNode.GetSourcePos(), ustringLiteralNode.ModuleId())->AddPointer(ustringLiteralNode.GetSourcePos(), 
            ustringLiteralNode.ModuleId())));
}

void ExpressionBinder::Visit(cmajor::ast::NullLiteralNode& nullLiteralNode)
{
    cmajor::symbols::TypeSymbol* nullPtrType = symbolTable.GetTypeByName(U"@nullptr_type");
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::NullValue(nullLiteralNode.GetSourcePos(), nullLiteralNode.ModuleId(), 
        nullPtrType)), nullPtrType));
}

void ExpressionBinder::Visit(cmajor::ast::UuidLiteralNode& uuidLiteralNode)
{
    expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::UuidValue(uuidLiteralNode.GetSourcePos(), uuidLiteralNode.ModuleId(), 
        boundCompileUnit.Install(uuidLiteralNode.GetUuid()))), symbolTable.GetTypeByName(U"void")->AddPointer(uuidLiteralNode.GetSourcePos(), uuidLiteralNode.ModuleId())));
}

void ExpressionBinder::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    boundCompileUnit.SetLatestIdentifier(&identifierNode);
    std::u32string name = identifierNode.Str();
    if (name == U"compileUnitUnwindInfoInitList")
    {
        int x = 0;
    }
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        BindSymbol(symbol, &identifierNode);
    }
    else
    {
        throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found", identifierNode.GetSourcePos(), identifierNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::TemplateIdNode& templateIdNode)
{
    int arity = templateIdNode.TemplateArguments().Count();
    templateIdNode.Primary()->Accept(*this);
    if (expression->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
    {
        cmajor::symbols::TypeSymbol* typeSymbol = expression->GetType();
        if (typeSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
        {
            cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(typeSymbol);
            typeSymbol = classGroup->GetClass(arity);
            if (templateIdNode.Primary()->GetNodeType() == cmajor::ast::NodeType::identifierNode)
            {
                cmajor::symbols::MapIdentifierToSymbolDefinition(static_cast<cmajor::ast::IdentifierNode*>(templateIdNode.Primary()), typeSymbol);
            }
            expression.reset(new BoundTypeExpression(sourcePos, moduleId, typeSymbol));
        }
    }
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    int n = arity;
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* templateArgumentNode = templateIdNode.TemplateArguments()[i];
        cmajor::symbols::TypeSymbol* type = ResolveType(templateArgumentNode, boundCompileUnit, containerScope);
        templateArgumentTypes.push_back(type);
    }
    if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
    {
        BoundFunctionGroupExpression* bfge = static_cast<BoundFunctionGroupExpression*>(expression.get());
        bfge->SetTemplateArgumentTypes(templateArgumentTypes);
    }
    else if (expression->GetBoundNodeType() == BoundNodeType::boundMemberExpression)
    {
        BoundMemberExpression* bme = static_cast<BoundMemberExpression*>(expression.get());
        if (bme->Member()->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
        {
            BoundFunctionGroupExpression* bfge = static_cast<BoundFunctionGroupExpression*>(bme->Member());
            bfge->SetTemplateArgumentTypes(templateArgumentTypes);
        }
        else if (bme->Member()->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
        {
            cmajor::symbols::TypeSymbol* typeSymbol = bme->Member()->GetType();
            if (typeSymbol->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(typeSymbol);
                if (classTypeSymbol->IsClassTemplate())
                {
                    int m = classTypeSymbol->TemplateParameters().size();
                    if (n < m)
                    {
                        boundCompileUnit.GetClassTemplateRepository().ResolveDefaultTemplateArguments(templateArgumentTypes, classTypeSymbol, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                    }
                    cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization = symbolTable.MakeClassTemplateSpecialization(classTypeSymbol, templateArgumentTypes,
                        templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                    if (!classTemplateSpecialization->IsBound())
                    {
                        boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(classTemplateSpecialization, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                    }
                    expression.reset(new BoundTypeExpression(sourcePos, moduleId, classTemplateSpecialization));
                }
            }
        }
        else
        {
            throw cmajor::symbols::Exception("function group or class group expected", templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
        }
    }
    else if (expression->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
    {
        cmajor::symbols::TypeSymbol* typeSymbol = expression->GetType();
        if (typeSymbol->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(typeSymbol);
            if (classTypeSymbol->IsClassTemplate())
            {
                int m = classTypeSymbol->TemplateParameters().size();
                if (n < m)
                {
                    boundCompileUnit.GetClassTemplateRepository().ResolveDefaultTemplateArguments(templateArgumentTypes, classTypeSymbol, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                }
                cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization = symbolTable.MakeClassTemplateSpecialization(classTypeSymbol, templateArgumentTypes, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                if (!classTemplateSpecialization->IsBound())
                {
                    boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(classTemplateSpecialization, containerScope, templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
                }
                expression.reset(new BoundTypeExpression(sourcePos, moduleId, classTemplateSpecialization));
            }
        }
    }
    else
    {
        throw cmajor::symbols::Exception("function group or class group expected", templateIdNode.GetSourcePos(), templateIdNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::ParameterNode& parameterNode)
{
    if (!parameterNode.Id())
    {
        throw cmajor::symbols::Exception("parameter not named", parameterNode.GetSourcePos(), parameterNode.ModuleId());
    }
    std::u32string name = parameterNode.Id()->Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol)
        {
            cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
            expression.reset(new BoundParameter(parameterNode.GetSourcePos(), parameterNode.ModuleId(), parameterSymbol));
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a parameter", parameterNode.GetSourcePos(), parameterNode.ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("parameter symbol '" + util::ToUtf8(name) + "' not found", parameterNode.GetSourcePos(), parameterNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::DotNode& dotNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    expression = BindExpression(dotNode.Subject(), boundCompileUnit, boundFunction, containerScope, statementBinder, false, true, true, false);
    cmajor::ast::IdentifierNode* idNode = boundCompileUnit.GetLatestIdentifier();
    boundCompileUnit.SetLatestIdentifier(dotNode.MemberId());
    if (expression->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
    {
        cmajor::symbols::TypeSymbol* typeSymbol = expression->GetType();
        if (typeSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
        {
            cmajor::symbols::ClassGroupTypeSymbol* classGroupTypeSymbol = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(typeSymbol);
            typeSymbol = classGroupTypeSymbol->GetClass(0);
            if (GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
            {
                symbolTable.MapSymbol(idNode, typeSymbol);
            }
            cmajor::symbols::MapIdentifierToSymbolDefinition(idNode, typeSymbol);
            if (!typeSymbol)
            {
                throw cmajor::symbols::Exception("ordinary class not found from class group '" + util::ToUtf8(classGroupTypeSymbol->FullName()) + "'", sourcePos, moduleId, classGroupTypeSymbol->GetSourcePos(), classGroupTypeSymbol->SourceModuleId());
            }
            else
            {
                expression.reset(new BoundTypeExpression(sourcePos, moduleId, typeSymbol));
            }
        }
    }
    if (expression->GetBoundNodeType() == BoundNodeType::boundNamespaceExpression)
    {
        BoundNamespaceExpression* bns = static_cast<BoundNamespaceExpression*>(expression.get());
        containerScope = bns->Ns()->GetContainerScope();
        std::u32string name = dotNode.MemberId()->Str();
        cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_);
        if (symbol)
        {
            BindSymbol(symbol, dotNode.MemberId());
            if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
            {
                BoundFunctionGroupExpression* bfe = static_cast<BoundFunctionGroupExpression*>(expression.get());
                bfe->SetScopeQualified();
                bfe->SetQualifiedScope(containerScope);
            }
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from namespace '" + util::ToUtf8(bns->Ns()->FullName()) + "'", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
        }
    }
    else
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType()->PlainType(dotNode.GetSourcePos(), dotNode.ModuleId());
        if (type->IsClassDelegateType())
        {
            cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(type);
            type = classDelegateType->ObjectDelegatePairType();
        }
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
            cmajor::symbols::ContainerScope* scope = classType->GetContainerScope();
            std::u32string name = dotNode.MemberId()->Str();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base);
            if (symbol)
            {
                std::unique_ptr<BoundExpression> classPtr;
                BoundExpression* plainClassPtr = expression.get();
                if (expression->GetType()->IsClassTypeSymbol())
                {
                    cmajor::symbols::TypeSymbol* type = expression->GetType()->AddPointer(dotNode.GetSourcePos(), dotNode.ModuleId());
                    classPtr.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(expression.release()), type));
                }
                else if (expression->GetType()->IsReferenceType())
                {
                    cmajor::symbols::TypeSymbol* type = expression->GetType()->RemoveReference(dotNode.GetSourcePos(), dotNode.ModuleId())->AddPointer(dotNode.GetSourcePos(), dotNode.ModuleId());
                    classPtr.reset(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(expression.release()), type));
                }
                else
                {
                    classPtr.reset(expression.release());
                }
                BindSymbol(symbol, dotNode.MemberId());
                if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                {
                    BoundFunctionGroupExpression* bfg = static_cast<BoundFunctionGroupExpression*>(expression.get());
                    if (!classPtr->GetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr))
                    {
                        cmajor::symbols::Symbol* parent = symbol->Parent();
                        Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
                            parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected");
                        cmajor::symbols::ClassTypeSymbol* owner = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
                        if (classType->HasBaseClass(owner))
                        {
                            if (classPtr->GetType()->IsConstType())
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), owner->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), containerScope, boundFunction, dotNode.GetSourcePos(), dotNode.ModuleId(), argumentMatch)));
                            }
                            else
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddPointer(sourcePos, moduleId), owner->AddPointer(sourcePos, moduleId), containerScope, boundFunction, dotNode.GetSourcePos(), dotNode.ModuleId(), argumentMatch)));
                            }
                        }
                    }
                    if (plainClassPtr->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
                    {
                        BoundTypeExpression* bte = static_cast<BoundTypeExpression*>(plainClassPtr);
                        bfg->SetScopeQualified();
                        bfg->SetQualifiedScope(bte->GetType()->GetContainerScope());
                    }
                    BoundMemberExpression* bme = new BoundMemberExpression(dotNode.GetSourcePos(), dotNode.ModuleId(), std::unique_ptr<BoundExpression>(classPtr.release()), std::move(expression));
                    expression.reset(bme);
                }
                else if (expression->GetBoundNodeType() == BoundNodeType::boundMemberVariable)
                {
                    BoundMemberVariable* bmv = static_cast<BoundMemberVariable*>(expression.get());
                    if (!bmv->GetMemberVariableSymbol()->IsStatic())
                    {
                        cmajor::symbols::Symbol* parent = symbol->Parent();
                        Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
                            parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected"); 
                        cmajor::symbols::ClassTypeSymbol* owner = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
                        if (classType->HasBaseClass(owner))
                        {
                            if (classPtr->GetType()->IsConstType())
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), owner->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), containerScope, boundFunction, dotNode.GetSourcePos(), dotNode.ModuleId(), argumentMatch)));
                            }
                            else
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddPointer(sourcePos, moduleId), owner->AddPointer(sourcePos, moduleId), containerScope, boundFunction, dotNode.GetSourcePos(), dotNode.ModuleId(), argumentMatch)));
                            }
                        }
                        bmv->SetClassPtr(std::unique_ptr<BoundExpression>(classPtr.release()));
                    }
                }
                else if (expression->GetBoundNodeType() != BoundNodeType::boundTypeExpression && expression->GetBoundNodeType() != BoundNodeType::boundConstant)
                {
                    throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a function group, member variable, or type", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from class '" + util::ToUtf8(classType->FullName()) + "'", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
            }
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(type->BaseType());
            cmajor::symbols::ContainerScope* scope = interfaceType->GetContainerScope();
            std::u32string name = dotNode.MemberId()->Str();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name, cmajor::symbols::ScopeLookup::this_);
            if (symbol)
            {
                std::unique_ptr<BoundExpression> interfacePtr;
                interfacePtr.reset(expression.release());
                BindSymbol(symbol, dotNode.MemberId());
                if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                {
                    BoundFunctionGroupExpression* bfg = static_cast<BoundFunctionGroupExpression*>(expression.get());
                    BoundMemberExpression* bme = new BoundMemberExpression(dotNode.GetSourcePos(), dotNode.ModuleId(), std::unique_ptr<BoundExpression>(interfacePtr.release()), std::move(expression));
                    expression.reset(bme);
                }
                else
                {
                    throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a function group", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from interface '" + util::ToUtf8(interfaceType->FullName()) + "'", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
            }
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol)
        {
            cmajor::symbols::EnumTypeSymbol* enumType = static_cast<cmajor::symbols::EnumTypeSymbol*>(type);
            cmajor::symbols::ContainerScope* scope = enumType->GetContainerScope();
            std::u32string name = dotNode.MemberId()->Str();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name);
            if (symbol)
            {
                BindSymbol(symbol, dotNode.MemberId());
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from enumerated type '" + util::ToUtf8(enumType->FullName()) + "'", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
            }
        }
        else if (type->GetSymbolType() == cmajor::symbols::SymbolType::arrayTypeSymbol)
        {
            cmajor::symbols::ArrayTypeSymbol* arrayType = static_cast<cmajor::symbols::ArrayTypeSymbol*>(type);
            cmajor::symbols::ContainerScope* scope = arrayType->GetContainerScope();
            std::u32string name = dotNode.MemberId()->Str();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name);
            if (symbol)
            {
                std::unique_ptr<BoundExpression> receiverPtr = std::move(expression);
                BindSymbol(symbol, dotNode.MemberId());
                if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                {
                    BoundFunctionGroupExpression* bfe = static_cast<BoundFunctionGroupExpression*>(expression.get());
                    bfe->SetScopeQualified();
                    bfe->SetQualifiedScope(scope);
                    bfe->SetClassPtr(std::move(receiverPtr));
                }
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from array type '" + util::ToUtf8(arrayType->FullName()) + "'", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
            }
        }
        else if (type->IsCharacterPointerType() && expression->GetBoundNodeType() == BoundNodeType::boundLiteral)
        {
            cmajor::symbols::TypeSymbol* stringFunctionContainer = symbolTable.GetTypeByName(U"@string_functions");
            cmajor::symbols::ContainerScope* scope = stringFunctionContainer->GetContainerScope();
            std::u32string name = dotNode.MemberId()->Str();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name);
            if (symbol)
            {
                std::unique_ptr<BoundExpression> receiverPtr = std::move(expression);
                BindSymbol(symbol, dotNode.MemberId());
                if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                {
                    BoundFunctionGroupExpression* bfe = static_cast<BoundFunctionGroupExpression*>(expression.get());
                    bfe->SetScopeQualified();
                    bfe->SetQualifiedScope(scope);
                    bfe->SetClassPtr(std::move(receiverPtr));
                }
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from string functions", dotNode.MemberId()->GetSourcePos(), dotNode.MemberId()->ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("expression must denote a namespace, class type, interface type, array type or an enumerated type type object", dotNode.GetSourcePos(), dotNode.ModuleId());
        }
    }
    containerScope = prevContainerScope;
}

void ExpressionBinder::BindArrow(cmajor::ast::Node& node, const std::u32string& name)
{
    if (expression->GetType()->IsPointerType())
    {
        std::unique_ptr<BoundExpression> classPtr(std::move(expression));
        if (classPtr->GetType()->BaseType()->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(classPtr->GetType()->BaseType());
            cmajor::symbols::ContainerScope* scope = classType->GetContainerScope();
            cmajor::symbols::Symbol* symbol = scope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base);
            if (symbol)
            {
                BindSymbol(symbol, nullptr);
                if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                {
                    BoundFunctionGroupExpression* bfg = static_cast<BoundFunctionGroupExpression*>(expression.get());
                    if (!classPtr->GetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr))
                    {
                        cmajor::symbols::Symbol* parent = symbol->Parent();
                        Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
                            parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected"); 
                        cmajor::symbols::ClassTypeSymbol* owner = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
                        if (classType->HasBaseClass(owner))
                        {
                            if (classPtr->GetType()->IsConstType())
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), owner->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), containerScope, boundFunction, node.GetSourcePos(), node.ModuleId(), argumentMatch)));
                            }
                            else
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddPointer(sourcePos, moduleId), owner->AddPointer(sourcePos, moduleId), containerScope, boundFunction, node.GetSourcePos(), node.ModuleId(), argumentMatch)));
                            }
                        }
                    }
                    BoundMemberExpression* bme = new BoundMemberExpression(node.GetSourcePos(), node.ModuleId(), std::unique_ptr<BoundExpression>(classPtr.release()), std::move(expression));
                    expression.reset(bme);
                }
                else if (expression->GetBoundNodeType() == BoundNodeType::boundMemberVariable)
                {
                    BoundMemberVariable* bmv = static_cast<BoundMemberVariable*>(expression.get());
                    if (!bmv->GetMemberVariableSymbol()->IsStatic())
                    {
                        cmajor::symbols::Symbol* parent = symbol->Parent();
                        Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
                            parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected"); 
                        cmajor::symbols::ClassTypeSymbol* owner = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
                        if (classType->HasBaseClass(owner))
                        {
                            if (classPtr->GetType()->IsConstType())
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), owner->AddConst(sourcePos, moduleId)->AddPointer(sourcePos, moduleId), containerScope, boundFunction, node.GetSourcePos(), node.ModuleId(), argumentMatch)));
                            }
                            else
                            {
                                ArgumentMatch argumentMatch;
                                classPtr.reset(new BoundConversion(std::unique_ptr<BoundExpression>(classPtr.release()),
                                    boundCompileUnit.GetConversion(classType->AddPointer(sourcePos, moduleId), owner->AddPointer(sourcePos, moduleId), containerScope, boundFunction, node.GetSourcePos(), node.ModuleId(), argumentMatch)));
                            }
                        }
                        bmv->SetClassPtr(std::unique_ptr<BoundExpression>(classPtr.release()));
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("member variable '" + util::ToUtf8(bmv->GetMemberVariableSymbol()->FullName()) + +"' is static", node.GetSourcePos(), node.ModuleId());
                    }
                }
                else
                {
                    throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' does not denote a function group or a member variable", node.GetSourcePos(), node.ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(name) + "' not found from class '" + util::ToUtf8(classType->FullName()) + "'", node.GetSourcePos(), node.ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("type of arrow expression subject must be pointer to class type", node.GetSourcePos(), node.ModuleId());
        }
    }
    else if (expression->GetType()->IsClassTypeSymbol())
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType();
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsBound())
            {
                boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
            }
        }
        cmajor::symbols::TypeSymbol* pointerType = type->AddPointer(node.GetSourcePos(), node.ModuleId());
        cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, node.GetSourcePos(), node.ModuleId());
        Assert(expression->GetBoundNodeType() == BoundNodeType::boundFunctionCall, "function call expected");
        BoundFunctionCall* boundFunctionCall = static_cast<BoundFunctionCall*>(expression.get());
        boundFunctionCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(node.GetSourcePos(), node.ModuleId(), temporary)), pointerType)));
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
            if (classType->Destructor())
            {
                std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                destructorCall->AddArgument(std::unique_ptr<BoundExpression>(boundFunctionCall->Arguments().back()->Clone()));
                boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
            }
        }
        expression.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
            new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)))), pointerType));
        BindUnaryOp(expression.release(), node, U"operator->");
        BindArrow(node, name);
    }
    else
    {
        throw cmajor::symbols::Exception("arrow operator member function must return a class type object or a pointer to a class type object", node.GetSourcePos(), node.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::ArrowNode& arrowNode)
{
    arrowNode.Subject()->Accept(*this);
    bool argIsExplicitThisOrBasePtr = expression->GetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
    if (expression->GetType()->IsReferenceType() && expression->GetType()->PlainType(arrowNode.GetSourcePos(), arrowNode.ModuleId())->IsClassTypeSymbol())
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType()->RemoveReference(arrowNode.GetSourcePos(), arrowNode.ModuleId())->AddPointer(arrowNode.GetSourcePos(), arrowNode.ModuleId());
        expression.reset(new BoundReferenceToPointerExpression(std::move(expression), type));
    }
    else if (expression->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType()->RemoveReference(arrowNode.GetSourcePos(), arrowNode.ModuleId())->AddPointer(arrowNode.GetSourcePos(), arrowNode.ModuleId());
        expression.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundDereferenceExpression(std::move(expression), type)), type->AddPointer(arrowNode.GetSourcePos(), arrowNode.ModuleId())));
    }
    else
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType()->AddPointer(arrowNode.GetSourcePos(), arrowNode.ModuleId());
        expression.reset(new BoundAddressOfExpression(std::move(expression), type));
    }
    BindUnaryOp(expression.release(), arrowNode, U"operator->");
    if (argIsExplicitThisOrBasePtr)
    {
        expression->SetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
    }
    BindArrow(arrowNode, arrowNode.MemberId()->Str());
    if (argIsExplicitThisOrBasePtr)
    {
        expression->SetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
    }
    boundCompileUnit.SetLatestIdentifier(arrowNode.MemberId());
}

void ExpressionBinder::Visit(cmajor::ast::DisjunctionNode& disjunctionNode)
{
    std::unique_ptr<BoundExpression> left = BindExpression(disjunctionNode.Left(), boundCompileUnit, boundFunction, containerScope, statementBinder);
    std::unique_ptr<BoundExpression> right = BindExpression(disjunctionNode.Right(), boundCompileUnit, boundFunction, containerScope, statementBinder);
    BoundDisjunction* boundDisjunction = new BoundDisjunction(disjunctionNode.GetSourcePos(), disjunctionNode.ModuleId(), std::move(left), std::move(right), symbolTable.GetTypeByName(U"bool"));
    cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"bool"), disjunctionNode.GetSourcePos(), disjunctionNode.ModuleId());
    boundDisjunction->SetTemporary(new BoundLocalVariable(disjunctionNode.GetSourcePos(), disjunctionNode.ModuleId(), temporary));
    expression.reset(boundDisjunction);
}

void ExpressionBinder::Visit(cmajor::ast::ConjunctionNode& conjunctionNode)
{
    std::unique_ptr<BoundExpression> left = BindExpression(conjunctionNode.Left(), boundCompileUnit, boundFunction, containerScope, statementBinder);
    std::unique_ptr<BoundExpression> right = BindExpression(conjunctionNode.Right(), boundCompileUnit, boundFunction, containerScope, statementBinder);
    BoundConjunction* boundConjunction = new BoundConjunction(conjunctionNode.GetSourcePos(), conjunctionNode.ModuleId(), std::move(left), std::move(right), symbolTable.GetTypeByName(U"bool"));
    cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"bool"), conjunctionNode.GetSourcePos(), conjunctionNode.ModuleId());
    boundConjunction->SetTemporary(new BoundLocalVariable(conjunctionNode.GetSourcePos(), conjunctionNode.ModuleId(), temporary));
    expression.reset(boundConjunction);
}

void ExpressionBinder::Visit(cmajor::ast::BitOrNode& bitOrNode)
{
    BindBinaryOp(bitOrNode, U"operator|");
}

void ExpressionBinder::Visit(cmajor::ast::BitXorNode& bitXorNode)
{
    BindBinaryOp(bitXorNode, U"operator^");
}

void ExpressionBinder::Visit(cmajor::ast::BitAndNode& bitAndNode)
{
    BindBinaryOp(bitAndNode, U"operator&");
}

void ExpressionBinder::Visit(cmajor::ast::EqualNode& equalNode)
{
    BindBinaryOp(equalNode, U"operator==");
}

void ExpressionBinder::Visit(cmajor::ast::NotEqualNode& notEqualNode)
{
    BindBinaryOp(notEqualNode, U"operator==");
    BindUnaryOp(expression.release(), notEqualNode, U"operator!");
}

void ExpressionBinder::Visit(cmajor::ast::LessNode& lessNode)
{
    BindBinaryOp(lessNode, U"operator<");
}

void ExpressionBinder::Visit(cmajor::ast::GreaterNode& greaterNode)
{
    greaterNode.Left()->Accept(*this);
    BoundExpression* left = expression.release();
    greaterNode.Right()->Accept(*this);
    BoundExpression* right = expression.release();
    BindBinaryOp(right, left, greaterNode, U"operator<");
}

void ExpressionBinder::Visit(cmajor::ast::LessOrEqualNode& lessOrEqualNode)
{
    lessOrEqualNode.Left()->Accept(*this);
    BoundExpression* left = expression.release();
    lessOrEqualNode.Right()->Accept(*this);
    BoundExpression* right = expression.release();
    BindBinaryOp(right, left, lessOrEqualNode, U"operator<");
    BindUnaryOp(expression.release(), lessOrEqualNode, U"operator!");
}

void ExpressionBinder::Visit(cmajor::ast::GreaterOrEqualNode& greaterOrEqualNode)
{
    BindBinaryOp(greaterOrEqualNode, U"operator<");
    BindUnaryOp(expression.release(), greaterOrEqualNode, U"operator!");
}

void ExpressionBinder::Visit(cmajor::ast::ShiftLeftNode& shiftLeftNode)
{
    BindBinaryOp(shiftLeftNode, U"operator<<");
}

void ExpressionBinder::Visit(cmajor::ast::ShiftRightNode& shiftRightNode)
{
    BindBinaryOp(shiftRightNode, U"operator>>");
}

void ExpressionBinder::Visit(cmajor::ast::AddNode& addNode)
{
    BindBinaryOp(addNode, U"operator+");
}

void ExpressionBinder::Visit(cmajor::ast::SubNode& subNode)
{
    BindBinaryOp(subNode, U"operator-");
}

void ExpressionBinder::Visit(cmajor::ast::MulNode& mulNode)
{
    BindBinaryOp(mulNode, U"operator*");
}

void ExpressionBinder::Visit(cmajor::ast::DivNode& divNode)
{
    BindBinaryOp(divNode, U"operator/");
}

void ExpressionBinder::Visit(cmajor::ast::RemNode& remNode)
{
    BindBinaryOp(remNode, U"operator%");
}

void ExpressionBinder::Visit(cmajor::ast::NotNode& notNode)
{
    BindUnaryOp(notNode, U"operator!");
}

void ExpressionBinder::Visit(cmajor::ast::UnaryPlusNode& unaryPlusNode)
{
    BindUnaryOp(unaryPlusNode, U"operator+");
}

void ExpressionBinder::Visit(cmajor::ast::UnaryMinusNode& unaryMinusNode)
{
    BindUnaryOp(unaryMinusNode, U"operator-");
}

void ExpressionBinder::Visit(cmajor::ast::PrefixIncrementNode& prefixIncrementNode)
{
    if (statementBinder->CompilingThrow())
    {
        throw cmajor::symbols::Exception("prefix increment in throw expression no allowed", prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId());
    }
    prefixIncrementNode.Subject()->Accept(*this);
    if (expression->GetType()->PlainType(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId())->IsClassTypeSymbol())
    {
        BindUnaryOp(prefixIncrementNode, U"operator++");
    }
    else
    {
        if (!inhibitCompile)
        {
            if (expression->GetType()->IsUnsignedType())
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::AddNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::ByteLiteralNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), 1u)));
                statementBinder->CompileStatement(&assignmentStatement, false);
            }
            else
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::AddNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), prefixIncrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::SByteLiteralNode(prefixIncrementNode.GetSourcePos(), prefixIncrementNode.ModuleId(), 1)));
                statementBinder->CompileStatement(&assignmentStatement, false);
            }
        }
        bool prevInhibitCompile = inhibitCompile;
        inhibitCompile = true;
        prefixIncrementNode.Subject()->Accept(*this);
        inhibitCompile = prevInhibitCompile;
    }
}

void ExpressionBinder::Visit(cmajor::ast::PrefixDecrementNode& prefixDecrementNode)
{
    if (statementBinder->CompilingThrow())
    {
        throw cmajor::symbols::Exception("prefix decrement in throw expression no allowed", prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId());
    }
    prefixDecrementNode.Subject()->Accept(*this);
    if (expression->GetType()->PlainType(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId())->IsClassTypeSymbol())
    {
        BindUnaryOp(prefixDecrementNode, U"operator--");
    }
    else
    {
        if (!inhibitCompile)
        {
            if (expression->GetType()->IsUnsignedType())
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::SubNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::ByteLiteralNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), 1u)));
                statementBinder->CompileStatement(&assignmentStatement, false);
            }
            else
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::SubNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), prefixDecrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::SByteLiteralNode(prefixDecrementNode.GetSourcePos(), prefixDecrementNode.ModuleId(), 1)));
                statementBinder->CompileStatement(&assignmentStatement, false);
            }
        }
        bool prevInhibitCompile = inhibitCompile;
        inhibitCompile = true;
        prefixDecrementNode.Subject()->Accept(*this);
        inhibitCompile = prevInhibitCompile;
    }
}

void ExpressionBinder::BindDerefExpr(cmajor::ast::Node& node)
{
    if (expression->GetType()->IsPointerType())
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType()->RemovePointer(node.GetSourcePos(), node.ModuleId());
        expression.reset(new BoundDereferenceExpression(std::unique_ptr<BoundExpression>(expression.release()), type));
    }
    else
    {
        cmajor::symbols::TypeSymbol* plainSubjectType = expression->GetType()->PlainType(node.GetSourcePos(), node.ModuleId());
        if (plainSubjectType->IsClassTypeSymbol())
        {
            if (expression->GetType()->IsReferenceType())
            {
                cmajor::symbols::TypeSymbol* type = expression->GetType()->RemoveReference(node.GetSourcePos(), node.ModuleId())->AddPointer(node.GetSourcePos(), node.ModuleId());
                expression.reset(new BoundReferenceToPointerExpression(std::move(expression), type));
            }
            else if (expression->GetType()->IsClassTypeSymbol())
            {
                cmajor::symbols::TypeSymbol* type = expression->GetType()->AddPointer(node.GetSourcePos(), node.ModuleId());
                expression.reset(new BoundAddressOfExpression(std::move(expression), type));
            }
            BindUnaryOp(expression.release(), node, U"operator*");
        }
        else
        {
            throw cmajor::symbols::Exception("dereference needs pointer or class type argument", node.GetSourcePos(), node.ModuleId());
        }
    }
}

void ExpressionBinder::Visit(cmajor::ast::DerefNode& derefNode)
{
    derefNode.Subject()->Accept(*this);
    BindDerefExpr(derefNode);
}

void ExpressionBinder::Visit(cmajor::ast::AddrOfNode& addrOfNode)
{
    addrOfNode.Subject()->Accept(*this);
    if (expression->IsLvalueExpression())
    {
        if (expression->GetType()->IsReferenceType())
        {
            cmajor::symbols::TypeSymbol* type = expression->GetType()->RemoveReference(addrOfNode.GetSourcePos(), addrOfNode.ModuleId())->AddPointer(addrOfNode.GetSourcePos(), addrOfNode.ModuleId());
            expression.reset(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(expression.release()), type));
        }
        else
        {
            cmajor::symbols::TypeSymbol* type = expression->GetType()->AddPointer(addrOfNode.GetSourcePos(), addrOfNode.ModuleId());
            expression.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(expression.release()), type));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("cannot take address of " + expression->TypeString(), addrOfNode.GetSourcePos(), addrOfNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::ComplementNode& complementNode)
{
    BindUnaryOp(complementNode, U"operator~");
}

void ExpressionBinder::Visit(cmajor::ast::IsNode& isNode)
{
    cmajor::symbols::TypeSymbol* rightType = ResolveType(isNode.TargetTypeExpr(), boundCompileUnit, containerScope);
    if (rightType->IsPointerType())
    {
        cmajor::symbols::TypeSymbol* rightBaseType = rightType->RemovePointer(sourcePos, moduleId);
        if (rightBaseType->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* rightClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(rightBaseType);
            if (rightClassType->IsPolymorphic())
            {
                std::unique_ptr<BoundExpression> boundExpr = BindExpression(isNode.Expr(), boundCompileUnit, boundFunction, containerScope, statementBinder, false, false, false, false);
                cmajor::symbols::TypeSymbol* leftType = boundExpr->GetType();
                if (leftType->IsPointerType())
                {
                    cmajor::symbols::TypeSymbol* leftBaseType = leftType->RemovePointer(sourcePos, moduleId);
                    if (leftBaseType->IsClassTypeSymbol())
                    {
                        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftBaseType);
                        if (leftClassType->IsPolymorphic())
                        {
                            std::unique_ptr<BoundLocalVariable> leftClassIdVar(new BoundLocalVariable(isNode.GetSourcePos(), isNode.ModuleId(),
                                boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"ulong"), isNode.GetSourcePos(), isNode.ModuleId())));
                            std::unique_ptr<BoundLocalVariable> rightClassIdVar(new BoundLocalVariable(isNode.GetSourcePos(), isNode.ModuleId(),
                                boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"ulong"), isNode.GetSourcePos(), isNode.ModuleId())));
                            expression.reset(new BoundIsExpression(std::move(boundExpr), rightClassType, symbolTable.GetTypeByName(U"bool"),
                                std::move(leftClassIdVar), std::move(rightClassIdVar)));
                        }
                        else
                        {
                            throw cmajor::symbols::Exception("left type in 'is' expression must be pointer to polymorphic class type", isNode.Expr()->GetSourcePos(), isNode.Expr()->ModuleId());
                        }
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("left type in 'is' expression must be pointer to polymorphic class type", isNode.Expr()->GetSourcePos(), isNode.Expr()->ModuleId());
                    }
                }
                else
                {
                    throw cmajor::symbols::Exception("left type in 'is' expression must be pointer to polymorphic class type", isNode.Expr()->GetSourcePos(), isNode.Expr()->ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("right type in 'is' expression must be pointer to polymorphic class type", isNode.TargetTypeExpr()->GetSourcePos(), isNode.TargetTypeExpr()->ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("right type in 'is' expression must be be pointer to polymorphic class type", isNode.TargetTypeExpr()->GetSourcePos(), isNode.TargetTypeExpr()->ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("right type in 'is' expression must be be pointer to polymorphic class type", isNode.TargetTypeExpr()->GetSourcePos(), isNode.TargetTypeExpr()->ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::AsNode& asNode)
{
    cmajor::symbols::TypeSymbol* rightType = ResolveType(asNode.TargetTypeExpr(), boundCompileUnit, containerScope);
    if (rightType->IsPointerType())
    {
        cmajor::symbols::TypeSymbol* rightBaseType = rightType->RemovePointer(sourcePos, moduleId);
        if (rightBaseType->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* rightClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(rightBaseType);
            if (rightClassType->IsPolymorphic())
            {
                std::unique_ptr<BoundExpression> boundExpr = BindExpression(asNode.Expr(), boundCompileUnit, boundFunction, containerScope, statementBinder, false, false, false, false);
                cmajor::symbols::TypeSymbol* leftType = boundExpr->GetType();
                if (leftType->IsPointerType())
                {
                    cmajor::symbols::TypeSymbol* leftBaseType = leftType->RemovePointer(sourcePos, moduleId);
                    if (leftBaseType->IsClassTypeSymbol())
                    {
                        cmajor::symbols::ClassTypeSymbol* leftClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(leftBaseType);
                        if (leftClassType->IsPolymorphic())
                        {
                            std::unique_ptr<BoundLocalVariable> leftClassIdVar(new BoundLocalVariable(asNode.GetSourcePos(), asNode.ModuleId(),
                                boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"ulong"), asNode.GetSourcePos(), asNode.ModuleId())));
                            std::unique_ptr<BoundLocalVariable> rightClassIdVar(new BoundLocalVariable(asNode.GetSourcePos(), asNode.ModuleId(),
                                boundFunction->GetFunctionSymbol()->CreateTemporary(symbolTable.GetTypeByName(U"ulong"), asNode.GetSourcePos(), asNode.ModuleId())));
                            expression.reset(new BoundAsExpression(std::move(boundExpr), rightClassType,
                                std::unique_ptr<BoundLocalVariable>(new BoundLocalVariable(asNode.GetSourcePos(), asNode.ModuleId(), boundFunction->GetFunctionSymbol()->CreateTemporary(
                                    rightClassType->AddPointer(asNode.GetSourcePos(), asNode.ModuleId()), asNode.GetSourcePos(), asNode.ModuleId()))),
                                std::move(leftClassIdVar), std::move(rightClassIdVar)));
                        }
                        else
                        {
                            throw cmajor::symbols::Exception("left type in 'as' expression must be pointer to polymorphic class type", asNode.Expr()->GetSourcePos(), asNode.Expr()->ModuleId());
                        }
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("left type in 'as' expression must be pointer to polymorphic class type", asNode.Expr()->GetSourcePos(), asNode.Expr()->ModuleId());
                    }
                }
                else
                {
                    throw cmajor::symbols::Exception("left type in 'as' expression must be pointer to polymorphic class type", asNode.Expr()->GetSourcePos(), asNode.Expr()->ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("right type in 'as' expression must be pointer to polymorphic class type", asNode.TargetTypeExpr()->GetSourcePos(), asNode.TargetTypeExpr()->ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("right type in 'as' expression must be be pointer to polymorphic class type", asNode.TargetTypeExpr()->GetSourcePos(), asNode.TargetTypeExpr()->ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("right type in 'as' expression must be be pointer to polymorphic class type", asNode.TargetTypeExpr()->GetSourcePos(), asNode.TargetTypeExpr()->ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::IndexingNode& indexingNode)
{
    indexingNode.Subject()->Accept(*this);
    std::unique_ptr<BoundExpression> subject = std::move(expression);
    indexingNode.Index()->Accept(*this);
    std::unique_ptr<BoundExpression> index = std::move(expression);
    cmajor::symbols::TypeSymbol* plainSubjectType = subject->GetType()->PlainType(indexingNode.GetSourcePos(), indexingNode.ModuleId());
    if (plainSubjectType->IsClassTypeSymbol())
    {
        BindBinaryOp(subject.release(), index.release(), indexingNode, U"operator[]");
    }
    else  if (plainSubjectType->IsPointerType())
    {
        BindBinaryOp(subject.release(), index.release(), indexingNode, U"operator+");
        BindDerefExpr(indexingNode);
    }
    else if (plainSubjectType->IsArrayType())
    {
        std::unique_ptr<cmajor::symbols::Value> value = Evaluate(&indexingNode, static_cast<cmajor::symbols::ArrayTypeSymbol*>(plainSubjectType)->ElementType(), containerScope, boundCompileUnit, true, boundFunction,
            indexingNode.GetSourcePos(), indexingNode.ModuleId());
        if (value)
        {
            expression.reset(new BoundLiteral(std::move(value), value->GetType(&symbolTable)));
        }
        else
        {
            BindBinaryOp(subject.release(), index.release(), indexingNode, U"operator[]");
        }
    }
    else
    {
        throw cmajor::symbols::Exception("subscript operator can be applied only to pointer, array or class type subject", indexingNode.GetSourcePos(), indexingNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::InvokeNode& invokeNode)
{
    cmajor::ast::IdentifierNode* prevIdentifier = boundCompileUnit.GetLatestIdentifier();
    boundCompileUnit.SetLatestIdentifier(nullptr);
    invokeNode.Subject()->Accept(*this);
    cmajor::ast::IdentifierNode* invokeId = boundCompileUnit.GetLatestIdentifier();
    boundCompileUnit.SetLatestIdentifier(prevIdentifier);
    bool argIsExplicitThisOrBasePtr = expression->GetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol = nullptr;
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    std::u32string groupName;
    bool scopeQualified = false;
    cmajor::symbols::LocalVariableSymbol* temporary = nullptr;
    if (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
    {
        BoundFunctionGroupExpression* bfge = static_cast<BoundFunctionGroupExpression*>(expression.get());
        functionGroupSymbol = bfge->FunctionGroup();
        templateArgumentTypes = bfge->TemplateArgumentTypes();
        groupName = functionGroupSymbol->Name();
        if (bfge->IsScopeQualified())
        {
            functionScopeLookups.clear();
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, bfge->QualifiedScope()));
            scopeQualified = true;
            if (bfge->ClassPtr())
            {
                arguments.push_back(std::unique_ptr<BoundExpression>(bfge->ReleaseClassPtr()));
            }
        }
    }
    else if (expression->GetBoundNodeType() == BoundNodeType::boundMemberExpression)
    {
        BoundMemberExpression* bme = static_cast<BoundMemberExpression*>(expression.get());
        if (bme->Member()->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
        {
            BoundFunctionGroupExpression* bfge = static_cast<BoundFunctionGroupExpression*>(bme->Member());
            functionGroupSymbol = bfge->FunctionGroup();
            templateArgumentTypes = bfge->TemplateArgumentTypes();
            groupName = functionGroupSymbol->Name();
            if (bfge->IsScopeQualified())
            {
                functionScopeLookups.clear();
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, bfge->QualifiedScope()));
                scopeQualified = true;
            }
            if (!scopeQualified)
            {
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base, bme->ClassPtr()->GetType()->BaseType()->ClassInterfaceOrNsScope()));
            }
            arguments.push_back(std::unique_ptr<BoundExpression>(bme->ReleaseClassPtr()));
            if (arguments.front()->GetType()->PlainType(sourcePos, moduleId)->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
            {
                if (arguments.front()->GetType()->IsReferenceType())
                {
                    cmajor::symbols::TypeSymbol* type = arguments.front()->GetType()->RemoveReference(sourcePos, moduleId)->AddPointer(sourcePos, moduleId);
                    arguments[0].reset(new BoundReferenceToPointerExpression(std::move(arguments[0]), type));
                }
                else
                {
                    cmajor::symbols::TypeSymbol* type = arguments.front()->GetType()->AddPointer(sourcePos, moduleId);
                    arguments[0].reset(new BoundAddressOfExpression(std::move(arguments[0]), type));
                }
            }
        }
        else
        {
            throw cmajor::symbols::Exception("invoke cannot be applied to this type of expression", invokeNode.Subject()->GetSourcePos(), invokeNode.Subject()->ModuleId());
        }
    }
    else if (expression->GetBoundNodeType() == BoundNodeType::boundTypeExpression)
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType();
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
        {
            cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
            cmajor::symbols::ClassTypeSymbol* classTypeSymbol = classGroup->GetClass(0);
            if (!classTypeSymbol)
            {
                throw cmajor::symbols::Exception("ordinary class not found from class group '" + util::ToUtf8(classGroup->FullName()) + "'", sourcePos, moduleId, classGroup->GetSourcePos(), classGroup->SourceModuleId());
            }
            expression.reset(new BoundTypeExpression(sourcePos, moduleId, classTypeSymbol));
            type = classTypeSymbol;
        }
        if (!scopeQualified)
        {
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base, type->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
        }
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsBound())
            {
                boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
            }
        }
        temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, invokeNode.GetSourcePos(), invokeNode.ModuleId());
        std::unique_ptr<BoundExpression> addrOfTemporary(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(invokeNode.GetSourcePos(), invokeNode.ModuleId(), temporary)), type->AddPointer(invokeNode.GetSourcePos(), invokeNode.ModuleId())));
        arguments.push_back(std::move(addrOfTemporary));
        groupName = U"@constructor";
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
            if (classType->Destructor())
            {
                std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                destructorCall->AddArgument(std::unique_ptr<BoundExpression>(arguments.back()->Clone()));
                boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
            }
        }
    }
    else if (expression->GetType()->PlainType(sourcePos, moduleId)->IsClassTypeSymbol())
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType();
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->PlainType(sourcePos, moduleId));
        groupName = U"operator()";
        functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->GetContainerScope()));
        if (type->IsReferenceType())
        {
            expression.reset(new BoundReferenceToPointerExpression(std::move(expression), type->RemoveReference(sourcePos, moduleId)->AddPointer(sourcePos, moduleId)));
        }
        else
        {
            expression.reset(new BoundAddressOfExpression(std::move(expression), type->AddPointer(sourcePos, moduleId)));
        }
        arguments.push_back(std::unique_ptr<BoundExpression>(expression.release()));
    }
    else if (expression->GetType()->PlainType(sourcePos, moduleId)->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol)
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType();
        if (type->IsReferenceType())
        {
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundDereferenceExpression(std::move(expression), type->RemoveReference(sourcePos, moduleId))));
        }
        else
        {
            arguments.push_back(std::move(expression));
        }
        cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol = static_cast<cmajor::symbols::DelegateTypeSymbol*>(type->BaseType());
        int n = invokeNode.Arguments().Count();
        if (n != delegateTypeSymbol->Arity())
        {
            throw cmajor::symbols::Exception("wrong number of arguments for calling delegate type '" + util::ToUtf8(delegateTypeSymbol->FullName()) + "'", sourcePos, moduleId);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TypeSymbol* delegateParameterType = delegateTypeSymbol->Parameters()[i]->GetType();
            cmajor::ast::Node* argument = invokeNode.Arguments()[i];
            argument->Accept(*this);
            cmajor::symbols::TypeSymbol* argumentType = expression->GetType();
            if (!TypesEqual(argumentType, delegateParameterType))
            {
                if (TypesEqual(argumentType->PlainType(sourcePos, moduleId), delegateParameterType->PlainType(sourcePos, moduleId)))
                {
                    if (argumentType->IsReferenceType() && !delegateParameterType->IsReferenceType())
                    {
                        cmajor::symbols::TypeSymbol* type = argumentType->RemoveReference(sourcePos, moduleId);
                        BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(expression), type);
                        expression.reset(dereferenceExpression);
                    }
                    else if (!argumentType->IsReferenceType() && (delegateParameterType->IsReferenceType() || delegateParameterType->IsClassTypeSymbol()))
                    {
                        cmajor::symbols::TypeSymbol* type = argumentType->AddLvalueReference(sourcePos, moduleId);
                        BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(expression), type);
                        expression.reset(addressOfExpression);
                    }
                }
                else
                {
                    ArgumentMatch argumentMatch;
                    cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(argumentType, delegateParameterType, containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                    if (conversionFun)
                    {
                        BoundConversion* conversion = new BoundConversion(std::move(expression), conversionFun);
                        expression.reset(conversion);
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("cannot convert '" + util::ToUtf8(argumentType->FullName()) + "' type argument to '" + util::ToUtf8(delegateParameterType->FullName()) + "' type parameter",
                            argument->GetSourcePos(), argument->ModuleId(), sourcePos, moduleId);
                    }
                }
            }
            arguments.push_back(std::unique_ptr<BoundExpression>(expression.release()));
        }
        BoundDelegateCall* delegateCall = new BoundDelegateCall(sourcePos, moduleId, delegateTypeSymbol);
        for (std::unique_ptr<BoundExpression>& argument : arguments)
        {
            delegateCall->AddArgument(std::move(argument));
        }
        cmajor::symbols::LocalVariableSymbol* temporary = nullptr;
        if (delegateTypeSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            cmajor::symbols::TypeSymbol* type = delegateTypeSymbol->ReturnType();
            if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
                if (!specialization->IsBound())
                {
                    boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
                }
            }
            temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, invokeNode.GetSourcePos(), invokeNode.ModuleId());
            delegateCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)),
                type->AddPointer(invokeNode.GetSourcePos(), invokeNode.ModuleId()))));
            if (type->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
                if (classType->Destructor())
                {
                    std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                    destructorCall->AddArgument(std::unique_ptr<BoundExpression>(delegateCall->Arguments().back()->Clone()));
                    boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
                }
            }
        }
        expression.reset(delegateCall);
        if (temporary)
        {
            expression.reset(new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary))));
            expression->SetFlag(BoundExpressionFlags::bindToRvalueReference);
        }
        return;
    }
    else if (expression->GetType()->PlainType(sourcePos, moduleId)->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
    {
        cmajor::symbols::TypeSymbol* type = expression->GetType();
        if (type->IsReferenceType())
        {
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundReferenceToPointerExpression(std::move(expression), type->RemoveReference(sourcePos, moduleId)->AddPointer(sourcePos, moduleId))));
        }
        else
        {
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::move(expression), type->AddPointer(sourcePos, moduleId))));
        }
        cmajor::symbols::ClassDelegateTypeSymbol* classDelegateTypeSymbol = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(type->BaseType());
        int n = invokeNode.Arguments().Count();
        if (n != classDelegateTypeSymbol->Arity())
        {
            throw cmajor::symbols::Exception("wrong number of arguments for calling delegate type '" + util::ToUtf8(classDelegateTypeSymbol->FullName()) + "'", sourcePos, moduleId);
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TypeSymbol* classDelegateParameterType = classDelegateTypeSymbol->Parameters()[i]->GetType();
            cmajor::ast::Node* argument = invokeNode.Arguments()[i];
            argument->Accept(*this);
            cmajor::symbols::TypeSymbol* argumentType = expression->GetType();
            if (!TypesEqual(argumentType, classDelegateParameterType))
            {
                if (TypesEqual(argumentType->PlainType(sourcePos, moduleId), classDelegateParameterType->PlainType(sourcePos, moduleId)))
                {
                    if (argumentType->IsReferenceType() && !classDelegateParameterType->IsReferenceType())
                    {
                        cmajor::symbols::TypeSymbol* type = argumentType->RemoveReference(sourcePos, moduleId);
                        BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(expression), type);
                        expression.reset(dereferenceExpression);
                    }
                    else if (!argumentType->IsReferenceType() && (classDelegateParameterType->IsReferenceType() || classDelegateParameterType->IsClassTypeSymbol()))
                    {
                        cmajor::symbols::TypeSymbol* type = argumentType->AddLvalueReference(sourcePos, moduleId);
                        BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(expression), type);
                        expression.reset(addressOfExpression);
                    }
                }
                else
                {
                    ArgumentMatch argumentMatch;
                    cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(argumentType, classDelegateParameterType, containerScope, boundFunction, sourcePos, moduleId, argumentMatch);
                    if (conversionFun)
                    {
                        BoundConversion* conversion = new BoundConversion(std::move(expression), conversionFun);
                        expression.reset(conversion);
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("cannot convert '" + util::ToUtf8(argumentType->FullName()) + "' type argument to '" + util::ToUtf8(classDelegateParameterType->FullName()) + "' type parameter",
                            argument->GetSourcePos(), argument->ModuleId(), sourcePos, moduleId);
                    }
                }
            }
            arguments.push_back(std::unique_ptr<BoundExpression>(expression.release()));
        }
        BoundClassDelegateCall* classDelegateCall = new BoundClassDelegateCall(sourcePos, moduleId, classDelegateTypeSymbol);
        for (std::unique_ptr<BoundExpression>& argument : arguments)
        {
            classDelegateCall->AddArgument(std::move(argument));
        }
        cmajor::symbols::LocalVariableSymbol* temporary = nullptr;
        if (classDelegateTypeSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            cmajor::symbols::TypeSymbol* type = classDelegateTypeSymbol->ReturnType();
            if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
                if (!specialization->IsBound())
                {
                    boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
                }
            }
            temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, invokeNode.GetSourcePos(), invokeNode.ModuleId());
            classDelegateCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)),
                type->AddPointer(invokeNode.GetSourcePos(), invokeNode.ModuleId()))));
            if (type->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
                if (classType->Destructor())
                {
                    std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                    destructorCall->AddArgument(std::unique_ptr<BoundExpression>(classDelegateCall->Arguments().back()->Clone()));
                    boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
                }
            }
        }
        expression.reset(classDelegateCall);
        if (temporary)
        {
            expression.reset(new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary))));
            expression->SetFlag(BoundExpressionFlags::bindToRvalueReference);
        }
        return;
    }
    else
    {
        throw cmajor::symbols::Exception("invoke cannot be applied to this type of expression", invokeNode.Subject()->GetSourcePos(), invokeNode.Subject()->ModuleId());
    }
    int n = invokeNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* argument = invokeNode.Arguments()[i];
        argument->Accept(*this);
        if (expression->GetType()->GetSymbolType() != cmajor::symbols::SymbolType::functionGroupTypeSymbol && !scopeQualified)
        {
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, expression->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
        }
        arguments.push_back(std::unique_ptr<BoundExpression>(expression.release()));
    }
    if (!scopeQualified)
    {
        functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    }
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::unique_ptr<cmajor::symbols::Exception> thisEx;
    std::unique_ptr<cmajor::symbols::Exception> nsEx;
    if (!arguments.empty() && arguments[0]->GetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr))
    {
        argIsExplicitThisOrBasePtr = true;
    }
    std::unique_ptr<BoundFunctionCall> functionCall = ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction,
        invokeNode.GetSourcePos(), invokeNode.ModuleId(), OverloadResolutionFlags::dontThrow, templateArgumentTypes, exception);
    if (!functionCall)
    {
        cmajor::symbols::ParameterSymbol* thisParam = boundFunction->GetFunctionSymbol()->GetThisParam();
        bool thisParamInserted = false;
        if (thisParam)
        {
            BoundParameter* boundThisParam = new BoundParameter(invokeNode.GetSourcePos(), invokeNode.ModuleId(), thisParam);
            arguments.insert(arguments.begin(), std::unique_ptr<BoundExpression>(boundThisParam));
            thisParamInserted = true;
            functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base, thisParam->GetType()->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
            functionCall = std::move(ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, invokeNode.GetSourcePos(), invokeNode.ModuleId(),
                OverloadResolutionFlags::dontThrow, templateArgumentTypes, thisEx));
        }
        if (!functionCall)
        {
            if (thisParamInserted)
            {
                arguments.erase(arguments.begin());
            }
            if (!arguments.empty())
            {
                arguments.erase(arguments.begin());
            }
            functionCall = std::move(ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, invokeNode.GetSourcePos(), invokeNode.ModuleId(),
                OverloadResolutionFlags::dontThrow, templateArgumentTypes, nsEx));
        }
    }
    if (!functionCall)
    {
        if (cmajor::symbols::CastOverloadException* castException = dynamic_cast<cmajor::symbols::CastOverloadException*>(exception.get()))
        {
            throw* exception;
        }
        if (cmajor::symbols::CastOverloadException* castException = dynamic_cast<cmajor::symbols::CastOverloadException*>(thisEx.get()))
        {
            throw* thisEx;
        }
        if (cmajor::symbols::CastOverloadException* castException = dynamic_cast<cmajor::symbols::CastOverloadException*>(nsEx.get()))
        {
            throw* nsEx;
        }
        if (cmajor::symbols::CannotBindConstToNonconstOverloadException* bindException = dynamic_cast<cmajor::symbols::CannotBindConstToNonconstOverloadException*>(exception.get()))
        {
            throw* exception;
        }
        if (cmajor::symbols::CannotBindConstToNonconstOverloadException* bindException = dynamic_cast<cmajor::symbols::CannotBindConstToNonconstOverloadException*>(thisEx.get()))
        {
            throw* thisEx;
        }
        if (cmajor::symbols::CannotBindConstToNonconstOverloadException* bindException = dynamic_cast<cmajor::symbols::CannotBindConstToNonconstOverloadException*>(nsEx.get()))
        {
            throw* nsEx;
        }
        if (cmajor::symbols::CannotAssignToConstOverloadException* assignmentException = dynamic_cast<cmajor::symbols::CannotAssignToConstOverloadException*>(exception.get()))
        {
            throw* exception;
        }
        if (cmajor::symbols::CannotAssignToConstOverloadException* assignmentException = dynamic_cast<cmajor::symbols::CannotAssignToConstOverloadException*>(thisEx.get()))
        {
            throw* thisEx;
        }
        if (cmajor::symbols::CannotAssignToConstOverloadException* assignmentException = dynamic_cast<cmajor::symbols::CannotAssignToConstOverloadException*>(nsEx.get()))
        {
            throw* nsEx;
        }
        cmajor::symbols::Exception* ex = exception.get();
        if (dynamic_cast<cmajor::symbols::NoViableFunctionException*>(ex) && thisEx)
        {
            ex = thisEx.get();
        }
        if (dynamic_cast<cmajor::symbols::NoViableFunctionException*>(ex) && nsEx)
        {
            ex = nsEx.get();
        }
        if (ex)
        {
            throw* ex;
        }
        else
        {
            throw cmajor::symbols::Exception("overload resolution failed: overload not found", invokeNode.GetSourcePos(), invokeNode.ModuleId());
        }
    }
    CheckAccess(boundFunction->GetFunctionSymbol(), functionCall->GetFunctionSymbol());
    cmajor::symbols::FunctionSymbol* functionSymbol = functionCall->GetFunctionSymbol();
    if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol && !functionSymbol->IsStatic() && functionSymbol->IsVirtualAbstractOrOverride())
    {
        if (argIsExplicitThisOrBasePtr)
        {
            if (functionSymbol->IsAbstract())
            {
                throw cmajor::symbols::Exception("cannot call abstract member function", sourcePos, moduleId, functionSymbol->GetSourcePos(), functionSymbol->SourceModuleId());
            }
        }
        else
        {
            functionCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
    }
    if (functionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::TypeSymbol* type = functionSymbol->ReturnType();
        if (type->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
            if (!specialization->IsBound())
            {
                boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, invokeNode.GetSourcePos(), invokeNode.ModuleId());
            }
        }
        temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(type, invokeNode.GetSourcePos(), invokeNode.ModuleId());
        functionCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(invokeNode.GetSourcePos(), invokeNode.ModuleId(), temporary)),
            type->AddPointer(invokeNode.GetSourcePos(), invokeNode.ModuleId()))));
        if (type->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
            if (classType->Destructor())
            {
                std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                destructorCall->AddArgument(std::unique_ptr<BoundExpression>(functionCall->Arguments().back()->Clone()));
                boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
            }
        }
    }
    expression.reset(functionCall.release());
    if (temporary)
    {
        expression.reset(new BoundConstructAndReturnTemporaryExpression(std::move(expression), std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary))));
        expression->SetFlag(BoundExpressionFlags::bindToRvalueReference);
    }
    if (functionSymbol->IsConstExpr())
    {
        cmajor::symbols::TypeSymbol* returnType = functionSymbol->ReturnType();
        if (returnType && !returnType->IsVoidType())
        {
            std::unique_ptr<cmajor::symbols::Value> value = Evaluate(&invokeNode, returnType, containerScope, boundCompileUnit, true, boundFunction, sourcePos, moduleId);
            if (value)
            {
                cmajor::symbols::TypeSymbol* type = value->GetType(&symbolTable);
                BoundLiteral* literal = new BoundLiteral(std::move(value), type);
                expression.reset(literal);
            }
        }
    }
    else if (functionSymbol->IsCompileTimePrimitiveFunction())
    {
        cmajor::symbols::TypeSymbol* returnType = functionSymbol->ReturnType();
        if (returnType && !returnType->IsVoidType())
        {
            std::unique_ptr<cmajor::symbols::Value> value = Evaluate(&invokeNode, returnType, containerScope, boundCompileUnit, true, boundFunction, sourcePos, moduleId);
            if (value)
            {
                cmajor::symbols::TypeSymbol* type = value->GetType(&symbolTable);
                BoundLiteral* literal = new BoundLiteral(std::move(value), type);
                expression.reset(literal);
            }
        }
    }
    if (functionSymbol->FullName() == U"System.CaptureCurrentException()")
    {
        if (!statementBinder->InsideCatch())
        {
            throw cmajor::symbols::Exception("System.CaptureCurrentException() can only be called from inside a catch block", sourcePos, moduleId);
        }
        else
        {
            expression->SetFlag(BoundExpressionFlags::exceptionCapture);
        }
    }
    if (GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && functionSymbol->HasSource())
    {
        symbolTable.MapInvoke(invokeId, functionSymbol);
    }
    if (functionSymbol->HasSource())
    {
        cmajor::symbols::MapIdentifierToSymbolDefinition(invokeId, functionSymbol);
    }
}

void ExpressionBinder::Visit(cmajor::ast::PostfixIncrementNode& postfixIncrementNode)
{
    if (statementBinder->CompilingThrow())
    {
        throw cmajor::symbols::Exception("postfix increment in throw expression no allowed", postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId());
    }
    bool prevInhibitCompile = inhibitCompile;
    inhibitCompile = true;
    postfixIncrementNode.Subject()->Accept(*this);
    inhibitCompile = prevInhibitCompile;
    if (!inhibitCompile)
    {
        if (expression->GetType()->PlainType(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId())->IsClassTypeSymbol())
        {
            cmajor::ast::CloneContext cloneContext;
            cmajor::ast::ExpressionStatementNode prefixIncrementExpression(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 
                new cmajor::ast::PrefixIncrementNode(postfixIncrementNode.GetSourcePos(),
                postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext)));
            statementBinder->CompileStatement(&prefixIncrementExpression, true);
        }
        else
        {
            if (expression->GetType()->IsUnsignedType())
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 
                    postfixIncrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::AddNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::ByteLiteralNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 1u)));
                statementBinder->CompileStatement(&assignmentStatement, true);
            }
            else
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 
                    postfixIncrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::AddNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), postfixIncrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::SByteLiteralNode(postfixIncrementNode.GetSourcePos(), postfixIncrementNode.ModuleId(), 1)));
                statementBinder->CompileStatement(&assignmentStatement, true);
            }
        }
    }
    postfixIncrementNode.Subject()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::ast::PostfixDecrementNode& postfixDecrementNode)
{
    if (statementBinder->CompilingThrow())
    {
        throw cmajor::symbols::Exception("postfix decrement in throw expression no allowed", postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId());
    }
    bool prevInhibitCompile = inhibitCompile;
    inhibitCompile = true;
    postfixDecrementNode.Subject()->Accept(*this);
    inhibitCompile = prevInhibitCompile;
    if (!inhibitCompile)
    {
        if (expression->GetType()->PlainType(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId())->IsClassTypeSymbol())
        {
            cmajor::ast::CloneContext cloneContext;
            cmajor::ast::ExpressionStatementNode prefixDecrementExpression(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), 
                new cmajor::ast::PrefixDecrementNode(postfixDecrementNode.GetSourcePos(),
                postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext)));
            statementBinder->CompileStatement(&prefixDecrementExpression, true);
        }
        else
        {
            if (expression->GetType()->IsUnsignedType())
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::SubNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::ByteLiteralNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), 1u)));
                statementBinder->CompileStatement(&assignmentStatement, true);
            }
            else
            {
                cmajor::ast::CloneContext cloneContext;
                cmajor::ast::AssignmentStatementNode assignmentStatement(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                    new cmajor::ast::SubNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), postfixDecrementNode.Subject()->Clone(cloneContext),
                        new cmajor::ast::SByteLiteralNode(postfixDecrementNode.GetSourcePos(), postfixDecrementNode.ModuleId(), 1)));
                statementBinder->CompileStatement(&assignmentStatement, true);
            }
        }
    }
    postfixDecrementNode.Subject()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::ast::SizeOfNode& sizeOfNode)
{
    sizeOfNode.Expression()->Accept(*this);
    if (expression->GetBoundNodeType() == BoundNodeType::boundTypeExpression && expression->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(expression->GetType());
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = classGroup->GetClass(0);
        if (classTypeSymbol)
        {
            expression.reset(new BoundTypeExpression(sourcePos, moduleId, classTypeSymbol));
        }
        else
        {
            throw cmajor::symbols::Exception("ordinary class not found from class group '" + util::ToUtf8(classGroup->FullName()) + "'", sourcePos, moduleId, classGroup->GetSourcePos(), classGroup->SourceModuleId());
        }
    }
    expression.reset(new BoundSizeOfExpression(sizeOfNode.GetSourcePos(), sizeOfNode.ModuleId(), symbolTable.GetTypeByName(U"long"), expression->GetType()->AddPointer(sizeOfNode.GetSourcePos(), sizeOfNode.ModuleId())));
}

void ExpressionBinder::Visit(cmajor::ast::TypeNameNode& typeNameNode)
{
    bool staticTypeName = typeNameNode.Static();
    std::unique_ptr<BoundExpression> expr = BindExpression(typeNameNode.Expression(), boundCompileUnit, boundFunction, containerScope, statementBinder, false, false, true, false);
    cmajor::symbols::TypeSymbol* type = expr->GetType();
    if (type->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(type);
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = classGroup->GetClass(0);
        if (!classTypeSymbol)
        {
            throw cmajor::symbols::Exception("ordinary class not found from class group '" + util::ToUtf8(classGroup->FullName()) + "'", sourcePos, moduleId, classGroup->GetSourcePos(), classGroup->SourceModuleId());
        }
        expr.reset(new BoundTypeExpression(sourcePos, moduleId, classTypeSymbol));
        type = classTypeSymbol;
        staticTypeName = true;
    }
    if (expr->GetType()->PlainType(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(expr->GetType()->BaseType());
        if (!staticTypeName && classType->IsPolymorphic())
        {
            if (expr->GetBoundNodeType() == BoundNodeType::boundDereferenceExpression)
            {
                BoundDereferenceExpression* derefExpr = static_cast<BoundDereferenceExpression*>(expr.get());
                expr.reset(derefExpr->Subject().release());
            }
            else
            {
                cmajor::symbols::TypeSymbol* ptrType = expr->GetType()->AddPointer(typeNameNode.GetSourcePos(), typeNameNode.ModuleId());
                expr.reset(new BoundAddressOfExpression(std::move(expr), ptrType));
            }
            expression.reset(new BoundTypeNameExpression(std::move(expr), symbolTable.GetTypeByName(U"char")->AddConst(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())->AddPointer(
                typeNameNode.GetSourcePos(), typeNameNode.ModuleId())));
        }
        else
        {
            expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(typeNameNode.GetSourcePos(), typeNameNode.ModuleId(), boundCompileUnit.Install(util::ToUtf8(classType->FullName())),
                util::ToUtf8(classType->FullName()))), symbolTable.GetTypeByName(U"char")->AddConst(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())->AddPointer(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())));
        }
    }
    else
    {
        expression.reset(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(typeNameNode.GetSourcePos(), typeNameNode.ModuleId(), boundCompileUnit.Install(util::ToUtf8(expr->GetType()->FullName())),
            util::ToUtf8(expr->GetType()->FullName()))), symbolTable.GetTypeByName(U"char")->AddConst(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())->AddPointer(typeNameNode.GetSourcePos(), typeNameNode.ModuleId())));
    }
}

void ExpressionBinder::Visit(cmajor::ast::TypeIdNode& typeIdNode)
{
    std::unique_ptr<BoundExpression> expr = BindExpression(typeIdNode.Expression(), boundCompileUnit, boundFunction, containerScope, statementBinder, false, false, true, false);
    if (expr->GetType()->IsPointerType())
    {
        cmajor::symbols::TypeSymbol* exprBaseType = expr->GetType()->RemovePointer(sourcePos, moduleId);
        if (exprBaseType->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* exprClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(exprBaseType);
            if (exprClassType->IsPolymorphic())
            {
                expression.reset(new BoundTypeIdExpression(std::move(expr), symbolTable.GetTypeByName(U"ulong")));
            }
            else
            {
                throw cmajor::symbols::Exception("typeid can be applied to a pointer to a polymorphic class type expression",
                    typeIdNode.GetSourcePos(), typeIdNode.ModuleId(), boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("typeid can be applied to a pointer to a polymorphic class type expression",
                typeIdNode.GetSourcePos(), typeIdNode.ModuleId(), boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("typeid can be applied to a pointer to a polymorphic class type expression",
            typeIdNode.GetSourcePos(), typeIdNode.ModuleId(), boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::CastNode& castNode)
{
    cmajor::symbols::TypeSymbol* targetType = ResolveType(castNode.TargetTypeExpr(), boundCompileUnit, containerScope);
    castNode.SourceExpr()->Accept(*this);
    std::vector<std::unique_ptr<BoundExpression>> targetExprArgs;
    targetExprArgs.push_back(std::unique_ptr<BoundExpression>(new BoundTypeExpression(castNode.GetSourcePos(), castNode.ModuleId(), targetType)));
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, targetType->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::unique_ptr<BoundFunctionCall> castFunctionCall = ResolveOverload(U"@return", containerScope, functionScopeLookups, targetExprArgs, boundCompileUnit, boundFunction, castNode.GetSourcePos(),
        castNode.ModuleId());
    std::vector<std::unique_ptr<BoundExpression>> castArguments;
    castArguments.push_back(std::move(expression));
    FunctionMatch functionMatch(castFunctionCall->GetFunctionSymbol());
    bool conversionFound = FindConversions(boundCompileUnit, castFunctionCall->GetFunctionSymbol(), castArguments, functionMatch, 
        cmajor::symbols::ConversionType::explicit_, containerScope, boundFunction,
        castNode.GetSourcePos(), castNode.ModuleId());
    if (conversionFound)
    {
        Assert(!functionMatch.argumentMatches.empty(), "argument match expected");
        ArgumentMatch& argumentMatch = functionMatch.argumentMatches[0];
        if (argumentMatch.preReferenceConversionFlags != cmajor::ir::OperationFlags::none)
        {
            if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
            {
                cmajor::symbols::TypeSymbol* type = castArguments[0]->GetType()->AddLvalueReference(sourcePos, moduleId);
                BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(castArguments[0]), type);
                castArguments[0].reset(addressOfExpression);
            }
            else if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
            {
                cmajor::symbols::TypeSymbol* type = castArguments[0]->GetType()->RemoveReference(sourcePos, moduleId);
                BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(castArguments[0]), type);
                castArguments[0].reset(dereferenceExpression);
            }
        }
        cmajor::symbols::FunctionSymbol* conversionFun = argumentMatch.conversionFun;
        if (conversionFun)
        {
            if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
            {
                BoundFunctionCall* constructorCall = new BoundFunctionCall(sourcePos, moduleId, conversionFun);
                if (conversionFun->ConversionTargetType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
                {
                    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(conversionFun->ConversionTargetType());
                    if (!specialization->IsBound())
                    {
                        boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
                    }
                }
                cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(conversionFun->ConversionTargetType(), sourcePos, moduleId);
                constructorCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)),
                    conversionFun->ConversionTargetType()->AddPointer(sourcePos, moduleId))));
                cmajor::symbols::TypeSymbol* conversionTargetType = conversionFun->ConversionTargetType();
                if (conversionTargetType->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(conversionTargetType);
                    if (classType->Destructor())
                    {
                        std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                        destructorCall->AddArgument(std::unique_ptr<BoundExpression>(constructorCall->Arguments()[0]->Clone()));
                        boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
                    }
                }
                constructorCall->AddArgument(std::move(castArguments[0]));
                BoundConstructAndReturnTemporaryExpression* conversion = new BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>(constructorCall),
                    std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)));
                castArguments[0].reset(conversion);
            }
            else
            {
                castArguments[0].reset(new BoundConversion(std::unique_ptr<BoundExpression>(castArguments[0].release()), conversionFun));
            }
        }
        if (argumentMatch.postReferenceConversionFlags != cmajor::ir::OperationFlags::none)
        {
            if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
            {
                cmajor::symbols::TypeSymbol* type = castArguments[0]->GetType()->AddLvalueReference(sourcePos, moduleId);
                BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(castArguments[0]), type);
                castArguments[0].reset(addressOfExpression);
            }
            else if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
            {
                cmajor::symbols::TypeSymbol* type = castArguments[0]->GetType()->RemoveReference(sourcePos, moduleId);
                BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(castArguments[0]), type);
                castArguments[0].reset(dereferenceExpression);
            }
        }
        castFunctionCall->SetArguments(std::move(castArguments));
    }
    else
    {
        throw cmajor::symbols::Exception("no explicit conversion from '" + util::ToUtf8(castArguments[0]->GetType()->FullName()) + "' to '" + util::ToUtf8(targetType->FullName()) + "' exists",
            castNode.GetSourcePos(), castNode.ModuleId(), boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId());
    }
    CheckAccess(boundFunction->GetFunctionSymbol(), castFunctionCall->GetFunctionSymbol());
    expression.reset(castFunctionCall.release());
}

void ExpressionBinder::Visit(cmajor::ast::ConstructNode& constructNode)
{
    cmajor::symbols::TypeSymbol* resultType = nullptr;
    int n = constructNode.Arguments().Count();
    if (n == 0)
    {
        throw cmajor::symbols::Exception("must supply at least one argument to construct expression", constructNode.GetSourcePos(), constructNode.ModuleId());
    }
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* argumentNode = constructNode.Arguments()[i];
        if (i == 0)
        {
            cmajor::ast::CloneContext cloneContext;
            cmajor::ast::CastNode castNode(constructNode.GetSourcePos(), constructNode.ModuleId(), new cmajor::ast::PointerNode(constructNode.GetSourcePos(), constructNode.ModuleId(),
                constructNode.TypeExpr()->Clone(cloneContext)), argumentNode->Clone(cloneContext));
            castNode.Accept(*this);
            resultType = expression->GetType();
            if (!resultType->IsPointerType())
            {
                throw cmajor::symbols::Exception("first argument of a construct expression must be of a pointer type", argumentNode->GetSourcePos(), argumentNode->ModuleId());
            }
            if (!resultType->RemovePointer(constructNode.GetSourcePos(), constructNode.ModuleId())->IsClassTypeSymbol())
            {
                expression->SetFlag(BoundExpressionFlags::deref);
            }
        }
        else
        {
            argumentNode->Accept(*this);
        }
        arguments.push_back(std::move(expression));
    }
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent,
        resultType->RemovePointer(constructNode.GetSourcePos(), constructNode.ModuleId())->ClassInterfaceEnumDelegateOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    expression = ResolveOverload(U"@constructor", containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, constructNode.GetSourcePos(), constructNode.ModuleId());
    expression.reset(new BoundConstructExpression(std::move(expression), resultType));
}

void ExpressionBinder::Visit(cmajor::ast::NewNode& newNode)
{
    cmajor::ast::CloneContext cloneContext;
    cmajor::ast::InvokeNode* invokeMemAlloc = nullptr;
    bool memDebug = boundCompileUnit.GetModule().IsSymbolDefined(U"MEM_DEBUG");
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (memDebug)
        {
            invokeMemAlloc = new cmajor::ast::InvokeNode(newNode.GetSourcePos(), newNode.ModuleId(), new cmajor::ast::IdentifierNode(newNode.GetSourcePos(), newNode.ModuleId(), U"RtMemAllocInfo"));
        }
        else
        {
            invokeMemAlloc = new cmajor::ast::InvokeNode(newNode.GetSourcePos(), newNode.ModuleId(), new cmajor::ast::IdentifierNode(newNode.GetSourcePos(), newNode.ModuleId(), U"RtMemAlloc"));
        }
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        invokeMemAlloc = new cmajor::ast::InvokeNode(newNode.GetSourcePos(), newNode.ModuleId(), new cmajor::ast::DotNode(newNode.GetSourcePos(), newNode.ModuleId(),
            new cmajor::ast::IdentifierNode(newNode.GetSourcePos(), newNode.ModuleId(), U"System"), 
            new cmajor::ast::IdentifierNode(newNode.GetSourcePos(), newNode.ModuleId(), U"MemAlloc")));
    }
    invokeMemAlloc->AddArgument(new cmajor::ast::SizeOfNode(newNode.GetSourcePos(), newNode.ModuleId(), newNode.TypeExpr()->Clone(cloneContext)));
    if (memDebug)
    {
        cmajor::ast::TypeNameNode* typeNameNode = new cmajor::ast::TypeNameNode(newNode.GetSourcePos(), newNode.ModuleId(), newNode.TypeExpr()->Clone(cloneContext));
        typeNameNode->SetStatic();
        invokeMemAlloc->AddArgument(typeNameNode);
    }
    cmajor::ast::CastNode castNode(newNode.GetSourcePos(), newNode.ModuleId(), new cmajor::ast::PointerNode(newNode.GetSourcePos(), newNode.ModuleId(), newNode.TypeExpr()->Clone(cloneContext)), invokeMemAlloc);
    castNode.Accept(*this);
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    cmajor::symbols::TypeSymbol* resultType = expression->GetType();
    if (!resultType->RemovePointer(newNode.GetSourcePos(), newNode.ModuleId())->IsClassTypeSymbol())
    {
        expression->SetFlag(BoundExpressionFlags::deref);
    }
    arguments.push_back(std::move(expression));
    int n = newNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        newNode.Arguments()[i]->Accept(*this);
        arguments.push_back(std::move(expression));
    }
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, resultType->RemovePointer(newNode.GetSourcePos(), newNode.ModuleId())->ClassInterfaceEnumDelegateOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    expression = ResolveOverload(U"@constructor", containerScope, functionScopeLookups, arguments, boundCompileUnit, boundFunction, newNode.GetSourcePos(), newNode.ModuleId());
    expression.reset(new BoundConstructExpression(std::move(expression), resultType));
}

void ExpressionBinder::Visit(cmajor::ast::ThisNode& thisNode)
{
    cmajor::symbols::ParameterSymbol* thisParam = boundFunction->GetFunctionSymbol()->GetThisParam();
    if (thisParam)
    {
        expression.reset(new BoundParameter(thisNode.GetSourcePos(), thisNode.ModuleId(), thisParam));
        expression->SetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
    }
    else
    {
        throw cmajor::symbols::Exception("'this' can only be used in member function context", thisNode.GetSourcePos(), thisNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::BaseNode& baseNode)
{
    cmajor::symbols::ParameterSymbol* thisParam = boundFunction->GetFunctionSymbol()->GetThisParam();
    if (thisParam)
    {
        cmajor::symbols::TypeSymbol* thisType = thisParam->GetType()->BaseType();
        if (thisType->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* thisClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(thisType);
            if (thisClassType->BaseClass())
            {
                cmajor::symbols::TypeSymbol* basePointerType = thisClassType->BaseClass()->AddPointer(baseNode.GetSourcePos(), baseNode.ModuleId());
                if (thisParam->GetType()->IsConstType())
                {
                    basePointerType = basePointerType->AddConst(baseNode.GetSourcePos(), baseNode.ModuleId());
                }
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* thisAsBaseConversionFunction = boundCompileUnit.GetConversion(thisParam->GetType(), basePointerType, containerScope, boundFunction, baseNode.GetSourcePos(), baseNode.ModuleId(), argumentMatch);
                if (thisAsBaseConversionFunction)
                {
                    expression.reset(new BoundConversion(std::unique_ptr<BoundExpression>(new BoundParameter(baseNode.GetSourcePos(), baseNode.ModuleId(), thisParam)), thisAsBaseConversionFunction));
                    expression->SetFlag(BoundExpressionFlags::argIsExplicitThisOrBasePtr);
                }
                else
                {
                    throw cmajor::symbols::Exception("cannot convert from '" + util::ToUtf8(thisParam->GetType()->FullName()) + "' to '" + util::ToUtf8(basePointerType->FullName()) + "'", baseNode.GetSourcePos(), baseNode.ModuleId());
                }
            }
            else
            {
                throw cmajor::symbols::Exception("class '" + util::ToUtf8(thisClassType->FullName()) + "' does not have a base class", baseNode.GetSourcePos(), baseNode.ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("'base' can only be used in member function context", baseNode.GetSourcePos(), baseNode.ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("'base' can only be used in member function context", baseNode.GetSourcePos(), baseNode.ModuleId());
    }
}

void ExpressionBinder::Visit(cmajor::ast::ParenthesizedExpressionNode& parenthesizedExpressionNode)
{
    parenthesizedExpressionNode.Subject()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::ast::FunctionPtrNode& functionPtrNode)
{
    BoundExpression* boundExpression = static_cast<BoundFunctionPtr*>(functionPtrNode.GetBoundExpression());
    expression.reset(boundExpression->Clone());
}

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder)
{
    return BindExpression(node, boundCompileUnit, boundFunction, containerScope, statementBinder, false);
}

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue)
{
    return BindExpression(node, boundCompileUnit, boundFunction, containerScope, statementBinder, lvalue, false);
}

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression)
{
    return BindExpression(node, boundCompileUnit, boundFunction, containerScope, statementBinder, lvalue, acceptFunctionGroupOrMemberExpression, false);
}

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression, bool acceptIncomplete)
{
    return BindExpression(node, boundCompileUnit, boundFunction, containerScope, statementBinder, lvalue, acceptFunctionGroupOrMemberExpression, acceptIncomplete, true);
}

std::unique_ptr<BoundExpression> BindExpression(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, 
    cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder, bool lvalue, bool acceptFunctionGroupOrMemberExpression, bool acceptIncomplete, 
    bool moveTemporaryDestructorCalls)
{
    ExpressionBinder expressionBinder(node->GetSourcePos(), node->ModuleId(), boundCompileUnit, boundFunction, containerScope, statementBinder, lvalue);
    node->Accept(expressionBinder);
    std::unique_ptr<BoundExpression> expression = expressionBinder.GetExpression();
    if (!expression)
    {
        throw cmajor::symbols::Exception("could not bind expression", node->GetSourcePos(), node->ModuleId());
    }
    if (moveTemporaryDestructorCalls)
    {
        boundFunction->MoveTemporaryDestructorCallsTo(*expression);
    }
    if (acceptFunctionGroupOrMemberExpression && (expression->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression || expression->GetBoundNodeType() == BoundNodeType::boundMemberExpression))
    {
        return expression;
    }
    if (!acceptIncomplete)
    {
        if (!expression->IsComplete())
        {
            throw cmajor::symbols::Exception("incomplete expression", node->GetSourcePos(), node->ModuleId());
        }
    }
    if (lvalue && !expression->IsLvalueExpression())
    {
        throw cmajor::symbols::Exception("not an lvalue expression", node->GetSourcePos(), node->ModuleId());
    }
    return expression;
}

std::unique_ptr<BoundExpression> BindUnaryOp(BoundExpression* operand, cmajor::ast::Node& node, const std::u32string& groupName,
    BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction, cmajor::symbols::ContainerScope* containerScope, StatementBinder* statementBinder)
{
    ExpressionBinder expressionBinder(node.GetSourcePos(), node.ModuleId(), boundCompileUnit, boundFunction, containerScope, statementBinder, false);
    expressionBinder.BindUnaryOp(operand, node, groupName);
    std::unique_ptr<BoundExpression> expression = expressionBinder.GetExpression();
    if (!expression)
    {
        throw cmajor::symbols::Exception("cound not bind expression", node.GetSourcePos(), node.ModuleId());
    }
    return expression;
}

} // namespace cmajor::binder
