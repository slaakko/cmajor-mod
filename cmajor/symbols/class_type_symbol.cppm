// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.classes;

import cmajor.symbols.type.symbol;
import cmajor.symbols.scope;
import cmajor.symbols.variable.symbol;
import soul.ast.span;
import cmajor.ir.emitter;
import cmajor.ast.classes;
import cmajor.ast.concepts;
import cmajor.ast.specifier;
import cmajor.ast.node;
import cmajor.ast.node.list;
import cmajor.ast.constant;
import soul.xml.element;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class InterfaceTypeSymbol;
class TemplateParameterSymbol;
class StaticConstructorSymbol;
class ConstructorSymbol;
class DestructorSymbol;
class MemberFunctionSymbol;
class ClassTemplateSpecializationSymbol;

class ClassGroupTypeSymbol : public TypeSymbol
{
public:
    ClassGroupTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    bool IsExportSymbol() const override { return false; }
    std::string TypeString(Context* context) const override { return "class_group"; }
    bool IsInComplete() const override { return true; }
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void AddClass(ClassTypeSymbol* classTypeSymbol);
    void RemoveClass(ClassTypeSymbol* classTypeSymbol);
    const ContainerScope* GetTypeScope() const override;
    ContainerScope* GetTypeScope() override;
    bool IsEmpty() const;
    ClassTypeSymbol* GetClass(int arity) const;
    bool HasProjectMembers() const override;
    void AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "ClassGroupTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "CL"; }
    std::string GetSymbolCategoryDescription() const override { return "class"; }
    std::string GetSymbolHelp() const override;
    bool IsValidCCClassGroup(Module* module, FunctionSymbol* fromFunction) const;
    std::vector<Symbol*> GetParamHelpSymbols() const override;
private:
    std::unordered_map<int, ClassTypeSymbol*> arityClassMap;
};

enum class ClassTypeSymbolFlags : uint16_t
{
    none = 0,
    abstract_ = 1 << 0,
    polymorphic = 1 << 1,
    vmtInitialized = 1 << 2,
    imtsInitialized = 1 << 3,
    layoutsComputed = 1 << 4,
    statementsNotBound = 1 << 5,
    recursiveComputed = 1 << 6,
    recursive = 1 << 7,
    vmtEmitted = 1 << 8,
    staticsEmitted = 1 << 9,
    hasXmlAttribute = 1 << 10
};

inline ClassTypeSymbolFlags operator|(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right)
{
    return ClassTypeSymbolFlags(uint16_t(left) | uint16_t(right));
}

inline ClassTypeSymbolFlags operator&(ClassTypeSymbolFlags left, ClassTypeSymbolFlags right)
{
    return ClassTypeSymbolFlags(uint16_t(left) & uint16_t(right));
}

inline ClassTypeSymbolFlags operator~(ClassTypeSymbolFlags operand)
{
    return ClassTypeSymbolFlags(~uint16_t(operand));
}

int32_t GetClassIdVmtIndexOffset();     // 128-bit class id, was 64-bit before
int32_t GetTypeIdVmtIndexOffset();      // 16-byte type id
int32_t GetClassNameVmtIndexOffset();   // class name pointer
int32_t GetImtsVmtIndexOffset();        // interface method table pointer
int32_t GetFunctionVmtIndexOffset();    // virtual method table

/* Indeces for LLVM backend:

const int32_t classIdVmtIndexOffset = 0;    // 128-bit class id, was 64-bit before
const int32_t typeIdVmtIndexOffset = 2;     // 16-byte type id
const int32_t classNameVmtIndexOffset = 4;  // class name pointer
const int32_t imtsVmtIndexOffset = 5;       // interface method table pointer
const int32_t functionVmtIndexOffset = 6;   // virtual method table

*/


class ClassTypeFlagMap
{
public:
    ClassTypeFlagMap();
    void Import(ClassTypeFlagMap& that);
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    bool GetSetFlagLocked(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag);
    bool GetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag);
    void SetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag);
    void ResetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag);
