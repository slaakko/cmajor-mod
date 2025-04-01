// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.function.symbol;

import cmajor.ast.function;
import cmajor.ast.concepts;
import cmajor.ast.specifier;
import cmajor.ast.node;
import cmajor.ast.node.list;
import cmajor.ast.statement;
import cmajor.ast.templates;
import cmajor.ast.parameter;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.type.symbol;
import cmajor.symbols.symbol;
import cmajor.symbols.container.symbol;
import cmajor.symbols.meta;
import soul.ast.span;
import soul.xml.element;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import util.code.formatter;
import std.core;


export namespace cmajor::symbols {

using namespace cmajor::ir;
class Value;
class FunctionSymbol;

struct FunctionSymbolsEqual
{
    bool operator()(FunctionSymbol* left, FunctionSymbol* right) const;
};

struct FunctionSymbolHash
{
    size_t operator()(FunctionSymbol* fun) const;
};

class ViableFunctionSet
{
public:
    const std::unordered_set<FunctionSymbol*, FunctionSymbolHash, FunctionSymbolsEqual>& Get() const { return set; }
    void Insert(FunctionSymbol* fun);
private:
    std::unordered_set<FunctionSymbol*, FunctionSymbolHash, FunctionSymbolsEqual> set;
};

using AccessCheckFunction = bool(*)(FunctionSymbol* fromFunction, Symbol* toSymbol);

void SetAccessCheckFunction(AccessCheckFunction accessCheckFunc);
AccessCheckFunction GetAccessCheckFunction();

class FunctionGroupSymbol : public Symbol
{
public:
    FunctionGroupSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    bool IsExportSymbol() const override { return false; }
    std::string TypeString(Context* context) const override { return "function_group"; }
    void ComputeMangledName(Context* context) override;
    void AddFunction(FunctionSymbol* function);
    void RemoveFunction(FunctionSymbol* function);
    bool IsEmpty() const;
    FunctionSymbol* GetFunction();
    void CollectViableFunctions(int arity, ViableFunctionSet& viableFunctions, Module* module);
    bool HasProjectMembers() const override;
    void AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "FunctionGroupSymbol"; }
    void Check() override;
    void CheckDuplicateFunctionSymbols();
    std::string GetSymbolCategoryStr() const override { return "FN"; }
    std::string GetSymbolCategoryDescription() const override { return "function"; }
    std::string GetSymbolHelp() const override;
    bool IsValidCCFunctionGroup(FunctionSymbol* fromFunction) const;
    std::vector<Symbol*> GetParamHelpSymbols() const override;
private:
    std::unordered_map<int, std::vector<FunctionSymbol*>> arityFunctionListMap;
    std::vector<FunctionSymbol*> varArgFunctions;
};

enum class ConversionType : uint8_t
{
    implicit_, explicit_
};

enum class FunctionSymbolFlags : uint32_t
{
    none = 0,
    inline_ = 1 << 0,
    constExpr = 1 << 1,
    cdecl_ = 1 << 2,
    suppress = 1 << 3,
    default_ = 1 << 4,
    explicit_ = 1 << 5,
    virtual_ = 1 << 6,
    override_ = 1 << 7,
    abstract_ = 1 << 8,
    new_ = 1 << 9,
    const_ = 1 << 10,
    conversion = 1 << 11,
    linkOnceOdrLinkage = 1 << 12,
    templateSpecialization = 1 << 13,
    hasTry = 1 << 14,
    hasSource = 1 << 15,
    includeConstraint = 1 << 16,
    copy = 1 << 17,
    varArg = 1 << 18,
    winapi = 1 << 19,
    dontReuse = 1 << 20,
    hasArtificialBody = 1 << 21,
    hasCleanup = 1 << 22,
    systemDefault = 1 << 23,
    immutable = 1 << 24
};

inline FunctionSymbolFlags operator|(FunctionSymbolFlags left, FunctionSymbolFlags right)
{
    return FunctionSymbolFlags(uint32_t(left) | uint32_t(right));
}

