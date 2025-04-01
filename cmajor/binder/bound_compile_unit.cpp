// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.bound.compile.unit;

import cmajor.binder.bound.node.visitor;
import cmajor.binder.overload.resolution;
import cmajor.binder.bound_namespace;
import cmajor.binder.bound.expression;
import cmajor.binder.statement.binder;
import cmajor.binder.bound.statement;
import cmajor.ir;
import std.filesystem;
import util;

namespace cmajor::binder {

class ClassTypeConversion : public cmajor::symbols::FunctionSymbol
{
public:
    ClassTypeConversion(const std::u32string& name_, cmajor::symbols::ConversionType conversionType_, uint8_t conversionDistance_, cmajor::symbols::TypeSymbol* sourceType_, 
        cmajor::symbols::TypeSymbol* targetType_);
    cmajor::symbols::ConversionType GetConversionType() const override { return conversionType; }
    uint8_t ConversionDistance() const override { return conversionDistance; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    const char* ClassName() const override { return "ClassTypeConversion"; }
private:
    cmajor::symbols::ConversionType conversionType;
    uint8_t conversionDistance;
    cmajor::symbols::TypeSymbol* sourceType;
    cmajor::symbols::TypeSymbol* targetType;
};

ClassTypeConversion::ClassTypeConversion(const std::u32string& name_, cmajor::symbols::ConversionType conversionType_, uint8_t conversionDistance_, cmajor::symbols::TypeSymbol* sourceType_, cmajor::symbols::TypeSymbol* targetType_) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), name_), conversionType(conversionType_), conversionDistance(conversionDistance_), sourceType(sourceType_), targetType(targetType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(sourceType);
    SetConversionTargetType(targetType);
}

void ClassTypeConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateBitCast(value, targetType->IrType(emitter, context)));
}

class NullPtrToPtrConversion : public cmajor::symbols::FunctionSymbol
{
public:
    NullPtrToPtrConversion(cmajor::symbols::TypeSymbol* nullPtrType_, cmajor::symbols::TypeSymbol* targetPointerType_, cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    std::unique_ptr<cmajor::symbols::Value> ConvertValue(const std::unique_ptr<cmajor::symbols::Value>& value, cmajor::symbols::Context* context) const override;
    const char* ClassName() const override { return "NullPtrToPtrConversion"; }
private:
    cmajor::symbols::TypeSymbol* nullPtrType;
    cmajor::symbols::TypeSymbol* targetPointerType;
};

NullPtrToPtrConversion::NullPtrToPtrConversion(cmajor::symbols::TypeSymbol* nullPtrType_, cmajor::symbols::TypeSymbol* targetPointerType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"nullptr2ptr"), nullPtrType(nullPtrType_), targetPointerType(targetPointerType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(nullPtrType->PlainType(context));
    SetConversionTargetType(targetPointerType->PlainType(context));

}

void NullPtrToPtrConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateBitCast(value, targetPointerType->IrType(emitter, context)));
}

std::unique_ptr<cmajor::symbols::Value> NullPtrToPtrConversion::ConvertValue(const std::unique_ptr<cmajor::symbols::Value>& value, cmajor::symbols::Context* context) const
{
    cmajor::symbols::TypeSymbol* type = value->GetType(&nullPtrType->GetModule()->GetSymbolTable(), context);
    if (type->IsPointerType())
    {
        return std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::PointerValue(value->GetSpan(), type, nullptr));
    }
    else
    {
        return std::unique_ptr<cmajor::symbols::Value>();
    }
}

class VoidPtrToPtrConversion : public cmajor::symbols::FunctionSymbol
{
public:
    VoidPtrToPtrConversion(cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::TypeSymbol* targetPointerType_, cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    const char* ClassName() const override { return "VoidPtrToPtrConversion"; }
private:
    cmajor::symbols::TypeSymbol* voidPtrType;
    cmajor::symbols::TypeSymbol* targetPointerType;
};

VoidPtrToPtrConversion::VoidPtrToPtrConversion(cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::TypeSymbol* targetPointerType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"voidPtr2ptr"), voidPtrType(voidPtrType_), targetPointerType(targetPointerType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(voidPtrType->PlainType(context));
    SetConversionTargetType(targetPointerType->PlainType(context));
}

void VoidPtrToPtrConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateBitCast(value, targetPointerType->IrType(emitter, context)));
}

class PtrToVoidPtrConversion : public cmajor::symbols::FunctionSymbol
{
public:
    PtrToVoidPtrConversion(cmajor::symbols::TypeSymbol* sourcePtrType_, cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 10; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    const char* ClassName() const override { return "PtrToVoidPtrConversion"; }
private:
    cmajor::symbols::TypeSymbol* sourcePtrType;
    cmajor::symbols::TypeSymbol* voidPtrType;
};

PtrToVoidPtrConversion::PtrToVoidPtrConversion(cmajor::symbols::TypeSymbol* sourcePtrType_, cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"ptr2voidPtr"), sourcePtrType(sourcePtrType_), voidPtrType(voidPtrType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(sourcePtrType->PlainType(context));
    SetConversionTargetType(voidPtrType->PlainType(context));
}

void PtrToVoidPtrConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateBitCast(value, voidPtrType->IrType(emitter, context)));
}

class PtrToULongConversion : public cmajor::symbols::FunctionSymbol
{
public:
    PtrToULongConversion(cmajor::symbols::TypeSymbol* ptrType_, cmajor::symbols::TypeSymbol* ulongType_, cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    const char* ClassName() const override { return "PtrToULongConversion"; }
private:
    cmajor::symbols::TypeSymbol* ptrType;
    cmajor::symbols::TypeSymbol* ulongType;
};

PtrToULongConversion::PtrToULongConversion(cmajor::symbols::TypeSymbol* ptrType_, cmajor::symbols::TypeSymbol* ulongType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"ptr2ulong"), ptrType(ptrType_), ulongType(ulongType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(ptrType->PlainType(context));
    SetConversionTargetType(ulongType->PlainType(context));
}

void PtrToULongConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreatePtrToInt(value, ulongType->IrType(emitter, context)));
}

