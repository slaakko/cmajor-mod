// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.env;

const char* RtmGetEnvironmentVariable(const char* envVarName)
{
    char* env = std::getenv(envVarName);
    if (env && *env)
    {
        return env;
    }
    return "";
}
