// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/info/bpm.schema' by cmajor binary protocol message generator version 5.0.0

module cmajor.binary.portmap.message;

import cmajor.binary.message.io;

namespace bpm {

HelloBinaryPortMapServerRequest::HelloBinaryPortMapServerRequest()
     : HelloBinaryPortMapServerRequest::HelloBinaryPortMapServerRequest(bmpHelloBinaryPortMapServerRequestId)
{
}

HelloBinaryPortMapServerRequest::HelloBinaryPortMapServerRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string HelloBinaryPortMapServerRequest::ClassName()
{
    return "bpm::HelloBinaryPortMapServerRequest";
}

void HelloBinaryPortMapServerRequest::Register()
{
    cmajor::bmp::RegisterMessage<HelloBinaryPortMapServerRequest>();
}

cmajor::bmp::BinaryMessage* HelloBinaryPortMapServerRequest::Create(uint32_t messageId)
{
    return new HelloBinaryPortMapServerRequest(messageId);
}

uint32_t HelloBinaryPortMapServerRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void HelloBinaryPortMapServerRequest::Write(util::MemoryWriter& writer) const
{
}

void HelloBinaryPortMapServerRequest::Read(util::MemoryReader& reader)
{
}

HelloBinaryPortMapServerReply::HelloBinaryPortMapServerReply()
     : HelloBinaryPortMapServerReply::HelloBinaryPortMapServerReply(bmpHelloBinaryPortMapServerReplyId)
{
}

HelloBinaryPortMapServerReply::HelloBinaryPortMapServerReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), version()
{
}

std::string HelloBinaryPortMapServerReply::ClassName()
{
    return "bpm::HelloBinaryPortMapServerReply";
}

void HelloBinaryPortMapServerReply::Register()
{
    cmajor::bmp::RegisterMessage<HelloBinaryPortMapServerReply>();
}

cmajor::bmp::BinaryMessage* HelloBinaryPortMapServerReply::Create(uint32_t messageId)
{
    return new HelloBinaryPortMapServerReply(messageId);
}

uint32_t HelloBinaryPortMapServerReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(version);
    return length;
}

void HelloBinaryPortMapServerReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, version);
}

void HelloBinaryPortMapServerReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, version);
}

GetFreePortNumberRequest::GetFreePortNumberRequest()
     : GetFreePortNumberRequest::GetFreePortNumberRequest(bmpGetFreePortNumberRequestId)
{
}

GetFreePortNumberRequest::GetFreePortNumberRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), processName(), pid()
{
}

std::string GetFreePortNumberRequest::ClassName()
{
    return "bpm::GetFreePortNumberRequest";
}

void GetFreePortNumberRequest::Register()
{
    cmajor::bmp::RegisterMessage<GetFreePortNumberRequest>();
}

cmajor::bmp::BinaryMessage* GetFreePortNumberRequest::Create(uint32_t messageId)
{
    return new GetFreePortNumberRequest(messageId);
}

uint32_t GetFreePortNumberRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(processName);
    length += cmajor::bmp::Length(pid);
    return length;
}

void GetFreePortNumberRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, processName);
    cmajor::bmp::Write(writer, pid);
}

void GetFreePortNumberRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, processName);
    cmajor::bmp::Read(reader, pid);
}

GetFreePortNumberReply::GetFreePortNumberReply()
     : GetFreePortNumberReply::GetFreePortNumberReply(bmpGetFreePortNumberReplyId)
{
}

GetFreePortNumberReply::GetFreePortNumberReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), portNumber()
{
}

std::string GetFreePortNumberReply::ClassName()
{
    return "bpm::GetFreePortNumberReply";
}

void GetFreePortNumberReply::Register()
{
    cmajor::bmp::RegisterMessage<GetFreePortNumberReply>();
}

cmajor::bmp::BinaryMessage* GetFreePortNumberReply::Create(uint32_t messageId)
{
    return new GetFreePortNumberReply(messageId);
}

uint32_t GetFreePortNumberReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(portNumber);
    return length;
}

void GetFreePortNumberReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, portNumber);
}

void GetFreePortNumberReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, portNumber);
}

ExtendPortLeaseRequest::ExtendPortLeaseRequest()
     : ExtendPortLeaseRequest::ExtendPortLeaseRequest(bmpExtendPortLeaseRequestId)
{
}

ExtendPortLeaseRequest::ExtendPortLeaseRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), processName(), pid(), portNumbers()
{
}

std::string ExtendPortLeaseRequest::ClassName()
{
    return "bpm::ExtendPortLeaseRequest";
}

void ExtendPortLeaseRequest::Register()
{
    cmajor::bmp::RegisterMessage<ExtendPortLeaseRequest>();
}

cmajor::bmp::BinaryMessage* ExtendPortLeaseRequest::Create(uint32_t messageId)
{
    return new ExtendPortLeaseRequest(messageId);
}

uint32_t ExtendPortLeaseRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(processName);
    length += cmajor::bmp::Length(pid);
    length += cmajor::bmp::Length(portNumbers);
    return length;
}

void ExtendPortLeaseRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, processName);
    cmajor::bmp::Write(writer, pid);
    cmajor::bmp::Write(writer, portNumbers);
}

void ExtendPortLeaseRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, processName);
    cmajor::bmp::Read(reader, pid);
    cmajor::bmp::Read(reader, portNumbers);
}

ExtendPortLeaseReply::ExtendPortLeaseReply()
     : ExtendPortLeaseReply::ExtendPortLeaseReply(bmpExtendPortLeaseReplyId)
{
}

ExtendPortLeaseReply::ExtendPortLeaseReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string ExtendPortLeaseReply::ClassName()
{
    return "bpm::ExtendPortLeaseReply";
}

void ExtendPortLeaseReply::Register()
{
    cmajor::bmp::RegisterMessage<ExtendPortLeaseReply>();
}

cmajor::bmp::BinaryMessage* ExtendPortLeaseReply::Create(uint32_t messageId)
{
    return new ExtendPortLeaseReply(messageId);
}

uint32_t ExtendPortLeaseReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void ExtendPortLeaseReply::Write(util::MemoryWriter& writer) const
{
}

void ExtendPortLeaseReply::Read(util::MemoryReader& reader)
{
}

StopPortMapServerRequest::StopPortMapServerRequest()
     : StopPortMapServerRequest::StopPortMapServerRequest(bmpStopPortMapServerRequestId)
{
}

StopPortMapServerRequest::StopPortMapServerRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopPortMapServerRequest::ClassName()
{
    return "bpm::StopPortMapServerRequest";
}

void StopPortMapServerRequest::Register()
{
    cmajor::bmp::RegisterMessage<StopPortMapServerRequest>();
}

cmajor::bmp::BinaryMessage* StopPortMapServerRequest::Create(uint32_t messageId)
{
    return new StopPortMapServerRequest(messageId);
}

uint32_t StopPortMapServerRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopPortMapServerRequest::Write(util::MemoryWriter& writer) const
{
}

void StopPortMapServerRequest::Read(util::MemoryReader& reader)
{
}

StopPortMapServerReply::StopPortMapServerReply()
     : StopPortMapServerReply::StopPortMapServerReply(bmpStopPortMapServerReplyId)
{
}

StopPortMapServerReply::StopPortMapServerReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopPortMapServerReply::ClassName()
{
    return "bpm::StopPortMapServerReply";
}

void StopPortMapServerReply::Register()
{
    cmajor::bmp::RegisterMessage<StopPortMapServerReply>();
}

cmajor::bmp::BinaryMessage* StopPortMapServerReply::Create(uint32_t messageId)
{
    return new StopPortMapServerReply(messageId);
}

uint32_t StopPortMapServerReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopPortMapServerReply::Write(util::MemoryWriter& writer) const
{
}

void StopPortMapServerReply::Read(util::MemoryReader& reader)
{
}

