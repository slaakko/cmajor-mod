// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/bs.schema' by cmajor binary protocol message generator version 5.0.0

module cmajor.build.server.message;

import cmajor.binary.message.io;

namespace bs {

StopBuildRequest::StopBuildRequest()
     : StopBuildRequest::StopBuildRequest(bmpStopBuildRequestId)
{
}

StopBuildRequest::StopBuildRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopBuildRequest::ClassName()
{
    return "bs::StopBuildRequest";
}

void StopBuildRequest::Register()
{
    cmajor::bmp::RegisterMessage<StopBuildRequest>();
}

cmajor::bmp::BinaryMessage* StopBuildRequest::Create(uint32_t messageId)
{
    return new StopBuildRequest(messageId);
}

uint32_t StopBuildRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopBuildRequest::Write(util::MemoryWriter& writer) const
{
}

void StopBuildRequest::Read(util::MemoryReader& reader)
{
}

StopBuildReply::StopBuildReply()
     : StopBuildReply::StopBuildReply(bmpStopBuildReplyId)
{
}

StopBuildReply::StopBuildReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopBuildReply::ClassName()
{
    return "bs::StopBuildReply";
}

void StopBuildReply::Register()
{
    cmajor::bmp::RegisterMessage<StopBuildReply>();
}

cmajor::bmp::BinaryMessage* StopBuildReply::Create(uint32_t messageId)
{
    return new StopBuildReply(messageId);
}

uint32_t StopBuildReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopBuildReply::Write(util::MemoryWriter& writer) const
{
}

void StopBuildReply::Read(util::MemoryReader& reader)
{
}

KeepAliveBuildRequest::KeepAliveBuildRequest()
     : KeepAliveBuildRequest::KeepAliveBuildRequest(bmpKeepAliveBuildRequestId)
{
}

KeepAliveBuildRequest::KeepAliveBuildRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string KeepAliveBuildRequest::ClassName()
{
    return "bs::KeepAliveBuildRequest";
}

void KeepAliveBuildRequest::Register()
{
    cmajor::bmp::RegisterMessage<KeepAliveBuildRequest>();
}

cmajor::bmp::BinaryMessage* KeepAliveBuildRequest::Create(uint32_t messageId)
{
    return new KeepAliveBuildRequest(messageId);
}

uint32_t KeepAliveBuildRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void KeepAliveBuildRequest::Write(util::MemoryWriter& writer) const
{
}

void KeepAliveBuildRequest::Read(util::MemoryReader& reader)
{
}

KeepAliveBuildReply::KeepAliveBuildReply()
     : KeepAliveBuildReply::KeepAliveBuildReply(bmpKeepAliveBuildReplyId)
{
}

KeepAliveBuildReply::KeepAliveBuildReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string KeepAliveBuildReply::ClassName()
{
    return "bs::KeepAliveBuildReply";
}

void KeepAliveBuildReply::Register()
{
    cmajor::bmp::RegisterMessage<KeepAliveBuildReply>();
}

cmajor::bmp::BinaryMessage* KeepAliveBuildReply::Create(uint32_t messageId)
{
    return new KeepAliveBuildReply(messageId);
}

uint32_t KeepAliveBuildReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void KeepAliveBuildReply::Write(util::MemoryWriter& writer) const
{
}

void KeepAliveBuildReply::Read(util::MemoryReader& reader)
{
}

BuildRequest::BuildRequest()
     : BuildRequest::BuildRequest(bmpBuildRequestId)
{
}

BuildRequest::BuildRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), backend(), config(), optimizationLevel(), filePath(), defines(), verbose(), quiet(), clean(), rebuild(), emitIR(), linkWithDebugRuntime(), disableModuleCache(), singleThreadedCompile(), buildAllDependencies()
{
}

std::string BuildRequest::ClassName()
{
    return "bs::BuildRequest";
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
    length += cmajor::bmp::Length(backend);
    length += cmajor::bmp::Length(config);
    length += cmajor::bmp::Length(optimizationLevel);
    length += cmajor::bmp::Length(filePath);
    length += cmajor::bmp::Length(defines);
    length += cmajor::bmp::Length(verbose);
    length += cmajor::bmp::Length(quiet);
    length += cmajor::bmp::Length(clean);
    length += cmajor::bmp::Length(rebuild);
    length += cmajor::bmp::Length(emitIR);
    length += cmajor::bmp::Length(linkWithDebugRuntime);
    length += cmajor::bmp::Length(disableModuleCache);
    length += cmajor::bmp::Length(singleThreadedCompile);
    length += cmajor::bmp::Length(buildAllDependencies);
    return length;
}

void BuildRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, backend);
    cmajor::bmp::Write(writer, config);
    cmajor::bmp::Write(writer, optimizationLevel);
    cmajor::bmp::Write(writer, filePath);
    cmajor::bmp::Write(writer, defines);
    cmajor::bmp::Write(writer, verbose);
    cmajor::bmp::Write(writer, quiet);
    cmajor::bmp::Write(writer, clean);
    cmajor::bmp::Write(writer, rebuild);
    cmajor::bmp::Write(writer, emitIR);
    cmajor::bmp::Write(writer, linkWithDebugRuntime);
    cmajor::bmp::Write(writer, disableModuleCache);
    cmajor::bmp::Write(writer, singleThreadedCompile);
    cmajor::bmp::Write(writer, buildAllDependencies);
}

void BuildRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, backend);
    cmajor::bmp::Read(reader, config);
    cmajor::bmp::Read(reader, optimizationLevel);
    cmajor::bmp::Read(reader, filePath);
    cmajor::bmp::Read(reader, defines);
    cmajor::bmp::Read(reader, verbose);
    cmajor::bmp::Read(reader, quiet);
    cmajor::bmp::Read(reader, clean);
    cmajor::bmp::Read(reader, rebuild);
    cmajor::bmp::Read(reader, emitIR);
    cmajor::bmp::Read(reader, linkWithDebugRuntime);
    cmajor::bmp::Read(reader, disableModuleCache);
    cmajor::bmp::Read(reader, singleThreadedCompile);
    cmajor::bmp::Read(reader, buildAllDependencies);
}

CompileError::CompileError()
     : CompileError::CompileError(bmpCompileErrorId)
{
}

CompileError::CompileError(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), message(), project(), file(), line(), scol(), ecol()
{
}

std::string CompileError::ClassName()
{
    return "bs::CompileError";
}

void CompileError::Register()
{
    cmajor::bmp::RegisterMessage<CompileError>();
}

cmajor::bmp::BinaryMessage* CompileError::Create(uint32_t messageId)
{
    return new CompileError(messageId);
}

uint32_t CompileError::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(message);
    length += cmajor::bmp::Length(project);
    length += cmajor::bmp::Length(file);
    length += cmajor::bmp::Length(line);
    length += cmajor::bmp::Length(scol);
    length += cmajor::bmp::Length(ecol);
    return length;
}

void CompileError::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, message);
    cmajor::bmp::Write(writer, project);
    cmajor::bmp::Write(writer, file);
    cmajor::bmp::Write(writer, line);
    cmajor::bmp::Write(writer, scol);
    cmajor::bmp::Write(writer, ecol);
}

void CompileError::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, message);
    cmajor::bmp::Read(reader, project);
    cmajor::bmp::Read(reader, file);
    cmajor::bmp::Read(reader, line);
    cmajor::bmp::Read(reader, scol);
    cmajor::bmp::Read(reader, ecol);
}

BuildReply::BuildReply()
     : BuildReply::BuildReply(bmpBuildReplyId)
{
}

BuildReply::BuildReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), requestValid(), requestErrorMessage(), logException(), time(), success(), errors()
{
}

std::string BuildReply::ClassName()
{
    return "bs::BuildReply";
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
    length += cmajor::bmp::Length(requestValid);
    length += cmajor::bmp::Length(requestErrorMessage);
    length += cmajor::bmp::Length(logException);
    length += cmajor::bmp::Length(time);
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(errors);
    return length;
}

void BuildReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, requestValid);
    cmajor::bmp::Write(writer, requestErrorMessage);
    cmajor::bmp::Write(writer, logException);
    cmajor::bmp::Write(writer, time);
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, errors);
}

void BuildReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, requestValid);
    cmajor::bmp::Read(reader, requestErrorMessage);
    cmajor::bmp::Read(reader, logException);
    cmajor::bmp::Read(reader, time);
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, errors);
}

LogBuildMessageRequest::LogBuildMessageRequest()
     : LogBuildMessageRequest::LogBuildMessageRequest(bmpLogBuildMessageRequestId)
{
}

LogBuildMessageRequest::LogBuildMessageRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), message()
{
}

std::string LogBuildMessageRequest::ClassName()
{
    return "bs::LogBuildMessageRequest";
}

void LogBuildMessageRequest::Register()
{
    cmajor::bmp::RegisterMessage<LogBuildMessageRequest>();
}

cmajor::bmp::BinaryMessage* LogBuildMessageRequest::Create(uint32_t messageId)
{
    return new LogBuildMessageRequest(messageId);
}

uint32_t LogBuildMessageRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(message);
    return length;
}

void LogBuildMessageRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, message);
}

void LogBuildMessageRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, message);
}

LogBuildMessageReply::LogBuildMessageReply()
     : LogBuildMessageReply::LogBuildMessageReply(bmpLogBuildMessageReplyId)
{
}

LogBuildMessageReply::LogBuildMessageReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), ok()
{
}

std::string LogBuildMessageReply::ClassName()
{
    return "bs::LogBuildMessageReply";
}

void LogBuildMessageReply::Register()
{
    cmajor::bmp::RegisterMessage<LogBuildMessageReply>();
}

cmajor::bmp::BinaryMessage* LogBuildMessageReply::Create(uint32_t messageId)
{
    return new LogBuildMessageReply(messageId);
}

uint32_t LogBuildMessageReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(ok);
    return length;
}

void LogBuildMessageReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, ok);
}

void LogBuildMessageReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, ok);
}

BuildProgressMessage::BuildProgressMessage()
     : BuildProgressMessage::BuildProgressMessage(bmpBuildProgressMessageId)
{
}

BuildProgressMessage::BuildProgressMessage(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string BuildProgressMessage::ClassName()
{
    return "bs::BuildProgressMessage";
}

void BuildProgressMessage::Register()
{
    cmajor::bmp::RegisterMessage<BuildProgressMessage>();
}

cmajor::bmp::BinaryMessage* BuildProgressMessage::Create(uint32_t messageId)
{
    return new BuildProgressMessage(messageId);
}

uint32_t BuildProgressMessage::Length() const
{
    uint32_t length = 0;
    return length;
}

void BuildProgressMessage::Write(util::MemoryWriter& writer) const
{
}

void BuildProgressMessage::Read(util::MemoryReader& reader)
{
}

GenericBuildErrorReply::GenericBuildErrorReply()
     : GenericBuildErrorReply::GenericBuildErrorReply(bmpGenericBuildErrorReplyId)
{
}

GenericBuildErrorReply::GenericBuildErrorReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), error()
{
}

std::string GenericBuildErrorReply::ClassName()
{
    return "bs::GenericBuildErrorReply";
}

void GenericBuildErrorReply::Register()
{
    cmajor::bmp::RegisterMessage<GenericBuildErrorReply>();
}

cmajor::bmp::BinaryMessage* GenericBuildErrorReply::Create(uint32_t messageId)
{
    return new GenericBuildErrorReply(messageId);
}

uint32_t GenericBuildErrorReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(error);
    return length;
}

void GenericBuildErrorReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, error);
}

void GenericBuildErrorReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, error);
}

CacheModuleRequest::CacheModuleRequest()
     : CacheModuleRequest::CacheModuleRequest(bmpCacheModuleRequestId)
{
}

CacheModuleRequest::CacheModuleRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), projectName(), projectFilePath(), backend(), config()
{
}

std::string CacheModuleRequest::ClassName()
{
    return "bs::CacheModuleRequest";
}

void CacheModuleRequest::Register()
{
    cmajor::bmp::RegisterMessage<CacheModuleRequest>();
}

cmajor::bmp::BinaryMessage* CacheModuleRequest::Create(uint32_t messageId)
{
    return new CacheModuleRequest(messageId);
}

uint32_t CacheModuleRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(projectName);
    length += cmajor::bmp::Length(projectFilePath);
    length += cmajor::bmp::Length(backend);
    length += cmajor::bmp::Length(config);
    return length;
}

void CacheModuleRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, projectName);
    cmajor::bmp::Write(writer, projectFilePath);
    cmajor::bmp::Write(writer, backend);
    cmajor::bmp::Write(writer, config);
}

void CacheModuleRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, projectName);
    cmajor::bmp::Read(reader, projectFilePath);
    cmajor::bmp::Read(reader, backend);
    cmajor::bmp::Read(reader, config);
}

CacheModuleReply::CacheModuleReply()
     : CacheModuleReply::CacheModuleReply(bmpCacheModuleReplyId)
{
}

CacheModuleReply::CacheModuleReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), error()
{
}

std::string CacheModuleReply::ClassName()
{
    return "bs::CacheModuleReply";
}

void CacheModuleReply::Register()
{
    cmajor::bmp::RegisterMessage<CacheModuleReply>();
}

cmajor::bmp::BinaryMessage* CacheModuleReply::Create(uint32_t messageId)
{
    return new CacheModuleReply(messageId);
}

uint32_t CacheModuleReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(error);
    return length;
}

void CacheModuleReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, error);
}

void CacheModuleReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, error);
}

DefinitionSourceLocation::DefinitionSourceLocation()
     : DefinitionSourceLocation::DefinitionSourceLocation(bmpDefinitionSourceLocationId)
{
}

