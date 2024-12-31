// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.global.flags;

namespace cmajor::symbols {

BackEnd backend = BackEnd::llvm;
GlobalFlags globalFlags = GlobalFlags::none;
int optimizationLevel = -1;
int numBuildThreads = -1;
std::string pass;

void SetBackEnd(BackEnd backend_)
{
    backend = backend_;
}

BackEnd GetBackEnd()
{
    return backend;
}

inline GlobalFlags operator|(GlobalFlags flags, GlobalFlags flag)
{
    return GlobalFlags(uint64_t(flags) | uint64_t(flag));
}

inline GlobalFlags operator&(GlobalFlags flags, GlobalFlags flag)
{
    return GlobalFlags(uint64_t(flags) & uint64_t(flag));
}

inline GlobalFlags operator~(GlobalFlags flags)
{
    return GlobalFlags(~uint64_t(flags));
}

void SetGlobalFlag(GlobalFlags flag)
{
    globalFlags = globalFlags | flag;
}

void ResetGlobalFlag(GlobalFlags flag)
{
    globalFlags = globalFlags & ~flag;
}

bool GetGlobalFlag(GlobalFlags flag)
{
    return (globalFlags & flag) != GlobalFlags::none;
}

std::string GetConfig()
{
    std::string config = "debug";
    if (GetGlobalFlag(GlobalFlags::release))
    {
        if (GetGlobalFlag(GlobalFlags::profile))
        {
            config = "profile";
        }
        else
        {
            config = "release";
        }
    }
    else if (GetGlobalFlag(GlobalFlags::trace))
    {
        config = "trace";
    }
    return config;
}

int GetOptimizationLevel()
{
    if (GetGlobalFlag(GlobalFlags::release))
    {
        if (optimizationLevel == -1)
        {
            return 2;
        }
        else
        {
            return optimizationLevel;
        }
    }
    else
    {
        return 0;
    }
}

void SetOptimizationLevel(int optimizationLevel_)
{
    optimizationLevel = optimizationLevel_;
}

std::string Pass()
{
    return pass;
}

void SetPass(const std::string& pass_)
{
    pass = pass_;
}

int GetNumBuildThreads()
{
    return numBuildThreads;
}

void SetNumBuildThreads(int numBuildThreads_)
{
    numBuildThreads = numBuildThreads_;
}

std::set<std::u32string> commandLineDefines;

void DefineCommandLineConditionalSymbol(const std::u32string& symbol)
{
    commandLineDefines.insert(symbol);
}

std::set<std::u32string> GetCommandLineDefines()
{
    return commandLineDefines;
}

std::string compilerVersion;

void SetCompilerVersion(const std::string& compilerVersion_)
{
    compilerVersion = compilerVersion_;
}

std::string GetCompilerVersion()
{
    return compilerVersion;
}

bool inUnitTest = false;

bool BeginUnitTest()
{
    bool prevUnitTest = inUnitTest;
    inUnitTest = true;
    return prevUnitTest;
}

bool InUnitTest()
{
    return inUnitTest;
}

int32_t unitTestAssertionNumber = 0;

void ResetUnitTestAssertionNumber()
{
    unitTestAssertionNumber = 0;
}

int32_t GetNextUnitTestAssertionNumber()
{
    return unitTestAssertionNumber++;
}

int32_t GetNumUnitTestAssertions()
{
    return unitTestAssertionNumber;
}

void EndUnitTest(bool prevUnitTest)
{
    inUnitTest = prevUnitTest;
}

std::vector<int32_t>* assertionLineNumberVector = nullptr;

void SetAssertionLineNumberVector(std::vector<int32_t>* assertionLineNumberVector_)
{
    assertionLineNumberVector = assertionLineNumberVector_;
}

void AddAssertionLineNumber(int32_t lineNumber)
{
    if (assertionLineNumberVector)
    {
        assertionLineNumberVector->push_back(lineNumber);
    }
}

void ResetGlobalFlags()
{
    backend = BackEnd::llvm;
    globalFlags = GlobalFlags::none;
    optimizationLevel = -1;
    numBuildThreads = -1;
    commandLineDefines.clear();
    inUnitTest = false;
    unitTestAssertionNumber = 0;
    assertionLineNumberVector = nullptr;
}

std::string unitTestFilePath;
std::string unitTestName;

void SetUnitTestFilePath(const std::string& filePath)
{
    unitTestFilePath = filePath;
}

const std::string& UnitTestFilePath()
{
    return unitTestFilePath;
}

void SetUnitTestName(const std::string& name)
{
    unitTestName = name;
}

const std::string& UnitTestName()
{
    return unitTestName;
}

} // namespace cmajor::symbols
