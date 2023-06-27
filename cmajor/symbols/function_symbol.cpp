// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.function.symbol;

import cmajor.symbols.class_template_specializations;
import cmajor.symbols.global.flags;
import cmajor.symbols.exception;
import cmajor.symbols.modules;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.templates;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.classes;
import cmajor.ast.parameter;
import soul.ast.source.pos;
import cmajor.ast.node;
import cmajor.ast.function;
import cmajor.ast.specifier;
import cmajor.ast.clone;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

AccessCheckFunction hasAccess;

void SetAccessCheckFunction(AccessCheckFunction accessCheckFunc)
{
    hasAccess = accessCheckFunc;
}

AccessCheckFunction GetAccessCheckFunction()
{
    if (!hasAccess)
    {
        throw std::runtime_error("access check functio not set");
    }
    return hasAccess;
}

bool FunctionSymbolsEqual::operator()(FunctionSymbol* left, FunctionSymbol* right) const
{
    if (left->Parent()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol && right->Parent()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
    {
        ClassTemplateSpecializationSymbol* leftSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(left->Parent());
        ClassTemplateSpecializationSymbol* rightSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(right->Parent());
        if (TypesEqual(leftSpecialization, rightSpecialization))
        {
            int leftIndex = left->GetIndex();
            int rightIndex = right->GetIndex();
            if (leftIndex == -1 || rightIndex == -1)
            {
                return left == right;
            }
            else
            {
                return leftIndex == rightIndex;
            }
        }
    }
    else if (left->IsTemplateSpecialization() && right->IsTemplateSpecialization())
    {
        if (left->FunctionTemplate() == right->FunctionTemplate())
        {
            int n = left->TemplateArgumentTypes().size();
            if (right->TemplateArgumentTypes().size() == n)
            {
                bool equal = true;
                for (int i = 0; i < n; ++i)
                {
                    if (!TypesEqual(left->TemplateArgumentTypes()[i], right->TemplateArgumentTypes()[i]))
                    {
                        equal = false;
                        break;
                    }
                }
                return equal;
            }
        }
    }
    else if (GetGlobalFlag(GlobalFlags::release) && !left->IsGeneratedFunction() && left->IsInline() && !right->IsGeneratedFunction() && right->IsInline())
    {
        return left->FunctionId() == right->FunctionId();
    }
    return left == right;
}

size_t FunctionSymbolHash::operator()(FunctionSymbol* fun) const
{
    if (fun->Parent()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
    {
        ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(fun->Parent());
        return HashValue(specialization->TypeId()) ^ fun->GetIndex();
    }
    else if (fun->IsTemplateSpecialization())
    {
        size_t hashValue = HashValue(fun->FunctionTemplate()->FunctionId());
        int n = fun->TemplateArgumentTypes().size();
        for (int i = 0; i < n; ++i)
        {
            hashValue ^= HashValue(fun->TemplateArgumentTypes()[i]->TypeId());
        }
        return hashValue;
    }
    else if (GetGlobalFlag(GlobalFlags::release) && !fun->IsGeneratedFunction() && fun->IsInline())
    {
        return HashValue(fun->FunctionId());
    }
    return std::hash<FunctionSymbol*>()(fun);
}

void ViableFunctionSet::Insert(FunctionSymbol* fun)
{
    auto it = set.find(fun);
    if (it != set.cend())
    {
        set.erase(it);
    }
    set.insert(fun);
}

class OperatorMangleMap
{
public:
    static OperatorMangleMap& Instance();
    std::u32string Mangle(const std::u32string& groupName);
private:
    static std::unique_ptr<OperatorMangleMap> instance;
    std::unordered_map<std::u32string, std::u32string> mangleMap;
    OperatorMangleMap();
};

OperatorMangleMap& OperatorMangleMap::Instance()
{
    static OperatorMangleMap instance;
    return instance;
}

OperatorMangleMap::OperatorMangleMap()
{
    mangleMap[U"operator<<"] = U"op_shl";
    mangleMap[U"operator>>"] = U"op_shr";
    mangleMap[U"operator=="] = U"op_eq";
    mangleMap[U"operator="] = U"op_assign";
    mangleMap[U"operator<"] = U"op_less";
    mangleMap[U"operator->"] = U"op_arrow";
    mangleMap[U"operator++"] = U"op_plusplus";
    mangleMap[U"operator--"] = U"op_minusminus";
    mangleMap[U"operator+"] = U"op_plus";
    mangleMap[U"operator-"] = U"op_minus";
    mangleMap[U"operator*"] = U"op_star";
    mangleMap[U"operator/"] = U"op_div";
    mangleMap[U"operator%"] = U"op_rem";
    mangleMap[U"operator&"] = U"op_and";
    mangleMap[U"operator|"] = U"op_or";
    mangleMap[U"operator^"] = U"op_xor";
    mangleMap[U"operator!"] = U"op_not";
    mangleMap[U"operator~"] = U"op_cpl";
    mangleMap[U"operator[]"] = U"op_index";
    mangleMap[U"operator()"] = U"op_apply";
}

std::u32string OperatorMangleMap::Mangle(const std::u32string& groupName)
{
    auto it = mangleMap.find(groupName);
    if (it != mangleMap.cend())
    {
        return it->second;
    }
    else
    {
        return U"operator";
    }
}

FunctionGroupSymbol::FunctionGroupSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : Symbol(SymbolType::functionGroupSymbol, sourcePos_, sourceModuleId_, name_)
{
}

void FunctionGroupSymbol::ComputeMangledName()
{
    std::u32string mangledName = util::ToUtf32(TypeString());
    if (Name().find(U"operator") != std::u32string::npos)
    {
        mangledName.append(1, U'_').append(OperatorMangleMap::Instance().Mangle(Name()));
    }
    else if (Name().find(U'@') == std::u32string::npos)
    {
        mangledName.append(1, U'_').append(Name());
    }
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()))));
    SetMangledName(mangledName);
}

void FunctionGroupSymbol::AddFunction(FunctionSymbol* function)
{
    if (function->IsProgramMain()) return;
    Assert(function->GroupName() == Name(), "wrong function group");
    if (function->IsVarArg())
    {
        varArgFunctions.push_back(function);
    }
    else
    {
        int arity = function->Arity();
        std::vector<FunctionSymbol*>& functionList = arityFunctionListMap[arity];
        functionList.push_back(function);
        function->SetFunctionGroup(this);
    }
}

void FunctionGroupSymbol::RemoveFunction(FunctionSymbol* function)
{
    if (function->IsVarArg())
    {
        auto end = std::remove(varArgFunctions.begin(), varArgFunctions.end(), function);
        varArgFunctions.erase(end, varArgFunctions.end());
    }
    else
    {
        int arity = function->Arity();
        std::vector<FunctionSymbol*>& functionList = arityFunctionListMap[arity];
        auto end = std::remove(functionList.begin(), functionList.end(), function);
        functionList.erase(end, functionList.end());
    }
}