class ULongToVoidPtrConversion : public cmajor::symbols::FunctionSymbol
{
public:
    ULongToVoidPtrConversion(cmajor::symbols::TypeSymbol* ulongType_, cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    const char* ClassName() const override { return "ULongToVoidPtrConversion"; }
private:
    cmajor::symbols::TypeSymbol* ulongType;
    cmajor::symbols::TypeSymbol* voidPtrType;
};

ULongToVoidPtrConversion::ULongToVoidPtrConversion(cmajor::symbols::TypeSymbol* ulongType_, cmajor::symbols::TypeSymbol* voidPtrType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"ulong2voidPtr"), ulongType(ulongType_), voidPtrType(voidPtrType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(ulongType->PlainType(context));
    SetConversionTargetType(voidPtrType->PlainType(context));
}

void ULongToVoidPtrConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
    cmajor::symbols::Context* context)
{
    void* value = emitter.Stack().Pop();
    emitter.Stack().Push(emitter.CreateIntToPtr(value, voidPtrType->IrType(emitter, context)));
}

class CharacterPointerLiteralToStringFunctionContainerConversion : public cmajor::symbols::FunctionSymbol
{
public:
    CharacterPointerLiteralToStringFunctionContainerConversion(cmajor::symbols::TypeSymbol* characterPtrType_, cmajor::symbols::TypeSymbol* stringFunctionContainerType_, 
        cmajor::symbols::Context* context);
    cmajor::symbols::ConversionType GetConversionType() const override { return cmajor::symbols::ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    bool IsBasicTypeOperation() const override { return true; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, 
        cmajor::symbols::Context* context) override;
    std::unique_ptr<cmajor::symbols::Value> ConvertValue(const std::unique_ptr<cmajor::symbols::Value>& value, cmajor::symbols::Context* context) const override;
    const char* ClassName() const override { return "CharacterPointerLiteralToStringFunctionContainerConversion"; }
private:
    cmajor::symbols::TypeSymbol* characterPtrType;
    cmajor::symbols::TypeSymbol* stringFunctionContainerType;
};

CharacterPointerLiteralToStringFunctionContainerConversion::CharacterPointerLiteralToStringFunctionContainerConversion(cmajor::symbols::TypeSymbol* characterPtrType_, 
    cmajor::symbols::TypeSymbol* stringFunctionContainerType_, cmajor::symbols::Context* context) :
    cmajor::symbols::FunctionSymbol(soul::ast::Span(), U"charlit2stringFun"), characterPtrType(characterPtrType_), stringFunctionContainerType(stringFunctionContainerType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetAccess(cmajor::symbols::SymbolAccess::public_);
    SetConversionSourceType(characterPtrType->PlainType(context));
    SetConversionTargetType(stringFunctionContainerType->PlainType(context));
}

void CharacterPointerLiteralToStringFunctionContainerConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, 
    cmajor::ir::OperationFlags flags, cmajor::symbols::Context* context)
{
    throw std::runtime_error(util::ToUtf8(Name()) + " function provides compile time calls only");
}

std::unique_ptr<cmajor::symbols::Value> CharacterPointerLiteralToStringFunctionContainerConversion::ConvertValue(const std::unique_ptr<cmajor::symbols::Value>& value, 
    cmajor::symbols::Context* context) const
{
    return std::unique_ptr<cmajor::symbols::Value>();
}

BoundCompileUnit::BoundCompileUnit(cmajor::symbols::Context* context_, cmajor::ast::CompileUnitNode* compileUnitNode_, AttributeBinder* attributeBinder_) :
    BoundNode(soul::ast::Span(), BoundNodeType::boundCompileUnit), context(context_), symbolTable(context->RootModule()->GetSymbolTable()),
    compileUnitNode(compileUnitNode_), attributeBinder(attributeBinder_), currentNamespace(nullptr),
    hasGotos(false), operationRepository(*this), functionTemplateRepository(*this), classTemplateRepository(*this), inlineFunctionRepository(*this),
    constExprFunctionRepository(*this), conversionTable(cmajor::symbols::ConversionTable::Owner::compileUnit, context, nullptr), bindingTypes(false), compileUnitIndex(-2), 
    immutable(false), nextExitEntryIndex(0),
    systemRuntimeUnwindInfoSymbol(nullptr), systemRuntimeAddCompileUnitFunctionSymbol(nullptr), pushCompileUnitUnwindInfoInitFunctionSymbol(nullptr),
    initUnwindInfoDelegateType(nullptr), globalInitFunctionSymbol(nullptr), latestIdentifierNode(nullptr), fileIndex(-1), moduleId(context->RootModule()->Id()),
    traceEntryTypeSymbol(nullptr), traceGuardTypeSymbol(nullptr), checkerTypeSymbol(nullptr), totalFunctions(0), functionsInlined(0)
{
    if (compileUnitNode)
    {
        SetSpan(compileUnitNode->GetSpan());
        fileIndex = compileUnitNode->FileIndex();
        std::filesystem::path fileName = std::filesystem::path(compileUnitNode->FilePath()).filename();
        std::filesystem::path directory = context->RootModule()->DirectoryPath();
        std::filesystem::path objectFileDirectory = context->RootModule()->ObjectFileDirectoryPath();
        std::filesystem::path llfp = (directory / fileName).replace_extension(".ll");
        std::filesystem::path cppfp = (directory / fileName).replace_extension(".cpp");
        std::filesystem::path bcfp = (directory / fileName).replace_extension(".bc");
        std::filesystem::path optbcfp = (directory / fileName).replace_extension(".opt.bc");
        std::filesystem::path optllfp = (directory / fileName).replace_extension(".opt.ll");
        std::filesystem::path objfp;
        std::filesystem::path asmfp;
#ifdef _WIN32
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
        {
            objfp = (objectFileDirectory / fileName).replace_extension(".obj");
        }
        else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
        {
            objfp = (objectFileDirectory / fileName).replace_extension(".o");
        }
        else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
        {
            objfp = (objectFileDirectory / fileName).replace_extension(".o");
        }
        else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
        {
            objfp = (objectFileDirectory / fileName).replace_extension(".obj");
            asmfp = (objectFileDirectory / fileName).replace_extension(".asm");
        }
        else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
        {
            objfp = (objectFileDirectory / fileName).replace_extension(".obj");
            asmfp = (objectFileDirectory / fileName).replace_extension(".asm");
        }
#else
        objfp = (objectFileDirectory / fileName).replace_extension(".o");
#endif
        llFilePath = util::GetFullPath(llfp.generic_string());
        cppFilePath = util::GetFullPath(cppfp.generic_string());
        bcFilePath = util::GetFullPath(bcfp.generic_string());
        optBCFilePath = util::GetFullPath(optbcfp.generic_string());
        optLLFilePath = util::GetFullPath(optllfp.generic_string());
        objectFilePath = util::GetFullPath(objfp.generic_string());
        asmFilePath = util::GetFullPath(asmfp.generic_string());
    }
}