PortLease::PortLease()
     : PortLease::PortLease(bmpPortLeaseId)
{
}

PortLease::PortLease(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), portNumber(), processName(), pid(), leaseStartTime(), leaseRenewalTime(), leaseState()
{
}

std::string PortLease::ClassName()
{
    return "bpm::PortLease";
}

void PortLease::Register()
{
    cmajor::bmp::RegisterMessage<PortLease>();
}

cmajor::bmp::BinaryMessage* PortLease::Create(uint32_t messageId)
{
    return new PortLease(messageId);
}

uint32_t PortLease::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(portNumber);
    length += cmajor::bmp::Length(processName);
    length += cmajor::bmp::Length(pid);
    length += cmajor::bmp::Length(leaseStartTime);
    length += cmajor::bmp::Length(leaseRenewalTime);
    length += cmajor::bmp::Length(leaseState);
    return length;
}

void PortLease::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, portNumber);
    cmajor::bmp::Write(writer, processName);
    cmajor::bmp::Write(writer, pid);
    cmajor::bmp::Write(writer, leaseStartTime);
    cmajor::bmp::Write(writer, leaseRenewalTime);
    cmajor::bmp::Write(writer, leaseState);
}

void PortLease::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, portNumber);
    cmajor::bmp::Read(reader, processName);
    cmajor::bmp::Read(reader, pid);
    cmajor::bmp::Read(reader, leaseStartTime);
    cmajor::bmp::Read(reader, leaseRenewalTime);
    cmajor::bmp::Read(reader, leaseState);
}

ViewPortLeaseRequest::ViewPortLeaseRequest()
     : ViewPortLeaseRequest::ViewPortLeaseRequest(bmpViewPortLeaseRequestId)
{
}

ViewPortLeaseRequest::ViewPortLeaseRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string ViewPortLeaseRequest::ClassName()
{
    return "bpm::ViewPortLeaseRequest";
}

void ViewPortLeaseRequest::Register()
{
    cmajor::bmp::RegisterMessage<ViewPortLeaseRequest>();
}

cmajor::bmp::BinaryMessage* ViewPortLeaseRequest::Create(uint32_t messageId)
{
    return new ViewPortLeaseRequest(messageId);
}

uint32_t ViewPortLeaseRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void ViewPortLeaseRequest::Write(util::MemoryWriter& writer) const
{
}

void ViewPortLeaseRequest::Read(util::MemoryReader& reader)
{
}

ViewPortLeaseReply::ViewPortLeaseReply()
     : ViewPortLeaseReply::ViewPortLeaseReply(bmpViewPortLeaseReplyId)
{
}

ViewPortLeaseReply::ViewPortLeaseReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), portLeases()
{
}

std::string ViewPortLeaseReply::ClassName()
{
    return "bpm::ViewPortLeaseReply";
}

void ViewPortLeaseReply::Register()
{
    cmajor::bmp::RegisterMessage<ViewPortLeaseReply>();
}

cmajor::bmp::BinaryMessage* ViewPortLeaseReply::Create(uint32_t messageId)
{
    return new ViewPortLeaseReply(messageId);
}

uint32_t ViewPortLeaseReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(portLeases);
    return length;
}

void ViewPortLeaseReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, portLeases);
}

void ViewPortLeaseReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, portLeases);
}


} // namespace bpm

namespace {

struct Reg
{
    Reg()
    {
        bpm::HelloBinaryPortMapServerRequest::Register();
        bpm::HelloBinaryPortMapServerReply::Register();
        bpm::GetFreePortNumberRequest::Register();
        bpm::GetFreePortNumberReply::Register();
        bpm::ExtendPortLeaseRequest::Register();
        bpm::ExtendPortLeaseReply::Register();
        bpm::StopPortMapServerRequest::Register();
        bpm::StopPortMapServerReply::Register();
        bpm::PortLease::Register();
        bpm::ViewPortLeaseRequest::Register();
        bpm::ViewPortLeaseReply::Register();
    }
};

Reg reg;

} // namespace

