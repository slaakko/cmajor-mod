// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.rt.string;

namespace cmajor::rt {

StringTable& StringTable::Instance()
{
    static StringTable instance;
    return instance;
}

StringTable::StringTable() : nextStringHandle(1)
{
}

std::mutex mtx;

int32_t StringTable::InstallString(const std::string& str)
{
    std::lock_guard<std::mutex> lock(mtx);
    int32_t stringHandle = nextStringHandle++;
    stringMap[stringHandle] = str;
    return stringHandle;
}

const char* StringTable::GetString(int32_t stringHandle)
{
    std::lock_guard<std::mutex> lock(mtx);
    auto it = stringMap.find(stringHandle);
    if (it != stringMap.cend())
    {
        return it->second.c_str();
    }
    return "";
}

void StringTable::DisposeString(int32_t stringHandle)
{
    std::lock_guard<std::mutex> lock(mtx);
    stringMap.erase(stringHandle);
}

int32_t InstallString(const std::string& str)
{
    return StringTable::Instance().InstallString(str);
}

const char* GetString(int32_t stringHandle)
{
    return StringTable::Instance().GetString(stringHandle);
}

void DisposeString(int32_t stringHandle)
{
    StringTable::Instance().DisposeString(stringHandle);
}

} // namespace cmajor::rt