void BoundCompileUnit::Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot load from compile unit", GetFullSpan());
}

void BoundCompileUnit::Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags)
{
    throw cmajor::symbols::Exception("cannot store to compile unit", GetFullSpan());
}

void BoundCompileUnit::Accept(BoundNodeVisitor& visitor)
{
    visitor.Visit(*this);
}

void BoundCompileUnit::AddFileScope(cmajor::symbols::FileScope* fileScope)
{
    fileScopes.push_back(std::unique_ptr<cmajor::symbols::FileScope>(fileScope));
}

void BoundCompileUnit::RemoveLastFileScope()
{
    if (fileScopes.empty())
    {
        throw cmajor::symbols::Exception("file scopes of bound compile unit is empty", GetFullSpan());
    }
    fileScopes.erase(fileScopes.end() - 1);
}

cmajor::symbols::FileScope* BoundCompileUnit::ReleaseLastFileScope()
{
    if (fileScopes.empty())
    {
        throw cmajor::symbols::Exception("file scopes of bound compile unit is empty", GetFullSpan());
    }
    cmajor::symbols::FileScope* fileScope = fileScopes.back().release();
    RemoveLastFileScope();
    return fileScope;
}

void BoundCompileUnit::AddBoundNode(std::unique_ptr<BoundNode>&& boundNode)
{
    if (immutable)
    {
        throw std::runtime_error("internal error: " + compileUnitNode->FilePath() + " is immutable");
    }
    if (currentNamespace)
    {
        currentNamespace->AddMember(std::move(boundNode));
    }
    else
    {
        boundNode->SetParent(this);
        boundNodes.push_back(std::move(boundNode));
    }
}

