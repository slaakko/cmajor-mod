// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cmbmtest/message.schema' by cmajor binary protocol message generator version 5.0.0

export module bm_test_message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;

export namespace BmTest {

const uint32_t bmpSimpleMessageId = 1894254629;

class SimpleMessage : public cmajor::bmp::BinaryMessage
{
public:
    SimpleMessage();
    SimpleMessage(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) override;
    void Read(util::MemoryReader& reader) override;
public:
    int32_t x;
};

const uint32_t bmpAllFieldsMessageId = 2707244940;

class AllFieldsMessage : public cmajor::bmp::BinaryMessage
{
public:
    AllFieldsMessage();
    AllFieldsMessage(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) override;
    void Read(util::MemoryReader& reader) override;
public:
    bool f0;
    int8_t f1;
    uint8_t f2;
    int16_t f3;
    uint16_t f4;
    int32_t f5;
    uint32_t f6;
    int64_t f7;
    uint64_t f8;
    float f9;
    double f10;
    char f11;
    char16_t f12;
    char32_t f13;
    std::string f14;
    cmajor::bmp::Number f15;
    util::uuid f16;
    util::Date f17;
    util::DateTime f18;
};

const uint32_t bmpStopServerRequestId = 1995547868;

class StopServerRequest : public cmajor::bmp::BinaryMessage
{
public:
    StopServerRequest();
    StopServerRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStopServerReplyId = 3361414145;

class StopServerReply : public cmajor::bmp::BinaryMessage
{
public:
    StopServerReply();
    StopServerReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) override;
    void Read(util::MemoryReader& reader) override;
public:
};


} // namespace BmTest