inline FunctionSymbolFlags operator&(FunctionSymbolFlags left, FunctionSymbolFlags right)
{
    return FunctionSymbolFlags(uint32_t(left) & uint32_t(right));
}

inline FunctionSymbolFlags operator~(FunctionSymbolFlags flag)
{
    return FunctionSymbolFlags(~uint32_t(flag));
}

std::string FunctionSymbolFlagStr(FunctionSymbolFlags flags);

class ParameterSymbol;
class TemplateParameterSymbol;
class BoundTemplateParameterSymbol;

class FunctionSymbol : public ContainerSymbol
{
public:
    FunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    FunctionSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    FunctionSymbol(const FunctionSymbol&) = delete;
    FunctionSymbol& operator=(const FunctionSymbol&) = delete;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void Accept(SymbolCollector* collector) override;
    const cmajor::ast::NodeList<cmajor::ast::Node>& UsingNodes() const { return usingNodes; }
    cmajor::ast::FunctionNode* GetFunctionNode() { return functionNode.get(); }
    cmajor::ast::ConstraintNode* Constraint() { return constraint.get(); }
    void SetConstraint(cmajor::ast::ConstraintNode* constraint_) { constraint.reset(constraint_); }
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void AddMember(Symbol* member, Context* context) override;
    bool IsFunctionSymbol() const override { return true; }
    std::string TypeString(Context* context) const override { return "function"; }
    bool IsExportSymbol() const override;
    virtual void ComputeName(Context* context);
    const std::u32string& InstantiatedName() const;
    void SetInstantiatedName(const std::u32string& instantiatedName_);
    std::u32string FullName() const override;
    std::u32string FullName(bool withParamNames) const;
    std::u32string FullNameWithSpecifiers() const override;
    std::u32string DocName(Context* context) const override;
    std::u32string CodeName() const override { return groupName; }
    std::string GetSpecifierStr() override;
    std::string Syntax(Context* context) override;
    int32_t GetIndex() const { return index; }
    void SetIndex(int32_t index_) { index = index_; }
    virtual ConversionType GetConversionType() const { return ConversionType::implicit_; }
    virtual uint8_t ConversionDistance() const { return 0; }
    TypeSymbol* ConversionSourceType() const { return conversionSourceType; }
    void SetConversionSourceType(TypeSymbol* conversionSourceType_);
    TypeSymbol* ConversionTargetType() const { return conversionTargetType; }
    void SetConversionTargetType(TypeSymbol* conversionTargetType_);
    virtual bool IsBasicTypeOperation() const { return false; }
    virtual bool IsGeneratedFunction() const { return false; }
    virtual bool IsLvalueReferenceCopyAssignment() const { return false; }
    virtual bool IsArrayElementAccess() const { return false; }
    virtual bool IsCompileTimePrimitiveFunction() const { return false; }
    virtual bool IsClassToInterfaceTypeConversion() const { return false; }
    virtual bool IsMemberFunctionToClassDelegateConversion() const { return false; }
    virtual void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context);
    void GenerateVirtualCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context);
    virtual std::unique_ptr<Value> ConstructValue(const std::vector<std::unique_ptr<Value>>& argumentValues, const soul::ast::Span& span, Value* receiver) const;
    virtual std::unique_ptr<Value> ConvertValue(const std::unique_ptr<Value>& value, Context* context) const;
    virtual ParameterSymbol* GetThisParam() const { return nullptr; }
    virtual bool IsConstructorDestructorOrNonstaticMemberFunction() const { return false; }
    virtual bool IsClassDelegateCopyConstructor() const { return false; }
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    bool IsDefaultConstructor(Context* context) const;
    bool IsCopyConstructor(Context* context) const;
    bool IsMoveConstructor(Context* context) const;
    bool IsCopyAssignment(Context* context) const;
    bool IsMoveAssignment(Context* context) const;
    virtual bool CanInline() const { return true; }
    virtual int StartParamIndex() const { return 0; }
    const util::uuid& FunctionId() const { Assert(!functionId.is_nil(), "function id not initialized"); return functionId; }
    void SetFunctionId(const util::uuid& functionId_) { functionId = functionId_; }
    const std::u32string& GroupName() const { return groupName; }
    void SetGroupName(const std::u32string& groupName_);
    const std::vector<TemplateParameterSymbol*>& TemplateParameters() const { return templateParameters; }
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    bool IsInline() const { return GetFlag(FunctionSymbolFlags::inline_); }
    void SetInline() { SetFlag(FunctionSymbolFlags::inline_); }
    bool IsConstExpr() const { return GetFlag(FunctionSymbolFlags::constExpr); }
    void SetConstExpr() { SetFlag(FunctionSymbolFlags::constExpr); }
    bool IsCDecl() const { return GetFlag(FunctionSymbolFlags::cdecl_); }
    void SetCDecl() { SetFlag(FunctionSymbolFlags::cdecl_); }
    bool IsDefault() const { return GetFlag(FunctionSymbolFlags::default_); }
    void SetDefault() { SetFlag(FunctionSymbolFlags::default_); }
    bool IsSuppressed() const { return GetFlag(FunctionSymbolFlags::suppress); }
    void SetSuppressed() { SetFlag(FunctionSymbolFlags::suppress); }
    bool IsExplicit() const { return GetFlag(FunctionSymbolFlags::explicit_); }
    void SetExplicit() { SetFlag(FunctionSymbolFlags::explicit_); }
    bool IsVirtual() const { return GetFlag(FunctionSymbolFlags::virtual_); }
    void SetVirtual() { SetFlag(FunctionSymbolFlags::virtual_); }
    bool IsOverride() const { return GetFlag(FunctionSymbolFlags::override_); }
    void SetOverride() { SetFlag(FunctionSymbolFlags::override_); }
    bool IsAbstract() const { return GetFlag(FunctionSymbolFlags::abstract_); }
    void SetAbstract() { SetFlag(FunctionSymbolFlags::abstract_); }
    bool IsVirtualAbstractOrOverride() const { return GetFlag(FunctionSymbolFlags::virtual_ | FunctionSymbolFlags::abstract_ | FunctionSymbolFlags::override_); }
    bool IsNew() const { return GetFlag(FunctionSymbolFlags::new_); }
    void SetNew() { SetFlag(FunctionSymbolFlags::new_); }
    bool IsConst() const { return GetFlag(FunctionSymbolFlags::const_); }
    void SetConst() { SetFlag(FunctionSymbolFlags::const_); }
    bool IsConversion() const { return GetFlag(FunctionSymbolFlags::conversion); }
    void SetConversion() { SetFlag(FunctionSymbolFlags::conversion); }
    bool HasLinkOnceOdrLinkage() const { return GetFlag(FunctionSymbolFlags::linkOnceOdrLinkage); }
    void SetLinkOnceOdrLinkage() { SetFlag(FunctionSymbolFlags::linkOnceOdrLinkage); }
    bool IsTemplateSpecialization() const { return GetFlag(FunctionSymbolFlags::templateSpecialization); }
    void SetTemplateSpecialization() { SetFlag(FunctionSymbolFlags::templateSpecialization); }
    bool HasTry() const { return GetFlag(FunctionSymbolFlags::hasTry); }
    void SetHasTry() { SetFlag(FunctionSymbolFlags::hasTry); }
    bool IsCopy() const { return GetFlag(FunctionSymbolFlags::copy); }
    void SetCopy() { SetFlag(FunctionSymbolFlags::copy); }
    bool IsVarArg() const { return GetFlag(FunctionSymbolFlags::varArg); }
    void SetVarArg() { SetFlag(FunctionSymbolFlags::varArg); }
    void SetHasArtificialBody() { SetFlag(FunctionSymbolFlags::hasArtificialBody); }
    bool HasArtificialBody() const { return GetFlag(FunctionSymbolFlags::hasArtificialBody); }
    bool HasCleanup() const { return GetFlag(FunctionSymbolFlags::hasCleanup); }
    void SetHasCleanup() { SetFlag(FunctionSymbolFlags::hasCleanup); }
    bool IsSystemDefault() const { return GetFlag(FunctionSymbolFlags::systemDefault); }
    void SetSystemDefault() { SetFlag(FunctionSymbolFlags::systemDefault); }
    bool IsImmutable() const override { return GetFlag(FunctionSymbolFlags::immutable); }
    void SetImmutable() { SetFlag(FunctionSymbolFlags::immutable); }
    void ResetImmutable() { ResetFlag(FunctionSymbolFlags::immutable); }
    virtual bool DontThrow() const { return (IsNothrow() || IsBasicTypeOperation()) && !HasCleanup(); }
    FunctionSymbolFlags GetFunctionSymbolFlags() const { return flags; }
    bool GetFlag(FunctionSymbolFlags flag) const { return (flags & flag) != FunctionSymbolFlags::none; }
    void SetFlag(FunctionSymbolFlags flag) { flags = flags | flag; }
    void ResetFlag(FunctionSymbolFlags flag) { flags = flags & ~flag; }
    void ComputeMangledName(Context* context) override;
    int Arity() const { return parameters.size(); }
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    void AddLocalVariable(LocalVariableSymbol* localVariable);
    const std::vector<LocalVariableSymbol*>& LocalVariables() const { return localVariables; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    TypeSymbol* ReturnType() const { return returnType; }
    ParameterSymbol* ReturnParam() { return returnParam.get(); }
    void SetReturnParam(ParameterSymbol* returnParam_);
    bool ReturnsClassInterfaceOrClassDelegateByValue() const;
    bool IsFunctionTemplate() const { return !templateParameters.empty(); }
    void CloneUsingNodes(const std::vector<cmajor::ast::Node*>& usingNodes_);
    LocalVariableSymbol* CreateTemporary(TypeSymbol* type, const soul::ast::Span& span, Context* context);
    virtual std::vector<LocalVariableSymbol*> CreateTemporariesTo(FunctionSymbol* currentFunction, Context* context);
    void* IrType(cmajor::ir::Emitter& emitter, Context* context);
    int32_t VmtIndex() const { return vmtIndex; }
    void SetVmtIndex(int32_t vmtIndex_) { vmtIndex = vmtIndex_; }
    int32_t ImtIndex() const { return imtIndex; }
    void SetImtIndex(int32_t imtIndex_) { imtIndex = imtIndex_; }
    FunctionGroupSymbol* FunctionGroup() { return functionGroup; }
    void SetFunctionGroup(FunctionGroupSymbol* functionGroup_) { functionGroup = functionGroup_; }
    IntrinsicFunction* GetIntrinsic() {  return intrinsic.get();  }
    void SetIntrinsic(IntrinsicFunction* intrinsic_) { intrinsic.reset(intrinsic_);  }
    bool IsProgramMain() const { return isProgramMain; }
    void SetProgramMain() { isProgramMain = true; }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    std::u32string Info() const override { return groupName; }
    const char* ClassName() const override { return "FunctionSymbol"; }
    virtual int ClassArity() const { return 0; }
    bool HasSource() const { return GetFlag(FunctionSymbolFlags::hasSource) && !IsDefault(); }
    void SetHasSource() { SetFlag(FunctionSymbolFlags::hasSource); }
    std::u32string Id() const override;
    FunctionSymbol* FunctionTemplate() { return functionTemplate; }
    void SetFunctionTemplate(FunctionSymbol* functionTemplate_) { functionTemplate = functionTemplate_; }
    void SetTemplateArgumentTypes(const std::vector<TypeSymbol*>& templateArgumentTypes_);
    const std::vector<TypeSymbol*>& TemplateArgumentTypes() const { return templateArgumentTypes; }
    void Check() override;
    FunctionSymbol* Master() const { return master; }
    void SetMaster(FunctionSymbol* master_) { master = master_; }
    void SetPrevUnwindInfoVar(LocalVariableSymbol* prevUnwindInfoVar_);
    LocalVariableSymbol* PrevUnwindInfoVar() const { return prevUnwindInfoVar.get(); }
    void SetUnwindInfoVar(LocalVariableSymbol* unwindInfoVar_);
    LocalVariableSymbol* UnwindInfoVar() const { return unwindInfoVar.get(); }
    LocalVariableSymbol* TraceEntryVar() const { return traceEntryVar; }
    void SetTraceEntryVar(LocalVariableSymbol* traceEntryVar_);
    LocalVariableSymbol* TraceGuardVar() const { return traceGuardVar; }
    void SetTraceGuardVar(LocalVariableSymbol* traceGuardVar_);
    LocalVariableSymbol* CheckerVar() const { return checkerVar; }
    void SetCheckerVar(LocalVariableSymbol* checkerVar_) { checkerVar = checkerVar_; }
    int NextTemporaryIndex();
    void CopyFrom(const Symbol* that) override;
    virtual FunctionSymbol* Copy() const;
    std::unique_ptr<Symbol> RemoveFromParent() override;
    std::string GetSymbolCategoryStr() const override { return "FN"; }
    std::string GetSymbolCategoryDescription() const override { return "function"; }
    std::string GetSymbolHelp() const override;
    void SetCompileUnitId(const std::string& compileUnitId_);
    const std::string& CompileUnitId() const { return compileUnitId; }
    const std::string& DigestSource() const { return digestSource; }
private:
    FunctionSymbol* functionTemplate;
    FunctionSymbol* master;
    util::uuid functionId;
    std::u32string groupName;
    std::vector<TemplateParameterSymbol*> templateParameters;
    std::vector<TypeSymbol*> templateArgumentTypes;
    std::vector<ParameterSymbol*> parameters;
    std::unique_ptr<ParameterSymbol> returnParam;
    std::vector<LocalVariableSymbol*> localVariables;
    std::unique_ptr<LocalVariableSymbol> unwindInfoVar;
    std::unique_ptr<LocalVariableSymbol> prevUnwindInfoVar;
    LocalVariableSymbol* traceEntryVar;
    LocalVariableSymbol* traceGuardVar;
    LocalVariableSymbol* checkerVar;
    TypeSymbol* returnType;
    FunctionSymbolFlags flags;
    int32_t index;
    int32_t vmtIndex;
    int32_t imtIndex;
    cmajor::ast::NodeList<cmajor::ast::Node> usingNodes;
    std::unique_ptr<cmajor::ast::FunctionNode> functionNode;
    std::unique_ptr<cmajor::ast::ConstraintNode> constraint;
    int nextTemporaryIndex;
    FunctionGroupSymbol* functionGroup;
    std::unique_ptr<IntrinsicFunction> intrinsic;
    bool isProgramMain;
    TypeSymbol* conversionSourceType;
    TypeSymbol* conversionTargetType;
    std::string compileUnitId;
    std::u32string instantiatedName;
    std::string digestSource;
};

