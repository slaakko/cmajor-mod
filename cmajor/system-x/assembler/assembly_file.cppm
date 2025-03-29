// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.assembly.file;

import cmajor.systemx.assembler.instruction;
import cmajor.systemx.assembler.symbol;
import util;
import std.core;

export namespace cmajor::systemx::assembler {

enum class AssemblyObjectKind : int
{
    function, structure, externObjects, linkOnceObjects, debugInfo
};

enum class AssemblySectionKind : int
{
    link, code, data, debug
};

enum class FunctionPart : int
{
    prologue, body, epilogue
};

class AssemblyObject
{
public:
    AssemblyObject(AssemblyObjectKind kind_);
    virtual ~AssemblyObject();
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    AssemblyObjectKind kind;
};

class AssemblyFunction : public AssemblyObject
{
public:
    AssemblyFunction(const std::string& name_);
    AssemblyFunction(const AssemblyFunction&) = delete;
    AssemblyFunction& operator=(const AssemblyFunction&) = delete;
    void SetComment(const std::string& comment_);
    const std::string& Comment() const { return comment; }
    void SetActiveFunctionPart(FunctionPart functionPart);
    void AddInstruction(Instruction* inst);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string name;
    std::string comment;
    FunctionPart activeFunctionPart;
    std::vector<std::unique_ptr<Instruction>> prologue;
    std::vector<std::unique_ptr<Instruction>> body;
    std::vector<std::unique_ptr<Instruction>> epilogue;
};

class AssemblyStruct : public AssemblyObject
{
public:
    AssemblyStruct(const std::string& name_);
    AssemblyStruct(const AssemblyStruct&) = delete;
    AssemblyStruct& operator=(const AssemblyStruct&) = delete;
    void AddInstruction(Instruction* inst);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string name;
    std::vector<std::unique_ptr<Instruction>> content;
};

class AssemblyExternObject : public AssemblyObject
{
public:
    AssemblyExternObject();
    AssemblyExternObject(const AssemblyExternObject&) = delete;
    AssemblyExternObject& operator=(const AssemblyExternObject&) = delete;
    void AddExternSymbol(GlobalSymbol* externSymbol);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<Instruction>> content;
};

class AssemblyLinkOnceObject : public AssemblyObject
{
public:
    AssemblyLinkOnceObject();
    AssemblyLinkOnceObject(const AssemblyLinkOnceObject&) = delete;
    AssemblyLinkOnceObject& operator=(const AssemblyLinkOnceObject&) = delete;
    void AddLinkOnceSymbol(GlobalSymbol* linkOnceSymbol);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<Instruction>> content;
};

class AssemblyDebugInfo : public AssemblyObject
{
public:
    AssemblyDebugInfo();
    AssemblyDebugInfo(const AssemblyDebugInfo&) = delete;
    AssemblyDebugInfo operator=(const AssemblyDebugInfo&) = delete;
    void AddInstruction(Instruction* inst);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::vector<std::unique_ptr<Instruction>> content;
};

class AssemblySection
{
public:
    AssemblySection(AssemblySectionKind kind_);
    AssemblySection(const AssemblySection&) = delete;
    AssemblySection& operator=(const AssemblySection&) = delete;
    AssemblyExternObject* GetOrCreateExternObject();
    AssemblyLinkOnceObject* GetOrCreateLinkOnceObject();
    AssemblyFunction* CreateFunction(const std::string& name);
    AssemblyStruct* CreateStructure(const std::string& name);
    AssemblyDebugInfo* CreateDebugInfo();
    void Write(util::CodeFormatter& formatter);
private:
    AssemblySectionKind kind;
    std::vector<std::unique_ptr<AssemblyObject>> objects;
    AssemblyExternObject* externObject;
    AssemblyLinkOnceObject* linkOnceObject;
};

class AssemblyFile
{
public:
    AssemblyFile(const std::string& filePath_);
    AssemblySection* GetLinkSection();
    AssemblySection* GetCodeSection();
    AssemblySection* GetDataSection();
    AssemblySection* GetDebugSection();
    AssemblyFunction* CreateFunction(const std::string& name);
    void Write();
private:
    std::string filePath;
    std::ofstream file;
    util::CodeFormatter formatter;
    std::unique_ptr<AssemblySection> linkSection;
    std::unique_ptr<AssemblySection> codeSection;
    std::unique_ptr<AssemblySection> dataSection;
    std::unique_ptr<AssemblySection> debugSection;
};

} // namespace cmajor::systemx::assembler
