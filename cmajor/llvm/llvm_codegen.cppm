// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.codegen;

import std.core;
import cmajor.codegen;
import cmajor.ir;
import cmajor.symbols;
import cmajor.binder;

export namespace cmajor::llvm {

struct Cleanup
{
    Cleanup(void* cleanupBlock_, void* handlerBlock_, cmajor::ir::Pad* currentPad_) : cleanupBlock(cleanupBlock_), handlerBlock(handlerBlock_), currentPad(currentPad_) {}
    void* cleanupBlock;
    void* handlerBlock;
    cmajor::ir::Pad* currentPad;
    std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>> destructors;
};

class LLVMCodeGenerator : public cmajor::codegen::CodeGenerator, public cmajor::ir::EmittingDelegate
{
public:
    LLVMCodeGenerator(cmajor::ir::Emitter* emitter_);
    cmajor::ir::Emitter* Emitter() const { return emitter; }
    void Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit) override;
    void Visit(cmajor::binder::BoundNamespace& boundNamespace) override;
    void Visit(cmajor::binder::BoundClass& boundClass) override;
    void Visit(cmajor::binder::BoundEnumTypeDefinition& boundEnumTypeDefinition) override;
    void Visit(cmajor::binder::BoundFunction& boundFunction) override;
    void Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement) override;
    void Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement) override;
    void Visit(cmajor::binder::BoundIfStatement& boundIfStatement) override;
    void Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement) override;
    void Visit(cmajor::binder::BoundDoStatement& boundDoStatement) override;
    void Visit(cmajor::binder::BoundForStatement& boundForStatement) override;
    void Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement) override;
    void Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement) override;
    void Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement) override;
    void Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement) override;
    void Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement) override;
    void Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement) override;
    void Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement) override;
    void Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement) override;
    void Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement) override;
    void Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement) override;
    void Visit(cmajor::binder::BoundParameter& boundParameter) override;
    void Visit(cmajor::binder::BoundLocalVariable& boundLocalVariable) override;
    void Visit(cmajor::binder::BoundMemberVariable& boundMemberVariable) override;
    void Visit(cmajor::binder::BoundConstant& boundConstant) override;
    void Visit(cmajor::binder::BoundEnumConstant& boundEnumConstant) override;
    void Visit(cmajor::binder::BoundLiteral& boundLiteral) override;
    void Visit(cmajor::binder::BoundTemporary& boundTemporary) override;
    void Visit(cmajor::binder::BoundSizeOfExpression& boundSizeOfExpression) override;
    void Visit(cmajor::binder::BoundAddressOfExpression& boundAddressOfExpression) override;
    void Visit(cmajor::binder::BoundDereferenceExpression& boundDereferenceExpression) override;
    void Visit(cmajor::binder::BoundReferenceToPointerExpression& boundReferenceToPointerExpression) override;
    void Visit(cmajor::binder::BoundFunctionCall& boundFunctionCall) override;
    void Visit(cmajor::binder::BoundDelegateCall& boundDelegateCall) override;
    void Visit(cmajor::binder::BoundClassDelegateCall& boundClassDelegateCall) override;
    void Visit(cmajor::binder::BoundConversion& boundConversion) override;
    void Visit(cmajor::binder::BoundConstructExpression& boundConstructExpression) override;
    void Visit(cmajor::binder::BoundConstructAndReturnTemporaryExpression& boundConstructAndReturnTemporaryExpression) override;
    void Visit(cmajor::binder::BoundClassOrClassDelegateConversionResult& boundClassOrClassDelegateConversionResult) override;
    void Visit(cmajor::binder::BoundIsExpression& boundIsExpression) override;
    void Visit(cmajor::binder::BoundAsExpression& boundAsExpression) override;
    void Visit(cmajor::binder::BoundTypeNameExpression& boundTypeNameExpression) override;
    void Visit(cmajor::binder::BoundBitCast& boundBitCast) override;
    void Visit(cmajor::binder::BoundFunctionPtr& boundFunctionPtr) override;
    void Visit(cmajor::binder::BoundDisjunction& boundDisjunction) override;
    void Visit(cmajor::binder::BoundConjunction& boundConjunction) override;
    void Visit(cmajor::binder::BoundGlobalVariable& boundGlobalVariable) override;
    void* GetGlobalStringPtr(int stringId) override;
    void* GetGlobalWStringConstant(int stringId, void*& arrayType) override;
    void* GetGlobalUStringConstant(int stringId, void*& arrayType) override;
    void* GetGlobalUuidConstant(int uuidId) override;
    void SetLineNumber(int32_t lineNumber) override;
    void* HandlerBlock() override;
    void* CleanupBlock() override;
    bool NewCleanupNeeded() override;
    bool InTryBlock() const override;
    int CurrentTryBlockId() const override;
    void CreateCleanup() override;
    std::string GetSourceFilePath(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    cmajor::ir::Pad* CurrentPad() override;
    void* CreateClassDIType(void* classPtr) override;
    int Install(const std::string& str) override;
    int Install(const std::u16string& str) override;
    int Install(const std::u32string& str) override;
protected:
    virtual void* GetPersonalityFunction() const = 0;
    virtual void GenerateCodeForCleanups() = 0;
    bool& DestructorCallGenerated() { return destructorCallGenerated; }
    bool& LastInstructionWasRet() { return lastInstructionWasRet; }
    bool& BasicBlockOpen() { return basicBlockOpen; }
    void SetTarget(cmajor::binder::BoundStatement* labeledStatement);
    cmajor::ir::Pad* CurrentPad() const { return currentPad; }
    void SetCurrentPad(cmajor::ir::Pad* currentPad_) { currentPad = currentPad_; }
    cmajor::binder::BoundStatement* SequenceSecond() const { return sequenceSecond; }
    void ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock);
    cmajor::binder::BoundFunction* CurrentFunction() const { return currentFunction; }
    void GenerateExitFunctionCode(cmajor::binder::BoundFunction& boundFunction);
    cmajor::binder::BoundCompoundStatement* CurrentBlock() const { return currentBlock; }
    cmajor::binder::BoundCompoundStatement* BreakTargetBlock() const { return breakTargetBlock; }
    cmajor::binder::BoundCompoundStatement* ContinueTargetBlock() const { return continueTargetBlock; }
    void* BreakTarget() const { return breakTarget; }
    void* ContinueTarget() const { return continueTarget; }
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* CurrentCaseMap() const { return currentCaseMap; }
    std::unordered_map<cmajor::binder::BoundStatement*, void*>& LabeledStatementMap() { return labeledStatementMap; }
    void* DefaultDest() const { return defaultDest; }
    void SetHandlerBlock(void* handlerBlock_) { handlerBlock = handlerBlock_; }
    void SetCleanupBlock(void* cleanupBlock_) { cleanupBlock = cleanupBlock_; }
    std::vector<std::unique_ptr<cmajor::ir::Pad>>& Pads() { return pads; }
    std::vector<cmajor::binder::BoundCompoundStatement*>& Blocks() { return blocks; }
    std::unordered_map<cmajor::binder::BoundCompoundStatement*, std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>>& BlockDestructionMap() { return blockDestructionMap; }
    void SetNewCleanupNeeded(bool newCleanupNeeded_) { newCleanupNeeded = newCleanupNeeded_; }
    std::vector<std::unique_ptr<Cleanup>>& Cleanups() { return cleanups; }
