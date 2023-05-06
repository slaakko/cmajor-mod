// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.enumerations;

import soul.ast.source.pos;
import cmajor.symbols.type.symbol;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import cmajor.symbols.modules;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

EnumTypeSymbol::EnumTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    TypeSymbol(SymbolType::enumTypeSymbol, sourcePos_, sourceModuleId_, name_), underlyingType()
{
}

void EnumTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(underlyingType->TypeId());
}

void EnumTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    util::uuid underlyingTypeId;
    reader.GetBinaryStreamReader().ReadUuid(underlyingTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, underlyingTypeId, 0);
}

void EnumTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    //Assert(index == 0, "invalid emplace type index");
    underlyingType = typeSymbol;
}

std::string EnumTypeSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("enum ");
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void EnumTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddEnumeratedType(this);
    }
}

void EnumTypeSymbol::CollectMembers(SymbolCollector* collector)
{
    TypeSymbol::Accept(collector);
}

void EnumTypeSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("underlying type: " + util::ToUtf8(underlyingType->FullName()));
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
    formatter.WriteLine("enumeration constants:");
    formatter.IncIndent();
    SymbolCollector collector;
    TypeSymbol::Accept(&collector);
    for (EnumConstantSymbol* enumConstant : collector.EnumerationConstants())
    {
        formatter.WriteLine();
        enumConstant->Dump(formatter);
    }
    formatter.DecIndent();
}

void EnumTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("enumerated type cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

std::vector<EnumConstantSymbol*> EnumTypeSymbol::GetEnumConstants()
{
    SymbolCollector collector;
    TypeSymbol::Accept(&collector);
    return collector.EnumerationConstants();
}

ValueType EnumTypeSymbol::GetValueType() const
{
    return underlyingType->GetValueType();
}

Value* EnumTypeSymbol::MakeValue() const
{
    return underlyingType->MakeValue();
}

std::u32string EnumTypeSymbol::Id() const
{
    return MangledName();
}

void* EnumTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    uint64_t sizeInBits = SizeInBits(emitter);
    uint32_t alignInBits = AlignmentInBits(emitter);
    std::vector<void*> elements;
    std::vector<EnumConstantSymbol*> enumConstants = GetEnumConstants();
    for (EnumConstantSymbol* enumConstant : enumConstants)
    {
        int64_t value = 0;
        if (underlyingType->IsUnsignedType())
        {
            Value* val = enumConstant->GetValue()->As(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"ulong"), false, GetSourcePos(), SourceModuleId(), true);
            if (val)
            {
                ULongValue* ulongValue = static_cast<ULongValue*>(val);
                value = static_cast<int64_t>(ulongValue->GetValue());
            }
        }
        else
        {
            Value* val = enumConstant->GetValue()->As(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"long"), false, GetSourcePos(), SourceModuleId(), true);
            if (val)
            {
                LongValue* longValue = static_cast<LongValue*>(val);
                value = longValue->GetValue();
            }
        }
        elements.push_back(emitter.CreateDITypeForEnumConstant(util::ToUtf8(enumConstant->Name()), value));
    }
    return emitter.CreateDITypeForEnumType(util::ToUtf8(Name()), util::ToUtf8(MangledName()), GetSourcePos(), SourceModuleId(), elements, sizeInBits, alignInBits, underlyingType->GetDIType(emitter));
}

void EnumTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (!underlyingType)
    {
        throw SymbolCheckException("enumerated type symbol has no underlying type", GetSourcePos(), SourceModuleId());
    }
}

EnumConstantSymbol::EnumConstantSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    Symbol(SymbolType::enumConstantSymbol, sourcePos_, sourceModuleId_, name_), evaluating(false)
{
}

void EnumConstantSymbol::SetValue(Value* value_)
{
    value.reset(value_);
}

void EnumConstantSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject())
    {
        collector->AddEnumerationConstant(this);
    }
}

void EnumConstantSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("value: " + value->ToString());
}

void EnumConstantSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    WriteValue(value.get(), writer.GetBinaryStreamWriter());
    writer.GetBinaryStreamWriter().Write(strValue);
}

void EnumConstantSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    value = ReadValue(reader.GetBinaryStreamReader(), GetSourcePos(), SourceModuleId());
    strValue = reader.GetBinaryStreamReader().ReadUtf32String();
}

EnumTypeDefaultConstructor::EnumTypeDefaultConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeDefaultConstructor, sourcePos_, sourceModuleId_, name_), underlyingTypeDefaultConstructor(nullptr)
{
}