bool FunctionGroupSymbol::IsEmpty() const
{
    for (const auto& p : arityFunctionListMap)
    {
        if (!p.second.empty()) return false;
    }
    return true;
}

FunctionSymbol* FunctionGroupSymbol::GetFunction()
{
    if (arityFunctionListMap.size() == 1)
    {
        std::vector<FunctionSymbol*>& functionList = arityFunctionListMap.begin()->second;
        if (functionList.size() == 1)
        {
            return functionList[0];
        }
    }
    return nullptr;
}

void FunctionGroupSymbol::CollectViableFunctions(int arity, ViableFunctionSet& viableFunctions, Module* module)
{
    auto it = arityFunctionListMap.find(arity);
    if (it != arityFunctionListMap.cend())
    {
        std::vector<FunctionSymbol*>& functionList = it->second;
        for (FunctionSymbol* function : functionList)
        {
            viableFunctions.Insert(function);
        }
    }
    for (FunctionSymbol* varArgFunction : varArgFunctions)
    {
        if (arity >= varArgFunction->Arity())
        {
            viableFunctions.Insert(varArgFunction);
        }
    }
}

bool FunctionGroupSymbol::HasProjectMembers() const
{
    for (const auto& p : arityFunctionListMap)
    {
        for (FunctionSymbol* fun : p.second)
        {
            if (fun->IsTemplateSpecialization()) continue;
            if (fun->IsProject())
            {
                return true;
            }
        }
    }
    return false;
}

void FunctionGroupSymbol::AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const
{
    for (const auto& p : arityFunctionListMap)
    {
        for (FunctionSymbol* fun : p.second)
        {
            if (fun->IsTemplateSpecialization()) continue;
            if (fun->IsProject())
            {
                std::unique_ptr<soul::xml::Element> functionElement = fun->ToDomElement(typeMap);
                element->AppendChild(functionElement.release());
            }
        }
    }
}

void FunctionGroupSymbol::Check()
{
    Symbol::Check();
    for (const auto& p : arityFunctionListMap)
    {
        for (const auto& q : p.second)
        {
            if (!q)
            {
                throw SymbolCheckException("function group symbol contains empty function pointer", GetSourcePos(), SourceModuleId());
            }
        }
    }
}

void FunctionGroupSymbol::CheckDuplicateFunctionSymbols()
{
    for (const auto& p : arityFunctionListMap)
    {
        std::set<std::u32string> names;
        for (const auto& functionSymbol : p.second)
        {
            if (!functionSymbol->IsFunctionTemplate() && !functionSymbol->IsTemplateSpecialization())
            {
                if (names.find(functionSymbol->FullName()) != names.cend())
                {
                    throw Exception("function with identical name '" + util::ToUtf8(functionSymbol->FullName()) + "' already defined.", GetSourcePos(), SourceModuleId());
                }
                names.insert(functionSymbol->FullName());
            }
        }
    }
}

std::string FunctionGroupSymbol::GetSymbolHelp() const
{
    if (arityFunctionListMap.size() == 1)
    {
        const std::vector<FunctionSymbol*>& v = arityFunctionListMap.begin()->second;
        if (v.size() == 1)
        {
            FunctionSymbol* fun = v.front();
            return fun->GetSymbolHelp();
        }
    }
    int n = 0;
    for (const auto& p : arityFunctionListMap)
    {
        n = n + p.second.size();
    }
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ").append(util::ToUtf8(FullName())).append(" (").append(std::to_string(n)).append(" overloads)");
    return help;
}

bool FunctionGroupSymbol::IsValidCCFunctionGroup(FunctionSymbol* fromFunction) const
{
    AccessCheckFunction canAccess = GetAccessCheckFunction();
    if (!fromFunction) return false;
    if (util::StartsWith(Name(), U"@")) return false;
    if (util::StartsWith(Name(), U"operator")) return false;
    for (const auto& p : arityFunctionListMap)
    {
        for (auto functionSymbol : p.second)
        {
            if (canAccess(fromFunction, functionSymbol)) return true;
        }
    }
    return false;
}

std::vector<Symbol*> FunctionGroupSymbol::GetParamHelpSymbols() const
{
    std::vector<Symbol*> paramHelpSymbols;
    for (const auto& p : arityFunctionListMap)
    {
        for (auto functionSymbol : p.second)
        {
            paramHelpSymbols.push_back(functionSymbol);
        }
    }
    return paramHelpSymbols;
}

