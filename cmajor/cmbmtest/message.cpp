// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cmbmtest/message.schema' by cmajor binary protocol message generator version 5.0.0

module bm_test_message;

import cmajor.binary.message.io;

namespace BmTest {

SimpleMessage::SimpleMessage()
     : SimpleMessage::SimpleMessage(bmpSimpleMessageId)
{
}

SimpleMessage::SimpleMessage(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), x()
{
}

std::string SimpleMessage::ClassName()
{
    return "BmTest::SimpleMessage";
}

void SimpleMessage::Register()
{
    cmajor::bmp::RegisterMessage<SimpleMessage>();
}

cmajor::bmp::BinaryMessage* SimpleMessage::Create(uint32_t messageId)
{
    return new SimpleMessage(messageId);
}

uint32_t SimpleMessage::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(x);
    return length;
}

void SimpleMessage::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, x);
}

void SimpleMessage::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, x);
}

AllFieldsMessage::AllFieldsMessage()
     : AllFieldsMessage::AllFieldsMessage(bmpAllFieldsMessageId)
{
}

AllFieldsMessage::AllFieldsMessage(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), f0(), f1(), f2(), f3(), f4(), f5(), f6(), f7(), f8(), f9(), f10(), f11(), f12(), f13(), f14(), f15(), f16(), f17(), f18()
{
}

std::string AllFieldsMessage::ClassName()
{
    return "BmTest::AllFieldsMessage";
}

void AllFieldsMessage::Register()
{
    cmajor::bmp::RegisterMessage<AllFieldsMessage>();
}

cmajor::bmp::BinaryMessage* AllFieldsMessage::Create(uint32_t messageId)
{
    return new AllFieldsMessage(messageId);
}

uint32_t AllFieldsMessage::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(f0);
    length += cmajor::bmp::Length(f1);
    length += cmajor::bmp::Length(f2);
    length += cmajor::bmp::Length(f3);
    length += cmajor::bmp::Length(f4);
    length += cmajor::bmp::Length(f5);
    length += cmajor::bmp::Length(f6);
    length += cmajor::bmp::Length(f7);
    length += cmajor::bmp::Length(f8);
    length += cmajor::bmp::Length(f9);
    length += cmajor::bmp::Length(f10);
    length += cmajor::bmp::Length(f11);
    length += cmajor::bmp::Length(f12);
    length += cmajor::bmp::Length(f13);
    length += cmajor::bmp::Length(f14);
    length += cmajor::bmp::Length(f15);
    length += cmajor::bmp::Length(f16);
    length += cmajor::bmp::Length(f17);
    length += cmajor::bmp::Length(f18);
    return length;
}

void AllFieldsMessage::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, f0);
    cmajor::bmp::Write(writer, f1);
    cmajor::bmp::Write(writer, f2);
    cmajor::bmp::Write(writer, f3);
    cmajor::bmp::Write(writer, f4);
    cmajor::bmp::Write(writer, f5);
    cmajor::bmp::Write(writer, f6);
    cmajor::bmp::Write(writer, f7);
    cmajor::bmp::Write(writer, f8);
    cmajor::bmp::Write(writer, f9);
    cmajor::bmp::Write(writer, f10);
    cmajor::bmp::Write(writer, f11);
    cmajor::bmp::Write(writer, f12);
    cmajor::bmp::Write(writer, f13);
    cmajor::bmp::Write(writer, f14);
    cmajor::bmp::Write(writer, f15);
    cmajor::bmp::Write(writer, f16);
    cmajor::bmp::Write(writer, f17);
    cmajor::bmp::Write(writer, f18);
}

void AllFieldsMessage::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, f0);
    cmajor::bmp::Read(reader, f1);
    cmajor::bmp::Read(reader, f2);
    cmajor::bmp::Read(reader, f3);
    cmajor::bmp::Read(reader, f4);
    cmajor::bmp::Read(reader, f5);
    cmajor::bmp::Read(reader, f6);
    cmajor::bmp::Read(reader, f7);
    cmajor::bmp::Read(reader, f8);
    cmajor::bmp::Read(reader, f9);
    cmajor::bmp::Read(reader, f10);
    cmajor::bmp::Read(reader, f11);
    cmajor::bmp::Read(reader, f12);
    cmajor::bmp::Read(reader, f13);
    cmajor::bmp::Read(reader, f14);
    cmajor::bmp::Read(reader, f15);
    cmajor::bmp::Read(reader, f16);
    cmajor::bmp::Read(reader, f17);
    cmajor::bmp::Read(reader, f18);
}

StopServerRequest::StopServerRequest()
     : StopServerRequest::StopServerRequest(bmpStopServerRequestId)
{
}

StopServerRequest::StopServerRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopServerRequest::ClassName()
{
    return "BmTest::StopServerRequest";
}

void StopServerRequest::Register()
{
    cmajor::bmp::RegisterMessage<StopServerRequest>();
}

cmajor::bmp::BinaryMessage* StopServerRequest::Create(uint32_t messageId)
{
    return new StopServerRequest(messageId);
}

uint32_t StopServerRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopServerRequest::Write(util::MemoryWriter& writer) const
{
}

void StopServerRequest::Read(util::MemoryReader& reader)
{
}

StopServerReply::StopServerReply()
     : StopServerReply::StopServerReply(bmpStopServerReplyId)
{
}

StopServerReply::StopServerReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopServerReply::ClassName()
{
    return "BmTest::StopServerReply";
}

void StopServerReply::Register()
{
    cmajor::bmp::RegisterMessage<StopServerReply>();
}

cmajor::bmp::BinaryMessage* StopServerReply::Create(uint32_t messageId)
{
    return new StopServerReply(messageId);
}

uint32_t StopServerReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopServerReply::Write(util::MemoryWriter& writer) const
{
}

void StopServerReply::Read(util::MemoryReader& reader)
{
}


} // namespace BmTest

namespace {

struct Reg
{
    Reg()
    {
        BmTest::SimpleMessage::Register();
        BmTest::AllFieldsMessage::Register();
        BmTest::StopServerRequest::Register();
        BmTest::StopServerReply::Register();
    }
};

Reg reg;

} // namespace