cmajor::symbols::FunctionSymbol* BoundCompileUnit::GetConversion(cmajor::symbols::TypeSymbol* sourceType, cmajor::symbols::TypeSymbol* targetType, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, ArgumentMatch& argumentMatch, cmajor::ast::Node* node)
{
    cmajor::symbols::FunctionSymbol* conversion = symbolTable.GetConversion(sourceType, targetType, context);
    if (conversion && conversion->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol)
    {
        argumentMatch.preReferenceConversionFlags = cmajor::ir::OperationFlags::addr;
    }
    if (!conversion)
    {
        conversion = conversionTable.GetConversion(sourceType, targetType, context);
        if (!conversion)
        {
            if (sourceType->IsNullPtrType() && targetType->IsPointerType() && !targetType->IsReferenceType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> nullPtrToPtrConversion(new NullPtrToPtrConversion(symbolTable.GetTypeByName(U"System.NullPtrType"), 
                    targetType, context));
                nullPtrToPtrConversion->SetParent(&symbolTable.GlobalNs());
                conversion = nullPtrToPtrConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(nullPtrToPtrConversion));
                return conversion;
            }
            else if (sourceType->IsVoidPtrType() && targetType->IsPointerType() && !targetType->IsReferenceType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> voidPtrToPtrConversion(new VoidPtrToPtrConversion(
                    symbolTable.GetTypeByName(U"void")->AddPointer(context), targetType, context));
                voidPtrToPtrConversion->SetParent(&symbolTable.GlobalNs());
                conversion = voidPtrToPtrConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(voidPtrToPtrConversion));
                return conversion;
            }
            else if (sourceType->PlainType(context)->IsPointerType() && targetType == symbolTable.GetTypeByName(U"ulong"))
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> ptrToULongConversion(new PtrToULongConversion(sourceType->PlainType(context), 
                    symbolTable.GetTypeByName(U"ulong"), context));
                ptrToULongConversion->SetParent(&symbolTable.GlobalNs());
                conversion = ptrToULongConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(ptrToULongConversion));
                return conversion;
            }
            else if (sourceType == symbolTable.GetTypeByName(U"ulong") && targetType->IsVoidPtrType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> ulongToVoidPtrConversion(new ULongToVoidPtrConversion(sourceType, targetType, context));
                ulongToVoidPtrConversion->SetParent(&symbolTable.GlobalNs());
                conversion = ulongToVoidPtrConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(ulongToVoidPtrConversion));
                return conversion;
            }
            else if (sourceType->IsDelegateType() && targetType->IsVoidPtrType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> dlgToVoidPtrConversion(new cmajor::symbols::DelegateToVoidPtrConversion(sourceType, targetType, context));
                dlgToVoidPtrConversion->SetParent(&symbolTable.GlobalNs());
                conversion = dlgToVoidPtrConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(dlgToVoidPtrConversion));
                return conversion;
            }
            else if (sourceType->IsVoidPtrType() && targetType->IsDelegateType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> voidPtrToDlgConversion(new cmajor::symbols::VoidPtrToDelegateConversion(sourceType, targetType,
                    symbolTable.GetTypeByName(U"ulong"), context));
                voidPtrToDlgConversion->SetParent(&symbolTable.GlobalNs());
                conversion = voidPtrToDlgConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(voidPtrToDlgConversion));
                return conversion;
            }
            else if (sourceType->PlainType(context)->IsPointerType() && targetType->RemoveConst(context)->IsVoidPtrType())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> ptrToVoidPtrConversion(new PtrToVoidPtrConversion(sourceType->PlainType(context), 
                    symbolTable.GetTypeByName(U"void")->AddPointer(context), context));
                ptrToVoidPtrConversion->SetParent(&symbolTable.GlobalNs());
                conversion = ptrToVoidPtrConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(ptrToVoidPtrConversion));
                return conversion;
            }
            else if (sourceType->PlainType(context)->IsCharacterPointerType() && targetType->IsStringFunctionContainer())
            {
                std::unique_ptr<cmajor::symbols::FunctionSymbol> charPtr2StringFunctionsConversion(
                    new CharacterPointerLiteralToStringFunctionContainerConversion(sourceType->PlainType(context), symbolTable.GetTypeByName(U"@string_functions"),
                        context));
                charPtr2StringFunctionsConversion->SetParent(&symbolTable.GlobalNs());
                conversion = charPtr2StringFunctionsConversion.get();
                conversionTable.AddConversion(conversion);
                conversionTable.AddGeneratedConversion(std::move(charPtr2StringFunctionsConversion));
                return conversion;
            }
            else if (sourceType->BaseType()->IsClassTypeSymbol() && targetType->BaseType()->IsClassTypeSymbol())
            {
                if (sourceType->PointerCount() == targetType->PointerCount() &&
                    (!sourceType->IsArrayType() && !targetType->IsArrayType()))
                {
                    cmajor::symbols::ClassTypeSymbol* sourceClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(sourceType->BaseType());
                    cmajor::symbols::ClassTypeSymbol* targetClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(targetType->BaseType());
                    uint8_t conversionDistance = 0;
                    if (sourceClassType->HasBaseClass(targetClassType, conversionDistance))
                    {
                        //if (targetType->IsLvalueReferenceType() && !sourceType->IsReferenceType()) // bug???
                        if (targetType->IsReferenceType() && !sourceType->IsReferenceType())
                        {
                            argumentMatch.preReferenceConversionFlags = cmajor::ir::OperationFlags::addr;
                            sourceType = sourceType->AddLvalueReference(context);
                            if (targetType->IsConstType())
                            {
                                sourceType = sourceType->AddConst(context);
                            }
                        }
                        std::u32string conversionName = sourceType->FullName() + U"2" + targetType->FullName();
                        std::unique_ptr<cmajor::symbols::FunctionSymbol> implicitClassTypeConversion(new ClassTypeConversion(conversionName, cmajor::symbols::ConversionType::implicit_, conversionDistance, sourceType, targetType));
                        conversion = implicitClassTypeConversion.get();
                        // do not add entry to the conversion table
                        conversionTable.AddGeneratedConversion(std::move(implicitClassTypeConversion));
                        return conversion;
                    }
                    else
                    {
                        uint8_t conversionDistance = 0;
                        if (targetClassType->HasBaseClass(sourceClassType, conversionDistance))
                        {
                            //if (targetType->IsLvalueReferenceType() && !sourceType->IsReferenceType()) // bug???
                            if (targetType->IsReferenceType() && !sourceType->IsReferenceType())
                            {
                                argumentMatch.preReferenceConversionFlags = cmajor::ir::OperationFlags::addr;
                                sourceType = sourceType->AddLvalueReference(context);
                                if (targetType->IsConstType())
                                {
                                    sourceType = sourceType->AddConst(context);
                                }
                            }
                            std::u32string conversionName = sourceType->FullName() + U"2" + targetType->FullName();
                            std::unique_ptr<cmajor::symbols::FunctionSymbol> explicitClassTypeConversion(new ClassTypeConversion(conversionName, cmajor::symbols::ConversionType::explicit_, conversionDistance, sourceType, targetType));
                            conversion = explicitClassTypeConversion.get();
                            // do not add entry to the conversion table
                            conversionTable.AddGeneratedConversion(std::move(explicitClassTypeConversion));
                            return conversion;
                        }
                    }
                }
            }
            else if ((sourceType->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol || 
                sourceType->GetSymbolType() == cmajor::symbols::SymbolType::memberExpressionTypeSymbol) &&
                targetType->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol)
            {
                std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
                cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol = nullptr;
                BoundMemberExpression* boundMemberExpression = nullptr;
                if (sourceType->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol)
                {
                    cmajor::symbols::FunctionGroupTypeSymbol* functionGroupTypeSymbol = static_cast<cmajor::symbols::FunctionGroupTypeSymbol*>(sourceType);
                    BoundFunctionGroupExpression* boundFunctionGroup = static_cast<BoundFunctionGroupExpression*>(functionGroupTypeSymbol->BoundFunctionGroup());
                    if (boundFunctionGroup)
                    {
                        templateArgumentTypes = boundFunctionGroup->TemplateArgumentTypes();
                    }
                    functionGroupSymbol = functionGroupTypeSymbol->FunctionGroup();
                }
                else if (sourceType->GetSymbolType() == cmajor::symbols::SymbolType::memberExpressionTypeSymbol)
                {
                    cmajor::symbols::MemberExpressionTypeSymbol* memberExpressionTypeSymbol = static_cast<cmajor::symbols::MemberExpressionTypeSymbol*>(sourceType);
                    boundMemberExpression = static_cast<BoundMemberExpression*>(memberExpressionTypeSymbol->BoundMemberExpression());
                    if (boundMemberExpression->Member()->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                    {
                        BoundFunctionGroupExpression* boundFunctionGroupExpression = static_cast<BoundFunctionGroupExpression*>(boundMemberExpression->Member());
                        templateArgumentTypes = boundFunctionGroupExpression->TemplateArgumentTypes();
                        functionGroupSymbol = boundFunctionGroupExpression->FunctionGroup();
                    }
                }
                if (functionGroupSymbol)
                {
                    cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol = static_cast<cmajor::symbols::DelegateTypeSymbol*>(targetType);
                    int arity = delegateTypeSymbol->Arity();
                    cmajor::symbols::ViableFunctionSet viableFunctions;
                    functionGroupSymbol->CollectViableFunctions(arity, viableFunctions, &GetModule());
                    for (cmajor::symbols::FunctionSymbol* viableFunction : viableFunctions.Get())
                    {
                        if (viableFunction->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol && !viableFunction->IsStatic()) continue;
                        int na = viableFunction->TemplateArgumentTypes().size();
                        if (na > 0)
                        {
                            if (na != templateArgumentTypes.size()) continue;
                            bool found = true;
                            for (int i = 0; i < na; ++i)
                            {
                                if (!cmajor::symbols::TypesEqual(viableFunction->TemplateArgumentTypes()[i], templateArgumentTypes[i]))
                                {
                                    found = false;
                                    break;
                                }
                            }
                            if (!found) continue;
                        }
                        bool found = true;
                        for (int i = 0; i < arity; ++i)
                        {
                            cmajor::symbols::ParameterSymbol* sourceParam = viableFunction->Parameters()[i];
                            cmajor::symbols::ParameterSymbol* targetParam = delegateTypeSymbol->Parameters()[i];
                            if (!TypesEqual(sourceParam->GetType(), targetParam->GetType()))
                            {
                                found = false;
                                break;
                            }
                        }
                        if (found)
                        {
                            found = TypesEqual(viableFunction->ReturnType(), delegateTypeSymbol->ReturnType());
                        }
                        if (found)
                        {
                            if (boundMemberExpression)
                            {
                                boundMemberExpression->ResetClassPtr();
                            }
                            if (viableFunction->IsFunctionTemplate())
                            {
                                if (sourceType->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol)
                                {
                                    cmajor::symbols::FunctionGroupTypeSymbol* functionGroupTypeSymbol = static_cast<cmajor::symbols::FunctionGroupTypeSymbol*>(sourceType);
                                    BoundFunctionGroupExpression* boundFunctionGroupExpression = static_cast<BoundFunctionGroupExpression*>(functionGroupTypeSymbol->BoundFunctionGroup());
                                    std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*> templateParameterMap;
                                    int n = viableFunction->TemplateParameters().size();
                                    if (boundFunctionGroupExpression->TemplateArgumentTypes().size() == n)
                                    {
                                        for (int i = 0; i < n; ++i)
                                        {
                                            cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = viableFunction->TemplateParameters()[i];
                                            cmajor::symbols::TypeSymbol* templateArgumentType = boundFunctionGroupExpression->TemplateArgumentTypes()[i];
                                            templateParameterMap[templateParameterSymbol] = templateArgumentType;
                                        }
                                        viableFunction = InstantiateFunctionTemplate(viableFunction, templateParameterMap, node);
                                    }
                                    else
                                    {
                                        continue;
                                    }
                                }
                                else
                                {
                                    continue;
                                }
                            }
                            std::unique_ptr<cmajor::symbols::FunctionSymbol> functionToDelegateConversion(new cmajor::symbols::FunctionToDelegateConversion(
                                sourceType, delegateTypeSymbol, viableFunction, context));
                            functionToDelegateConversion->SetParent(&symbolTable.GlobalNs());
                            conversion = functionToDelegateConversion.get();
                            // do not add entry to the conversion table
                            conversionTable.AddGeneratedConversion(std::move(functionToDelegateConversion));
                            return conversion;
                        }
                    }
                }
            }
            else if ((sourceType->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol || sourceType->GetSymbolType() == cmajor::symbols::SymbolType::memberExpressionTypeSymbol) &&
                targetType->PlainType(context)->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol && currentFunction)
            {
                cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(targetType->PlainType(context));
                cmajor::symbols::FunctionGroupSymbol* functionGroup = nullptr;
                if (sourceType->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol)
                {
                    cmajor::symbols::FunctionGroupTypeSymbol* functionGroupTypeSymbol = static_cast<cmajor::symbols::FunctionGroupTypeSymbol*>(sourceType);
                    BoundFunctionGroupExpression* boundFunctionGroup = static_cast<BoundFunctionGroupExpression*>(functionGroupTypeSymbol->BoundFunctionGroup());
                    functionGroup = functionGroupTypeSymbol->FunctionGroup();
                }
                else if (sourceType->GetSymbolType() == cmajor::symbols::SymbolType::memberExpressionTypeSymbol)
                {
                    cmajor::symbols::MemberExpressionTypeSymbol* memberExpressionType = static_cast<cmajor::symbols::MemberExpressionTypeSymbol*>(sourceType);
                    BoundMemberExpression* boundMemberExpr = static_cast<BoundMemberExpression*>(memberExpressionType->BoundMemberExpression());
                    if (boundMemberExpr->Member()->GetBoundNodeType() == BoundNodeType::boundFunctionGroupExpression)
                    {
                        BoundFunctionGroupExpression* boundFunctionGroup = static_cast<BoundFunctionGroupExpression*>(boundMemberExpr->Member());
                        functionGroup = boundFunctionGroup->FunctionGroup();
                    }
                }
                if (functionGroup)
                {
                    int arity = classDelegateType->Arity();
                    cmajor::symbols::ViableFunctionSet viableFunctions;
                    functionGroup->CollectViableFunctions(arity + 1, viableFunctions, &GetModule());
                    for (cmajor::symbols::FunctionSymbol* viableFunction : viableFunctions.Get())
                    {
                        bool found = true;
                        for (int i = 1; i < arity + 1; ++i)
                        {
                            cmajor::symbols::ParameterSymbol* sourceParam = viableFunction->Parameters()[i];
                            cmajor::symbols::ParameterSymbol* targetParam = classDelegateType->Parameters()[i - 1];
                            if (!TypesEqual(sourceParam->GetType(), targetParam->GetType()))
                            {
                                found = false;
                                break;
                            }
                        }
                        if (found)
                        {
                            found = TypesEqual(viableFunction->ReturnType(), classDelegateType->ReturnType());
                        }
                        if (found)
                        {
                            //LocalVariableSymbol* objectDelegatePairVariable = currentFunction->GetFunctionSymbol()->CreateTemporary(classDelegateType->ObjectDelegatePairType(), sourcePos);
                            std::unique_ptr<cmajor::symbols::FunctionSymbol> memberFunctionToClassDelegateConversion(
                                new cmajor::symbols::MemberFunctionToClassDelegateConversion(node->GetSpan(), sourceType, classDelegateType, viableFunction, context));
                            memberFunctionToClassDelegateConversion->SetParent(&symbolTable.GlobalNs());
                            conversion = memberFunctionToClassDelegateConversion.get();
                            // do not add entry to the conversion table
                            conversionTable.AddGeneratedConversion(std::move(memberFunctionToClassDelegateConversion));
                            return conversion;
                        }
                    }
                }
            }
            else if (targetType->PlainType(context)->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol && currentFunction)
            {
                cmajor::symbols::InterfaceTypeSymbol* targetInterfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(targetType->PlainType(context));
                if (sourceType->PlainType(context)->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* sourceClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(sourceType->PlainType(context));
                    int32_t n = sourceClassType->ImplementedInterfaces().size();
                    for (int32_t i = 0; i < n; ++i)
                    {
                        cmajor::symbols::InterfaceTypeSymbol* sourceInterfaceType = sourceClassType->ImplementedInterfaces()[i];
                        if (TypesEqual(targetInterfaceType, sourceInterfaceType))
                        {
                            //LocalVariableSymbol* temporaryInterfaceObjectVar = currentFunction->GetFunctionSymbol()->CreateTemporary(targetInterfaceType, sourcePos);
                            std::unique_ptr<cmajor::symbols::FunctionSymbol> classToInterfaceConversion(
                                new cmajor::symbols::ClassToInterfaceConversion(sourceClassType, targetInterfaceType, i, node->GetSpan(), context));
                            classToInterfaceConversion->SetParent(&symbolTable.GlobalNs());
                            classToInterfaceConversion->SetModule(&GetModule());
                            conversion = classToInterfaceConversion.get();
                            // do not add entry to the conversion table
                            conversionTable.AddGeneratedConversion(std::move(classToInterfaceConversion));
                            return conversion;
                        }
                    }
                }
            }
        }
    }
    if (conversion)
    {
        if (conversion->Parent() && !conversion->IsGeneratedFunction() && conversion->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(conversion->Parent());
            if (specialization->GetModule() != &GetModule())
            {
                specialization = symbolTable.GetCurrentClassTemplateSpecialization(specialization);
                int index = conversion->GetIndex();
                conversion = specialization->GetFunctionByIndex(index);
            }
            cmajor::symbols::FunctionSymbol* instantiatedConversion = InstantiateClassTemplateMemberFunction(conversion, containerScope, currentFunction, node);
            if (!instantiatedConversion)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(conversion->Parent());
                std::lock_guard<std::recursive_mutex> lock(symbolTable.GetModule()->GetLock());
                cmajor::symbols::ClassTemplateSpecializationSymbol* copy = symbolTable.CopyClassTemplateSpecialization(specialization);
                classTemplateRepository.BindClassTemplateSpecialization(copy, symbolTable.GlobalNs().GetContainerScope(), node);
                int index = conversion->GetIndex();
                conversion = copy->GetFunctionByIndex(index);
                conversion = InstantiateClassTemplateMemberFunction(conversion, containerScope, currentFunction, node);
                if (!conversion)
                {
                    throw cmajor::symbols::Exception("internal error: could not instantiate member function of a class template specialization '" + 
                        util::ToUtf8(specialization->FullName()) + "'", specialization->GetFullSpan());
                }
            }
        }
        else if (GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && conversion->IsInline())
        {
            conversion = InstantiateInlineFunction(conversion, containerScope, node);
        }
    }
    return conversion;
}