std::string FunctionSymbolFlagStr(FunctionSymbolFlags flags)
{
    std::string s;
    if ((flags & FunctionSymbolFlags::inline_) != FunctionSymbolFlags::none)
    {
        s.append("inline");
    }
    if ((flags & FunctionSymbolFlags::constExpr) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("constexpr");
    }
    if ((flags & FunctionSymbolFlags::cdecl_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("cdecl");
    }
    if ((flags & FunctionSymbolFlags::suppress) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("suppress");
    }
    if ((flags & FunctionSymbolFlags::default_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("default");
    }
    if ((flags & FunctionSymbolFlags::explicit_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("explicit");
    }
    if ((flags & FunctionSymbolFlags::virtual_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("virtual");
    }
    if ((flags & FunctionSymbolFlags::override_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("override");
    }
    if ((flags & FunctionSymbolFlags::abstract_) != FunctionSymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("abstract");
    }
    return s;
}

FunctionSymbol::FunctionSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    ContainerSymbol(SymbolType::functionSymbol, sourcePos_, sourceModuleId_, name_), functionTemplate(nullptr), master(nullptr),
    functionId(util::nil_uuid()), groupName(), parameters(), localVariables(),
    returnType(), flags(FunctionSymbolFlags::none), index(-1), vmtIndex(-1), imtIndex(-1),
    nextTemporaryIndex(0), functionGroup(nullptr), isProgramMain(false), unwindInfoVar(nullptr),
    conversionSourceType(nullptr), conversionTargetType(nullptr)
{
    if (Name() == U"Deallocate(String<char> this)")
    {
        int x = 0;
    }
}

FunctionSymbol::FunctionSymbol(SymbolType symbolType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    ContainerSymbol(symbolType_, sourcePos_, sourceModuleId_, name_), functionTemplate(nullptr), master(nullptr),
    functionId(util::nil_uuid()), groupName(), parameters(), localVariables(),
    returnType(), flags(FunctionSymbolFlags::none), index(-1), vmtIndex(-1), imtIndex(-1),
    nextTemporaryIndex(0), functionGroup(nullptr), isProgramMain(false), unwindInfoVar(nullptr),
    conversionSourceType(nullptr), conversionTargetType(nullptr)
{
    if (Name() == U"Deallocate(String<char> this)")
    {
        int x = 0;
    }
}

void FunctionSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    Assert(!functionId.is_nil(), "function id not initialized");
    writer.GetBinaryStreamWriter().Write(functionId);
    writer.GetBinaryStreamWriter().Write(index);
    writer.GetBinaryStreamWriter().Write(groupName);
    writer.GetBinaryStreamWriter().Write(static_cast<uint32_t>(flags));
    util::uuid functionTemplateId = util::nil_uuid();
    if (functionTemplate)
    {
        functionTemplateId = functionTemplate->FunctionId();
    }
    writer.GetBinaryStreamWriter().Write(functionTemplateId);
    if (IsFunctionTemplate() || (GetGlobalFlag(GlobalFlags::release) && IsInline()) || IsConstExpr())
    {
        usingNodes.Write(writer.GetAstWriter());
        cmajor::ast::Node* node = GetRootModuleForCurrentThread()->GetSymbolTable().GetNode(this);
        writer.GetAstWriter().Write(node);
    }
    util::uuid returnTypeId = util::nil_uuid();
    if (returnType)
    {
        returnTypeId = returnType->TypeId();
    }
    writer.GetBinaryStreamWriter().Write(returnTypeId);
    writer.GetBinaryStreamWriter().Write(vmtIndex);
    writer.GetBinaryStreamWriter().Write(imtIndex);
    bool hasReturnParam = returnParam != nullptr;
    writer.GetBinaryStreamWriter().Write(hasReturnParam);
    if (hasReturnParam)
    {
        writer.Write(returnParam.get());
    }
    if (constraint)
    {
        writer.GetBinaryStreamWriter().Write(true);
        writer.GetAstWriter().Write(constraint.get());
    }
    else
    {
        writer.GetBinaryStreamWriter().Write(false);
    }
    uint32_t n = templateArgumentTypes.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        TypeSymbol* templateArgumentType = templateArgumentTypes[i];
        writer.GetBinaryStreamWriter().Write(templateArgumentType->TypeId());
    }
    if (IsConversion())
    {
        writer.GetBinaryStreamWriter().Write(conversionSourceType->TypeId());
        writer.GetBinaryStreamWriter().Write(conversionTargetType->TypeId());
    }
}

void FunctionSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(functionId);
    index = reader.GetBinaryStreamReader().ReadInt();
    reader.GetSymbolTable()->AddFunctionSymbolToFunctionIdMap(this);
    groupName = reader.GetBinaryStreamReader().ReadUtf32String();
    flags = static_cast<FunctionSymbolFlags>(reader.GetBinaryStreamReader().ReadUInt());
    util::uuid functionTemplateId;
    reader.GetBinaryStreamReader().ReadUuid(functionTemplateId);
    if (!functionTemplateId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, functionTemplateId, 0);
    }
    if (IsFunctionTemplate() || (GetGlobalFlag(GlobalFlags::release) && IsInline()) || IsConstExpr())
    {
        usingNodes.Read(reader.GetAstReader());
        cmajor::ast::Node* node = reader.GetAstReader().ReadNode();
        cmajor::ast::FunctionNode* funNode = static_cast<cmajor::ast::FunctionNode*>(node);
        functionNode.reset(funNode);
    }
    util::uuid returnTypeId;
    reader.GetBinaryStreamReader().ReadUuid(returnTypeId);
    if (!returnTypeId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, returnTypeId, 0);
    }
    vmtIndex = reader.GetBinaryStreamReader().ReadInt();
    imtIndex = reader.GetBinaryStreamReader().ReadInt();
    bool hasReturnParam = reader.GetBinaryStreamReader().ReadBool();
    if (hasReturnParam)
    {
        returnParam.reset(reader.ReadParameterSymbol(this));
    }
    bool hasConstraint = reader.GetBinaryStreamReader().ReadBool();
    if (hasConstraint)
    {
        constraint.reset(reader.GetAstReader().ReadConstraintNode());
    }
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    templateArgumentTypes.resize(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        util::uuid templateArgumentId;
        reader.GetBinaryStreamReader().ReadUuid(templateArgumentId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, templateArgumentId, -1 - i);
    }
    if (IsConversion())
    {
        util::uuid conversionSourceTypeId;
        reader.GetBinaryStreamReader().ReadUuid(conversionSourceTypeId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, conversionSourceTypeId, 10000);
        util::uuid conversionTargetTypeId;
        reader.GetBinaryStreamReader().ReadUuid(conversionTargetTypeId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, conversionTargetTypeId, 10001);
        reader.AddConversion(this);
    }
}

void FunctionSymbol::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    if (index == 0)
    {
        functionTemplate = functionSymbol;
    }
    else
    {
        Assert(false, "invalid emplace function index");
    }
}

void FunctionSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_ && !IsGeneratedFunction())
    {
        collector->AddFunction(this);
    }
}

void FunctionSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 10000)
    {
        conversionSourceType = typeSymbol;
    }
    else if (index == 10001)
    {
        conversionTargetType = typeSymbol;
    }
    else if (index == 0)
    {
        returnType = typeSymbol;
    }
    else if (index < 0)
    {
        int templateArgumentIndex = -(index + 1);
        if (templateArgumentIndex < 0 || templateArgumentIndex >= templateArgumentTypes.size())
        {
            throw Exception("invalid emplace template argument index '" + std::to_string(index) + "'", GetSourcePos(), SourceModuleId());
        }
        templateArgumentTypes[templateArgumentIndex] = typeSymbol;
    }
    else
    {
        throw Exception("invalid emplace type index '" + std::to_string(index) + "'", GetSourcePos(), SourceModuleId());
    }
}

void FunctionSymbol::AddMember(Symbol* member)
{
    ContainerSymbol::AddMember(member);
    if (member->GetSymbolType() == SymbolType::templateParameterSymbol)
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(member));
    }
    else if (member->GetSymbolType() == SymbolType::parameterSymbol)
    {
        parameters.push_back(static_cast<ParameterSymbol*>(member));
    }
}

bool FunctionSymbol::IsExportSymbol() const
{
    if (IsTemplateSpecialization() && Parent()->GetSymbolType() != SymbolType::classTemplateSpecializationSymbol) return false;
    if (IsGeneratedFunction()) return false;
    if (intrinsic) return false;
    if (IsCopy()) return false;
    return ContainerSymbol::IsExportSymbol();
}

void FunctionSymbol::ComputeName()
{
    std::u32string name;
    name.append(groupName);
    name.append(1, U'(');
    int n = parameters.size();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            name.append(U", ");
        }
        ParameterSymbol* parameter = parameters[i];
        if (i == 0 && (groupName == U"@constructor" || groupName == U"operator=" || IsConstructorDestructorOrNonstaticMemberFunction()))
        {
            name.append(parameter->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->FullName());
        }
        else
        {
            name.append(parameter->GetType()->FullName());
        }
        name.append(1, U' ');
        name.append(std::u32string(parameter->Name()));
    }
    name.append(1, U')');
    SetName(name);
    if (!IsBasicTypeOperation())
    {
        ComputeMangledName();
    }
}

