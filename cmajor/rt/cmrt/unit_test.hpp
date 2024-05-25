// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_UNIT_TEST_INCLUDED
#define CMRT_CPP_UNIT_TEST_INCLUDED
#include <stdint.h>

extern "C" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);
extern "C" void RtmEndUnitTest(const char* testName, int32_t exitCode);
extern "C" void RtmSetUnitTestAssertionResult(int32_t assertionIndex, bool assertionResult, int32_t line);
extern "C" int64_t RtmEvaluateUnaryOp(int64_t left, int type, int operation);
extern "C" int64_t RtmEvaluateBinOp(int64_t left, int64_t right, int type, int operation);
extern "C" float RtmEvaluateFloatUnaryOp(float operand, int operation);
extern "C" float RtmEvaluateFloatBinaryOp(float left, float right, int operation);
extern "C" double RtmEvaluateDoubleUnaryOp(double operand, int operation);
extern "C" double RtmEvaluateDoubleBinaryOp(double left, double right, int operation);

#endif // CMRT_CPP_UNIT_TEST_INCLUDED
