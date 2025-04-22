// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.backend.codegen;

import cmajor.systemx.backend.emitter;
import cmajor.codegen;
import cmajor.binder;
import cmajor.symbols;
import cmajor.ir;
import cmajor.systemx.ir;
import soul.ast.span;
import std.core;

export namespace cmajor::systemx::backend {

const int64_t fileInfoNodeType = 0;
const int64_t funcInfoNodeType = 1;
const int64_t lineInfoNodeType = 2;
const int64_t cfgNodeType = 3;
const int64_t beginTryNodeType = 4;
const int64_t endTryNodeType = 5;
const int64_t catchNodeType = 6;

struct Cleanup
{
    Cleanup(void* cleanupBlock_) : cleanupBlock(cleanupBlock_) {}
    void* cleanupBlock;
    std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>> destructors;
};

class SystemXCodeGenerator : public cmajor::codegen::CodeGenerator, public cmajor::ir::EmittingDelegate
{
public:
    SystemXCodeGenerator(cmajor::ir::Emitter* emitter_);
    SystemXCodeGenerator(const SystemXCodeGenerator&) = delete;
    SystemXCodeGenerator& operator=(const SystemXCodeGenerator&) = delete;
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
    int GetLineNumber(const soul::ast::Span& span);
    void AddCFGItem(int prevLineNumber, int nextLineNumber);
private:
    cmajor::ir::Emitter* emitter;
    cmajor::symbols::Context* context;
    cmajor::symbols::SymbolTable* symbolTable;
    cmajor::symbols::Module* module;
    cmajor::binder::BoundCompileUnit* compileUnit;
    soul::ast::FullSpan fullSpan;
    cmajor::systemx::ir::CompileUnit* nativeCompileUnit;
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
    cmajor::binder::BoundStatement* sequenceSecond;
    cmajor::binder::BoundFunction* currentFunction;
    cmajor::binder::BoundCompoundStatement* currentBlock;
    cmajor::binder::BoundCompoundStatement* breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* continueTargetBlock;
    std::unordered_map<cmajor::binder::BoundStatement*, void*> labeledStatementMap;
    std::unordered_map<cmajor::binder::BoundCompoundStatement*, std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>> blockDestructionMap;
    std::vector<std::unique_ptr<Cleanup>> cleanups;
    std::vector<cmajor::binder::BoundCompoundStatement*> blocks;
    void* lastAlloca;
    cmajor::binder::BoundClass* currentClass;
    std::stack<cmajor::binder::BoundClass*> classStack;
    bool basicBlockOpen;
    void* defaultDest;
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* currentCaseMap;
    std::unordered_map<int, void*> utf8stringMap;
    std::unordered_map<int, void*> utf16stringMap;
    std::unordered_map<int, void*> utf32stringMap;
    std::unordered_map<int, void*> uuidMap;
    std::string compileUnitId;
    bool generateLineNumbers;
    int64_t currentTryBlockId;
    int64_t nextTryBlockId;
    void* currentTryNextBlock;
    void* handlerBlock;
    void* cleanupBlock;
    bool newCleanupNeeded;
    bool inTryBlock;
    int beginLineNumber;
    int endLineNumber;
    void* cfg;
};

} // namespace cmajor::systemx::backend