void FunctionSymbol::ComputeMangledName()
{
    if (IsCDecl())
    {
        SetMangledName(GroupName());
        return;
    }
    std::u32string mangledName = util::ToUtf32(TypeString());
    if (groupName.find(U"operator") != std::u32string::npos)
    {
        mangledName.append(1, U'_').append(OperatorMangleMap::Instance().Mangle(GroupName()));
    }
    else if (groupName.find(U'@') == std::u32string::npos)
    {
        mangledName.append(1, U'_').append(GroupName());
    }
    SymbolType symbolType = GetSymbolType();
    switch (symbolType)
    {
    case SymbolType::staticConstructorSymbol: case SymbolType::constructorSymbol: case SymbolType::destructorSymbol: case SymbolType::memberFunctionSymbol:
    {
        Symbol* parentClass = Parent();
        mangledName.append(1, U'_').append(parentClass->SimpleName());
    }
    }
    std::string templateArgumentString;
    for (TypeSymbol* templateArgumentType : templateArgumentTypes)
    {
        templateArgumentString.append("_").append(util::ToUtf8(templateArgumentType->FullName()));
    }
    std::string constraintString;
    if (Constraint())
    {
        constraintString = " " + Constraint()->ToString();
    }
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()) + templateArgumentString + constraintString)));
    SetMangledName(mangledName);
}

std::u32string FunctionSymbol::FullName(bool withParamNames) const
{
    std::u32string fullName;
    if (!Parent())
    {
        throw Exception("function symbol has no parent", GetSourcePos(), SourceModuleId());
    }
    std::u32string parentFullName = Parent()->FullName();
    fullName.append(parentFullName);
    if (!parentFullName.empty())
    {
        fullName.append(1, U'.');
    }
    if (groupName == U"@constructor")
    {
        fullName.append(Parent()->Name());
    }
    else if (groupName == U"@destructor")
    {
        fullName.append(U"~").append(Parent()->Name());
    }
    else
    {
        fullName.append(groupName);
    }
    fullName.append(1, U'(');
    int n = parameters.size();
    int startParamIndex = StartParamIndex();
    for (int i = startParamIndex; i < n; ++i)
    {
        if (i > startParamIndex)
        {
            fullName.append(U", ");
        }
        ParameterSymbol* parameter = parameters[i];
        fullName.append(parameter->GetType()->FullName());
        if (withParamNames)
        {
            fullName.append(U" ").append(parameter->Name());
        }
    }
    fullName.append(1, U')');
    if (IsConst())
    {
        fullName.append(U" const");
    }
    return fullName;
}

std::u32string FunctionSymbol::FullName() const
{
    return FullName(false);
}

std::u32string FunctionSymbol::FullNameWithSpecifiers() const
{
    std::u32string fullNameWithSpecifiers = util::ToUtf32(SymbolFlagStr(GetStableSymbolFlags()));
    std::u32string f = util::ToUtf32(FunctionSymbolFlagStr(flags));
    if (!f.empty())
    {
        if (!fullNameWithSpecifiers.empty())
        {
            fullNameWithSpecifiers.append(1, U' ');
        }
        fullNameWithSpecifiers.append(f);
    }
    if (!fullNameWithSpecifiers.empty())
    {
        fullNameWithSpecifiers.append(1, U' ');
    }
    fullNameWithSpecifiers.append(FullName());
    return fullNameWithSpecifiers;
}

std::u32string FunctionSymbol::DocName() const
{
    std::u32string docName = groupName;
    if (!templateParameters.empty())
    {
        docName.append(1, '<');
        bool first = true;
        for (TemplateParameterSymbol* templateParameter : templateParameters)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                docName.append(U", ");
            }
            docName.append(templateParameter->DocName());
        }
        docName.append(1, '>');
    }
    docName.append(1, '(');
    int n = parameters.size();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            docName.append(U", ");
        }
        ParameterSymbol* parameter = parameters[i];
        if (parameter->GetType()->Ns() == Ns())
        {
            docName.append(parameter->GetType()->Name());
        }
        else
        {
            docName.append(parameter->GetType()->FullName());
        }
        docName.append(1, ' ').append(parameter->Name());
    }
    docName.append(1, ')');
    return docName;
}

std::string FunctionSymbol::GetSpecifierStr() const
{
    std::string specifierStr = SymbolFlagStr(GetStableSymbolFlags());
    std::string f = FunctionSymbolFlagStr(flags);
    if (!f.empty())
    {
        if (!specifierStr.empty())
        {
            specifierStr.append(1, ' ');
        }
        specifierStr.append(f);
    }
    return specifierStr;
}

std::string FunctionSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    if (ReturnType())
    {
        syntax.append(util::ToUtf8(ReturnType()->DocName()));
        syntax.append(1, ' ');
    }
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void FunctionSymbol::SetConversionSourceType(TypeSymbol* conversionSourceType_)
{
    conversionSourceType = conversionSourceType_;
}

void FunctionSymbol::SetConversionTargetType(TypeSymbol* conversionTargetType_)
{
    conversionTargetType = conversionTargetType_;
}

void FunctionSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if ((flags & cmajor::ir::OperationFlags::virtualCall) != cmajor::ir::OperationFlags::none)
    {
        GenerateVirtualCall(emitter, genObjects, flags, sourcePos, moduleId);
        return;
    }
    int na = genObjects.size();
    for (int i = 0; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        genObject->Load(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
    }
    emitter.SetCurrentDebugLocation(sourcePos);
    void* functionType = IrType(emitter);
    void* callee = emitter.GetOrInsertFunction(util::ToUtf8(MangledName()), functionType, DontThrow());
    std::vector<void*> args;
    int n = parameters.size();
    if (ReturnsClassInterfaceOrClassDelegateByValue())
    {
        ++n;
    }
    args.resize(n);
    for (int i = 0; i < n; ++i)
    {
        void* arg = emitter.Stack().Pop();
        args[n - i - 1] = arg;
    }
    void* handlerBlock = emitter.HandlerBlock();
    void* cleanupBlock = emitter.CleanupBlock();
    bool newCleanupNeeded = emitter.NewCleanupNeeded();
    cmajor::ir::Pad* currentPad = emitter.CurrentPad();
    std::vector<void*> bundles;
    if (currentPad != nullptr)
    {
        bundles.push_back(currentPad->value);
    }
    if (ReturnType() && ReturnType()->GetSymbolType() != SymbolType::voidTypeSymbol && !ReturnsClassInterfaceOrClassDelegateByValue())
    {
        if (DontThrow() || GetSymbolType() == SymbolType::destructorSymbol || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                emitter.Stack().Push(emitter.CreateCall(callee, args));
            }
            else
            {
                emitter.Stack().Push(emitter.CreateCallInst(callee, args, bundles, sourcePos));
            }
        }
        else
        {
            void* nextBlock = nullptr;
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                nextBlock = emitter.CreateBasicBlock("next");
            }
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block");
            }
            if (currentPad == nullptr)
            {
                emitter.Stack().Push(emitter.CreateInvoke(callee, nextBlock, unwindBlock, args));
            }
            else
            {
                emitter.Stack().Push(emitter.CreateInvokeInst(callee, nextBlock, unwindBlock, args, bundles, sourcePos));
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
    else
    {
        if (DontThrow() || GetSymbolType() == SymbolType::destructorSymbol || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                emitter.CreateCall(callee, args);
            }
            else
            {
                emitter.CreateCallInst(callee, args, bundles, sourcePos);
            }
        }
        else
        {
            void* nextBlock = nullptr;
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                nextBlock = emitter.CreateBasicBlock("next");
            }
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block");
            }
            if (currentPad == nullptr)
            {
                emitter.CreateInvoke(callee, nextBlock, unwindBlock, args);
            }
            else
            {
                emitter.CreateInvokeInst(callee, nextBlock, unwindBlock, args, bundles, sourcePos);
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
}