EnumTypeDefaultConstructor::EnumTypeDefaultConstructor(EnumTypeSymbol* enumType_) :
    FunctionSymbol(SymbolType::enumTypeDefaultConstructor, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeDefaultConstructor"), underlyingTypeDefaultConstructor(nullptr)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"this");
    thisParam->SetType(enumType_->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeDefaultConstructor = basicTypeSymbol->DefaultConstructor();
}

void EnumTypeDefaultConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeDefaultConstructor, "underlying default constructor not set");
    const util::uuid& defaultConstructorId = underlyingTypeDefaultConstructor->FunctionId();
    writer.GetBinaryStreamWriter().Write(defaultConstructorId);
}

void EnumTypeDefaultConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid defaultConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(defaultConstructorId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, defaultConstructorId, 0);
}

void EnumTypeDefaultConstructor::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeDefaultConstructor = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeDefaultConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeDefaultConstructor, "underlying default constructor not set");
    underlyingTypeDefaultConstructor->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeDefaultConstructor::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeDefaultConstructor)
    {
        throw SymbolCheckException("enumerated type default constructor has no underlying type default constructor", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeCopyConstructor::EnumTypeCopyConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeCopyConstructor, sourcePos_, sourceModuleId_, name_), underlyingTypeCopyConstructor(nullptr)
{
}

EnumTypeCopyConstructor::EnumTypeCopyConstructor(EnumTypeSymbol* enumType_) :
    FunctionSymbol(SymbolType::enumTypeCopyConstructor, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeCopyConstructor"), underlyingTypeCopyConstructor(nullptr)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"this");
    thisParam->SetType(enumType_->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"that");
    thatParam->SetType(enumType_);
    AddMember(thatParam);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeCopyConstructor = basicTypeSymbol->CopyConstructor();
}

void EnumTypeCopyConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeCopyConstructor, "underlying copy constructor not set");
    const util::uuid& copyConstructorId = underlyingTypeCopyConstructor->FunctionId();
    writer.GetBinaryStreamWriter().Write(copyConstructorId);
}

void EnumTypeCopyConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid copyConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(copyConstructorId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyConstructorId, 0);
}

void EnumTypeCopyConstructor::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeCopyConstructor = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeCopyConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeCopyConstructor, "underlying copy constructor not set");
    underlyingTypeCopyConstructor->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeCopyConstructor::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeCopyConstructor)
    {
        throw SymbolCheckException("enumerated type copy constructor has no underlying type copy constructor", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeMoveConstructor::EnumTypeMoveConstructor(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeMoveConstructor, sourcePos_, sourceModuleId_, name_), underlyingTypeMoveConstructor(nullptr)
{
}

EnumTypeMoveConstructor::EnumTypeMoveConstructor(EnumTypeSymbol* enumType_) :
    FunctionSymbol(SymbolType::enumTypeMoveConstructor, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeMoveConstructor"), underlyingTypeMoveConstructor(nullptr)
{
    SetGroupName(U"@constructor");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"this");
    thisParam->SetType(enumType_->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"that");
    thatParam->SetType(enumType_->AddRvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeMoveConstructor = basicTypeSymbol->MoveConstructor();
}

void EnumTypeMoveConstructor::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeMoveConstructor, "underlying move constructor not set");
    const util::uuid& moveConstructorId = underlyingTypeMoveConstructor->FunctionId();
    writer.GetBinaryStreamWriter().Write(moveConstructorId);
}

void EnumTypeMoveConstructor::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid moveConstructorId;
    reader.GetBinaryStreamReader().ReadUuid(moveConstructorId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveConstructorId, 0);
}

void EnumTypeMoveConstructor::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeMoveConstructor = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeMoveConstructor::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeMoveConstructor, "underlying move constructor not set");
    underlyingTypeMoveConstructor->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeMoveConstructor::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeMoveConstructor)
    {
        throw SymbolCheckException("enumerated type move constructor has no underlying type move constructor", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeCopyAssignment::EnumTypeCopyAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeCopyAssignment, sourcePos_, sourceModuleId_, name_), underlyingTypeCopyAssignment(nullptr)
{
}

EnumTypeCopyAssignment::EnumTypeCopyAssignment(EnumTypeSymbol* enumType_, TypeSymbol* voidType_) :
    FunctionSymbol(SymbolType::enumTypeCopyAssignment, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeCopyAssignment"), underlyingTypeCopyAssignment(nullptr)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"this");
    thisParam->SetType(enumType_->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"that");
    thatParam->SetType(enumType_);
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeCopyAssignment = basicTypeSymbol->CopyAssignment();
}

void EnumTypeCopyAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeCopyAssignment, "underlying copy assignment not set");
    const util::uuid& copyAssignmentId = underlyingTypeCopyAssignment->FunctionId();
    writer.GetBinaryStreamWriter().Write(copyAssignmentId);
}

void EnumTypeCopyAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid copyAssignmentId;
    reader.GetBinaryStreamReader().ReadUuid(copyAssignmentId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyAssignmentId, 0);
}