void BoundCompileUnit::CollectViableFunctions(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, std::vector<std::unique_ptr<BoundExpression>>& arguments,
    BoundFunction* currentFunction, cmajor::symbols::ViableFunctionSet& viableFunctions, std::unique_ptr<cmajor::symbols::Exception>& exception, 
    cmajor::ast::Node* node, CollectFlags flags)
{
    operationRepository.CollectViableFunctions(groupName, containerScope, arguments, currentFunction, viableFunctions, exception, node, flags);
}

cmajor::symbols::FunctionSymbol* BoundCompileUnit::InstantiateFunctionTemplate(cmajor::symbols::FunctionSymbol* functionTemplate, const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMapping,
    cmajor::ast::Node* node)
{
    return functionTemplateRepository.Instantiate(functionTemplate, templateParameterMapping, node);
}

cmajor::symbols::FunctionSymbol* BoundCompileUnit::InstantiateClassTemplateMemberFunction(
    cmajor::symbols::FunctionSymbol* memberFunction, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    return classTemplateRepository.Instantiate(memberFunction, containerScope, currentFunction, node);
}

cmajor::symbols::FunctionSymbol* BoundCompileUnit::InstantiateInlineFunction(cmajor::symbols::FunctionSymbol* inlineFunction, cmajor::symbols::ContainerScope* containerScope, 
    cmajor::ast::Node* node)
{
    return inlineFunctionRepository.Instantiate(inlineFunction, containerScope, node);
}

