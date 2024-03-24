// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.strings;

class Strings
{
public:
    static Strings& Instance();
    int AllocateString(const char* s);
    void FreeString(int handle);
    const char* GetString(int handle);
private:
    Strings();
    std::vector<std::unique_ptr<std::string>> strings;
    std::mutex mtx;
};

Strings& Strings::Instance()
{
    static Strings instance;
    return instance;
}

Strings::Strings()
{
    strings.push_back(std::unique_ptr<std::string>(new std::string()));
}

int Strings::AllocateString(const char* s)
{
    std::lock_guard<std::mutex> lock(mtx);
    int handle = strings.size();
    strings.push_back(std::unique_ptr<std::string>(new std::string(s)));
    return handle;
}

void Strings::FreeString(int handle)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (handle > 0 && handle < strings.size())
    {
        strings[handle].get()->clear();
    }
}

const char* Strings::GetString(int handle) 
{
    std::lock_guard<std::mutex> lock(mtx);
    if (handle > 0 && handle < strings.size())
    {
        return strings[handle].get()->c_str();
    }
    return "";
}

int RtmAllocateString(const char* s)
{
    return Strings::Instance().AllocateString(s);
}

void RtmFreeString(int stringHandle)
{
    Strings::Instance().FreeString(stringHandle);
}

const char* RtmGetString(int stringHandle)
{
    return Strings::Instance().GetString(stringHandle);
}