private:
    cmajor::symbols::SymbolTable* symbolTable;
    cmajor::symbols::Module* symbolsModule;
    cmajor::ir::Emitter* emitter;
    void* module;
    bool debugInfo;
    cmajor::binder::BoundCompileUnit* compileUnit;
    cmajor::binder::BoundClass* currentClass;
    cmajor::binder::BoundFunction* currentFunction;
    std::stack<cmajor::binder::BoundClass*> classStack;
    std::unordered_map<int, void*> utf8stringMap;
    std::unordered_map<int, std::pair<void*, void*>> utf16stringMap;
    std::unordered_map<int, std::pair<void*, void*>> utf32stringMap;
    std::unordered_map<int, void*> uuidMap;
    void* trueBlock;
    void* falseBlock;
    void* breakTarget;
    void* continueTarget;
    void* handlerBlock;
    void* cleanupBlock;
    void* entryBasicBlock;
    bool newCleanupNeeded;
    cmajor::ir::Pad* currentPad;
    int prevLineNumber;
    bool destructorCallGenerated;
    bool lastInstructionWasRet;
    bool basicBlockOpen;
    void* lastAlloca;
    int compoundLevel;
    std::string compileUnitId;
    cmajor::binder::BoundStatement* sequenceSecond;
    cmajor::binder::BoundCompoundStatement* currentBlock;
    std::vector<std::unique_ptr<Cleanup>> cleanups;
    std::vector<std::unique_ptr<cmajor::ir::Pad>> pads;
    std::unordered_map<cmajor::binder::BoundStatement*, void*> labeledStatementMap;
    void* function;
    std::unordered_map<cmajor::binder::BoundCompoundStatement*, std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>> blockDestructionMap;
    std::vector<cmajor::binder::BoundCompoundStatement*> blocks;
    bool genJumpingBoolCode;
    cmajor::binder::BoundCompoundStatement* breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* continueTargetBlock;
    void* defaultDest;
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* currentCaseMap;
    std::set<cmajor::symbols::FunctionSymbol*> compileUnitFunctions;
    void GenJumpingBoolCode();
    void GenerateInitUnwindInfoFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit);
    void GenerateInitCompileUnitFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit);
    void GenerateGlobalInitFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit);
    void GenerateEnterFunctionCode(cmajor::binder::BoundFunction& boundFunction);
};

} // namespace cmajor::llvm
