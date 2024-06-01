// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.global.flags;

import std.core;

export namespace cmajor::symbols {

enum class GlobalFlags : uint64_t
{
    none = 0,
    verbose = 1 << 0,
    quiet = 1 << 1,
    release = 1 << 2,
    clean = 1 << 3,
    debugParsing = 1 << 4,
    emitLlvm = 1 << 5,
    emitOptLlvm = 1 << 6,
    //linkWithDebugRuntime = 1 << 7,
    linkUsingMsLink = 1 << 8,
    ide = 1 << 9,
    strictNothrow = 1 << 10,
    time = 1 << 11,
    info = 1 << 12,
    unitTest = 1 << 13,
    profile = 1 << 14,
    generateDebugInfo = 1 << 15,
    msbuild = 1 << 16,
    ast2xml = 1 << 17,
    sym2xml = 1 << 18,
    bdt2xml = 1 << 19,
    cmdoc = 1 << 20,
    optimizeCmDoc = 1 << 21,
    singleThreadedCompile = 1 << 22,
    debugCompile = 1 << 23,
    rebuild = 1 << 24,
    disableCodeGen = 1 << 25,
    disableSystem = 1 << 26,
    justMyCodeDebugging = 1 << 27,
    buildAll = 1 << 28,
    printDebugMessages = 1 << 29,
    repository = 1 << 30,
    updateSourceFileModuleMap = 1ll << 31,
    trace = 1ll << 32,
    cpp = 1ll << 33,
    fnxml = 1ll << 34,
    dtxml = 1ll << 35,
    print = 1ll << 36
};

enum class BackEnd : int
{
    llvm = 0, systemx = 1, cpp = 2, masm = 3
};

void SetBackEnd(BackEnd backend_);
BackEnd GetBackEnd();

void ResetGlobalFlags();
void SetGlobalFlag(GlobalFlags flag);
void ResetGlobalFlag(GlobalFlags flag);
bool GetGlobalFlag(GlobalFlags flag);

std::string GetConfig();
int GetOptimizationLevel();
void SetOptimizationLevel(int optimizationLevel_);
std::string Pass();
void SetPass(const std::string& passes_);
int GetNumBuildThreads();
void SetNumBuildThreads(int numBuildThreads_);

void SetCompilerVersion(const std::string& compilerVersion_);
std::string GetCompilerVersion();

void DefineCommandLineConditionalSymbol(const std::u32string& symbol);
std::set<std::u32string> GetCommandLineDefines();

bool BeginUnitTest();
bool InUnitTest();
void ResetUnitTestAssertionNumber();
int32_t GetNextUnitTestAssertionNumber();
int32_t GetNumUnitTestAssertions();
void EndUnitTest(bool prevUnitTest);
void SetAssertionLineNumberVector(std::vector<int32_t>* assertionLineNumberVector_);
void AddAssertionLineNumber(int32_t lineNumber);
void SetUnitTestFilePath(const std::string& filePath);
const std::string& UnitTestFilePath();
void SetUnitTestName(const std::string& name);
const std::string& UnitTestName();

} // namespace cmajor::symbols