void EnumTypeCopyAssignment::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeCopyAssignment = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeCopyAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeCopyAssignment, "underlying copy assignment not set");
    underlyingTypeCopyAssignment->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeCopyAssignment::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeCopyAssignment)
    {
        throw SymbolCheckException("enumerated type copy assignment has no underlying type copy assignment", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeMoveAssignment::EnumTypeMoveAssignment(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeMoveAssignment, sourcePos_, sourceModuleId_, name_), underlyingTypeMoveAssignment(nullptr)
{
}

EnumTypeMoveAssignment::EnumTypeMoveAssignment(EnumTypeSymbol* enumType_, TypeSymbol* voidType_) :
    FunctionSymbol(SymbolType::enumTypeMoveAssignment, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeMoveAssignment"), underlyingTypeMoveAssignment(nullptr)
{
    SetGroupName(U"operator=");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* thisParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"this");
    thisParam->SetType(enumType_->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thisParam);
    ParameterSymbol* thatParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"that");
    thatParam->SetType(enumType_->AddRvalueReference(soul::ast::SourcePos(), util::nil_uuid()));
    AddMember(thatParam);
    SetReturnType(voidType_);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeMoveAssignment = basicTypeSymbol->MoveAssignment();
}

void EnumTypeMoveAssignment::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeMoveAssignment, "underlying move assignment not set");
    const util::uuid& moveAssignmentId = underlyingTypeMoveAssignment->FunctionId();
    writer.GetBinaryStreamWriter().Write(moveAssignmentId);
}

void EnumTypeMoveAssignment::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid moveAssignmentId;
    reader.GetBinaryStreamReader().ReadUuid(moveAssignmentId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveAssignmentId, 0);
}

void EnumTypeMoveAssignment::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeMoveAssignment = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeMoveAssignment::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeMoveAssignment, "underlying move assignment not set");
    underlyingTypeMoveAssignment->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeMoveAssignment::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeMoveAssignment)
    {
        throw SymbolCheckException("enumerated type move assignment has no underlying type move assignment", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeReturn::EnumTypeReturn(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeReturn, sourcePos_, sourceModuleId_, name_), underlyingTypeReturn(nullptr)
{
}

EnumTypeReturn::EnumTypeReturn(EnumTypeSymbol* enumType_) :
    FunctionSymbol(SymbolType::enumTypeReturn, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeReturn"), underlyingTypeReturn(nullptr)
{
    SetGroupName(U"@return");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* valueParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"value");
    valueParam->SetType(enumType_);
    AddMember(valueParam);
    SetReturnType(enumType_);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeReturn = basicTypeSymbol->ReturnFun();
}

void EnumTypeReturn::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeReturn, "underlying return not set");
    const util::uuid& returnId = underlyingTypeReturn->FunctionId();
    writer.GetBinaryStreamWriter().Write(returnId);
}

void EnumTypeReturn::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid returnId;
    reader.GetBinaryStreamReader().ReadUuid(returnId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, returnId, 0);
}

void EnumTypeReturn::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeReturn = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeReturn::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeReturn, "underlying return not set");
    underlyingTypeReturn->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeReturn::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeReturn)
    {
        throw SymbolCheckException("enumerated type return has no underlying type return", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeEqualityOp::EnumTypeEqualityOp(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeEquality, sourcePos_, sourceModuleId_, name_), underlyingTypeEquality(nullptr)
{
}

EnumTypeEqualityOp::EnumTypeEqualityOp(EnumTypeSymbol* enumType_, TypeSymbol* boolType_) :
    FunctionSymbol(SymbolType::enumTypeEquality, enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"enumTypeEquality"), underlyingTypeEquality(nullptr)
{
    SetGroupName(U"operator==");
    SetAccess(SymbolAccess::public_);
    ParameterSymbol* leftParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"left");
    leftParam->SetType(enumType_);
    AddMember(leftParam);
    ParameterSymbol* rightParam = new ParameterSymbol(enumType_->GetSourcePos(), enumType_->SourceModuleId(), U"right");
    rightParam->SetType(enumType_);
    AddMember(rightParam);
    SetReturnType(boolType_);
    ComputeName();
    TypeSymbol* underlyingType = enumType_->UnderlyingType();
    //Assert(underlyingType->IsBasicTypeSymbol(), "basic type expected");
    BasicTypeSymbol* basicTypeSymbol = static_cast<BasicTypeSymbol*>(underlyingType);
    underlyingTypeEquality = basicTypeSymbol->EqualityOp();
}

void EnumTypeEqualityOp::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    //Assert(underlyingTypeEquality, "underlying equality not set");
    const util::uuid& equalityId = underlyingTypeEquality->FunctionId();
    writer.GetBinaryStreamWriter().Write(equalityId);
}

void EnumTypeEqualityOp::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid equalityId;
    reader.GetBinaryStreamReader().ReadUuid(equalityId);
    reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, equalityId, 0);
}

