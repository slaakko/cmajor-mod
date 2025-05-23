// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ir.emitter;

import std.core;
import soul.ast.source.pos;
import soul.ast.span;
import cmajor.ir.value.stack;
import util.uuid;
import cmajor.ir.emitting.context;

export namespace cmajor::ir {

struct Pad
{
    Pad() : parent(nullptr), value(nullptr) {}
    Pad* parent;
    void* value;
};

class EmittingDelegate
{
public:
    virtual void* GetGlobalStringPtr(int stringId) = 0;
    virtual void* GetGlobalWStringConstant(int stringId, void*& arrayType) = 0;
    virtual void* GetGlobalUStringConstant(int stringId, void*& arrayType) = 0;
    virtual void* GetGlobalUuidConstant(int uuidId) = 0;
    virtual void SetLineNumber(int32_t lineNumber) {}
    virtual void SetSpan(const soul::ast::Span& span) {}
    virtual void* HandlerBlock() { return nullptr; }
    virtual void* CleanupBlock() { return nullptr; }
    virtual bool NewCleanupNeeded() { return false; }
    virtual bool InTryBlock() const { return false; }
    virtual int CurrentTryBlockId() const { return 0; }
    virtual void CreateCleanup() { }
    virtual std::string GetSourceFilePath(int fileIndex, const util::uuid& moduleId) { return std::string(); }
    virtual cmajor::ir::Pad* CurrentPad() { return nullptr; }
    virtual void* CreateClassDIType(void* classPtr) { return nullptr; }
    virtual int Install(const std::string& str) = 0;
    virtual int Install(const std::u16string& str) = 0;
    virtual int Install(const std::u32string& str) = 0;
};

class Emitter
{
public:
    Emitter();
    virtual ~Emitter();
    void SetStack(ValueStack* stack_) { stack = stack_; }
    ValueStack& Stack() { return *stack; }
    virtual cmajor::ir::EmittingContext* EmittingContext() const = 0;
    virtual void SetEmittingDelegate(EmittingDelegate* emittingDelegate_) = 0;
    virtual void* GetCompileUnit() { return nullptr; }
    virtual void SetFilePath(const std::string& filePath) {}
    virtual void* GetIrTypeForBool() = 0;
    virtual void* GetIrTypeForSByte() = 0;
    virtual void* GetIrTypeForByte() = 0;
    virtual void* GetIrTypeForShort() = 0;
    virtual void* GetIrTypeForUShort() = 0;
    virtual void* GetIrTypeForInt() = 0;
    virtual void* GetIrTypeForUInt() = 0;
    virtual void* GetIrTypeForLong() = 0;
    virtual void* GetIrTypeForULong() = 0;
    virtual void* GetIrTypeForFloat() = 0;
    virtual void* GetIrTypeForDouble() = 0;
    virtual void* GetIrTypeForChar() = 0;
    virtual void* GetIrTypeForWChar() = 0;
    virtual void* GetIrTypeForUChar() = 0;
    virtual void* GetIrTypeForVoid() = 0;
    virtual void* GetIrTypeForFunction(void* retType, const std::vector<void*>& paramTypes) = 0;
    virtual void* GetIrTypeForVariableParamFunction(void* retType) = 0;
    virtual void* GetIrTypeByTypeId(const util::uuid& typeId) = 0;
    virtual void SetIrTypeByTypeId(const util::uuid& typeId, void* irType) = 0;
    virtual void* GetIrTypeForArrayType(void* elementType, int64_t size) = 0;
    virtual void* GetIrTypeForClassType(const std::vector<void*>& elementTypes) = 0;
    virtual void* CreateFwdIrTypeForClassType() = 0;
    virtual void SetFwdIrTypeBody(void* forwardDeclaredType, const std::vector<void*>& elementTypes) = 0;
    virtual void* GetIrTypeForDelegateType(void* retType, const std::vector<void*>& paramTypes) = 0;
    virtual void* GetIrTypeForVoidPtrType() = 0;
    virtual void* GetIrTypeForStructType(const std::vector<void*>& elementTypes) = 0;
    virtual void* GetIrTypeForPtrType(void* baseIrType) = 0;
    virtual std::string GetIrTypeName(void* irType) = 0;
    virtual std::string MakeVmtVariableName(const std::string& vmtObjectName) = 0;
    virtual std::string MakeSymbolName(const std::string& name) { return name; }
    virtual void* CreateDefaultIrValueForArrayType(void* arrayIrType, const std::vector<void*>& arrayOfDefaults) = 0;
    virtual void* CreateDefaultIrValueForBool() = 0;
    virtual void* CreateTrue() = 0;
    virtual void* CreateFalse() = 0;
    virtual void* CreateDefaultIrValueForSByte() = 0;
    virtual void* CreateDefaultIrValueForByte() = 0;
    virtual void* CreateDefaultIrValueForShort() = 0;
    virtual void* CreateDefaultIrValueForUShort() = 0;
    virtual void* CreateDefaultIrValueForInt() = 0;
    virtual void* CreateDefaultIrValueForUInt() = 0;
    virtual void* CreateDefaultIrValueForLong() = 0;
    virtual void* CreateDefaultIrValueForULong() = 0;
    virtual void* CreateDefaultIrValueForFloat() = 0;
    virtual void* CreateDefaultIrValueForDouble() = 0;
    virtual void* CreateDefaultIrValueForChar() = 0;
    virtual void* CreateDefaultIrValueForWChar() = 0;
    virtual void* CreateDefaultIrValueForUChar() = 0;
    virtual void* CreateDefaultIrValueForStruct(void* irType, const std::vector<void*>& defaultMembers) = 0;
    virtual void* CreateDefaultIrValueForDelegateType(void* irType) = 0;
    virtual void* CreateDefaultIrValueForVoidPtrType() = 0;
    virtual void* CreateDefaultIrValueForDerivedType(void* irType) = 0;
    virtual void* CreateDefaultIrValueForPtrType(void* irType) = 0;
    virtual void* CreateIrValueForBool(bool value) = 0;
    virtual void* CreateIrValueForSByte(int8_t value) = 0;
    virtual void* CreateIrValueForByte(uint8_t value) = 0;
    virtual void* CreateIrValueForShort(int16_t value) = 0;
    virtual void* CreateIrValueForUShort(uint16_t value) = 0;
    virtual void* CreateIrValueForInt(int32_t value) = 0;
    virtual void* CreateIrValueForUInt(uint32_t value) = 0;
    virtual void* CreateIrValueForLong(int64_t value) = 0;
    virtual void* CreateIrValueForULong(uint64_t value) = 0;
    virtual void* CreateIrValueForFloat(float value) = 0;
    virtual void* CreateIrValueForDouble(double value) = 0;
    virtual void* CreateIrValueForChar(uint8_t value) = 0;
    virtual void* CreateIrValueForWChar(uint16_t value) = 0;
    virtual void* CreateIrValueForUChar(uint32_t value) = 0;
    virtual void* CreateIrValueForWString(void* type, void* wstringConstant) = 0;
    virtual void* CreateIrValueForUString(void* type, void* ustringConstant) = 0;
    virtual void* CreateIrValueForConstantArray(void* arrayIrType, const std::vector<void*>& elementConstants, const std::string& prefix) = 0;
    virtual void* CreateIrValueForConstantStruct(void* structIrType, const std::vector<void*>& elementConstants) = 0;
    virtual void* CreateIrValueForUuid(void* type, void* uuidConstant) = 0;
    virtual void* GetConversionValue(void* type, void* from) = 0;
    virtual void* CreateGlobalStringPtr(const std::string& name) = 0;
    virtual void* CreateGlobalWStringPtr(const std::u16string& name) = 0;
    virtual void* CreateGlobalUStringPtr(const std::u32string& name) = 0;
    virtual void* GetGlobalStringPtr(int stringId) = 0;
    virtual void* GetGlobalWStringConstant(int stringId, void*& arrayType) = 0;
    virtual void* GetGlobalUStringConstant(int stringId, void*& arrayType) = 0;
    virtual void* GetGlobalUuidConstant(int uuidId) = 0;
    virtual void* CreateDITypeForBool() = 0;
    virtual void* CreateDITypeForSByte() = 0;
    virtual void* CreateDITypeForByte() = 0;
    virtual void* CreateDITypeForShort() = 0;
    virtual void* CreateDITypeForUShort() = 0;
    virtual void* CreateDITypeForInt() = 0;
    virtual void* CreateDITypeForUInt() = 0;
    virtual void* CreateDITypeForLong() = 0;
    virtual void* CreateDITypeForULong() = 0;
    virtual void* CreateDITypeForFloat() = 0;
    virtual void* CreateDITypeForDouble() = 0;
    virtual void* CreateDITypeForChar() = 0;
    virtual void* CreateDITypeForWChar() = 0;
    virtual void* CreateDITypeForUChar() = 0;
    virtual void* CreateDITypeForVoid() = 0;
    virtual void* CreateDITypeForArray(void* elementDIType, const std::vector<void*>& elements) = 0;
    virtual void* CreateDITypeForEnumConstant(const std::string& name, int64_t value) = 0;
    virtual void* CreateDITypeForEnumType(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        const std::vector<void*>& enumConstantElements, uint64_t sizeInBits, uint32_t alignInBits, void* underlyingDIType) = 0;
    virtual void* CreateIrDIForwardDeclaration(void* irType, const std::string& name, const std::string& mangledName,
        const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen) = 0;
    virtual uint64_t GetOffsetInBits(void* classIrType, int layoutIndex) = 0;
    virtual void* CreateDITypeForClassType(void* irType, const std::vector<void*>& memberVariableElements, const soul::ast::FullSpan& fullSpan,
        const soul::ast::LineColLen& lineColLen, const std::string& name, void* vtableHolderClass, const std::string& mangledName, void* baseClassDIType) = 0;
    virtual void MapFwdDeclaration(void* fwdDeclaration, const util::uuid& typeId) = 0;
    virtual void* GetDITypeByTypeId(const util::uuid& typeId) const = 0;
    virtual void SetDITypeByTypeId(const util::uuid& typeId, void* diType, const std::string& typeName) = 0;
    virtual void* GetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId) = 0;
    virtual void SetDIMemberType(const std::pair<util::uuid, int32_t>& memberVariableId, void* diType) = 0;
    virtual void* CreateDIMemberType(void* scope, const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        uint64_t sizeInBits, uint64_t alignInBits, uint64_t offsetInBits, void* diType) = 0;
    virtual void* CreateConstDIType(void* diType) = 0;
    virtual void* CreateLValueRefDIType(void* diType) = 0;
    virtual void* CreateRValueRefDIType(void* diType) = 0;
    virtual void* CreatePointerDIType(void* diType) = 0;
    virtual void* CreateUnspecifiedDIType(const std::string& name) = 0;
    virtual void MapClassPtr(const util::uuid& typeId, void* classPtr, const std::string& className) = 0;
    virtual uint64_t GetSizeInBits(void* irType) = 0;
    virtual uint64_t GetAlignmentInBits(void* irType) = 0;
    virtual void SetCurrentDebugLocation(const soul::ast::LineColLen& lineColLen) = 0;
    virtual void SetCurrentDebugLocation(const soul::ast::Span& span) = 0;
    virtual void* GetArrayBeginAddress(void* arrayType, void* arrayPtr) = 0;
    virtual void* GetArrayEndAddress(void* arrayType, void* arrayPtr, uint64_t size) = 0;
    virtual void* CreateBasicBlock(const std::string& name) = 0;
    virtual void* CreateIncludeBasicBlockInstruction(void* basicBlock) = 0;
    virtual void PushParentBlock() = 0;
    virtual void PopParentBlock() = 0;
    virtual void SetHandlerBlock(void* tryBlock, void* catchBlock) = 0;
    virtual void SetCleanupBlock(void* cleanupBlock) = 0;
    virtual int GetBasicBlockId(void* basicBlock) = 0;
    virtual void CreateBr(void* targetBasicBlock) = 0;
    virtual void* CurrentBasicBlock() const = 0;
    virtual void SetCurrentBasicBlock(void* basicBlock) = 0;
    virtual void CreateCondBr(void* cond, void* trueBasicBlock, void* falseBasicBlock) = 0;
    virtual void* CreateArrayIndexAddress(void* arrayType, void* arrayPtr, void* elementType, void* index) = 0;
    virtual void CreateStore(void* value, void* ptr) = 0;
    virtual void* CreateLoad(void* type, void* ptr) = 0;
    virtual void* CreateAdd(void* left, void* right) = 0;
    virtual void* CreateFAdd(void* left, void* right) = 0;
    virtual void* CreateSub(void* left, void* right) = 0;
    virtual void* CreateFSub(void* left, void* right) = 0;
    virtual void* CreateMul(void* left, void* right) = 0;
    virtual void* CreateFMul(void* left, void* right) = 0;
    virtual void* CreateUDiv(void* left, void* right) = 0;
    virtual void* CreateSDiv(void* left, void* right) = 0;
    virtual void* CreateFDiv(void* left, void* right) = 0;
    virtual void* CreateURem(void* left, void* right) = 0;
    virtual void* CreateSRem(void* left, void* right) = 0;
    virtual void* CreateAnd(void* left, void* right) = 0;
    virtual void* CreateOr(void* left, void* right) = 0;
    virtual void* CreateXor(void* left, void* right) = 0;
    virtual void* CreateShl(void* left, void* right) = 0;
    virtual void* CreateAShr(void* left, void* right) = 0;
    virtual void* CreateLShr(void* left, void* right) = 0;
    virtual void* CreateICmpEQ(void* left, void* right) = 0;
    virtual void* CreateFCmpOEQ(void* left, void* right) = 0;
    virtual void* CreateICmpULT(void* leftValue, void* rightValue) = 0;
    virtual void* CreateICmpSLT(void* leftValue, void* rightValue) = 0;
    virtual void* CreateFCmpOLT(void* leftValue, void* rightValue) = 0;
    virtual void* CreateSExt(void* operand, void* destinationType) = 0;
    virtual void* CreateZExt(void* operand, void* destinationType) = 0;
    virtual void* CreateFPExt(void* operand, void* destinationType) = 0;
    virtual void* CreateTrunc(void* operand, void* destinationType) = 0;
    virtual void* CreateFPTrunc(void* operand, void* destinationType) = 0;
    virtual void* CreateBitCast(void* operand, void* destinationType) = 0;
    virtual void* CreateUIToFP(void* operand, void* destinationType) = 0;
    virtual void* CreateSIToFP(void* operand, void* destinationType) = 0;
    virtual void* CreateFPToUI(void* operand, void* destinationType) = 0;
    virtual void* CreateFPToSI(void* operand, void* destinationType) = 0;
    virtual void* CreateIntToPtr(void* intValue, void* destinationType) = 0;
    virtual void* CreatePtrToInt(void* ptrValue, void* destinationType) = 0;
    virtual void* CreateNot(void* value) = 0;
    virtual void* CreateNeg(void* value) = 0;
    virtual void* CreateFNeg(void* value) = 0;
    virtual void* CreateNop() = 0;
    virtual void* CreateSave() = 0;
    virtual void* CreateBeginTry() = 0;
    virtual void* CreateEndTry(void* nextDest) = 0;
    virtual void* CreateBeginCatch() = 0;
    virtual void* CreateEndCatch(void* nextDest) = 0;
    virtual std::string GetVmtObjectName(void* symbol) const = 0;
    virtual void SetVmtObjectName(void* symbol, const std::string& vmtObjectName) = 0;
    virtual std::string GetImtArrayObjectName(void* symbol) const = 0;
    virtual void SetImtArrayObjectName(void* symbol, const std::string& imtArrayObjectName) = 0;
    virtual void* GetVmtObjectType(void* symbol) const = 0;
    virtual void SetVmtObjectType(void* symbol, void* vmtObjectType) = 0;
    virtual void* GetStaticObjectType(void* symbol) const = 0;
    virtual void SetStaticObjectType(void* symbol, void* type) = 0;
    virtual std::string GetStaticObjectName(void* symbol) const = 0;
    virtual void SetStaticObjectName(void* symbol, const std::string& staticObjectName) = 0;
    virtual void* GetOrInsertGlobal(const std::string& name, void* type) = 0;
    virtual void* GetOrInsertAnyComdat(const std::string& name, void* global) = 0;
    virtual void* GetOrInsertAnyFunctionComdat(const std::string& name, void* function) = 0;
    virtual void* GetOrInsertFunction(const std::string& name, void* type, bool nothrow) = 0;
    virtual void SetSystemType(void* type, int8_t systemType) = 0;
    virtual void* MakeSymbolValue(void* type, const std::string& name) { return nullptr; }
    virtual void SetInitializer(void* global, void* initializer) = 0;
    virtual void SetPrivateLinkage(void* global) = 0;
    virtual bool IsVmtObjectCreated(void* symbol) const = 0;
    virtual void SetVmtObjectCreated(void* symbol) = 0;
    virtual bool IsStaticObjectCreated(void* symbol) const = 0;
    virtual void SetStaticObjectCreated(void* symbol) = 0;
    virtual void* HandlerBlock() = 0;
    virtual void* CleanupBlock() = 0;
    virtual bool NewCleanupNeeded() = 0;
    virtual void CreateCleanup() = 0;
    virtual Pad* CurrentPad() = 0;
    virtual void* CreateCleanupPadWithParent(void* parentPad, const std::vector<void*>& args) = 0;
    virtual void* CreateCleanupPad(const std::vector<void*>& args) = 0;
    virtual void* CreateCleanupRet(void* cleanupPad, void* unwindTarget) = 0;
    virtual void* CreateCatchRet(void* catchPad, void* returnTarget) = 0;
    virtual void* CreateCatchSwitch(void* unwindBlock) = 0;
    virtual void* CreateCatchSwitchWithParent(void* parentPad, void* unwindBlock) = 0;
    virtual void AddHandlerToCatchSwitch(void* catchSwitch, void* target) = 0;
    virtual void* CreateCatchPad(void* parentPad, const std::vector<void*>& args) = 0;
    virtual void* CreateClassDIType(void* classPtr) = 0;
    virtual void* CreateCall(void* functionType, void* callee, const std::vector<void*>& args) = 0;
    virtual void* CreateCallInst(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, const soul::ast::LineColLen& lineColLen) = 0;
    virtual void* CreateCallInstToBasicBlock(void* functionType, void* callee, const std::vector<void*>& args, const std::vector<void*>& bundles, void* basicBlock,
        const soul::ast::LineColLen& lineColLen) = 0;
    virtual void* CreateInvoke(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args) = 0;
    virtual void* CreateInvokeInst(void* functionType, void* callee, void* normalBlock, void* unwindBlock, const std::vector<void*>& args, const std::vector<void*>& bundles,
        const soul::ast::LineColLen& lineColLen) = 0;
    virtual void* DIBuilder() = 0;
    virtual void SetCurrentDIBuilder(void* diBuilder_) = 0;
    virtual void* GetObjectFromClassDelegate(void* classDelegateType, void* classDelegatePtr) = 0;
    virtual void* GetDelegateFromClassDelegate(void* classDelegateType, void* classDelegatePtr) = 0;
    virtual void* GetObjectFromInterface(void* interfaceType, void* interfaceTypePtr) = 0;
    virtual void* GetObjectPtrFromInterface(void* interfaceType, void* interfaceTypePtr) = 0;
    virtual void* GetImtPtrPtrFromInterface(void* interfaceType, void* interfaceTypePtr) = 0;
    virtual void* GetImtPtrFromInterface(void* interfaceType, void* interfaceTypePtr) = 0;
    virtual void* GetInterfaceMethod(void* interfaceType, void* imtPtr, int32_t methodIndex, void* interfaceMethodType) = 0;
    virtual void* GetImtsArrayPtrFromVmt(void* vmtPtr, void* vmtArrayType, int32_t imtsVmtIndexOffset) = 0;
    virtual void* GetImtPtrFromImtsPtr(void* imtsPtr, int32_t interfaceIndex, int32_t interfaceCount) = 0;
    virtual void* GetFunctionIrType(void* functionSymbol) const = 0;
    virtual void SetFunctionIrType(void* symbol, void* irType) = 0;
    virtual void* GetVmtPtr(void* classType, void* thisPtr, int32_t vmtPtrIndex, void* vmtPtrType) = 0;
    virtual void* GetMethodPtr(void* vmtType, void* vmtPtr, int32_t vmtIndex) = 0;
    //virtual void* GetImtArray(void* vmtType, void* vmtObjectPtr, int32_t imtsVmtIndexOffset) = 0;
    //virtual void* GetImt(void* imtArrayType, void* imtArray, int32_t interfaceIndex) = 0;
    virtual void* GetIrObject(void* symbol) const = 0;
    virtual void SetIrObject(void* symbol, void* irObject) = 0;
    virtual void* GetMemberVariablePtr(void* classType, void* classPtr, int32_t memberVariableLayoutIndex) = 0;
    virtual int64_t GetFieldOffset(void* structureType, int64_t fieldIndex) const { return -1; }
    virtual void* SizeOf(void* elementType, void* ptrType) = 0;
    virtual void SetLineNumber(int32_t lineNumber) = 0;
    virtual void SaveObjectPointer(void* objectPointer_) = 0;
    virtual void SetObjectPointer(void* objectPointer_) = 0;
    virtual void* GetObjectPointer() = 0;
    virtual void* GetClassIdPtr(void* vmtArrayType, void* vmtPtr, int32_t classIdVmtIndexOffset) = 0;
    virtual void* GetClassName(void* vmtArrayType, void* vmtPtr, int32_t classNameVmtIndexOffset) = 0;
    virtual void* ComputeAddress(void* type, void* ptr, void* index) = 0;
    virtual void* CreatePtrDiff(void* elementType, void* left, void* right) = 0;
    virtual uint32_t GetPrivateFlag() = 0;
    virtual uint32_t GetProtectedFlag() = 0;
    virtual uint32_t GetPublicFlag() = 0;
    virtual uint32_t GetNoFlags() = 0;
    virtual void* CreateModule(const std::string& moduleName) = 0;
    virtual void DestroyModule(void* module) = 0;
    virtual void SetModule(void* module_) = 0;
    virtual void SetTargetTriple(const std::string& targetTriple) = 0;
    virtual void SetDataLayout(void* dataLayout_) = 0;
    virtual void SetSourceFileName(const std::string& sourceFileName) = 0;
    virtual void SetDICompileUnit(void* diCompileUnit_) = 0;
    virtual void SetDIFile(void* diFile_) = 0;
    virtual void ResetCurrentDebugLocation() = 0;
    virtual void StartDebugInfo(const std::string& sourceFilePath, const std::string& compilerVersion, bool optimized) = 0;
    virtual void FinalizeDebugInfo() = 0;
    virtual void EndDebugInfo() = 0;
    virtual void Compile(const std::string& objectFilePath) = 0;
    virtual void VerifyModule() = 0;
    virtual void* CreateDebugInfoForNamespace(void* scope, const std::string& name) = 0;
    virtual void* GetDebugInfoForFile(const soul::ast::FullSpan& fullSpan) = 0;
    virtual void PushScope(void* scope) = 0;
    virtual void PopScope() = 0;
    virtual void* CurrentScope() = 0;
    virtual uint64_t GetClassTypeSizeInBits(void* classIrType) = 0;
    virtual uint64_t GetClassTypeAlignmentInBits(void* classIrType) = 0;
    virtual void AddInlineFunctionAttribute(void* function) = 0;
    virtual void SetFunctionLinkage(void* function, bool setInline) = 0;
    virtual void SetFunctionLinkageToLinkOnceODRLinkage(void* function) = 0;
    virtual void SetFunctionCallConventionToStdCall(void* function) = 0;
    virtual void SetFunction(void* function_, int32_t fileIndex, const util::uuid& moduleId, const util::uuid& functionId) = 0;
    virtual void SetFunctionName(const std::string& functionName) = 0;
    virtual void SetFunctionComment(void* function, const std::string& functionComment) = 0;
    virtual void BeginScope() = 0;
    virtual void EndScope() = 0;
    virtual int16_t GetCurrentScopeId() const = 0;
    virtual void SetCurrentScopeId(int16_t scopeId) = 0;
    virtual int32_t AddControlFlowGraphNode() = 0;
    virtual void SetCurrentControlFlowGraphNodeId(int32_t controlFlowGraphNodeId) = 0;
    virtual void AddControlFlowGraphEdge(int32_t startNodeId, int32_t endNodeId) = 0;
    virtual void AddLocalVariable(const std::string& localVariableName, const util::uuid& typeId, void* irObject) = 0;
    virtual void BeginInstructionFlag(int16_t flag) = 0;
    virtual void EndInstructionFlag(int16_t flag) = 0;
    virtual void SetInPrologue(bool inPrologue_) = 0;
    virtual void* CreateSubroutineType(const std::vector<void*>& elementTypes) = 0;
    virtual unsigned GetPureVirtualVirtuality() = 0;
    virtual unsigned GetVirtualVirtuality() = 0;
    virtual unsigned GetFunctionFlags(bool isStatic, unsigned accessFlags, bool isExplicit) = 0;
    virtual void* CreateDIMethod(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        void* subroutineType, unsigned virtuality, unsigned vtableIndex, void* vtableHolder, unsigned flags) = 0;
    virtual void* CreateDIFunction(const std::string& name, const std::string& mangledName, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        void* subroutineType, unsigned flags) = 0;
    virtual void SetDISubprogram(void* function, void* subprogram) = 0;
    virtual void* CreateAlloca(void* irType) = 0;
    virtual void SetLocalMetadataRef(void* local, void* mdRef) {}
    virtual void* CreateDIParameterVariable(const std::string& name, int index, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        void* irType, void* allocaInst) = 0;
    virtual void* CreateDIAutoVariable(const std::string& name, const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen,
        void* irType, void* allocaInst) = 0;
    virtual void* GetFunctionArgument(void* function, int argumentIndex) = 0;
    virtual void SetDebugLoc(void* callInst) = 0;
    virtual void* CreateRet(void* value) = 0;
    virtual void* CreateRetVoid() = 0;
    virtual void SetPersonalityFunction(void* function, void* personalityFunction) = 0;
    virtual void AddNoUnwindAttribute(void* function) = 0;
    virtual void AddUWTableAttribute(void* function) = 0;
    virtual void* CreateLexicalBlock(const soul::ast::FullSpan& fullSpan, const soul::ast::LineColLen& lineColLen) = 0;
    virtual void* CreateSwitch(void* condition, void* defaultDest, unsigned numCases) = 0;
    virtual void AddCase(void* switchInst, void* caseValue, void* caseDest) = 0;
    virtual void* GenerateTrap(const std::vector<void*>& args) = 0;
    virtual void SetCompileUnitId(const std::string& compileUnitId) = 0;
    virtual const std::string& CompileUnitId() const = 0;
    virtual void SetCompileUnitMetadataRef(void* metadataRef) {}
    virtual void* GetClsIdValue(const std::string& typeId) = 0;
    virtual void* CreateMDBool(bool value) = 0;
    virtual void* CreateMDLong(int64_t value) = 0;
    virtual void* CreateMDString(const std::string& value) = 0;
    virtual void* CreateMDStructRef(int id) = 0;
    virtual void* CreateMDStruct() = 0;
    virtual void* CreateMDArray() = 0;
    virtual void* CreateMDBasicBlockRef(void* bb) = 0;
    virtual int GetMDStructId(void* mdStruct) = 0;
    virtual void AddMDItem(void* mdStruct, const std::string& fieldName, void* mdItem) = 0;
    virtual void AddMDArrayItem(void* mdArray, void* mdItem) = 0;
    virtual void SetFunctionMdId(void* function, int mdId) = 0;
    virtual void* GetMDStructRefForSourceFile(const std::string& sourceFileName) = 0;
    virtual void SetCurrentSourcePos(int32_t lineNumber, int16_t scol, int16_t ecol) = 0;
    virtual void SetCurrentLineColLen(const soul::ast::LineColLen& lineColLen) = 0;
    virtual int32_t GetLineColLenIndex(const soul::ast::LineColLen& lineColLen) const = 0;
    bool GenerateLocationInfo() const { return generateLocationInfo; }
    void SetGenerateLocationInfo(bool generateLocationInfo_) { generateLocationInfo = generateLocationInfo_; }
    virtual void SetSpan(const soul::ast::Span& span) {}
    virtual void SetMetadataRef(void* inst, void* mdStructRef) = 0;
    virtual void SetMetadataRefForStructType(void* structType, void* mdRef) = 0;
    virtual void* GetMetadataRefForStructType(void* structType) const = 0;
    virtual void SetMetadataRefForArrayType(void* arrayType, void* mdRef) = 0;
    virtual void* GetMetadataRefForArrayType(void* arrayType) const = 0;
    virtual void SetMetadataRefForFunctionPointerType(void* functionPointerType, void* mdRef) = 0;
    virtual void* GetMetadataRefForFunctionPointerType(void* functionPointerType) const = 0;
    virtual int GetTypeId(void* type) const = 0;
    virtual int GetBaseTypeId(void* type) const = 0;
    virtual void FinalizeFunction(void* function, bool hasCleanup) = 0;
    virtual int Install(const std::string& str) = 0;
    virtual int Install(const std::u16string& str) = 0;
    virtual int Install(const std::u32string& str) = 0;
    virtual void* CreateLandingPad(void* lpType) = 0;
    virtual void SetLandindPadAsCleanup(void* landingPad) = 0;
    virtual void MoveAllocaIntoBasicBlock(void* allocaInst, void* lastAlloca, void* basicBlock) = 0;
    virtual void AddClauseToLangdingPad(void* landingPad, void* exceptionTypeId) = 0;
    virtual void* CreateExtractValue(void* aggregate, const std::vector<unsigned int>& indeces) = 0;
    virtual void* CreateInsertValue(void* aggregate, void* value, const std::vector<unsigned int>& indeces) = 0;
    virtual void* CreateUndefValue(void* type) = 0;
    virtual void CreateResume(void* exception) = 0;
    virtual void DebugPrintDebugInfo(const std::string& filePath) = 0;
    virtual void BeginSubstituteLineNumber(int32_t lineNumber) = 0;
    virtual void EndSubstituteLineNumber() = 0;
    virtual void SetBoundCompileUnit(void* boundCompileUnit) = 0;
    virtual void* GetBoundCompileUnit() const = 0;
    virtual void PrintModule() = 0;
    virtual void SetCurrentFunctionMain() = 0;
    void* Context() { return context; }
    void SetContext(void* context_) { context = context_; }
private:
    ValueStack* stack;
    bool generateLocationInfo;
    void* context;
};

} // namespace cmajor::ir

