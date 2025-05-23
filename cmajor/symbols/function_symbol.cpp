// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.function.symbol;

import cmajor.symbols.context;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.global.flags;
import cmajor.symbols.exception;
import cmajor.symbols.modules;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.templates;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.classes;
import cmajor.symbols.value;
import cmajor.ast.parameter;
import soul.ast.span;
import cmajor.ast.node;
import cmajor.ast.function;
import cmajor.ast.specifier;
import cmajor.ast.clone;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

std::mutex instantiationMutex;

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

FunctionGroupSymbol::FunctionGroupSymbol(const soul::ast::Span& span_, const std::u32string& name_) : Symbol(SymbolType::functionGroupSymbol, span_, name_)
{
}

void FunctionGroupSymbol::ComputeMangledName(Context* context)
{
    std::u32string mangledName = util::ToUtf32(TypeString(context));
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
                throw SymbolCheckException("function group symbol contains empty function pointer", GetFullSpan());
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
                    throw Exception("function with identical name '" + util::ToUtf8(functionSymbol->FullName()) + "' already defined.", GetFullSpan(), 
                        functionSymbol->GetFullSpan());
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

FunctionSymbol::FunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    ContainerSymbol(SymbolType::functionSymbol, span_, name_), functionTemplate(nullptr), master(nullptr),
    functionId(util::nil_uuid()), groupName(), parameters(), localVariables(),
    returnType(), flags(FunctionSymbolFlags::none), index(-1), vmtIndex(-1), imtIndex(-1),
    nextTemporaryIndex(0), functionGroup(nullptr), isProgramMain(false), isUserMain(false), unwindInfoVar(nullptr),
    conversionSourceType(nullptr), conversionTargetType(nullptr), traceEntryVar(nullptr), traceGuardVar(nullptr), 
    checkerVar(nullptr)
{
}

FunctionSymbol::FunctionSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_) :
    ContainerSymbol(symbolType_, span_, name_), functionTemplate(nullptr), master(nullptr),
    functionId(util::nil_uuid()), groupName(), parameters(), localVariables(),
    returnType(), flags(FunctionSymbolFlags::none), index(-1), vmtIndex(-1), imtIndex(-1),
    nextTemporaryIndex(0), functionGroup(nullptr), isProgramMain(false), isUserMain(false), unwindInfoVar(nullptr),
    conversionSourceType(nullptr), conversionTargetType(nullptr), traceEntryVar(nullptr), traceGuardVar(nullptr),
    checkerVar(nullptr)
{
}

void FunctionSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    Context* context = writer.GetContext();
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
        cmajor::ast::Node* node = context->RootModule()->GetSymbolTable().GetNode(this);
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
    writer.GetBinaryStreamWriter().Write(instantiatedName);
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
    instantiatedName = reader.GetBinaryStreamReader().ReadUtf32String();
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
            throw Exception("invalid emplace template argument index '" + std::to_string(index) + "'", GetFullSpan());
        }
        templateArgumentTypes[templateArgumentIndex] = typeSymbol;
    }
    else
    {
        throw Exception("invalid emplace type index '" + std::to_string(index) + "'", GetFullSpan());
    }
}

void FunctionSymbol::AddMember(Symbol* member, Context* context)
{
    ContainerSymbol::AddMember(member, context);
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

void FunctionSymbol::ComputeName(Context* context)
{
    std::u32string name;
    name.append(groupName);
    if (!templateArgumentTypes.empty())
    {
        name.append(1, '<');
        bool first = true;
        for (TypeSymbol* type : templateArgumentTypes)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                name.append(U", ");
            }
            name.append(type->FullName());
        }
        name.append(1, '>');
    }
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
            name.append(parameter->GetType()->RemovePointer(context)->FullName());
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
        ComputeMangledName(context);
    }
}