void EnumTypeEqualityOp::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        underlyingTypeEquality = functionSymbol;
    }
    else
    {
        //Assert(false, "invalid emplace function index");
    }
}

void EnumTypeEqualityOp::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
    //Assert(underlyingTypeEquality, "underlying equality not set");
    underlyingTypeEquality->GenerateCall(emitter, genObjects, flags, span, moduleId);
}

void EnumTypeEqualityOp::Check()
{
    FunctionSymbol::Check();
    if (!underlyingTypeEquality)
    {
        throw SymbolCheckException("enumerated type equality operation has no underlying type equality operation", GetSourcePos(), SourceModuleId());
    }
}

EnumTypeToUnderlyingTypeConversion::EnumTypeToUnderlyingTypeConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::enumTypeToUnderlyingType, sourcePos_, sourceModuleId_, name_), sourceType(), targetType()
{
    SetGroupName(U"@conversion");
}

EnumTypeToUnderlyingTypeConversion::EnumTypeToUnderlyingTypeConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_, TypeSymbol* sourceType_, TypeSymbol* targetType_) :
    FunctionSymbol(SymbolType::enumTypeToUnderlyingType, sourcePos_, sourceModuleId_, name_), sourceType(sourceType_), targetType(targetType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetConversionSourceType(sourceType->PlainType(GetSourcePos(), SourceModuleId()));
    SetConversionTargetType(targetType->PlainType(GetSourcePos(), SourceModuleId()));
}

void EnumTypeToUnderlyingTypeConversion::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(sourceType->TypeId());
    writer.GetBinaryStreamWriter().Write(targetType->TypeId());
}

void EnumTypeToUnderlyingTypeConversion::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid sourceTypeId;
    reader.GetBinaryStreamReader().ReadUuid(sourceTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, sourceTypeId, 1);
    util::uuid targetTypeId;
    reader.GetBinaryStreamReader().ReadUuid(targetTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, targetTypeId, 2);
}

void EnumTypeToUnderlyingTypeConversion::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        sourceType = typeSymbol;
    }
    else if (index == 2)
    {
        targetType = typeSymbol;
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void EnumTypeToUnderlyingTypeConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
}

void EnumTypeToUnderlyingTypeConversion::Check()
{
    FunctionSymbol::Check();
    if (!sourceType)
    {
        throw SymbolCheckException("enum type to underlying type conversion has no source type", GetSourcePos(), SourceModuleId());
    }
    if (!targetType)
    {
        throw SymbolCheckException("enum type to underlying type conversion has no target type", GetSourcePos(), SourceModuleId());
    }
}

UnderlyingTypeToEnumTypeConversion::UnderlyingTypeToEnumTypeConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_)
    : FunctionSymbol(SymbolType::underlyingToEnumType, sourcePos_, sourceModuleId_, name_), sourceType(), targetType()
{
    SetGroupName(U"@conversion");
}

UnderlyingTypeToEnumTypeConversion::UnderlyingTypeToEnumTypeConversion(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_, TypeSymbol* sourceType_, TypeSymbol* targetType_)
    : FunctionSymbol(SymbolType::underlyingToEnumType, sourcePos_, sourceModuleId_, name_), sourceType(sourceType_), targetType(targetType_)
{
    SetConversion();
    SetGroupName(U"@conversion");
    SetConversionSourceType(sourceType->PlainType(GetSourcePos(), SourceModuleId()));
    SetConversionTargetType(targetType->PlainType(GetSourcePos(), SourceModuleId()));
}

void UnderlyingTypeToEnumTypeConversion::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(sourceType->TypeId());
    writer.GetBinaryStreamWriter().Write(targetType->TypeId());
}

void UnderlyingTypeToEnumTypeConversion::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    util::uuid sourceTypeId;
    reader.GetBinaryStreamReader().ReadUuid(sourceTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, sourceTypeId, 1);
    util::uuid targetTypeId;
    reader.GetBinaryStreamReader().ReadUuid(targetTypeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, targetTypeId, 2);
}

void UnderlyingTypeToEnumTypeConversion::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 1)
    {
        sourceType = typeSymbol;
    }
    else if (index == 2)
    {
        targetType = typeSymbol;
    }
    else
    {
        FunctionSymbol::EmplaceType(typeSymbol, index);
    }
}

void UnderlyingTypeToEnumTypeConversion::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& span, const util::uuid& moduleId)
{
}

void UnderlyingTypeToEnumTypeConversion::Check()
{
    FunctionSymbol::Check();
    if (!sourceType)
    {
        throw SymbolCheckException("underlying type to enum type conversion has no source type", GetSourcePos(), SourceModuleId());
    }
    if (!targetType)
    {
        throw SymbolCheckException("underlying type to enum type conversion has no target type", GetSourcePos(), SourceModuleId());
    }
}
} // namespace cmajor::symbols
