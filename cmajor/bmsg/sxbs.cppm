// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/sxbs.schema' by cmajor binary protocol message generator version 5.0.0

export module cmajor.systemx.build.server.message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;

export namespace sxbs {

const uint32_t bmpBuildRequestId = 3665341171;

class BuildRequest : public cmajor::bmp::BinaryMessage
{
public:
    BuildRequest();
    BuildRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string projectOrSolutionFilePath;
};

const uint32_t bmpBuildReplyId = 1168368853;

class BuildReply : public cmajor::bmp::BinaryMessage
{
public:
    BuildReply();
    BuildReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool buildSucceeded;
    std::string resultMessage;
};

const uint32_t bmpLogLineRequestId = 3432820639;

class LogLineRequest : public cmajor::bmp::BinaryMessage
{
public:
    LogLineRequest();
    LogLineRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string logLine;
};

const uint32_t bmpLogLineReplyId = 1664150504;

class LogLineReply : public cmajor::bmp::BinaryMessage
{
public:
    LogLineReply();
    LogLineReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};


} // namespace sxbs
