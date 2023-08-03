// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/info/bpm.schema' by cmajor binary protocol message generator version 5.0.0

export module cmajor.binary.portmap.message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;

export namespace bpm {

const uint32_t bmpHelloBinaryPortMapServerRequestId = 173601034;

class HelloBinaryPortMapServerRequest : public cmajor::bmp::BinaryMessage
{
public:
    HelloBinaryPortMapServerRequest();
    HelloBinaryPortMapServerRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpHelloBinaryPortMapServerReplyId = 1016753872;

class HelloBinaryPortMapServerReply : public cmajor::bmp::BinaryMessage
{
public:
    HelloBinaryPortMapServerReply();
    HelloBinaryPortMapServerReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string version;
};

const uint32_t bmpGetFreePortNumberRequestId = 3776504131;

class GetFreePortNumberRequest : public cmajor::bmp::BinaryMessage
{
public:
    GetFreePortNumberRequest();
    GetFreePortNumberRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string processName;
    int32_t pid;
};

const uint32_t bmpGetFreePortNumberReplyId = 1924696896;

class GetFreePortNumberReply : public cmajor::bmp::BinaryMessage
{
public:
    GetFreePortNumberReply();
    GetFreePortNumberReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    int32_t portNumber;
};

const uint32_t bmpExtendPortLeaseRequestId = 403726601;

class ExtendPortLeaseRequest : public cmajor::bmp::BinaryMessage
{
public:
    ExtendPortLeaseRequest();
    ExtendPortLeaseRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string processName;
    int32_t pid;
    std::vector<int32_t> portNumbers;
};

const uint32_t bmpExtendPortLeaseReplyId = 1096702587;

class ExtendPortLeaseReply : public cmajor::bmp::BinaryMessage
{
public:
    ExtendPortLeaseReply();
    ExtendPortLeaseReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStopPortMapServerRequestId = 2491345278;

class StopPortMapServerRequest : public cmajor::bmp::BinaryMessage
{
public:
    StopPortMapServerRequest();
    StopPortMapServerRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStopPortMapServerReplyId = 3455039786;

class StopPortMapServerReply : public cmajor::bmp::BinaryMessage
{
public:
    StopPortMapServerReply();
    StopPortMapServerReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpPortLeaseId = 915526961;

class PortLease : public cmajor::bmp::BinaryMessage
{
public:
    PortLease();
    PortLease(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    int32_t portNumber;
    std::string processName;
    int32_t pid;
    util::DateTime leaseStartTime;
    util::DateTime leaseRenewalTime;
    std::string leaseState;
};

const uint32_t bmpViewPortLeaseRequestId = 1227712957;

class ViewPortLeaseRequest : public cmajor::bmp::BinaryMessage
{
public:
    ViewPortLeaseRequest();
    ViewPortLeaseRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpViewPortLeaseReplyId = 400122130;

class ViewPortLeaseReply : public cmajor::bmp::BinaryMessage
{
public:
    ViewPortLeaseReply();
    ViewPortLeaseReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::vector<PortLease> portLeases;
};


} // namespace bpm