void FunctionSymbol::GenerateVirtualCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    int na = genObjects.size();
    Assert(na > 0, "nonempty argument list expected");
    Assert(VmtIndex() != -1, "member function has invalid vmt index");
    cmajor::ir::GenObject* classPtrArg = genObjects[0];
    TypeSymbol* type = static_cast<TypeSymbol*>(classPtrArg->GetType());
    Assert(type, "type expected");
    Assert(type->BaseType()->IsClassTypeSymbol(), "class type pointer expected");
    ClassTypeSymbol* classType = static_cast<ClassTypeSymbol*>(type->BaseType());
    ClassTypeSymbol* vmtPtrHolderClass = classType->VmtPtrHolderClass();
    void* callee = nullptr;
    for (int i = 0; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        genObject->Load(emitter, cmajor::ir::OperationFlags::none);
        if (i == 0)
        {
            emitter.Stack().Dup();
            void* thisPtr = emitter.Stack().Pop();
            if (classType != vmtPtrHolderClass)
            {
                thisPtr = emitter.CreateBitCast(thisPtr, vmtPtrHolderClass->AddPointer(GetSourcePos(), SourceModuleId())->IrType(emitter));
            }
            void* vmtPtr = emitter.GetVmtPtr(thisPtr, vmtPtrHolderClass->VmtPtrIndex(), classType->VmtPtrType(emitter));
            void* methodPtr = emitter.GetMethodPtr(vmtPtr, VmtIndex() + GetFunctionVmtIndexOffset());
            callee = emitter.CreateBitCast(methodPtr, emitter.GetIrTypeForPtrType(IrType(emitter)));
        }
    }
    std::vector<void*> args;
    int n = Parameters().size();
    if (ReturnsClassInterfaceOrClassDelegateByValue())
    {
        ++n;
    }
    args.resize(n);
    for (int i = 0; i < n; ++i)
    {
        void* arg = emitter.Stack().Pop();
        args[n - i - 1] = arg;
    }
    emitter.SetCurrentDebugLocation(sourcePos);
    void* handlerBlock = emitter.HandlerBlock();
    void* cleanupBlock = emitter.CleanupBlock();
    bool newCleanupNeeded = emitter.NewCleanupNeeded();
    std::vector<void*> bundles;
    cmajor::ir::Pad* currentPad = emitter.CurrentPad();
    if (currentPad != nullptr)
    {
        bundles.push_back(currentPad->value);
    }
    if (ReturnType() && !ReturnType()->IsVoidType() && !ReturnsClassInterfaceOrClassDelegateByValue())
    {
        if (DontThrow() || GetSymbolType() == SymbolType::destructorSymbol || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                emitter.Stack().Push(emitter.CreateCall(callee, args));
            }
            else
            {
                emitter.Stack().Push(emitter.CreateCallInst(callee, args, bundles, sourcePos));
            }
        }
        else
        {
            void* nextBlock = nullptr;
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                nextBlock = emitter.CreateBasicBlock("next");
            }
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block");
            }
            if (currentPad == nullptr)
            {
                emitter.Stack().Push(emitter.CreateInvoke(callee, nextBlock, unwindBlock, args));
            }
            else
            {
                emitter.Stack().Push(emitter.CreateInvokeInst(callee, nextBlock, unwindBlock, args, bundles, sourcePos));
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
    else
    {
        if (DontThrow() || GetSymbolType() == SymbolType::destructorSymbol || (!handlerBlock && !cleanupBlock && !newCleanupNeeded))
        {
            if (currentPad == nullptr)
            {
                emitter.CreateCall(callee, args);
            }
            else
            {
                emitter.CreateCallInst(callee, args, bundles, sourcePos);
            }
        }
        else
        {
            void* nextBlock = nullptr;
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                nextBlock = emitter.CreateBasicBlock("next");
            }
            if (newCleanupNeeded)
            {
                emitter.CreateCleanup();
                cleanupBlock = emitter.CleanupBlock();
            }
            void* unwindBlock = cleanupBlock;
            if (unwindBlock == nullptr)
            {
                unwindBlock = handlerBlock;
                Assert(unwindBlock, "no unwind block");
            }
            if (currentPad == nullptr)
            {
                emitter.CreateInvoke(callee, nextBlock, unwindBlock, args);
            }
            else
            {
                emitter.CreateInvokeInst(callee, nextBlock, unwindBlock, args, bundles, sourcePos);
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
}

std::unique_ptr<Value> FunctionSymbol::ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, Value* receiver) const
{
    return std::unique_ptr<Value>();
}

std::unique_ptr<Value> FunctionSymbol::ConvertValue(const std::unique_ptr<Value>& value) const
{
    return std::unique_ptr<Value>();
}

void FunctionSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("group name: " + util::ToUtf8(groupName));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    if (!MangledName().empty())
    {
        formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    }
    if (constraint)
    {
        formatter.WriteLine("constraint: " + constraint->ToString());
    }
    if (IsConversion())
    {
        formatter.WriteLine("conversion:");
        formatter.IncIndent();
        std::string conversionTypeStr;
        if (GetConversionType() == ConversionType::implicit_)
        {
            conversionTypeStr = "implicit";
        }
        else if (GetConversionType() == ConversionType::explicit_)
        {
            conversionTypeStr = "explicit";
        }
        formatter.WriteLine("conversion type: " + conversionTypeStr);
        formatter.WriteLine("source type: " + util::ToUtf8(ConversionSourceType()->FullName()));
        formatter.WriteLine("target type: " + util::ToUtf8(ConversionTargetType()->FullName()));
        formatter.WriteLine("distance: " + std::to_string(ConversionDistance()));
        formatter.DecIndent();
    }
}