void FunctionSymbol::ComputeMangledName(Context* context)
{
    if (IsCDecl())
    {
        SetMangledName(GroupName());
        return;
    }
    std::u32string mangledName = util::ToUtf32(TypeString(context));
    if (groupName.find(U"operator") != std::u32string::npos)
    {
        mangledName.append(1, U'_').append(OperatorMangleMap::Instance().Mangle(GroupName()));
    }
    else if (groupName.find(U'@') == std::u32string::npos)
    {
        mangledName.append(1, U'_').append(GroupName());
    }
    SymbolType symbolType = GetSymbolType();
    std::string parentClassStr;
    switch (symbolType)
    {
    case SymbolType::staticConstructorSymbol: case SymbolType::constructorSymbol: case SymbolType::destructorSymbol: case SymbolType::memberFunctionSymbol:
    {
        Symbol* parentClass = Parent();
        mangledName.append(1, U'_').append(parentClass->SimpleName());
        parentClassStr.append(1, '.').append(util::ToUtf8(parentClass->MangledName()));
        break;
    }
    }
    std::string templateArgumentStr;
    for (const auto& templateArgumentType : TemplateArgumentTypes())
    {
        templateArgumentStr.append(".").append(
            util::ToUtf8(templateArgumentType->BaseType()->FullName()).append(".").append(util::ToUtf8(templateArgumentType->FullName())));
    }
    std::string constraintStr;
    if (constraint)
    {
        constraintStr = constraint->ToString();
    }
    digestSource = util::ToUtf8(FullNameWithSpecifiers()) + "." + parentClassStr + "." + templateArgumentStr + "." + compileUnitId + "." + constraintStr;
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(digestSource)));
    SetMangledName(mangledName);
}

const std::u32string& FunctionSymbol::InstantiatedName() const
{
    std::lock_guard<std::mutex> lock(instantiationMutex);
    if (!instantiatedName.empty()) return instantiatedName;
    return MangledName();
}

void FunctionSymbol::SetInstantiatedName(const std::u32string& instantiatedName_)
{
    std::lock_guard<std::mutex> lock(instantiationMutex);
    if (!instantiatedName.empty()) return;
    instantiatedName = instantiatedName_;
}

std::u32string FunctionSymbol::FullName(bool withParamNames) const
{
    std::u32string fullName;
    if (!Parent())
    {
        throw Exception("function symbol has no parent", GetFullSpan());
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
    if (!templateArgumentTypes.empty())
    {
        fullName.append(1, '<');
        bool first = true;
        for (TypeSymbol* type : templateArgumentTypes)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                fullName.append(U", ");
            }
            fullName.append(type->FullName());
        }
        fullName.append(1, '>');
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
        if (!parameter->GetType())
        {
            throw Exception("function symbol parameter has no type", GetFullSpan());
        }
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

std::u32string FunctionSymbol::DocName(Context* context) const
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
            docName.append(templateParameter->DocName(context));
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
        if (parameter->GetType()->Ns(context) == Ns(context))
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

std::string FunctionSymbol::GetSpecifierStr() 
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

std::string FunctionSymbol::Syntax(Context* context)
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    if (ReturnType())
    {
        syntax.append(util::ToUtf8(ReturnType()->DocName(context)));
        syntax.append(1, ' ');
    }
    syntax.append(util::ToUtf8(DocName(context)));
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

void FunctionSymbol::GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
{
    if ((flags & cmajor::ir::OperationFlags::virtualCall) != cmajor::ir::OperationFlags::none)
    {
        GenerateVirtualCall(emitter, genObjects, flags, context);
        return;
    }
    int na = genObjects.size();
    for (int i = 0; i < na; ++i)
    {
        cmajor::ir::GenObject* genObject = genObjects[i];
        genObject->Load(emitter, flags & cmajor::ir::OperationFlags::functionCallFlags);
    }
    void* functionType = IrType(emitter, context);
    // MangledName changed to InstantiatedName:
    void* callee = emitter.GetOrInsertFunction(util::ToUtf8(InstantiatedName()), functionType, DontThrow());
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
                void* functionType = IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateCall(functionType, callee, args));
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen));
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
                void* functionType = IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args));
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen));
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
                void* functionType = IrType(emitter, context);
                emitter.CreateCall(functionType, callee, args);
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen);
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
                void* functionType = IrType(emitter, context);
                emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args);
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen);
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
}