class StaticConstructorSymbol : public FunctionSymbol
{
public:
    StaticConstructorSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::string TypeString(Context* context) const override { return "static_constructor"; }
    void Accept(SymbolCollector* collector) override {}
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    std::u32string FullNameWithSpecifiers() const override;
    std::u32string CodeName() const override;
    std::u32string Info() const override { return std::u32string(); }
    const char* ClassName() const override { return "StaticConstructorSymbol"; }
    int StartParamIndex() const override { return 0; }
    FunctionSymbol* Copy() const override;
};

class ConstructorSymbol : public FunctionSymbol
{
public:
    ConstructorSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::string TypeString(Context* context) const override;
    std::u32string DocName(Context* context) const override;
    std::u32string CodeName() const override;
    ParameterSymbol* GetThisParam() const override { return Parameters()[0]; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    uint8_t ConversionDistance() const override;
    std::u32string Info() const override { return std::u32string(); }
    const char* ClassName() const override { return "ConstructorSymbol"; }
    int StartParamIndex() const override { return 1; }
    FunctionSymbol* Copy() const override;
};

class DestructorSymbol : public FunctionSymbol
{
public:
    DestructorSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsExportSymbol() const override;
    void Accept(SymbolCollector* collector) override {}
    std::string TypeString(Context* context) const override { return "destructor"; }
    ParameterSymbol* GetThisParam() const override { return Parameters()[0]; }
    std::u32string CodeName() const override;
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    bool IsGeneratedFunction() const override { return generated; }
    bool CanInline() const { return false; }
    bool DontThrow() const override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void SetGenerated() { generated = true; }
    std::u32string Info() const override { return std::u32string(); }
    const char* ClassName() const override { return "DestructorSymbol"; }
    int StartParamIndex() const override { return 1; }
    FunctionSymbol* Copy() const override;
private:
    bool generated;
};

class MemberFunctionSymbol : public FunctionSymbol
{
public:
    MemberFunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::string TypeString(Context* context) const override;
    std::u32string DocName(Context* context) const override;
    ParameterSymbol* GetThisParam() const override { if (IsStatic()) return nullptr; else return Parameters()[0]; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return !IsStatic(); }
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    const char* ClassName() const override { return "MemberFunctionSymbol"; }
    int StartParamIndex() const override;
    FunctionSymbol* Copy() const override;
};

class ConversionFunctionSymbol : public FunctionSymbol
{
public:
    ConversionFunctionSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::string TypeString(Context* context) const override { return "conversion_function"; }
    std::u32string DocName(Context* context) const override;
    ParameterSymbol* GetThisParam() const override { return Parameters()[0]; }
    bool IsConstructorDestructorOrNonstaticMemberFunction() const override { return true; }
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    std::u32string Info() const override { return std::u32string(); }
    const char* ClassName() const override { return "ConversionFunctionSymbol"; }
    int StartParamIndex() const override { return 0; }
    FunctionSymbol* Copy() const override;
};

class FunctionGroupTypeSymbol : public TypeSymbol
{
public:
    FunctionGroupTypeSymbol(FunctionGroupSymbol* functionGroup_, void* boundFunctionGroup_, const soul::ast::Span& span_, int fileIndex_, const util::uuid& moduleId_);
    bool IsExportSymbol() const override { return false; }
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override { Assert(false, "tried to get ir type of function group type"); return nullptr; }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override { Assert(false, "tried to get default ir value of function group type"); return nullptr; }
    const FunctionGroupSymbol* FunctionGroup() const { return functionGroup; }
    FunctionGroupSymbol* FunctionGroup() { return functionGroup; }
    void* BoundFunctionGroup() const { return boundFunctionGroup; }
    const char* ClassName() const override { return "FunctionGroupTypeSymbol"; }
    int FileIndex() const override { return fileIndex; }
    const util::uuid& ModuleId() const override { return moduleId; }
private:
    FunctionGroupSymbol* functionGroup;
    void* boundFunctionGroup;
    int fileIndex;
    util::uuid moduleId;
};

class MemberExpressionTypeSymbol : public TypeSymbol
{
public:
    MemberExpressionTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_, void* boundMemberExpression_);
    bool IsExportSymbol() const override { return false; }
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override { Assert(false, "tried to get ir type of member expression type");  return nullptr; }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override { Assert(false, "tried to get default ir value of member expression type"); return nullptr; }
    std::string TypeString(Context* context) const override { return "member_expression_type"; }
    void* BoundMemberExpression() const { return boundMemberExpression; }
    const char* ClassName() const override { return "MemberExpressionTypeSymbol"; }
private:
    void* boundMemberExpression;
};

} // namespace cmajor::symbols

