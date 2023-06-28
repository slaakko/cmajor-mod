// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/sxbs.schema' by cmajor binary protocol message generator version 5.0.0

module cmajor.systemx.build.server.message;

import cmajor.binary.message.io;

namespace sxbs {

BuildRequest::BuildRequest()
     : BuildRequest::BuildRequest(bmpBuildRequestId)
{
}

BuildRequest::BuildRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), projectOrSolutionFilePath()
{
}

std::string BuildRequest::ClassName()
{
    return "sxbs::BuildRequest";
}

void BuildRequest::Register()
{
    cmajor::bmp::RegisterMessage<BuildRequest>();
}

cmajor::bmp::BinaryMessage* BuildRequest::Create(uint32_t messageId)
{
    return new BuildRequest(messageId);
}

uint32_t BuildRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(projectOrSolutionFilePath);
    return length;
}

void BuildRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, projectOrSolutionFilePath);
}

void BuildRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, projectOrSolutionFilePath);
}

BuildReply::BuildReply()
     : BuildReply::BuildReply(bmpBuildReplyId)
{
}

BuildReply::BuildReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), buildSucceeded(), resultMessage()
{
}

std::string BuildReply::ClassName()
{
    return "sxbs::BuildReply";
}

void BuildReply::Register()
{
    cmajor::bmp::RegisterMessage<BuildReply>();
}

cmajor::bmp::BinaryMessage* BuildReply::Create(uint32_t messageId)
{
    return new BuildReply(messageId);
}

uint32_t BuildReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(buildSucceeded);
    length += cmajor::bmp::Length(resultMessage);
    return length;
}

void BuildReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, buildSucceeded);
    cmajor::bmp::Write(writer, resultMessage);
}

void BuildReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, buildSucceeded);
    cmajor::bmp::Read(reader, resultMessage);
}

LogLineRequest::LogLineRequest()
     : LogLineRequest::LogLineRequest(bmpLogLineRequestId)
{
}

LogLineRequest::LogLineRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), logLine()
{
}

std::string LogLineRequest::ClassName()
{
    return "sxbs::LogLineRequest";
}

void LogLineRequest::Register()
{
    cmajor::bmp::RegisterMessage<LogLineRequest>();
}

cmajor::bmp::BinaryMessage* LogLineRequest::Create(uint32_t messageId)
{
    return new LogLineRequest(messageId);
}

uint32_t LogLineRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(logLine);
    return length;
}

void LogLineRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, logLine);
}

void LogLineRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, logLine);
}

LogLineReply::LogLineReply()
     : LogLineReply::LogLineReply(bmpLogLineReplyId)
{
}

LogLineReply::LogLineReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string LogLineReply::ClassName()
{
    return "sxbs::LogLineReply";
}

void LogLineReply::Register()
{
    cmajor::bmp::RegisterMessage<LogLineReply>();
}

cmajor::bmp::BinaryMessage* LogLineReply::Create(uint32_t messageId)
{
    return new LogLineReply(messageId);
}

uint32_t LogLineReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void LogLineReply::Write(util::MemoryWriter& writer) const
{
}

void LogLineReply::Read(util::MemoryReader& reader)
{
}


} // namespace sxbs

namespace {

struct Reg
{
    Reg()
    {
        sxbs::BuildRequest::Register();
        sxbs::BuildReply::Register();
        sxbs::LogLineRequest::Register();
        sxbs::LogLineReply::Register();
    }
};

Reg reg;

} // namespace