bool FunctionSymbol::IsDefaultConstructor() const
{
    return parameters.size() == 1 && groupName == U"@constructor" && parameters[0]->GetType()->PointerCount() == 1 && parameters[0]->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->IsClassTypeSymbol();
}

bool FunctionSymbol::IsCopyConstructor() const
{
    return parameters.size() == 2 && groupName == U"@constructor" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddConst(GetSourcePos(), SourceModuleId())->AddLvalueReference(GetSourcePos(), SourceModuleId()), parameters[1]->GetType());
}

bool FunctionSymbol::IsMoveConstructor() const
{
    return parameters.size() == 2 && groupName == U"@constructor" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddRvalueReference(GetSourcePos(), SourceModuleId()), parameters[1]->GetType());
}

bool FunctionSymbol::IsCopyAssignment() const
{
    return parameters.size() == 2 && groupName == U"operator=" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddConst(GetSourcePos(), SourceModuleId())->AddLvalueReference(GetSourcePos(), SourceModuleId()), parameters[1]->GetType());
}

bool FunctionSymbol::IsMoveAssignment() const
{
    return parameters.size() == 2 && groupName == U"operator=" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(GetSourcePos(), SourceModuleId())->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddRvalueReference(GetSourcePos(), SourceModuleId()), parameters[1]->GetType());
}

void FunctionSymbol::AddLocalVariable(LocalVariableSymbol* localVariable)
{
    localVariables.push_back(localVariable);
}

bool FunctionSymbol::ReturnsClassInterfaceOrClassDelegateByValue() const
{
    return returnType && (returnType->IsClassTypeSymbol() || returnType->GetSymbolType() == SymbolType::classDelegateTypeSymbol || returnType->GetSymbolType() == SymbolType::interfaceTypeSymbol);
}

void FunctionSymbol::SetReturnParam(ParameterSymbol* returnParam_)
{
    returnParam.reset(returnParam_);
}

void FunctionSymbol::SetGroupName(const std::u32string& groupName_)
{
    groupName = groupName_;
}

void FunctionSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only constructors can be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        SetExternal();
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only special member functions can be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only special member functions can be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        SetCDecl();
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("function symbol cannot be throw and nothrow at the same time", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        if (!GetGlobalFlag(GlobalFlags::unitTest))
        {
            throw Exception("function symbol cannot be unit_test", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::winapi) != cmajor::ast::Specifiers::none)
    {
        SetFlag(FunctionSymbolFlags::winapi);
    }
}

void FunctionSymbol::CloneUsingNodes(const std::vector<cmajor::ast::Node*>& usingNodes_)
{
    cmajor::ast::CloneContext cloneContext;
    for (cmajor::ast::Node* usingNode : usingNodes_)
    {
        usingNodes.Add(usingNode->Clone(cloneContext));
    }
}

LocalVariableSymbol* FunctionSymbol::CreateTemporary(TypeSymbol* type, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    LocalVariableSymbol* temporary = new LocalVariableSymbol(sourcePos, moduleId, U"@t" + util::ToUtf32(std::to_string(nextTemporaryIndex++)));
    temporary->SetType(type);
    AddMember(temporary);
    AddLocalVariable(temporary);
    return temporary;
}

std::vector<LocalVariableSymbol*> FunctionSymbol::CreateTemporariesTo(FunctionSymbol* currentFunction)
{
    return std::vector<LocalVariableSymbol*>();
}

void* FunctionSymbol::IrType(cmajor::ir::Emitter& emitter)
{
    void* localIrType = emitter.GetFunctionIrType(this);
    if (!localIrType)
    {
        void* retType = emitter.GetIrTypeForVoid();
        if (returnType && returnType->GetSymbolType() != SymbolType::voidTypeSymbol && !ReturnsClassInterfaceOrClassDelegateByValue())
        {
            retType = returnType->IrType(emitter);
        }
        bool interfaceMemFun = Parent()->GetSymbolType() == SymbolType::interfaceTypeSymbol;
        std::vector<void*> paramTypes;
        int np = parameters.size();
        for (int i = 0; i < np; ++i)
        {
            if (i == 0 && interfaceMemFun)
            {
                paramTypes.push_back(emitter.GetIrTypeForVoidPtrType());
            }
            else
            {
                ParameterSymbol* parameter = parameters[i];
                TypeSymbol* paramType = parameter->GetType();
                if (paramType->IsClassTypeSymbol() || paramType->GetSymbolType() == SymbolType::classDelegateTypeSymbol || paramType->GetSymbolType() == SymbolType::interfaceTypeSymbol)
                {
                    paramType = paramType->AddConst(GetSourcePos(), SourceModuleId())->AddLvalueReference(GetSourcePos(), SourceModuleId());
                }
                paramTypes.push_back(paramType->IrType(emitter));
            }
        }
        if (returnParam)
        {
            paramTypes.push_back(returnParam->GetType()->IrType(emitter));
        }
        localIrType = emitter.GetIrTypeForFunction(retType, paramTypes);
        emitter.SetFunctionIrType(this, localIrType);
    }
    return localIrType;
}

std::unique_ptr<soul::xml::Element> FunctionSymbol::CreateDomElement(TypeMap& typeMap)
{
    if (IsTemplateSpecialization()) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("FunctionSymbol"));
    if (returnType)
    {
        std::unique_ptr<soul::xml::Element> returnTypeElement(soul::xml::MakeElement("returnType"));
        int typeId = typeMap.GetOrInsertType(returnType);
        returnTypeElement->SetAttribute("ref", "type_" + std::to_string(typeId));
        element->AppendChild(returnTypeElement.release());
    }
    return element;
}

std::u32string FunctionSymbol::Id() const
{
    if (IsFunctionTemplate())
    {
        return ContainerSymbol::Id();
    }
    else if (IsTemplateSpecialization())
    {
        if (Parent()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            Symbol* parent = const_cast<Symbol*>(Parent());
            ClassTemplateSpecializationSymbol* parentSpecialization = static_cast<ClassTemplateSpecializationSymbol*>(parent);
            if (parentSpecialization->IsPrototype())
            {
                return ContainerSymbol::Id();
            }
            else
            {
                ClassTypeSymbol* classTemplate = parentSpecialization->GetClassTemplate();
                ClassTemplateSpecializationSymbol* prototype = classTemplate->Prototype();
                if (prototype)
                {
                    FunctionSymbol* functionSymbol = prototype->GetFunctionByIndex(index);
                    if (functionSymbol)
                    {
                        return functionSymbol->Id();
                    }
                    else
                    {
                        throw Exception("function symbol " + std::to_string(index) + " not found", GetSourcePos(), SourceModuleId());
                    }
                }
                else
                {
                    throw Exception("prototype not found", GetSourcePos(), SourceModuleId());
                }
            }
        }
        else if (functionTemplate)
        {
            return functionTemplate->Id();
        }
        else
        {
            throw Exception("function template expected", GetSourcePos(), SourceModuleId());
        }
    }
    else
    {
        return ContainerSymbol::Id();
    }
}