cmajor::ast::FunctionNode* BoundCompileUnit::GetFunctionNodeFor(cmajor::symbols::FunctionSymbol* constExprFunctionSymbol)
{
    return constExprFunctionRepository.GetFunctionNodeFor(constExprFunctionSymbol);
}

void BoundCompileUnit::GenerateCopyConstructorFor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
    BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    operationRepository.GenerateCopyConstructorFor(classTypeSymbol, containerScope, currentFunction, node);
}

void BoundCompileUnit::GenerateCopyConstructorFor(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::symbols::ContainerScope* containerScope, 
    cmajor::ast::Node* node)
{
    operationRepository.GenerateCopyConstructorFor(interfaceTypeSymbol, containerScope, context, node);
}

int BoundCompileUnit::Install(const std::string& str)
{
    return utf8StringRepository.Install(str);
}

int BoundCompileUnit::Install(const std::u16string& str)
{
    return utf16StringRepository.Install(str);
}

int BoundCompileUnit::Install(const std::u32string& str)
{
    return utf32StringRepository.Install(str);
}

int BoundCompileUnit::Install(const util::uuid& uuid)
{
    return uuidRepository.Install(uuid);
}

const std::string& BoundCompileUnit::GetUtf8String(int stringId) const
{
    return utf8StringRepository.GetString(stringId);
}

