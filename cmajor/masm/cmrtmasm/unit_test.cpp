// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.unit_test;

import cmajor.masm.rt.init_done;
import util;
import soul.xml.dom;

const int assertionResultEmpty = 0;
const int assertionResultPassed = 1;
const int assertionResulFailed = 2;

struct AssertionResult
{
    AssertionResult(int result_, int32_t line_) : result(result_), line(line_) {}
    int result;
    int32_t line;
};

class UnitTestEngine
{
public:
    UnitTestEngine();
    void StartUnitTest(int numAssertions_, const std::string& unitTestFilePath_);
    void EndUnitTest(const std::string& testName, int32_t exitCode);
    void SetUnitTestAssertionResult(int32_t assertionIndex, bool assertionResult, int32_t line);
private:
    int numAssertions;
    std::string unitTestFilePath;
    std::vector<AssertionResult> assertionResults;
};

UnitTestEngine::UnitTestEngine(): numAssertions(0)
{
}

void UnitTestEngine::StartUnitTest(int numAssertions_, const std::string& unitTestFilePath_)
{
    numAssertions = numAssertions_;
    unitTestFilePath = unitTestFilePath_;
    for (int32_t i = 0; i < numAssertions; ++i)
    {
        assertionResults.push_back(AssertionResult(assertionResultEmpty, 0));
    }
}

void UnitTestEngine::EndUnitTest(const std::string& testName, int32_t exitCode)
{
    try
    {
        std::ofstream testXmlFile(unitTestFilePath);
        if (!testXmlFile)
        {
            throw std::runtime_error("could not open '" + unitTestFilePath + "'");
        }
        util::CodeFormatter formatter(testXmlFile);
        formatter.SetIndentSize(2);
        soul::xml::Document document;
        std::unique_ptr<soul::xml::Element> testElement(soul::xml::MakeElement("test"));
        testElement->SetAttribute("name", testName);
        for (int32_t i = 0; i < numAssertions; ++i)
        {
            std::unique_ptr<soul::xml::Element> assertionElement(soul::xml::MakeElement("assertion"));
            assertionElement->SetAttribute("index", std::to_string(i));
            const AssertionResult& assertionResult = assertionResults[i];
            std::string assertionResultStr = "empty";
            if (assertionResult.result == assertionResultPassed)
            {
                assertionResultStr = "passed";
            }
            else if (assertionResult.result == assertionResulFailed)
            {
                assertionResultStr = "failed";
            }
            assertionElement->SetAttribute("result", assertionResultStr);
            assertionElement->SetAttribute("line", std::to_string(assertionResult.line));
            testElement->AppendChild(assertionElement.release());
        }
        testElement->SetAttribute("exitCode", std::to_string(exitCode));
        testElement->SetAttribute("count", std::to_string(numAssertions));
        document.AppendChild(testElement.release());
        document.Write(formatter);
    }
    catch (std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
    }
}

void UnitTestEngine::SetUnitTestAssertionResult(int32_t assertionIndex, bool assertionResult, int32_t line)
{
    AssertionResult& ar = assertionResults[assertionIndex];
    if (assertionResult)
    {
        if (ar.result == assertionResultEmpty || ar.result == assertionResultPassed)
        {
            assertionResults[assertionIndex] = AssertionResult(assertionResultPassed, line);
        }
        else
        {
            assertionResults[assertionIndex] = AssertionResult(assertionResulFailed, line);
        }
    }
    else
    {
        assertionResults[assertionIndex] = AssertionResult(assertionResulFailed, line);
    }
}

std::unique_ptr<UnitTestEngine> engine;

void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath)
{
    RtmInit();
    engine.reset(new UnitTestEngine());
    engine->StartUnitTest(numAssertions, unitTestFilePath);
}

void RtmEndUnitTest(const char* testName, int32_t exitCode)
{
    engine->EndUnitTest(testName, exitCode);
    RtmDone();
}

void RtmSetUnitTestAssertionResult(int32_t assertionIndex, bool assertionResult, int32_t line)
{
    engine->SetUnitTestAssertionResult(assertionIndex, assertionResult, line);
}

enum class Type
{
    sbyte, byte, short_, ushort, int_, uint, long_, ulong
};

enum class UnaryOp
{
    plus, neg, cpl
};

template<typename T>
int64_t EvalUnaryOp(T operand, UnaryOp op)
{
    switch (op)
    {
        case UnaryOp::plus: return static_cast<int64_t>(static_cast<T>(+operand));
        case UnaryOp::neg: return static_cast<int64_t>(static_cast<T>(-operand));
        case UnaryOp::cpl: return static_cast<int64_t>(static_cast<T>(~operand));
    }
    return 0;
}