void FunctionSymbol::GenerateVirtualCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context)
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
                thisPtr = emitter.CreateBitCast(thisPtr, vmtPtrHolderClass->AddPointer(context)->IrType(emitter, context));
            }
            void* vmtPtr = emitter.GetVmtPtr(vmtPtrHolderClass->IrType(emitter, context), thisPtr, vmtPtrHolderClass->VmtPtrIndex(), classType->VmtPtrType(emitter));
            void* vmtType = vmtPtrHolderClass->VmtArrayType(emitter);
            void* methodPtr = emitter.GetMethodPtr(vmtType, vmtPtr, VmtIndex() + GetFunctionVmtIndexOffset());
            callee = emitter.CreateBitCast(methodPtr, emitter.GetIrTypeForPtrType(IrType(emitter, context)));
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
                void* functionType = IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateCall(functionType, callee, args));
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen));
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
                void* functionType = IrType(emitter, context);
                emitter.Stack().Push(emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args));
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.Stack().Push(emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen));
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
                void* functionType = IrType(emitter, context);
                emitter.CreateCall(functionType, callee, args);
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateCallInst(functionType, callee, args, bundles, lineColLen);
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
                void* functionType = IrType(emitter, context);
                emitter.CreateInvoke(functionType, callee, nextBlock, unwindBlock, args);
            }
            else
            {
                void* functionType = IrType(emitter, context);
                soul::ast::FullSpan fullSpan = GetFullSpan();
                soul::ast::LineColLen lineColLen = GetLineColLen(fullSpan);
                emitter.CreateInvokeInst(functionType, callee, nextBlock, unwindBlock, args, bundles, lineColLen);
            }
            if (GetBackEnd() == BackEnd::llvm || GetBackEnd() == BackEnd::cpp)
            {
                emitter.SetCurrentBasicBlock(nextBlock);
            }
        }
    }
}

std::unique_ptr<Value> FunctionSymbol::ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::Span& span, Value* receiver) const
{
    return std::unique_ptr<Value>();
}

std::unique_ptr<Value> FunctionSymbol::ConvertValue(const std::unique_ptr<Value>& value, Context* context) const
{
    return std::unique_ptr<Value>();
}

void FunctionSymbol::Dump(util::CodeFormatter& formatter, Context* context)
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

bool FunctionSymbol::IsDefaultConstructor(Context* context) const
{
    return parameters.size() == 1 && groupName == U"@constructor" && parameters[0]->GetType()->PointerCount() == 1 && 
        parameters[0]->GetType()->RemovePointer(context)->IsClassTypeSymbol();
}

bool FunctionSymbol::IsCopyConstructor(Context* context) const
{
    return parameters.size() == 2 && groupName == U"@constructor" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(context)->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddConst(context)->AddLvalueReference(context), parameters[1]->GetType());
}

bool FunctionSymbol::IsMoveConstructor(Context* context) const
{
    return parameters.size() == 2 && groupName == U"@constructor" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(context)->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddRvalueReference(context), parameters[1]->GetType());
}

bool FunctionSymbol::IsCopyAssignment(Context* context) const
{
    return parameters.size() == 2 && groupName == U"operator=" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(context)->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddConst(context)->AddLvalueReference(context), parameters[1]->GetType());
}