const std::u16string& BoundCompileUnit::GetUtf16String(int stringId) const
{
    return utf16StringRepository.GetString(stringId);
}

const std::u32string& BoundCompileUnit::GetUtf32String(int stringId) const
{
    return utf32StringRepository.GetString(stringId);
}

const unsigned char* BoundCompileUnit::GetUtf8CharPtr(int stringId) const
{
    return utf8StringRepository.CharPtr(stringId);
}

const char16_t* BoundCompileUnit::GetUtf16CharPtr(int stringId) const
{
    return utf16StringRepository.CharPtr(stringId);
}

const char32_t* BoundCompileUnit::GetUtf32CharPtr(int stringId) const
{
    return utf32StringRepository.CharPtr(stringId);
}

const util::uuid& BoundCompileUnit::GetUuid(int uuidId) const
{
    return uuidRepository.GetUuid(uuidId);
}

void BoundCompileUnit::AddConstantArray(cmajor::symbols::ConstantSymbol* constantArraySymbol)
{
    constantArrayRepository.AddConstantArray(constantArraySymbol);
}

void BoundCompileUnit::AddConstantStructure(cmajor::symbols::ConstantSymbol* constantStructureSymbol)
{
    constantStructureRepository.AddConstantStructure(constantStructureSymbol);
}

void BoundCompileUnit::PushBindingTypes()
{
    bindingTypesStack.push(bindingTypes);
    bindingTypes = true;
}

void BoundCompileUnit::PopBindingTypes()
{
    bindingTypes = bindingTypesStack.top();
    bindingTypesStack.pop();
}

void BoundCompileUnit::FinalizeBinding(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization)
{
    if (classTemplateSpecialization->GetModule() == context->RootModule() && classTemplateSpecialization->StatementsNotBound())
    {
        classTemplateSpecialization->ResetStatementsNotBound();
        cmajor::symbols::FileScope* fileScope = classTemplateSpecialization->ReleaseFileScope();
        bool fileScopeAdded = false;
        if (fileScope)
        {
            AddFileScope(fileScope);
            fileScopeAdded = true;
        }
        StatementBinder statementBinder(*this);
        classTemplateSpecialization->GlobalNs()->Accept(statementBinder);
        if (fileScopeAdded)
        {
            RemoveLastFileScope();
        }
    }
}

void BoundCompileUnit::PushNamespace(BoundNamespace* ns)
{
    ns->SetParent(this);
    namespaceStack.push(currentNamespace);
    currentNamespace = ns;
}

void BoundCompileUnit::PopNamespace()
{
    currentNamespace = namespaceStack.top();
    namespaceStack.pop();
}

bool BoundCompileUnit::HasCopyConstructorFor(const util::uuid& typeId) const
{
    return copyConstructorMap.find(typeId) != copyConstructorMap.cend();
}

cmajor::symbols::FunctionSymbol* BoundCompileUnit::GetCopyConstructorFor(const util::uuid& typeId) const
{
    auto it = copyConstructorMap.find(typeId);
    if (it != copyConstructorMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("internal error: copy constructor for type not found from compile unit");
    }
}

void BoundCompileUnit::AddCopyConstructorFor(const util::uuid& typeId, std::unique_ptr<cmajor::symbols::FunctionSymbol>&& copyConstructor)
{
    copyConstructorMap[typeId] = copyConstructor.get();
    copyConstructors.push_back(std::move(copyConstructor));
}

void BoundCompileUnit::AddCopyConstructorToMap(const util::uuid& typeId, cmajor::symbols::FunctionSymbol* copyConstructor)
{
    copyConstructorMap[typeId] = copyConstructor;
}

void BoundCompileUnit::AddGlobalNs(std::unique_ptr<cmajor::ast::NamespaceNode>&& globalNs)
{
    globalNamespaceNodes.push_back(std::move(globalNs));
}

bool BoundCompileUnit::IsGeneratedDestructorInstantiated(cmajor::symbols::DestructorSymbol* generatedDestructorSymbol) const
{
    return instantiatedGeneratedDestructors.find(generatedDestructorSymbol) != instantiatedGeneratedDestructors.cend();
}

void BoundCompileUnit::SetGeneratedDestructorInstantiated(cmajor::symbols::DestructorSymbol* generatedDestructorSymbol)
{
    instantiatedGeneratedDestructors.insert(generatedDestructorSymbol);
}

void BoundCompileUnit::SetSystemRuntimeUnwindInfoSymbol(cmajor::symbols::TypeSymbol* systemRuntimeUnwindInfoSymbol_)
{
    systemRuntimeUnwindInfoSymbol = systemRuntimeUnwindInfoSymbol_;
}

void BoundCompileUnit::GenerateInitUnwindInfoFunctionSymbol(const soul::ast::Span& span)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm) return;
    std::string compileUnitId = compileUnitNode->Id();
    std::u32string groupName = U"InitUnwindInfo_" + util::ToUtf32(compileUnitId);
    cmajor::symbols::FunctionSymbol* functionSymbol = new cmajor::symbols::FunctionSymbol(span, groupName);
    functionSymbol->SetParent(&symbolTable.GlobalNs());
    functionSymbol->SetGroupName(groupName);
    functionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    functionSymbol->SetCDecl();
    functionSymbol->SetNothrow();
    functionSymbol->ComputeMangledName(context);
    functionSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));
    symbolTable.SetFunctionIdFor(functionSymbol);
    initUnwindInfoFunctionSymbol.reset(functionSymbol);
}

