// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.error;

void RtmPanic(const char* message)
{
    std::cerr << "error: " << message << "\n";
    std::exit(1);
}

void RtmFailAssertion(const char* assertion, const char* function, const char* sourceFilePath, int lineNumber)
{
    std::cerr << "assertion '" << assertion << "' failed: function=" << function << ", file=" << sourceFilePath << ", line=" << lineNumber << "\n";
    std::exit(1);
}