void FunctionSymbol::SetTemplateArgumentTypes(const std::vector<TypeSymbol*>& templateArgumentTypes_)
{
    templateArgumentTypes = templateArgumentTypes_;
}

void FunctionSymbol::Check()
{
    ContainerSymbol::Check();
    if (functionId.is_nil())
    {
        throw SymbolCheckException("function symbol has no id", GetSourcePos(), SourceModuleId());
    }
    if (groupName.empty())
    {
        throw SymbolCheckException("function symbol has empty group name", GetSourcePos(), SourceModuleId());
    }
}

void FunctionSymbol::SetPrevUnwindInfoVar(LocalVariableSymbol* prevUnwindInfoVar_)
{
    prevUnwindInfoVar.reset(prevUnwindInfoVar_);
}

void FunctionSymbol::SetUnwindInfoVar(LocalVariableSymbol* unwindInfoVar_)
{
    unwindInfoVar.reset(unwindInfoVar_);
}

int FunctionSymbol::NextTemporaryIndex()
{
    return nextTemporaryIndex++;
}

void FunctionSymbol::CopyFrom(const Symbol* that)
{
    ContainerSymbol::CopyFrom(that);
    const FunctionSymbol* thatFunctionSymbol = static_cast<const FunctionSymbol*>(that);
    functionTemplate = thatFunctionSymbol->functionTemplate;
    master = thatFunctionSymbol->master;
    functionId = thatFunctionSymbol->functionId;
    groupName = thatFunctionSymbol->groupName;
    templateParameters = thatFunctionSymbol->templateParameters;
    templateArgumentTypes = thatFunctionSymbol->templateArgumentTypes;
    parameters = thatFunctionSymbol->parameters;
    if (thatFunctionSymbol->returnParam)
    {
        SetReturnParam(thatFunctionSymbol->returnParam->Clone());
    }
    localVariables = thatFunctionSymbol->localVariables;
    returnType = thatFunctionSymbol->returnType;
    flags = thatFunctionSymbol->flags;
    index = thatFunctionSymbol->index;
    vmtIndex = thatFunctionSymbol->vmtIndex;
    imtIndex = thatFunctionSymbol->imtIndex;
    functionGroup = thatFunctionSymbol->functionGroup;
    isProgramMain = thatFunctionSymbol->isProgramMain;
    conversionSourceType = thatFunctionSymbol->conversionSourceType;
    conversionTargetType = thatFunctionSymbol->conversionTargetType;
}

FunctionSymbol* FunctionSymbol::Copy() const
{
    FunctionSymbol* copy = new FunctionSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::unique_ptr<Symbol> FunctionSymbol::RemoveFromParent()
{
    std::unique_ptr<Symbol> symbol = ContainerSymbol::RemoveFromParent();
    if (functionGroup)
    {
        functionGroup->RemoveFunction(this);
        if (functionGroup->IsEmpty())
        {
            std::unique_ptr<Symbol> functionGroupSymbol = functionGroup->RemoveFromParent();
        }
    }
    return symbol;
}

std::string FunctionSymbol::GetSymbolHelp() const
{
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    if (returnType)
    {
        help.append(util::ToUtf8(returnType->FullName())).append(" ");
    }
    help.append(util::ToUtf8(FullName(true)));
    return help;
}

StaticConstructorSymbol::StaticConstructorSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::staticConstructorSymbol, sourcePos_, sourceModuleId_, name_)
{
    SetGroupName(U"@static_constructor");
}