void BoundCompileUnit::GenerateCompileUnitInitialization(const soul::ast::Span& span)
{
    if (context->RootModule()->IsCore()) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm) return;
    std::string compileUnitId = compileUnitNode->Id();
    std::u32string groupName = U"InitCompileUnit_" + util::ToUtf32(compileUnitId);
    cmajor::symbols::FunctionSymbol* functionSymbol = new cmajor::symbols::FunctionSymbol(span, groupName);
    functionSymbol->SetParent(&symbolTable.GlobalNs());
    functionSymbol->SetGroupName(groupName);
    functionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    functionSymbol->SetCDecl();
    functionSymbol->SetNothrow();
    functionSymbol->ComputeMangledName(context);
    functionSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));
    symbolTable.SetFunctionIdFor(functionSymbol);
    initCompileUnitFunctionSymbol.reset(functionSymbol);
    cmajor::symbols::Symbol* symbol = symbolTable.GlobalNs().GetContainerScope()->Lookup(U"System.Runtime.PushCompileUnitUnwindInfoInit", context);
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupSymbol)
        {
            cmajor::symbols::FunctionGroupSymbol* functionGroup = static_cast<cmajor::symbols::FunctionGroupSymbol*>(symbol);
            pushCompileUnitUnwindInfoInitFunctionSymbol = functionGroup->GetFunction();
        }
    }
    if (!pushCompileUnitUnwindInfoInitFunctionSymbol)
    {
        throw std::runtime_error("internal error: 'System.Runtime.PushCompileUnitUnwindInfoInit' symbol not found");
    }
    compileUnitUnwindInfoVarSymbol.reset(new cmajor::symbols::GlobalVariableSymbol(span, U"unwindInfoInit_" + util::ToUtf32(compileUnitId)));
    compileUnitUnwindInfoVarSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    compileUnitUnwindInfoVarSymbol->ComputeMangledName(context);
    cmajor::symbols::Symbol* cuUnwindInfoTypeSymbol = symbolTable.GlobalNs().GetContainerScope()->Lookup(U"System.Runtime.CompileUnitUnwindInfo", context);
    if (cuUnwindInfoTypeSymbol && cuUnwindInfoTypeSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        cmajor::symbols::ClassGroupTypeSymbol* classGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(cuUnwindInfoTypeSymbol);
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = classGroup->GetClass(0);
        if (!classTypeSymbol)
        {
            throw std::runtime_error("internal error: 'System.Runtime.CompileUnitUnwindInfo' class not found");
        }
        compileUnitUnwindInfoVarSymbol->SetType(classTypeSymbol);
        BoundGlobalVariable* compileUnitUnwindInfoVar = new BoundGlobalVariable(span, compileUnitUnwindInfoVarSymbol.get());
        AddBoundNode(std::unique_ptr<BoundNode>(compileUnitUnwindInfoVar));
    }
    else
    {
        throw std::runtime_error("internal error: 'System.Runtime.CompileUnitUnwindInfo' symbol not found");
    }
    cmajor::symbols::Symbol* initUnwindInfoDelegateSymbol = symbolTable.GlobalNs().GetContainerScope()->Lookup(U"System.Runtime.InitCompileUnitUnwindInfoFunction", context);
    if (initUnwindInfoDelegateSymbol && initUnwindInfoDelegateSymbol->IsTypeSymbol())
    {
        initUnwindInfoDelegateType = static_cast<cmajor::symbols::TypeSymbol*>(initUnwindInfoDelegateSymbol);
    }
    else
    {
        throw std::runtime_error("internal error: 'System.Runtime.InitCompileUnitUnwindInfoFunction' symbol not found");
    }
}

void BoundCompileUnit::GenerateGlobalInitializationFunction(const soul::ast::Span& span)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp) return;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm) return;
    std::u32string groupName = U"GlobalInitCompileUnits";
    globalInitFunctionSymbol = new cmajor::symbols::FunctionSymbol(span, groupName);
    globalInitFunctionSymbol->SetParent(&symbolTable.GlobalNs());
    globalInitFunctionSymbol->SetGroupName(groupName);
    globalInitFunctionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    globalInitFunctionSymbol->SetCDecl();
    globalInitFunctionSymbol->SetNothrow();
    globalInitFunctionSymbol->ComputeMangledName(context);
    globalInitFunctionSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));
    symbolTable.SetFunctionIdFor(globalInitFunctionSymbol);
    symbolTable.GlobalNs().AddMember(globalInitFunctionSymbol, context);
    const std::set<std::string>& compileUnitIds = symbolTable.GetModule()->AllCompileUnitIds();
    for (const std::string& compileUnitId : compileUnitIds)
    {
        Assert(!compileUnitId.empty(), "compiler unit ID is empty"); 
        std::u32string groupName = U"InitCompileUnit_" + util::ToUtf32(compileUnitId);
        cmajor::symbols::FunctionSymbol* initFunctionSymbol = new cmajor::symbols::FunctionSymbol(span, groupName);
        initFunctionSymbol->SetParent(&symbolTable.GlobalNs());
        initFunctionSymbol->SetGroupName(groupName);
        initFunctionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
        initFunctionSymbol->SetCDecl();
        initFunctionSymbol->SetNothrow();
        initFunctionSymbol->ComputeMangledName(context);
        initFunctionSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));
        symbolTable.SetFunctionIdFor(initFunctionSymbol);
        allCompileUnitInitFunctionSymbols.push_back(std::unique_ptr<cmajor::symbols::FunctionSymbol>(initFunctionSymbol));
    }
}

bool BoundCompileUnit::CodeGenerated(cmajor::symbols::FunctionSymbol* functionSymbol) const
{
    return codeGenerated.find(functionSymbol) != codeGenerated.cend();
}

void BoundCompileUnit::SetCodeGenerated(cmajor::symbols::FunctionSymbol* functionSymbol)
{
    codeGenerated.insert(functionSymbol);
}

bool BoundCompileUnit::CanReuse(cmajor::symbols::FunctionSymbol* functionSymbol) const
{
    return canReuse.find(functionSymbol) != canReuse.cend();
}

void BoundCompileUnit::SetCanReuse(cmajor::symbols::FunctionSymbol* functionSymbol)
{
    canReuse.insert(functionSymbol);
}

} // namespace cmajor::binder
