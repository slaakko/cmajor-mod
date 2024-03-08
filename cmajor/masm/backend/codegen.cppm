// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.backend.codegen;

import cmajor.masm.backend.emitter;
import cmajor.codegen;
import cmajor.binder;
import cmajor.symbols;
import cmajor.ir;
import cmajor.masm.ir;
import soul.ast.span;
import std.core;

export namespace cmajor::masm::backend {

class MasmCodeGenerator : public cmajor::codegen::CodeGenerator, public cmajor::ir::EmittingDelegate
{
public:
    MasmCodeGenerator(cmajor::ir::Emitter* emitter_);
    void Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit) override;
    void Visit(cmajor::binder::BoundNamespace& boundNamespace) override;
    void Visit(cmajor::binder::BoundClass& boundClass) override;
    void Visit(cmajor::binder::BoundFunction& boundFunction) override;
    void Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement) override;
    void Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement) override;
    void Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement) override;
    void Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement) override;
    void Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement) override;
    void Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement) override;
    void Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement) override;
    void Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement) override;
    void Visit(cmajor::binder::BoundIfStatement& boundIfStatement) override;
    void Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement) override;
    void Visit(cmajor::binder::BoundDoStatement& boundDoStatement) override;
    void Visit(cmajor::binder::BoundForStatement& boundForStatement) override;
    void Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement) override;
    void Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement) override;
    void Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement) override;
    void Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement) override;
    void Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement) override;
    void Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement) override;
    void Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement) override;
    void Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement) override;
    void Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement) override;
    void Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement) override;
    void Visit(cmajor::binder::BoundTryStatement& boundTryStatement) override;
    void Visit(cmajor::binder::BoundCatchStatement& boundCatchStatement) override;
    void Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement) override;
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
    void GenJumpingBoolCode();
    void SetTarget(cmajor::binder::BoundStatement* labeledStatement);
    void ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock);
    void* GetGlobalStringPtr(int stringId) override;
    void* GetGlobalWStringConstant(int stringId, void*& arrayType) override;
    void* GetGlobalUStringConstant(int stringId, void*& arrayType) override;
    void* GetGlobalUuidConstant(int uuidId) override;
    void* HandlerBlock() override;
    void* CleanupBlock() override;
    bool NewCleanupNeeded() override;
    void CreateCleanup() override;
    void GenerateCodeForCleanups();
    bool InTryBlock() const override;
    int CurrentTryBlockId() const override;
    int Install(const std::string& str) override;
    int Install(const std::u16string& str) override;
    int Install(const std::u32string& str) override;
private:
    int fileIndex;
    cmajor::ir::Emitter* emitter;
    cmajor::symbols::SymbolTable* symbolTable;
    cmajor::symbols::Module* module;
    cmajor::binder::BoundCompileUnit* compileUnit;
    cmajor::masm::ir::CompileUnit* nativeCompileUnit;
    std::string compileUnitId;
    bool generateLineNumbers;
    void* function;
    void* entryBasicBlock;
    bool lastInstructionWasRet;
    bool prevWasTerminator;
    bool destructorCallGenerated;
    bool genJumpingBoolCode;
    void* trueBlock;
    void* falseBlock;
    void* breakTarget;
    void* continueTarget;
    bool basicBlockOpen;
    void* lastAlloca;
    void* currentTryNextBlock;
    void* handlerBlock;
    void* cleanupBlock;
    bool newCleanupNeeded;
    bool inTryBlock;
    int currentTryBlockId;
    int nextTryBlockId;
    cmajor::binder::BoundReturnStatement* latestRet;
    std::map<cmajor::binder::BoundStatement*, void*> labeledStatementMap;
    std::map<cmajor::binder::BoundCompoundStatement*, std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>> blockDestructionMap;
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* currentCaseMap;
    void* defaultDest;
    std::map<int, void*> utf8stringMap;
    std::map<int, void*> utf16stringMap;
    std::map<int, void*> utf32stringMap;
    std::map<int, void*> uuidMap;
    cmajor::binder::BoundStatement* sequenceSecond;
    std::stack<cmajor::binder::BoundClass*> classStack;
    cmajor::binder::BoundClass* currentClass;
    cmajor::binder::BoundFunction* currentFunction;
    cmajor::binder::BoundCompoundStatement* currentBlock;
    cmajor::binder::BoundCompoundStatement* breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* continueTargetBlock;
    std::vector<cmajor::binder::BoundCompoundStatement*> blocks;
};

} // namespace cmajor::masm::backend
