// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.assembly.file;

import util;

namespace cmajor::systemx::assembler {

AssemblyObject::AssemblyObject(AssemblyObjectKind kind_) : kind(kind_), creatorThreadId(std::this_thread::get_id())
{
}

AssemblyObject::~AssemblyObject()
{
}

AssemblyFunction::AssemblyFunction(const std::string& name_) : 
    AssemblyObject(AssemblyObjectKind::function), name(name_), activeFunctionPart(FunctionPart::body)
{
}

AssemblyFunction::~AssemblyFunction()
{
    prologue.clear(); 
    body.clear();
    epilogue.clear();
}

void AssemblyFunction::SetComment(const std::string& comment_)
{
    comment = comment_;
}

void AssemblyFunction::SetActiveFunctionPart(FunctionPart functionPart)
{
    activeFunctionPart = functionPart;
}

void AssemblyFunction::AddInstruction(Instruction* inst)
{
#ifdef THREAD_ID_CHECK
    if (inst->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    switch (activeFunctionPart)
    {
        case FunctionPart::prologue:
        {
            prologue.push_back(std::unique_ptr<Instruction>(inst));
            break;
        }
        case FunctionPart::body:
        {
            body.push_back(std::unique_ptr<Instruction>(inst));
            break;
        }
        case FunctionPart::epilogue:
        {
            epilogue.push_back(std::unique_ptr<Instruction>(inst));
            break;
        }
    }
}

void AssemblyFunction::Write(util::CodeFormatter& formatter)
{
    if (!comment.empty())
    {
        formatter.WriteLine("// " + comment);
        formatter.WriteLine();
    }
    formatter.WriteLine(name + " FUNC");
    formatter.IncIndent();
    for (const auto& instruction : prologue)
    {
#ifdef THREAD_ID_CHECK
        if (instruction->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        instruction->Write(formatter);
    }
    for (const auto& instruction : body)
    {
#ifdef THREAD_ID_CHECK
        if (instruction->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        instruction->Write(formatter);
    }
    for (const auto& instruction : epilogue)
    {
#ifdef THREAD_ID_CHECK
        if (instruction->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        instruction->Write(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine(name + " ENDF");
}

AssemblyStruct::AssemblyStruct(const std::string& name_) : AssemblyObject(AssemblyObjectKind::structure), name(name_)
{
}

AssemblyStruct::~AssemblyStruct()
{
    content.clear();
}

void AssemblyStruct::AddInstruction(Instruction* inst)
{
#ifdef THREAD_ID_CHECK
    if (inst->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    content.push_back(std::unique_ptr<Instruction>(inst));
}

void AssemblyStruct::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine(name + " STRUCT");
    formatter.IncIndent();
    for (const auto& instruction : content)
    {
#ifdef THREAD_ID_CHECK
        if (instruction->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        instruction->Write(formatter);
    }
    formatter.DecIndent();
    formatter.WriteLine(name + " ENDS");
}

AssemblyExternObject::AssemblyExternObject() : AssemblyObject(AssemblyObjectKind::externObjects)
{
}

AssemblyExternObject::~AssemblyExternObject()
{
    content.clear();
}

void AssemblyExternObject::AddExternSymbol(GlobalSymbol* externSymbol)
{
#ifdef THREAD_ID_CHECK
    if (externSymbol->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    content.push_back(std::unique_ptr<Instruction>(new Instruction(EXTERN)));
    content.back()->AddOperand(externSymbol);
}

void AssemblyExternObject::Write(util::CodeFormatter& formatter)
{
    formatter.IncIndent();
    for (const auto& inst : content)
    {
#ifdef THREAD_ID_CHECK
        if (inst->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        inst->Write(formatter);
    }
    formatter.DecIndent();
}

AssemblyLinkOnceObject::AssemblyLinkOnceObject() : AssemblyObject(AssemblyObjectKind::linkOnceObjects)
{
}

AssemblyLinkOnceObject::~AssemblyLinkOnceObject()
{
    content.clear();
}

void AssemblyLinkOnceObject::AddLinkOnceSymbol(GlobalSymbol* linkOnceSymbol)
{
#ifdef THREAD_ID_CHECK
    if (linkOnceSymbol->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    content.push_back(std::unique_ptr<Instruction>(new Instruction(LINKONCE)));
    content.back()->AddOperand(linkOnceSymbol);
}

void AssemblyLinkOnceObject::Write(util::CodeFormatter& formatter)
{
    formatter.IncIndent();
    for (const auto& inst : content)
    {
#ifdef THREAD_ID_CHECK
        if (inst->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        inst->Write(formatter);
    }
    formatter.DecIndent();
}

AssemblyDebugInfo::AssemblyDebugInfo() : AssemblyObject(AssemblyObjectKind::debugInfo)
{
}

AssemblyDebugInfo::~AssemblyDebugInfo()
{
    content.clear();
}

void AssemblyDebugInfo::AddInstruction(Instruction* inst)
{
#ifdef THREAD_ID_CHECK
    if (inst->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    content.push_back(std::unique_ptr<Instruction>(inst));
}

void AssemblyDebugInfo::Write(util::CodeFormatter& formatter)
{
    formatter.IncIndent();
    for (const auto& inst : content)
    {
#ifdef THREAD_ID_CHECK
        if (inst->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        inst->Write(formatter);
    }
    formatter.DecIndent();
}

AssemblySection::AssemblySection(AssemblySectionKind kind_) : kind(kind_), externObject(nullptr), linkOnceObject(nullptr), creatorThreadId(std::this_thread::get_id())
{
}

AssemblySection::~AssemblySection()
{
    objects.clear();
}

AssemblyExternObject* AssemblySection::GetOrCreateExternObject()
{
    if (!externObject)
    {
        externObject = new AssemblyExternObject();
        objects.push_back(std::unique_ptr<AssemblyObject>(externObject));
    }
#ifdef THREAD_ID_CHECK
    if (externObject->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return externObject;
}

AssemblyLinkOnceObject* AssemblySection::GetOrCreateLinkOnceObject()
{
    if (!linkOnceObject)
    {
        linkOnceObject = new AssemblyLinkOnceObject();
        objects.push_back(std::unique_ptr<AssemblyObject>(linkOnceObject));
    }
#ifdef THREAD_ID_CHECK
    if (linkOnceObject->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return linkOnceObject;
}

AssemblyFunction* AssemblySection::CreateFunction(const std::string& name)
{
    AssemblyFunction* function = new AssemblyFunction(name);
    objects.push_back(std::unique_ptr<AssemblyObject>(function));
    return function;
}

AssemblyStruct* AssemblySection::CreateStructure(const std::string& name)
{
    AssemblyStruct* structure = new AssemblyStruct(name);
    objects.push_back(std::unique_ptr<AssemblyObject>(structure));
    return structure;
}

AssemblyDebugInfo* AssemblySection::CreateDebugInfo()
{
    AssemblyDebugInfo* debugInfo = new AssemblyDebugInfo();
    objects.push_back(std::unique_ptr<AssemblyObject>(debugInfo));
    return debugInfo;
}

void AssemblySection::Write(util::CodeFormatter& formatter)
{
    switch (kind)
    {
        case AssemblySectionKind::link:
        {
            formatter.IncIndent();
            formatter.WriteLine(".LINK");
            formatter.WriteLine();
            formatter.DecIndent();
            break;
        }
        case AssemblySectionKind::code:
        {
            formatter.IncIndent();
            formatter.WriteLine(".CODE");
            formatter.WriteLine();
            formatter.DecIndent();
            break;
        }
        case AssemblySectionKind::data:
        {
            formatter.IncIndent();
            formatter.WriteLine(".DATA");
            formatter.WriteLine();
            formatter.DecIndent();
            break;
        }
        case AssemblySectionKind::debug:
        {
            formatter.IncIndent();
            formatter.WriteLine(".DEBUG");
            formatter.WriteLine();
            formatter.DecIndent();
            break;
        }
    }
    bool first = true;
    for (const auto& object : objects)
    {
#ifdef THREAD_ID_CHECK
        if (object->CreatorThreadId() != CreatorThreadId())
        {
            throw util::UnexpectedExecutorThread();
        }
#endif // THREAD_ID_CHECK
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        object->Write(formatter);
    }
}

AssemblyFile::AssemblyFile(const std::string& filePath_) : filePath(filePath_), file(filePath), formatter(file), creatorThreadId(std::this_thread::get_id())
{
    formatter.SetIndentSize(8);
}

AssemblyFile::~AssemblyFile()
{
    linkSection.reset();
    codeSection.reset(); 
    dataSection.reset();
    debugSection.reset();
}

AssemblyFunction* AssemblyFile::CreateFunction(const std::string& name)
{
    return GetCodeSection()->CreateFunction(name);
}

AssemblySection* AssemblyFile::GetLinkSection()
{
    if (!linkSection)
    {
        linkSection.reset(new AssemblySection(AssemblySectionKind::link));
    }
#ifdef THREAD_ID_CHECK
    if (linkSection->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return linkSection.get();
}

AssemblySection* AssemblyFile::GetCodeSection()
{
    if (!codeSection)
    {
        codeSection.reset(new AssemblySection(AssemblySectionKind::code));
    }
#ifdef THREAD_ID_CHECK
    if (codeSection->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return codeSection.get();
}

AssemblySection* AssemblyFile::GetDataSection()
{
    if (!dataSection)
    {
        dataSection.reset(new AssemblySection(AssemblySectionKind::data));
    }
#ifdef THREAD_ID_CHECK
    if (dataSection->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return dataSection.get();
}

AssemblySection* AssemblyFile::GetDebugSection()
{
    if (!debugSection)
    {
        debugSection.reset(new AssemblySection(AssemblySectionKind::debug));
    }
#ifdef THREAD_ID_CHECK
    if (debugSection->CreatorThreadId() != CreatorThreadId())
    {
        throw util::UnexpectedExecutorThread();
    }
#endif // THREAD_ID_CHECK
    return debugSection.get();
}

void AssemblyFile::Write()
{
    if (linkSection)
    {
        linkSection->Write(formatter);
    }
    if (codeSection)
    {
        if (linkSection)
        {
            formatter.WriteLine();
        }
        codeSection->Write(formatter);
    }
    if (dataSection)
    {
        if (linkSection || codeSection)
        {
            formatter.WriteLine();
        }
        dataSection->Write(formatter);
    }
    if (debugSection)
    {
        if (linkSection || codeSection || dataSection)
        {
            formatter.WriteLine();
        }
        debugSection->Write(formatter);
    }
}

} // namespace cmajor::systemx::assembler