bool FunctionSymbol::IsMoveAssignment(Context* context) const
{
    return parameters.size() == 2 && groupName == U"operator=" &&
        parameters[0]->GetType()->PointerCount() == 1 &&
        parameters[0]->GetType()->RemovePointer(context)->IsClassTypeSymbol() &&
        TypesEqual(parameters[0]->GetType()->BaseType()->AddRvalueReference(context), parameters[1]->GetType());
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
        throw Exception("only member functions can be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only constructors can be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        SetExternal();
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only special member functions can be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only special member functions can be default", GetFullSpan());
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
            throw Exception("function symbol cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("only member functions can be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        if (!GetGlobalFlag(GlobalFlags::unitTest))
        {
            throw Exception("function symbol cannot be unit_test", GetFullSpan());
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

LocalVariableSymbol* FunctionSymbol::CreateTemporary(TypeSymbol* type, const soul::ast::Span& span, Context* context, bool add)
{
    LocalVariableSymbol* temporary = new LocalVariableSymbol(span, U"@t" + util::ToUtf32(std::to_string(nextTemporaryIndex++)));
    temporary->SetTemporary();
    temporary->SetType(type);
    if (add)
    {
        AddMember(temporary, context);
        AddLocalVariable(temporary);
    }
    return temporary;
}

std::vector<LocalVariableSymbol*> FunctionSymbol::CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context, bool add)
{
    return std::vector<LocalVariableSymbol*>();
}

void* FunctionSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localIrType = emitter.GetFunctionIrType(this);
    if (!localIrType)
    {
        void* retType = emitter.GetIrTypeForVoid();
        if (returnType && returnType->GetSymbolType() != SymbolType::voidTypeSymbol && !ReturnsClassInterfaceOrClassDelegateByValue())
        {
            retType = returnType->IrType(emitter, context);
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
                    paramType = paramType->AddConst(context)->AddLvalueReference(context);
                }
                paramTypes.push_back(paramType->IrType(emitter, context));
            }
        }
        if (returnParam)
        {
            paramTypes.push_back(returnParam->GetType()->IrType(emitter, context));
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
                        throw Exception("function symbol " + std::to_string(index) + " not found", GetFullSpan());
                    }
                }
                else
                {
                    throw Exception("prototype not found", GetFullSpan());
                }
            }
        }
        else if (functionTemplate)
        {
            return functionTemplate->Id();
        }
        else
        {
            throw Exception("function template expected", GetFullSpan());
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
        throw SymbolCheckException("function symbol has no id", GetFullSpan());
    }
    if (groupName.empty())
    {
        throw SymbolCheckException("function symbol has empty group name", GetFullSpan());
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

void FunctionSymbol::SetTraceEntryVar(LocalVariableSymbol* traceEntryVar_)
{
    traceEntryVar = traceEntryVar_;
}

void FunctionSymbol::SetTraceGuardVar(LocalVariableSymbol* traceGuardVar_)
{
    traceGuardVar = traceGuardVar_;
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
    FunctionSymbol* copy = new FunctionSymbol(GetSpan(), Name());
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

void FunctionSymbol::SetCompileUnitId(const std::string& compileUnitId_)
{
    compileUnitId = compileUnitId_;
}

StaticConstructorSymbol::StaticConstructorSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::staticConstructorSymbol, span_, name_)
{
    SetGroupName(U"@static_constructor");
}

FunctionSymbol* StaticConstructorSymbol::Copy() const
{
    StaticConstructorSymbol* copy = new StaticConstructorSymbol(GetSpan(), Name());
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
        throw Exception("static constructor cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("static constructor cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("static constructor cannot be unit_test", GetFullSpan());
    }
}

ConstructorSymbol::ConstructorSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::constructorSymbol, span_, name_)
{
    SetGroupName(U"@constructor");
}