DefinitionSourceLocation::DefinitionSourceLocation(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), file(), line(), scol()
{
}

std::string DefinitionSourceLocation::ClassName()
{
    return "bs::DefinitionSourceLocation";
}

void DefinitionSourceLocation::Register()
{
    cmajor::bmp::RegisterMessage<DefinitionSourceLocation>();
}

cmajor::bmp::BinaryMessage* DefinitionSourceLocation::Create(uint32_t messageId)
{
    return new DefinitionSourceLocation(messageId);
}

uint32_t DefinitionSourceLocation::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(file);
    length += cmajor::bmp::Length(line);
    length += cmajor::bmp::Length(scol);
    return length;
}

void DefinitionSourceLocation::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, file);
    cmajor::bmp::Write(writer, line);
    cmajor::bmp::Write(writer, scol);
}

void DefinitionSourceLocation::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, file);
    cmajor::bmp::Read(reader, line);
    cmajor::bmp::Read(reader, scol);
}

GetDefinitionRequest::GetDefinitionRequest()
     : GetDefinitionRequest::GetDefinitionRequest(bmpGetDefinitionRequestId)
{
}

GetDefinitionRequest::GetDefinitionRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), projectName(), projectFilePath(), backend(), config(), identifier(), identifierLocation()
{
}

std::string GetDefinitionRequest::ClassName()
{
    return "bs::GetDefinitionRequest";
}

void GetDefinitionRequest::Register()
{
    cmajor::bmp::RegisterMessage<GetDefinitionRequest>();
}

cmajor::bmp::BinaryMessage* GetDefinitionRequest::Create(uint32_t messageId)
{
    return new GetDefinitionRequest(messageId);
}

uint32_t GetDefinitionRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(projectName);
    length += cmajor::bmp::Length(projectFilePath);
    length += cmajor::bmp::Length(backend);
    length += cmajor::bmp::Length(config);
    length += cmajor::bmp::Length(identifier);
    length += cmajor::bmp::Length(identifierLocation);
    return length;
}

void GetDefinitionRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, projectName);
    cmajor::bmp::Write(writer, projectFilePath);
    cmajor::bmp::Write(writer, backend);
    cmajor::bmp::Write(writer, config);
    cmajor::bmp::Write(writer, identifier);
    cmajor::bmp::Write(writer, identifierLocation);
}

void GetDefinitionRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, projectName);
    cmajor::bmp::Read(reader, projectFilePath);
    cmajor::bmp::Read(reader, backend);
    cmajor::bmp::Read(reader, config);
    cmajor::bmp::Read(reader, identifier);
    cmajor::bmp::Read(reader, identifierLocation);
}

GetDefinitionReply::GetDefinitionReply()
     : GetDefinitionReply::GetDefinitionReply(bmpGetDefinitionReplyId)
{
}

GetDefinitionReply::GetDefinitionReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), definitionLocation(), ok(), error()
{
}

std::string GetDefinitionReply::ClassName()
{
    return "bs::GetDefinitionReply";
}

void GetDefinitionReply::Register()
{
    cmajor::bmp::RegisterMessage<GetDefinitionReply>();
}

cmajor::bmp::BinaryMessage* GetDefinitionReply::Create(uint32_t messageId)
{
    return new GetDefinitionReply(messageId);
}

uint32_t GetDefinitionReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(definitionLocation);
    length += cmajor::bmp::Length(ok);
    length += cmajor::bmp::Length(error);
    return length;
}

void GetDefinitionReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, definitionLocation);
    cmajor::bmp::Write(writer, ok);
    cmajor::bmp::Write(writer, error);
}

void GetDefinitionReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, definitionLocation);
    cmajor::bmp::Read(reader, ok);
    cmajor::bmp::Read(reader, error);
}


} // namespace bs

namespace {

struct Reg
{
    Reg()
    {
        bs::StopBuildRequest::Register();
        bs::StopBuildReply::Register();
        bs::KeepAliveBuildRequest::Register();
        bs::KeepAliveBuildReply::Register();
        bs::BuildRequest::Register();
        bs::CompileError::Register();
        bs::BuildReply::Register();
        bs::LogBuildMessageRequest::Register();
        bs::LogBuildMessageReply::Register();
        bs::BuildProgressMessage::Register();
        bs::GenericBuildErrorReply::Register();
        bs::CacheModuleRequest::Register();
        bs::CacheModuleReply::Register();
        bs::DefinitionSourceLocation::Register();
        bs::GetDefinitionRequest::Register();
        bs::GetDefinitionReply::Register();
    }
};

Reg reg;

} // namespace

