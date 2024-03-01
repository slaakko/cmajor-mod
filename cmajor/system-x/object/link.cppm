// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.link;

import std.core;

export namespace cmajor::systemx::object {

class ObjectFile;
class LinkSection;

enum class LinkCode : uint8_t
{
    once = 0, forwardLongJump = 1, forwardShortJump = 2, absoluteAddrValue = 3, farOcta = 4, clsid = 5, end = 6
};

std::string LinkCodeStr(LinkCode linkCode);

class ObjectFile;

class LinkCommand
{
public:
    LinkCommand(LinkCode linkCode_);
    LinkCommand(LinkCode linkCode_, uint64_t address_);
    virtual ~LinkCommand();
    virtual LinkCommand* Clone() const = 0;
    virtual std::string ToString() const = 0;
    virtual void Emit(LinkSection* linkSection);
    virtual void Read(LinkSection* linkSection);
    virtual void Apply(ObjectFile* objectFile, uint64_t value);
    LinkCode Code() const { return linkCode; }
    uint64_t Address() const { return address; }
    void SetId(int32_t id_) { id = id_; }
    int32_t Id() const { return id; }
private:
    LinkCode linkCode;
    uint64_t address;
    int32_t id;
};

class LinkOnceCommand : public LinkCommand
{
public:
    LinkOnceCommand();
    LinkCommand* Clone() const override;
    std::string ToString() const override;
};

class LinkForwardLongJumpCommand : public LinkCommand
{
public:
    LinkForwardLongJumpCommand();
    LinkForwardLongJumpCommand(uint64_t address_, int32_t symbolIndex_);
    void Emit(LinkSection* linkSection) override;
    void Read(LinkSection* linkSection) override;
    void Apply(ObjectFile* objectFile, uint64_t value) override;
    int32_t SymbolIndex() const { return symbolIndex; }
    LinkCommand* Clone() const override;
    std::string ToString() const override;
private:
    int32_t symbolIndex;
};

class LinkForwardShortJumpCommand : public LinkCommand
{
public:
    LinkForwardShortJumpCommand();
    LinkForwardShortJumpCommand(uint64_t address_, int32_t symbolIndex_);
    void Emit(LinkSection* linkSection) override;
    void Read(LinkSection* linkSection) override;
    void Apply(ObjectFile* objectFile, uint64_t value) override;
    int32_t SymbolIndex() const { return symbolIndex; }
    LinkCommand* Clone() const override;
    std::string ToString() const override;
private:
    int32_t symbolIndex;
};

class LinkAbsoluteAddressCommand : public LinkCommand
{
public:
    LinkAbsoluteAddressCommand();
    LinkAbsoluteAddressCommand(uint64_t address_, int32_t symbolIndex_);
    void Emit(LinkSection* linkSection) override;
    void Read(LinkSection* linkSection) override;
    void Apply(ObjectFile* objectFile, uint64_t value) override;
    int32_t SymbolIndex() const { return symbolIndex; }
    LinkCommand* Clone() const override;
    std::string ToString() const override;
private:
    int32_t symbolIndex;
};

class LinkFarOctaCommand : public LinkCommand
{
public:
    LinkFarOctaCommand();
    LinkFarOctaCommand(uint64_t address_, int32_t symbolIndex_);
    void Emit(LinkSection* linkSection) override;
    void Read(LinkSection* linkSection) override;
    void Apply(ObjectFile* objectFile, uint64_t value) override;
    int32_t SymbolIndex() const { return symbolIndex; }
    LinkCommand* Clone() const override;
    std::string ToString() const override;
private:
    int32_t symbolIndex;
};

class LinkClsIdCommand : public LinkCommand
{
public:
    LinkClsIdCommand();
    LinkClsIdCommand(uint64_t address_, uint64_t typeId1_, uint64_t typeId2_);
    void Emit(LinkSection* linkSection) override;
    void Read(LinkSection* linkSection) override;
    void Apply(ObjectFile* objectFile, uint64_t value) override;
    uint64_t TypeId1() const { return typeId1; }
    uint64_t TypeId2() const { return typeId2; }
    LinkCommand* Clone() const override;
    std::string ToString() const override;
private:
    uint64_t typeId1;
    uint64_t typeId2;
};

class LinkEndCommamnd : public LinkCommand
{
public:
    LinkEndCommamnd();
    LinkCommand* Clone() const override;
    std::string ToString() const override;
};

LinkCommand* MakeLinkCommand(LinkCode linkCode);

LinkCommand* ReadLinkCommand(LinkSection* linkSection);

void LinkInternal(ObjectFile* objectFile);
void Link(int logStreamId, const std::string& executableFilePath, const std::vector<std::string>& binaryFileNames, const std::string& clsIdFileName, bool verbose);

} // namespace cmajor::systemx::object
