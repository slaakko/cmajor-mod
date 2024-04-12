// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.classes;

import cmajor.masm.rt.io;

import util;

class ClassInfo
{
public:
    ClassInfo();
    void Read(util::BinaryStreamReader& reader);
    const util::uuid& TypeId() const { return typeId; }
    const util::uuid& BaseTypeId() const { return baseTypeId; }
    const std::string& ClassName() const { return className; }
private:
    util::uuid typeId;
    util::uuid baseTypeId;
    std::string className;
};

ClassInfo::ClassInfo() : typeId(), baseTypeId(), className()
{
}

void ClassInfo::Read(util::BinaryStreamReader& reader)
{
    reader.ReadUuid(typeId);
    reader.ReadUuid(baseTypeId);
    className = reader.ReadUtf8String();
}

class ClassIndex
{
public:
    void Read(util::BinaryStreamReader& reader);
    ClassInfo* GetClassInfo(const util::uuid& id) const;
private:
    std::map<util::uuid, ClassInfo*> classInfoMap;
    std::vector<std::unique_ptr<ClassInfo>> classInfos;
};

void ClassIndex::Read(util::BinaryStreamReader& reader)
{
    int32_t n = reader.ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        ClassInfo* classInfo = new ClassInfo();
        classInfos.push_back(std::unique_ptr<ClassInfo>(classInfo));
        classInfo->Read(reader);
        classInfoMap[classInfo->TypeId()] = classInfo;
    }
}

ClassInfo* ClassIndex::GetClassInfo(const util::uuid& id) const
{
    auto it = classInfoMap.find(id);
    if (it != classInfoMap.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

std::mutex classMtx;

ClassIndex* classIndex = nullptr;

ClassIndex* GetClassIndex()
{
    std::lock_guard<std::mutex> lock(classMtx);
    if (!classIndex)
    {
        classIndex = new ClassIndex();
        std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
        util::BinaryResourcePtr resource(currentExecutableName, "CLASS_INDEX");
        util::MemoryStream memoryStream(resource.Data(), resource.Size());
        util::BinaryStreamReader reader(memoryStream);
        classIndex->Read(reader);
    }
    return classIndex;
}

const char* RtmClassName(void* typeId)
{
    try
    {
        if (!typeId)
        {
            throw std::runtime_error("error: RtmClassName: type id is null");
        }
        util::uuid* id = static_cast<util::uuid*>(typeId);
        ClassIndex* classIndex = GetClassIndex();
        ClassInfo* classInfo = classIndex->GetClassInfo(*id);
        if (classInfo)
        {
            return classInfo->ClassName().c_str();
        }
        else
        {
            throw std::runtime_error("error: RtmClassName: class info not found");
        }
    }
    catch (const std::exception& ex)
    {
        int32_t errorId = 0;
        void* stderr = RtmOpenStdFile(2, errorId);
        std::string errorMessage = "error: ";
        errorMessage.append(ex.what()).append("\n");
        RtmPrintToFile(stderr, errorMessage.c_str());
        std::exit(1);
    }
}

bool IsSameOrDerivedFrom(const util::uuid& left, const util::uuid& right)
{
    if (left == right) return true;
    ClassIndex* classIndex = GetClassIndex();
    ClassInfo* classInfo = classIndex->GetClassInfo(left);
    if (!classInfo)
    {
        throw std::runtime_error("error: RtmIs: class info not found");
    }
    if (classInfo->BaseTypeId().is_nil())
    {
        return false;
    }
    return IsSameOrDerivedFrom(classInfo->BaseTypeId(), right);
}

bool RtmIs(void* typeId, void* baseTypeId)
{
    try
    {
        if (!typeId)
        {
            throw std::runtime_error("error: RtmIs: type id is null");
        }
        if (!baseTypeId)
        {
            throw std::runtime_error("error: RtmIs: baseTypeId id is null");
        }
        util::uuid* idLeft = static_cast<util::uuid*>(typeId);
        util::uuid* idRight = static_cast<util::uuid*>(baseTypeId);
        return IsSameOrDerivedFrom(*idLeft, *idRight);
    }
    catch (const std::exception& ex)
    {
        int32_t errorId = 0;
        void* stderr = RtmOpenStdFile(2, errorId);
        std::string errorMessage = "error: ";
        errorMessage.append(ex.what()).append("\n");
        RtmPrintToFile(stderr, errorMessage.c_str());
        std::exit(1);
    }
}

std::recursive_mutex staticInitMutex;

void RtmBeginStaticInitCriticalSection()
{
    staticInitMutex.lock();
}

void RtmEndtaticInitCriticalSection()
{
    staticInitMutex.unlock();
}

using Destructor = void (*)(void*);

class DestructionList
{
public:
    static DestructionList& Instance();
    void EnqueueDestruction(void* destructor, void* destructorThisArg);
    void RunDestructors();
private:
    std::vector<std::pair<void*, void*>> destructionList;
    DestructionList();
};

DestructionList::DestructionList()
{
}

DestructionList& DestructionList::Instance()
{
    static DestructionList instance;
    return instance;
}

void DestructionList::EnqueueDestruction(void* destructor, void* destructorThisArg)
{
    destructionList.push_back(std::make_pair(destructor, destructorThisArg));
}

void DestructionList::RunDestructors()
{
    int n = destructionList.size();
    for (int i = n - 1; i >= 0; --i)
    {
        std::pair<void*, void*>& destruction = destructionList[i];
        Destructor destructor = static_cast<Destructor>(destruction.first);
        destructor(destruction.second);
    }
    destructionList.clear();
}

void RtmEnqueueDestruction(void* destructor, void* destructorThisArg)
{
    DestructionList::Instance().EnqueueDestruction(destructor, destructorThisArg);
}

namespace cmajor::masm::rt {

void RunDestructors()
{
    DestructionList::Instance().RunDestructors();
}

} // cmajor::masm::rt;
