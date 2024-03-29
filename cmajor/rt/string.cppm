// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.rt.string;

import std.core;

export namespace cmajor::rt {

class StringTable
{
public:
    static StringTable& Instance();
    int32_t InstallString(const std::string& str);
    const char* GetString(int32_t stringHandle);
    void DisposeString(int32_t stringHandle);
private:
    StringTable();
    int32_t nextStringHandle;
    std::unordered_map<int32_t, std::string> stringMap;
};

int32_t InstallString(const std::string& str);
const char* GetString(int32_t stringHandle);
void DisposeString(int32_t stringHandle);

} // namespace cmajor::rt