FunctionSymbol* ConstructorSymbol::Copy() const
{
    ConstructorSymbol* copy = new ConstructorSymbol(GetSpan(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::string ConstructorSymbol::TypeString(Context* context) const
{
    if (IsDefaultConstructor(context))
    {
        return "default_constructor";
    }
    else if (IsCopyConstructor(context))
    {
        return "copy_constructor";
    }
    else if (IsMoveConstructor(context))
    {
        return "move_constructor";
    }
    else
    {
        return "constructor";
    }
}

std::u32string ConstructorSymbol::DocName(Context* context) const
{
    std::u32string docName;
    docName.append(Parent()->DocName(context));
    docName.append(1, '(');
    int n = Parameters().size();
    for (int i = 1; i < n; ++i)
    {
        if (i > 1)
        {
            docName.append(U", ");
        }
        ParameterSymbol* parameter = Parameters()[i];
        if (parameter->GetType()->Ns(context) == Ns(context))
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
        throw Exception("ordinary constructor cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be abstract", GetFullSpan());
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
        throw Exception("constructor cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        if (IsInline())
        {
            throw Exception("suppressed member function cannot be inline", GetFullSpan());
        }
        SetSuppressed();
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        if (IsSuppressed())
        {
            throw Exception("constructor cannot be default and suppressed at the same time", GetFullSpan());
        }
        if (IsInline())
        {
            throw Exception("default member function cannot be inline", GetFullSpan());
        }
        SetDefault();
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("constructor cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("constructor cannot be unit_test", GetFullSpan());
    }
}

DestructorSymbol::DestructorSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::destructorSymbol, span_, name_), generated(false)
{
    SetGroupName(U"@destructor");
}

FunctionSymbol* DestructorSymbol::Copy() const
{
    DestructorSymbol* copy = new DestructorSymbol(GetSpan(), Name());
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
        throw Exception("destructor must be public", GetFullSpan());
    }
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        SetVirtual();
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtual())
        {
            throw Exception("destructor cannot be virtual and override at the same time", GetFullSpan());
        }
        SetOverride();
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        SetDefault();
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor is implicitly nothrow", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be throw", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("destructor cannot be unit_test", GetFullSpan());
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

MemberFunctionSymbol::MemberFunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::memberFunctionSymbol, span_, name_)
{
}

FunctionSymbol* MemberFunctionSymbol::Copy() const
{
    MemberFunctionSymbol* copy = new MemberFunctionSymbol(GetSpan(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::string MemberFunctionSymbol::TypeString(Context* context) const
{
    if (IsCopyAssignment(context))
    {
        return "copy_assignment";
    }
    else if (IsMoveAssignment(context))
    {
        return "move_assignment";
    }
    else
    {
        return "member_function";
    }
}

std::u32string MemberFunctionSymbol::DocName(Context* context) const
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
            if (classTemplateSpecializationSymbol->GetClassTemplate()->Ns(context) == Ns(context))
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
            if (parameter->GetType()->Ns(context) == Ns(context))
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
            throw Exception("member function cannot be virtual and override at the same time", GetFullSpan());
        }
        SetOverride();
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtual() || IsOverride())
        {
            throw Exception("member function cannot be abstract and virtual or override at the same time", GetFullSpan());
        }
        SetAbstract();
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        if (IsInline())
        {
            throw Exception("suppressed member function cannot be inline", GetFullSpan());
        }
        if (GroupName() == U"operator=")
        {
            SetSuppressed();
        }
        else
        {
            throw Exception("only special member functions can be suppressed", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        if (IsSuppressed())
        {
            throw Exception("member function cannot be default and suppressed at the same time", GetFullSpan());
        }
        if (IsInline())
        {
            throw Exception("default member function cannot be inline", GetFullSpan());
        }
        if (GroupName() == U"operator=")
        {
            SetDefault();
        }
        else
        {
            throw Exception("only special member functions can be default", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("member function cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        if (IsVirtualAbstractOrOverride())
        {
            throw Exception("member function cannot be new and virtual, abstract or overridden at the same time", GetFullSpan());
        }
        SetNew();
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        SetConst();
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member function cannot be unit_test", GetFullSpan());
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

ConversionFunctionSymbol::ConversionFunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    FunctionSymbol(SymbolType::conversionFunctionSymbol, span_, name_)
{
    SetConversion();
}

FunctionSymbol* ConversionFunctionSymbol::Copy() const
{
    ConversionFunctionSymbol* copy = new ConversionFunctionSymbol(GetSpan(), Name());
    copy->CopyFrom(this);
    return copy;
}

std::u32string ConversionFunctionSymbol::DocName(Context* context) const
{
    std::u32string docName;
    docName.append(U"operator ");
    TypeSymbol* type = ReturnType();
    if (type->Ns(context) == Ns(context))
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
        throw Exception("conversion function cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        SetInline();
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        SetConstExpr();
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        SetNothrow();
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        if (IsNothrow())
        {
            throw Exception("conversion function cannot be throw and nothrow at the same time", GetFullSpan());
        }
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        SetConst();
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("conversion function cannot be unit_test", GetFullSpan());
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

FunctionGroupTypeSymbol::FunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup_, void* boundFunctionGroup_, 
    const soul::ast::Span& span_, int fileIndex_, const util::uuid& moduleId_) :
    TypeSymbol(SymbolType::functionGroupTypeSymbol, span_, functionGroup_->Name()), functionGroup(functionGroup_), boundFunctionGroup(boundFunctionGroup_),
    fileIndex(fileIndex_), moduleId(moduleId_)
{
    SetModule(functionGroup->GetModule());
}

MemberExpressionTypeSymbol::MemberExpressionTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_, void* boundMemberExpression_) :
    TypeSymbol(SymbolType::memberExpressionTypeSymbol, span_, name_), boundMemberExpression(boundMemberExpression_)
{
}

} // namespace cmajor::symbols