int64_t RtmEvaluateUnaryOp(int64_t operand, int type, int operation)
{
    Type t = static_cast<Type>(type);
    UnaryOp op = static_cast<UnaryOp>(operation);
    switch (t)
    {
        case Type::sbyte: return EvalUnaryOp<int8_t>(static_cast<int8_t>(operand), op);
        case Type::byte: return EvalUnaryOp<uint8_t>(static_cast<uint8_t>(operand), op);
        case Type::short_: return EvalUnaryOp<int16_t>(static_cast<int16_t>(operand), op);
        case Type::ushort: return EvalUnaryOp<uint16_t>(static_cast<uint16_t>(operand), op);
        case Type::int_: return EvalUnaryOp<int32_t>(static_cast<int32_t>(operand), op);
        case Type::uint: return EvalUnaryOp<uint32_t>(static_cast<uint32_t>(operand), op);
        case Type::long_: return EvalUnaryOp<int64_t>(static_cast<int64_t>(operand), op);
        case Type::ulong: return EvalUnaryOp<uint64_t>(static_cast<uint64_t>(operand), op);
    }
    return 0;
}

enum class BinOp
{
    add, sub, mul, div, mod, and_, or_, xor_, shl, shr
};

template<typename T>
int64_t EvalBinOp(T left, T right, BinOp op)
{
    switch (op)
    {
        case BinOp::add: return static_cast<int64_t>(static_cast<T>(left + right));
        case BinOp::sub: return static_cast<int64_t>(static_cast<T>(left - right));
        case BinOp::mul: return static_cast<int64_t>(static_cast<T>(left * right));
        case BinOp::div: return static_cast<int64_t>(static_cast<T>(left / right));
        case BinOp::mod: return static_cast<int64_t>(static_cast<T>(left % right));
        case BinOp::and_: return static_cast<int64_t>(static_cast<T>(left & right));
        case BinOp::or_: return static_cast<int64_t>(static_cast<T>(left | right));
        case BinOp::xor_: return static_cast<int64_t>(static_cast<T>(left ^ right));
        case BinOp::shl: return static_cast<int64_t>(static_cast<T>(left << right));
        case BinOp::shr: return static_cast<int64_t>(static_cast<T>(left >> right));
    }
    return 0;
}

int64_t RtmEvaluateBinOp(int64_t left, int64_t right, int type, int operation)
{
    Type t = static_cast<Type>(type);
    BinOp op = static_cast<BinOp>(operation);
    switch (t)
    {
        case Type::sbyte: return EvalBinOp<int8_t>(static_cast<int8_t>(left), static_cast<int8_t>(right), op);
        case Type::byte: return EvalBinOp<uint8_t>(static_cast<uint8_t>(left), static_cast<uint8_t>(right), op);
        case Type::short_: return EvalBinOp<int16_t>(static_cast<int16_t>(left), static_cast<int16_t>(right), op);
        case Type::ushort: return EvalBinOp<uint16_t>(static_cast<uint16_t>(left), static_cast<uint16_t>(right), op);
        case Type::int_: return EvalBinOp<int32_t>(static_cast<int32_t>(left), static_cast<int32_t>(right), op);
        case Type::uint: return EvalBinOp<uint32_t>(static_cast<uint32_t>(left), static_cast<uint32_t>(right), op);
        case Type::long_: return EvalBinOp<int64_t>(static_cast<int64_t>(left), static_cast<int64_t>(right), op);
        case Type::ulong: return EvalBinOp<uint64_t>(static_cast<uint64_t>(left), static_cast<uint64_t>(right), op);
    }
    return 0;
}

float RtmEvaluateFloatUnaryOp(float operand, int operation)
{
    UnaryOp op = static_cast<UnaryOp>(operation);
    switch (op)
    {
        case UnaryOp::plus: { return +operand; }
        case UnaryOp::neg: { return -operand; }
    }
    return 0.0f;
}

float RtmEvaluateFloatBinaryOp(float left, float right, int operation)
{
    BinOp op = static_cast<BinOp>(operation);
    switch (op)
    {
        case BinOp::add: return static_cast<float>(left + right);
        case BinOp::sub: return static_cast<float>(left - right);
        case BinOp::mul: return static_cast<float>(left * right);
        case BinOp::div: return static_cast<float>(left / right);
    }
    return 0.0f;
}

double RtmEvaluateDoubleUnaryOp(double operand, int operation)
{
    UnaryOp op = static_cast<UnaryOp>(operation);
    switch (op)
    {
        case UnaryOp::plus: { return +operand; }
        case UnaryOp::neg: { return -operand; }
    }
    return 0.0;
}

double RtmEvaluateDoubleBinaryOp(double left, double right, int operation)
{
    BinOp op = static_cast<BinOp>(operation);
    switch (op)
    {
        case BinOp::add: return static_cast<double>(left + right);
        case BinOp::sub: return static_cast<double>(left - right);
        case BinOp::mul: return static_cast<double>(left * right);
        case BinOp::div: return static_cast<double>(left / right);
    }
    return 0.0;
}