FunctionSymbol* StaticConstructorSymbol::Copy() const
{
    StaticConstructorSymbol* copy = new StaticConstructorSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::u32string StaticConstructorSymbol::FullNameWithSpecifiers() const
{
    std::u32string fullNameWithSpecifiers = util::ToUtf32(SymbolFlagStr(GetStableSymbolFlags(), true));
    std::u32string f = util::ToUtf32(FunctionSymbolFlagStr(GetFunctionSymbolFlags()));
    if (!f.empty())
    {
        if (!fullNameWithSpecifiers.empty())
        {
            fullNameWithSpecifiers.append(1, U' ');
        }
        fullNameWithSpecifiers.append(f);
    }
    if (!fullNameWithSpecifiers.empty())
    {
        fullNameWithSpecifiers.append(1, U' ');
    }
    fullNameWithSpecifiers.append(FullName());
    return fullNameWithSpecifiers;
}

std::u32string StaticConstructorSymbol::CodeName() const
{
    return Parent()->CodeName();
}

void StaticConstructorSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    SetStatic();
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("static constructor cannot be throw and nothrow at the same time", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

ConstructorSymbol::ConstructorSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::constructorSymbol, sourcePos_, sourceModuleId_, name_)
{
    SetGroupName(U"@constructor");
}

FunctionSymbol* ConstructorSymbol::Copy() const
{
    ConstructorSymbol* copy = new ConstructorSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::string ConstructorSymbol::TypeString() const
{
    if (IsDefaultConstructor())
    {
        return "default_constructor";
    }
    else if (IsCopyConstructor())
    {
        return "copy_constructor";
    }
    else if (IsMoveConstructor())
    {
        return "move_constructor";
    }
    else
    {
        return "constructor";
    }
}

std::u32string ConstructorSymbol::DocName() const
{
    std::u32string docName;
    docName.append(Parent()->DocName());
    docName.append(1, '(');
    int n = Parameters().size();
    for (int i = 1; i < n; ++i)
    {
        if (i > 1)
        {
            docName.append(U", ");
        }
        ParameterSymbol* parameter = Parameters()[i];
        if (parameter->GetType()->Ns() == Ns())
        {
            docName.append(parameter->GetType()->Name());
        }
        else
        {
            docName.append(parameter->GetType()->FullName());
        }
        docName.append(1, ' ').append(parameter->Name());
    }
    docName.append(1, ')');
    return docName;
}

std::u32string ConstructorSymbol::CodeName() const
{
    return Parent()->CodeName();
}

uint8_t ConstructorSymbol::ConversionDistance() const
{
    return 5;
}

void ConstructorSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("ordinary constructor cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        SetExplicit();
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        if (IsInline())
        {
            throw Exception("suppressed member function cannot be inline", GetSourcePos(), SourceModuleId());
        }
        SetSuppressed();
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        if (IsSuppressed())
        {
            throw Exception("constructor cannot be default and suppressed at the same time", GetSourcePos(), SourceModuleId());
        }
        if (IsInline())
        {
            throw Exception("default member function cannot be inline", GetSourcePos(), SourceModuleId());
        }
        SetDefault();
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("constructor cannot be throw and nothrow at the same time", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

DestructorSymbol::DestructorSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::destructorSymbol, sourcePos_, sourceModuleId_, name_), generated(false)
{
    SetGroupName(U"@destructor");
}

FunctionSymbol* DestructorSymbol::Copy() const
{
    DestructorSymbol* copy = new DestructorSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    copy->generated = generated;
    return copy;
}

bool DestructorSymbol::IsExportSymbol() const
{
    if (Parent()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
    {
        const ClassTemplateSpecializationSymbol* classTemplateSpecialization = static_cast<const ClassTemplateSpecializationSymbol*>(Parent());
        if (classTemplateSpecialization->IsPrototype())
        {
            return false;
        }
    }
    return true;
}

void DestructorSymbol::Write(SymbolWriter& writer)
{
    FunctionSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(generated);
}

void DestructorSymbol::Read(SymbolReader& reader)
{
    FunctionSymbol::Read(reader);
    generated = reader.GetBinaryStreamReader().ReadBool();
}

void DestructorSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    if (accessSpecifiers != cmajor::ast::Specifiers::public_)
    {
        throw Exception("destructor must be public", GetSourcePos(), SourceModuleId());
    }
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        SetVirtual();
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtual())
        {
            throw Exception("destructor cannot be virtual and override at the same time", GetSourcePos(), SourceModuleId());
        }
        SetOverride();
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        SetDefault();
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor is implicitly nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

std::u32string DestructorSymbol::CodeName() const
{
    return U"~" + Parent()->CodeName();
}

bool DestructorSymbol::DontThrow() const
{
    if (GetBackEnd() == BackEnd::cpp)
    {
        return !HasCleanup();
    }
    return true;
}

MemberFunctionSymbol::MemberFunctionSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::memberFunctionSymbol, sourcePos_, sourceModuleId_, name_)
{
}

FunctionSymbol* MemberFunctionSymbol::Copy() const
{
    MemberFunctionSymbol* copy = new MemberFunctionSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::string MemberFunctionSymbol::TypeString() const
{
    if (IsCopyAssignment())
    {
        return "copy_assignment";
    }
    else if (IsMoveAssignment())
    {
        return "move_assignment";
    }
    else
    {
        return "member_function";
    }
}

std::u32string MemberFunctionSymbol::DocName() const
{
    std::u32string docName;
    docName.append(GroupName());
    docName.append(1, '(');
    int n = Parameters().size();
    int start = 1;
    if (IsStatic())
    {
        start = 0;
    }
    for (int i = start; i < n; ++i)
    {
        if (i > start)
        {
            docName.append(U", ");
        }
        ParameterSymbol* parameter = Parameters()[i];
        if (parameter->GetType()->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            ClassTemplateSpecializationSymbol* classTemplateSpecializationSymbol = static_cast<ClassTemplateSpecializationSymbol*>(parameter->GetType());
            if (classTemplateSpecializationSymbol->GetClassTemplate()->Ns() == Ns())
            {
                docName.append(classTemplateSpecializationSymbol->Name());
            }
            else
            {
                docName.append(classTemplateSpecializationSymbol->FullName());
            }
        }
        else
        {
            if (parameter->GetType()->Ns() == Ns())
            {
                docName.append(parameter->GetType()->Name());
            }
            else
            {
                docName.append(parameter->GetType()->FullName());
            }
        }
        docName.append(1, ' ').append(parameter->Name());
    }
    docName.append(1, ')');
    if (IsConst())
    {
        docName.append(U" const");
    }
    return docName;
}

void MemberFunctionSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        SetStatic();
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        SetVirtual();
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtual())
        {
            throw Exception("member function cannot be virtual and override at the same time", GetSourcePos(), SourceModuleId());
        }
        SetOverride();
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtual() || IsOverride())
        {
            throw Exception("member function cannot be abstract and virtual or override at the same time", GetSourcePos(), SourceModuleId());
        }
        SetAbstract();
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        if (IsInline())
        {
            throw Exception("suppressed member function cannot be inline", GetSourcePos(), SourceModuleId());
        }
        if (GroupName() == U"operator=")
        {
            SetSuppressed();
        }
        else
        {
            throw Exception("only special member functions can be suppressed", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        if (IsSuppressed())
        {
            throw Exception("member function cannot be default and suppressed at the same time", GetSourcePos(), SourceModuleId());
        }
        if (IsInline())
        {
            throw Exception("default member function cannot be inline", GetSourcePos(), SourceModuleId());
        }
        if (GroupName() == U"operator=")
        {
            SetDefault();
        }
        else
        {
            throw Exception("only special member functions can be default", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("member function cannot be throw and nothrow at the same time", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtualAbstractOrOverride())
        {
            throw Exception("member function cannot be new and virtual, abstract or overridden at the same time", GetSourcePos(), SourceModuleId());
        }
        SetNew();
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        SetConst();
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

int MemberFunctionSymbol::StartParamIndex() const
{
    if (IsStatic())
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

ConversionFunctionSymbol::ConversionFunctionSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::conversionFunctionSymbol, sourcePos_, sourceModuleId_, name_)
{
    SetConversion();
}

FunctionSymbol* ConversionFunctionSymbol::Copy() const
{
    ConversionFunctionSymbol* copy = new ConversionFunctionSymbol(GetSourcePos(), SourceModuleId(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::u32string ConversionFunctionSymbol::DocName() const
{
    std::u32string docName;
    docName.append(U"operator ");
    TypeSymbol* type = ReturnType();
    if (type->Ns() == Ns())
    {
        docName.append(type->Name());
    }
    else
    {
        docName.append(type->FullName());
    }
    docName.append(U"()");
    if (IsConst())
    {
        docName.append(U" const");
    }
    return docName;
}

void ConversionFunctionSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("conversion function cannot be throw and nothrow at the same time", GetSourcePos(), SourceModuleId());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        SetConst();
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

std::unique_ptr<soul::xml::Element> ConversionFunctionSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("ConversionFunctionSymbol"));
    if (ReturnType())
    {
        std::unique_ptr<soul::xml::Element> returnTypeElement(soul::xml::MakeElement("returnType"));
        int typeId = typeMap.GetOrInsertType(ReturnType());
        returnTypeElement->SetAttribute("ref", "type_" + std::to_string(typeId));
        element->AppendChild(returnTypeElement.release());
    }
    return element;
}

FunctionGroupTypeSymbol::FunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup_, void* boundFunctionGroup_) :
    TypeSymbol(SymbolType::functionGroupTypeSymbol, functionGroup_->GetSourcePos(), functionGroup_->SourceModuleId(), functionGroup_->Name()), functionGroup(functionGroup_), boundFunctionGroup(boundFunctionGroup_)
{
    SetModule(functionGroup->GetModule());
}

MemberExpressionTypeSymbol::MemberExpressionTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_, void* boundMemberExpression_) :
    TypeSymbol(SymbolType::memberExpressionTypeSymbol, sourcePos_, sourceModuleId_, name_), boundMemberExpression(boundMemberExpression_)
{
}

} // namespace cmajor::symbols