private:
    std::map<std::u32string, ClassTypeSymbolFlags> flagMap;
    std::recursive_mutex mtx;
};

class ClassTypeSymbol : public TypeSymbol
{
public:
    ClassTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    ClassTypeSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    const cmajor::ast::NodeList<cmajor::ast::Node>& UsingNodes() const { return usingNodes; }
    cmajor::ast::ClassNode* GetClassNode() { return classNode.get(); }
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void AddMember(Symbol* member, Context* context) override;
    bool IsClassTypeSymbol() const override { return true; }
    bool IsParentSymbol() const override { return true; }
    std::string TypeString(Context* context) const override { return "class"; }
    std::u32string SimpleName() const override { return groupName; }
    std::string GetSpecifierStr() override;
    bool HasNontrivialDestructor() override;
    void Accept(SymbolCollector* collector) override;
    const ContainerScope* GetArrowScope() const override;
    ContainerScope* GetArrowScope() override;
    void CollectMembers(SymbolCollector* collector);
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    // TODO
    bool IsRecursive(TypeSymbol* type, std::unordered_set<util::uuid, util::UuidHash>& tested) override;
    virtual bool IsPrototypeTemplateSpecialization() const { return false; }
    bool CompletelyBound() override { return IsBound() && !StatementsNotBound(); }
    void CreateDestructorSymbol(Context* context);
    const std::u32string& GroupName() const { return groupName; }
    void SetGroupName(const std::u32string& groupName_);
    std::u32string CodeName() const override { return groupName; }
    int MinArity() const { return minArity; }
    int MaxArity() const { return templateParameters.size(); }
    void ComputeMinArity();
    const ClassTypeSymbol* BaseClass() const { return baseClass; }
    ClassTypeSymbol* BaseClass() { return baseClass; }
    void SetBaseClass(ClassTypeSymbol* baseClass_) { baseClass = baseClass_; }
    bool HasBaseClass(ClassTypeSymbol* cls) const;
    bool HasBaseClass(ClassTypeSymbol* cls, uint8_t& distance) const;
    const std::vector<InterfaceTypeSymbol*>& ImplementedInterfaces() const { return implementedInterfaces; }
    void AddImplementedInterface(InterfaceTypeSymbol* interfaceTypeSymbol);
    const std::vector<TemplateParameterSymbol*>& TemplateParameters() const { return templateParameters; }
    bool IsClassTemplate() const { return !templateParameters.empty(); }
    void CloneUsingNodes(const std::vector<cmajor::ast::Node*>& usingNodes_);
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void ComputeName(Context* context);
    void ComputeMangledName(Context* context) override;
    bool IsPolymorphicType() override { return IsPolymorphic(); }
    void SetConstraint(cmajor::ast::ConstraintNode* constraint_) { constraint.reset(constraint_); }
    cmajor::ast::ConstraintNode* Constraint() { return constraint.get(); }
    bool IsLiteralClassType();
    StaticConstructorSymbol* StaticConstructor() { return staticConstructor; }
    ConstructorSymbol* DefaultConstructor() { return defaultConstructor; }
    void SetDefaultConstructor(ConstructorSymbol* defaultConstructor_) { defaultConstructor = defaultConstructor_; }
    ConstructorSymbol* CopyConstructor() { return copyConstructor; }
    void SetCopyConstructor(ConstructorSymbol* copyConstructor_) { copyConstructor = copyConstructor_; }
    ConstructorSymbol* MoveConstructor() { return moveConstructor; }
    void SetMoveConstructor(ConstructorSymbol* moveConstructor_) { moveConstructor = moveConstructor_; }
    DestructorSymbol* Destructor() { return destructor; }
    void SetDestructor(DestructorSymbol* destructor_) { destructor = destructor_; }
    MemberFunctionSymbol* CopyAssignment() { return copyAssignment; }
    void SetCopyAssignment(MemberFunctionSymbol* copyAssignment_) { copyAssignment = copyAssignment_; }
    MemberFunctionSymbol* MoveAssignment() { return moveAssignment; }
    void SetMoveAssignment(MemberFunctionSymbol* moveAssignment_) { moveAssignment = moveAssignment_; }
    void SetSpecialMemberFunctions(Context* context);
    const std::vector<MemberVariableSymbol*>& MemberVariables() const { return memberVariables; }
    const std::vector<MemberVariableSymbol*>& StaticMemberVariables() const { return staticMemberVariables; }
    const std::vector<MemberFunctionSymbol*>& MemberFunctions() const { return memberFunctions; }
    const std::vector<FunctionSymbol*>& AllMemberFunctions() const { return allMemberFunctions; }
    MemberVariableSymbol* InitializedVar() { return initializedVar.get(); }
    void SetInitializedVar(MemberVariableSymbol* initializedVar_);
    bool IsAbstract() { return GetFlag(ClassTypeSymbolFlags::abstract_); }
    void SetAbstract() { SetFlag(ClassTypeSymbolFlags::abstract_); }
    bool IsPolymorphic() { return GetFlag(ClassTypeSymbolFlags::polymorphic); }
    void SetPolymorphic() { SetFlag(ClassTypeSymbolFlags::polymorphic); }
    bool IsVmtInitialized() { return GetFlag(ClassTypeSymbolFlags::vmtInitialized); }
    void SetVmtInitialized() { SetFlag(ClassTypeSymbolFlags::vmtInitialized); }
    bool IsImtsInitialized() { return GetFlag(ClassTypeSymbolFlags::imtsInitialized); }
    void SetImtsInitialized() { SetFlag(ClassTypeSymbolFlags::imtsInitialized); }
    bool IsLayoutsComputed() { return GetFlag(ClassTypeSymbolFlags::layoutsComputed); }
    void SetLayoutsComputed() { SetFlag(ClassTypeSymbolFlags::layoutsComputed); }
    bool StatementsNotBound() { return GetFlag(ClassTypeSymbolFlags::statementsNotBound); }
    void SetStatementsNotBound() { SetFlag(ClassTypeSymbolFlags::statementsNotBound); }
    void ResetStatementsNotBound() { ResetFlag(ClassTypeSymbolFlags::statementsNotBound); }
    bool RecursiveComputed() { return GetFlag(ClassTypeSymbolFlags::recursiveComputed); }
    void SetRecursiveComputed() { SetFlag(ClassTypeSymbolFlags::recursiveComputed); }
    bool HasXmlAttribute() { return GetFlag(ClassTypeSymbolFlags::hasXmlAttribute); }
    void SetHasXmlAttribute() { SetFlag(ClassTypeSymbolFlags::hasXmlAttribute); }
    bool IsRecursive();
    bool Recursive() { return GetFlag(ClassTypeSymbolFlags::recursive); }
    void SetRecursive() { SetFlag(ClassTypeSymbolFlags::recursive); }
    bool VmtEmitted() { return GetSetFlagLocked(ClassTypeSymbolFlags::vmtEmitted); }
    bool IsVmtEmitted() { return GetFlag(ClassTypeSymbolFlags::vmtEmitted); }
    void SetVmtEmitted() { SetFlag(ClassTypeSymbolFlags::vmtEmitted); };
    bool StaticsEmitted() { return GetSetFlagLocked(ClassTypeSymbolFlags::staticsEmitted); }
    ClassTypeSymbolFlags GetClassTypeSymbolFlags() const { return flags; }
    bool GetSetFlagLocked(ClassTypeSymbolFlags flag);
    bool GetFlag(ClassTypeSymbolFlags flag);
    void SetFlag(ClassTypeSymbolFlags flag);
    void ResetFlag(ClassTypeSymbolFlags flag);
    void InitVmt(Context* context);
    void InitImts(Context* context);
    void CreateLayouts(Context* context);
    const std::vector<TypeSymbol*>& ObjectLayout() const { return objectLayout; }
    const std::vector<FunctionSymbol*>& Vmt() const { return vmt; }
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDIType(cmajor::ir::Emitter& emitter, Context* context) override;
    void* CreateDIForwardDeclaration(cmajor::ir::Emitter& emitter, Context* context);
    void* VmtObject(cmajor::ir::Emitter& emitter, bool create, Context* context);
    void* VmtArrayType(cmajor::ir::Emitter& emitter);
    void* VmtPtrType(cmajor::ir::Emitter& emitter);
    std::string VmtObjectName(cmajor::ir::Emitter& emitter);
    std::string VmtObjectNameStr(cmajor::ir::Emitter& emitter);
    std::string ImtArrayObjectName(cmajor::ir::Emitter& emitter);
    std::string ImtObjectName(int index, cmajor::ir::Emitter& emitter);
    int32_t VmtPtrIndex() const { return vmtPtrIndex; }
    ClassTypeSymbol* VmtPtrHolderClass();
    void* StaticObject(cmajor::ir::Emitter& emitter, bool create, Context* context);
    void* StaticObjectType(cmajor::ir::Emitter& emitter, Context* context);
    std::string StaticObjectName(cmajor::ir::Emitter& emitter);
    void SetPrototype(ClassTemplateSpecializationSymbol* prototype_) { prototype = prototype_; }
    ClassTemplateSpecializationSymbol* Prototype() const { return prototype; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    std::u32string Id() const override;
    std::u32string Info() const override { return groupName; }
    const char* ClassName() const override { return "ClassTypeSymbol"; }
    void Check() override;
    void SetClassGroup(ClassGroupTypeSymbol* classGroup_) { classGroup = classGroup_; }
    std::unique_ptr<Symbol> RemoveFromParent() override;
    std::string GetSymbolCategoryStr() const override { return "CL"; }
    std::string GetSymbolCategoryDescription() const override { return "class"; }
private:
    std::u32string groupName;
    int minArity;
    ClassTypeSymbol* baseClass;
    ClassTypeSymbolFlags flags;
    std::vector<InterfaceTypeSymbol*> implementedInterfaces;
    std::vector<TemplateParameterSymbol*> templateParameters;
    std::vector<MemberVariableSymbol*> memberVariables;
    std::vector<MemberVariableSymbol*> staticMemberVariables;
    std::unique_ptr<MemberVariableSymbol> initializedVar;
    StaticConstructorSymbol* staticConstructor;
    std::vector<ConstructorSymbol*> constructors;
    ConstructorSymbol* defaultConstructor;
    ConstructorSymbol* copyConstructor;
    ConstructorSymbol* moveConstructor;
    DestructorSymbol* destructor;
    MemberFunctionSymbol* copyAssignment;
    MemberFunctionSymbol* moveAssignment;
    std::vector<MemberFunctionSymbol*> memberFunctions;
    std::vector<FunctionSymbol*> allMemberFunctions;
    std::vector<FunctionSymbol*> vmt;
    std::vector<std::vector<FunctionSymbol*>> imts;
    std::vector<TypeSymbol*> objectLayout;
    std::vector<TypeSymbol*> staticLayout;
    int32_t vmtPtrIndex;
    cmajor::ast::NodeList<cmajor::ast::Node> usingNodes;
    std::unique_ptr<cmajor::ast::ClassNode> classNode;
    std::unique_ptr<cmajor::ast::ConstraintNode> constraint;
    ClassTemplateSpecializationSymbol* prototype;
    ClassGroupTypeSymbol* classGroup;
    std::recursive_mutex mtx;
    void InitVmt(std::vector<FunctionSymbol*>& vmtToInit);
    void* CreateImt(cmajor::ir::Emitter& emitter, int index, Context* context);
    void* CreateImts(cmajor::ir::Emitter& emitter, Context* context);
};

cmajor::ast::ConstantNode* MakePolymorphicClassArray(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::u32string& arrayName);
cmajor::ast::ConstantNode* MakeStaticClassArray(const std::set<ClassTypeSymbol*>& classesHavingStaticConstructor, const std::u32string& arrayName);
void MakeClassIdFile(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::string& classIdFileName);
void MakeClassIndexFile(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::string& classIndexFilePath);

} // namespace cmajor::symbols
