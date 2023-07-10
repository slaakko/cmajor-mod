// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/db.schema' by cmajor binary protocol message generator version 5.0.0

module cmajor.debug.message;

import cmajor.binary.message.io;

namespace db {

Location::Location()
     : Location::Location(bmpLocationId)
{
}

Location::Location(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), func(), file(), line(), scol(), ecol(), level()
{
}

std::string Location::ClassName()
{
    return "db::Location";
}

void Location::Register()
{
    cmajor::bmp::RegisterMessage<Location>();
}

cmajor::bmp::BinaryMessage* Location::Create(uint32_t messageId)
{
    return new Location(messageId);
}

uint32_t Location::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(func);
    length += cmajor::bmp::Length(file);
    length += cmajor::bmp::Length(line);
    length += cmajor::bmp::Length(scol);
    length += cmajor::bmp::Length(ecol);
    length += cmajor::bmp::Length(level);
    return length;
}

void Location::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, func);
    cmajor::bmp::Write(writer, file);
    cmajor::bmp::Write(writer, line);
    cmajor::bmp::Write(writer, scol);
    cmajor::bmp::Write(writer, ecol);
    cmajor::bmp::Write(writer, level);
}

void Location::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, func);
    cmajor::bmp::Read(reader, file);
    cmajor::bmp::Read(reader, line);
    cmajor::bmp::Read(reader, scol);
    cmajor::bmp::Read(reader, ecol);
    cmajor::bmp::Read(reader, level);
}

Type::Type()
     : Type::Type(bmpTypeId)
{
}

Type::Type(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), name(), id()
{
}

std::string Type::ClassName()
{
    return "db::Type";
}

void Type::Register()
{
    cmajor::bmp::RegisterMessage<Type>();
}

cmajor::bmp::BinaryMessage* Type::Create(uint32_t messageId)
{
    return new Type(messageId);
}

uint32_t Type::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(name);
    length += cmajor::bmp::Length(id);
    return length;
}

void Type::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, name);
    cmajor::bmp::Write(writer, id);
}

void Type::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, name);
    cmajor::bmp::Read(reader, id);
}

Result::Result()
     : Result::Result(bmpResultId)
{
}

Result::Result(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), staticType(), dynamicType(), initialized(), value()
{
}

std::string Result::ClassName()
{
    return "db::Result";
}

void Result::Register()
{
    cmajor::bmp::RegisterMessage<Result>();
}

cmajor::bmp::BinaryMessage* Result::Create(uint32_t messageId)
{
    return new Result(messageId);
}

uint32_t Result::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(staticType);
    length += cmajor::bmp::Length(dynamicType);
    length += cmajor::bmp::Length(initialized);
    length += cmajor::bmp::Length(value);
    return length;
}

void Result::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, staticType);
    cmajor::bmp::Write(writer, dynamicType);
    cmajor::bmp::Write(writer, initialized);
    cmajor::bmp::Write(writer, value);
}

void Result::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, staticType);
    cmajor::bmp::Read(reader, dynamicType);
    cmajor::bmp::Read(reader, initialized);
    cmajor::bmp::Read(reader, value);
}

ChildResult::ChildResult()
     : ChildResult::ChildResult(bmpChildResultId)
{
}

ChildResult::ChildResult(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), expr(), name(), type(), dynType(), value(), count()
{
}

std::string ChildResult::ClassName()
{
    return "db::ChildResult";
}

void ChildResult::Register()
{
    cmajor::bmp::RegisterMessage<ChildResult>();
}

cmajor::bmp::BinaryMessage* ChildResult::Create(uint32_t messageId)
{
    return new ChildResult(messageId);
}

uint32_t ChildResult::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(expr);
    length += cmajor::bmp::Length(name);
    length += cmajor::bmp::Length(type);
    length += cmajor::bmp::Length(dynType);
    length += cmajor::bmp::Length(value);
    length += cmajor::bmp::Length(count);
    return length;
}

void ChildResult::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, expr);
    cmajor::bmp::Write(writer, name);
    cmajor::bmp::Write(writer, type);
    cmajor::bmp::Write(writer, dynType);
    cmajor::bmp::Write(writer, value);
    cmajor::bmp::Write(writer, count);
}

void ChildResult::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, expr);
    cmajor::bmp::Read(reader, name);
    cmajor::bmp::Read(reader, type);
    cmajor::bmp::Read(reader, dynType);
    cmajor::bmp::Read(reader, value);
    cmajor::bmp::Read(reader, count);
}

TargetState::TargetState()
     : TargetState::TargetState(bmpTargetStateId)
{
}

TargetState::TargetState(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), stopReason(), exitCode(), breakpointId(), signalName(), signalMeaning()
{
}

std::string TargetState::ClassName()
{
    return "db::TargetState";
}

void TargetState::Register()
{
    cmajor::bmp::RegisterMessage<TargetState>();
}

cmajor::bmp::BinaryMessage* TargetState::Create(uint32_t messageId)
{
    return new TargetState(messageId);
}

uint32_t TargetState::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(stopReason);
    length += cmajor::bmp::Length(exitCode);
    length += cmajor::bmp::Length(breakpointId);
    length += cmajor::bmp::Length(signalName);
    length += cmajor::bmp::Length(signalMeaning);
    return length;
}

void TargetState::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, stopReason);
    cmajor::bmp::Write(writer, exitCode);
    cmajor::bmp::Write(writer, breakpointId);
    cmajor::bmp::Write(writer, signalName);
    cmajor::bmp::Write(writer, signalMeaning);
}

void TargetState::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, stopReason);
    cmajor::bmp::Read(reader, exitCode);
    cmajor::bmp::Read(reader, breakpointId);
    cmajor::bmp::Read(reader, signalName);
    cmajor::bmp::Read(reader, signalMeaning);
}

BreakpointInfo::BreakpointInfo()
     : BreakpointInfo::BreakpointInfo(bmpBreakpointInfoId)
{
}

BreakpointInfo::BreakpointInfo(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), breakpointId(), numInsts(), location(), error()
{
}

std::string BreakpointInfo::ClassName()
{
    return "db::BreakpointInfo";
}

void BreakpointInfo::Register()
{
    cmajor::bmp::RegisterMessage<BreakpointInfo>();
}

cmajor::bmp::BinaryMessage* BreakpointInfo::Create(uint32_t messageId)
{
    return new BreakpointInfo(messageId);
}

uint32_t BreakpointInfo::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(breakpointId);
    length += cmajor::bmp::Length(numInsts);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(error);
    return length;
}

void BreakpointInfo::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, breakpointId);
    cmajor::bmp::Write(writer, numInsts);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, error);
}

void BreakpointInfo::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, breakpointId);
    cmajor::bmp::Read(reader, numInsts);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, error);
}

StartDebugRequest::StartDebugRequest()
     : StartDebugRequest::StartDebugRequest(bmpStartDebugRequestId)
{
}

StartDebugRequest::StartDebugRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), breakpointLocations()
{
}

std::string StartDebugRequest::ClassName()
{
    return "db::StartDebugRequest";
}

void StartDebugRequest::Register()
{
    cmajor::bmp::RegisterMessage<StartDebugRequest>();
}

cmajor::bmp::BinaryMessage* StartDebugRequest::Create(uint32_t messageId)
{
    return new StartDebugRequest(messageId);
}

uint32_t StartDebugRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(breakpointLocations);
    return length;
}

void StartDebugRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, breakpointLocations);
}

void StartDebugRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, breakpointLocations);
}

StartDebugReply::StartDebugReply()
     : StartDebugReply::StartDebugReply(bmpStartDebugReplyId)
{
}

StartDebugReply::StartDebugReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), breakpointInfos(), error()
{
}

std::string StartDebugReply::ClassName()
{
    return "db::StartDebugReply";
}

void StartDebugReply::Register()
{
    cmajor::bmp::RegisterMessage<StartDebugReply>();
}

cmajor::bmp::BinaryMessage* StartDebugReply::Create(uint32_t messageId)
{
    return new StartDebugReply(messageId);
}

uint32_t StartDebugReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(breakpointInfos);
    length += cmajor::bmp::Length(error);
    return length;
}

void StartDebugReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, breakpointInfos);
    cmajor::bmp::Write(writer, error);
}

void StartDebugReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, breakpointInfos);
    cmajor::bmp::Read(reader, error);
}

StopDebugRequest::StopDebugRequest()
     : StopDebugRequest::StopDebugRequest(bmpStopDebugRequestId)
{
}

StopDebugRequest::StopDebugRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopDebugRequest::ClassName()
{
    return "db::StopDebugRequest";
}

void StopDebugRequest::Register()
{
    cmajor::bmp::RegisterMessage<StopDebugRequest>();
}

cmajor::bmp::BinaryMessage* StopDebugRequest::Create(uint32_t messageId)
{
    return new StopDebugRequest(messageId);
}

uint32_t StopDebugRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopDebugRequest::Write(util::MemoryWriter& writer) const
{
}

void StopDebugRequest::Read(util::MemoryReader& reader)
{
}

StopDebugReply::StopDebugReply()
     : StopDebugReply::StopDebugReply(bmpStopDebugReplyId)
{
}

StopDebugReply::StopDebugReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StopDebugReply::ClassName()
{
    return "db::StopDebugReply";
}

void StopDebugReply::Register()
{
    cmajor::bmp::RegisterMessage<StopDebugReply>();
}

cmajor::bmp::BinaryMessage* StopDebugReply::Create(uint32_t messageId)
{
    return new StopDebugReply(messageId);
}

uint32_t StopDebugReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void StopDebugReply::Write(util::MemoryWriter& writer) const
{
}

void StopDebugReply::Read(util::MemoryReader& reader)
{
}

GenericDebugErrorReply::GenericDebugErrorReply()
     : GenericDebugErrorReply::GenericDebugErrorReply(bmpGenericDebugErrorReplyId)
{
}

GenericDebugErrorReply::GenericDebugErrorReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), errorMessage()
{
}

std::string GenericDebugErrorReply::ClassName()
{
    return "db::GenericDebugErrorReply";
}

void GenericDebugErrorReply::Register()
{
    cmajor::bmp::RegisterMessage<GenericDebugErrorReply>();
}

cmajor::bmp::BinaryMessage* GenericDebugErrorReply::Create(uint32_t messageId)
{
    return new GenericDebugErrorReply(messageId);
}

uint32_t GenericDebugErrorReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(errorMessage);
    return length;
}

void GenericDebugErrorReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, errorMessage);
}

void GenericDebugErrorReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, errorMessage);
}

ContinueRequest::ContinueRequest()
     : ContinueRequest::ContinueRequest(bmpContinueRequestId)
{
}

ContinueRequest::ContinueRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string ContinueRequest::ClassName()
{
    return "db::ContinueRequest";
}

void ContinueRequest::Register()
{
    cmajor::bmp::RegisterMessage<ContinueRequest>();
}

cmajor::bmp::BinaryMessage* ContinueRequest::Create(uint32_t messageId)
{
    return new ContinueRequest(messageId);
}

uint32_t ContinueRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void ContinueRequest::Write(util::MemoryWriter& writer) const
{
}

void ContinueRequest::Read(util::MemoryReader& reader)
{
}

ContinueReply::ContinueReply()
     : ContinueReply::ContinueReply(bmpContinueReplyId)
{
}

ContinueReply::ContinueReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), error()
{
}

std::string ContinueReply::ClassName()
{
    return "db::ContinueReply";
}

void ContinueReply::Register()
{
    cmajor::bmp::RegisterMessage<ContinueReply>();
}

cmajor::bmp::BinaryMessage* ContinueReply::Create(uint32_t messageId)
{
    return new ContinueReply(messageId);
}

uint32_t ContinueReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(error);
    return length;
}

void ContinueReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, error);
}

void ContinueReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, error);
}

NextRequest::NextRequest()
     : NextRequest::NextRequest(bmpNextRequestId)
{
}

NextRequest::NextRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string NextRequest::ClassName()
{
    return "db::NextRequest";
}

void NextRequest::Register()
{
    cmajor::bmp::RegisterMessage<NextRequest>();
}

cmajor::bmp::BinaryMessage* NextRequest::Create(uint32_t messageId)
{
    return new NextRequest(messageId);
}

uint32_t NextRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void NextRequest::Write(util::MemoryWriter& writer) const
{
}

void NextRequest::Read(util::MemoryReader& reader)
{
}

NextReply::NextReply()
     : NextReply::NextReply(bmpNextReplyId)
{
}

NextReply::NextReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), error()
{
}

std::string NextReply::ClassName()
{
    return "db::NextReply";
}

void NextReply::Register()
{
    cmajor::bmp::RegisterMessage<NextReply>();
}

cmajor::bmp::BinaryMessage* NextReply::Create(uint32_t messageId)
{
    return new NextReply(messageId);
}

uint32_t NextReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(error);
    return length;
}

void NextReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, error);
}

void NextReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, error);
}

StepRequest::StepRequest()
     : StepRequest::StepRequest(bmpStepRequestId)
{
}

StepRequest::StepRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string StepRequest::ClassName()
{
    return "db::StepRequest";
}

void StepRequest::Register()
{
    cmajor::bmp::RegisterMessage<StepRequest>();
}

cmajor::bmp::BinaryMessage* StepRequest::Create(uint32_t messageId)
{
    return new StepRequest(messageId);
}

uint32_t StepRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void StepRequest::Write(util::MemoryWriter& writer) const
{
}

void StepRequest::Read(util::MemoryReader& reader)
{
}

StepReply::StepReply()
     : StepReply::StepReply(bmpStepReplyId)
{
}

StepReply::StepReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), error()
{
}

std::string StepReply::ClassName()
{
    return "db::StepReply";
}

void StepReply::Register()
{
    cmajor::bmp::RegisterMessage<StepReply>();
}

cmajor::bmp::BinaryMessage* StepReply::Create(uint32_t messageId)
{
    return new StepReply(messageId);
}

uint32_t StepReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(error);
    return length;
}

void StepReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, error);
}

void StepReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, error);
}

FinishRequest::FinishRequest()
     : FinishRequest::FinishRequest(bmpFinishRequestId)
{
}

FinishRequest::FinishRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string FinishRequest::ClassName()
{
    return "db::FinishRequest";
}

void FinishRequest::Register()
{
    cmajor::bmp::RegisterMessage<FinishRequest>();
}

cmajor::bmp::BinaryMessage* FinishRequest::Create(uint32_t messageId)
{
    return new FinishRequest(messageId);
}

uint32_t FinishRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void FinishRequest::Write(util::MemoryWriter& writer) const
{
}

void FinishRequest::Read(util::MemoryReader& reader)
{
}

FinishReply::FinishReply()
     : FinishReply::FinishReply(bmpFinishReplyId)
{
}

FinishReply::FinishReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), error()
{
}

std::string FinishReply::ClassName()
{
    return "db::FinishReply";
}

void FinishReply::Register()
{
    cmajor::bmp::RegisterMessage<FinishReply>();
}

cmajor::bmp::BinaryMessage* FinishReply::Create(uint32_t messageId)
{
    return new FinishReply(messageId);
}

uint32_t FinishReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(error);
    return length;
}

void FinishReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, error);
}

void FinishReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, error);
}

UntilRequest::UntilRequest()
     : UntilRequest::UntilRequest(bmpUntilRequestId)
{
}

UntilRequest::UntilRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), sourceLoc()
{
}

std::string UntilRequest::ClassName()
{
    return "db::UntilRequest";
}

void UntilRequest::Register()
{
    cmajor::bmp::RegisterMessage<UntilRequest>();
}

cmajor::bmp::BinaryMessage* UntilRequest::Create(uint32_t messageId)
{
    return new UntilRequest(messageId);
}

uint32_t UntilRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(sourceLoc);
    return length;
}

void UntilRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, sourceLoc);
}

void UntilRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, sourceLoc);
}

UntilReply::UntilReply()
     : UntilReply::UntilReply(bmpUntilReplyId)
{
}

UntilReply::UntilReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), location(), state(), error()
{
}

std::string UntilReply::ClassName()
{
    return "db::UntilReply";
}

void UntilReply::Register()
{
    cmajor::bmp::RegisterMessage<UntilReply>();
}

cmajor::bmp::BinaryMessage* UntilReply::Create(uint32_t messageId)
{
    return new UntilReply(messageId);
}

uint32_t UntilReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(location);
    length += cmajor::bmp::Length(state);
    length += cmajor::bmp::Length(error);
    return length;
}

void UntilReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, location);
    cmajor::bmp::Write(writer, state);
    cmajor::bmp::Write(writer, error);
}

void UntilReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, location);
    cmajor::bmp::Read(reader, state);
    cmajor::bmp::Read(reader, error);
}

BreakRequest::BreakRequest()
     : BreakRequest::BreakRequest(bmpBreakRequestId)
{
}

BreakRequest::BreakRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), breakpointLocation()
{
}

std::string BreakRequest::ClassName()
{
    return "db::BreakRequest";
}

void BreakRequest::Register()
{
    cmajor::bmp::RegisterMessage<BreakRequest>();
}

cmajor::bmp::BinaryMessage* BreakRequest::Create(uint32_t messageId)
{
    return new BreakRequest(messageId);
}

uint32_t BreakRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(breakpointLocation);
    return length;
}

void BreakRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, breakpointLocation);
}

void BreakRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, breakpointLocation);
}

BreakReply::BreakReply()
     : BreakReply::BreakReply(bmpBreakReplyId)
{
}

BreakReply::BreakReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), breakpointInfo()
{
}

std::string BreakReply::ClassName()
{
    return "db::BreakReply";
}

void BreakReply::Register()
{
    cmajor::bmp::RegisterMessage<BreakReply>();
}

cmajor::bmp::BinaryMessage* BreakReply::Create(uint32_t messageId)
{
    return new BreakReply(messageId);
}

uint32_t BreakReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(breakpointInfo);
    return length;
}

void BreakReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, breakpointInfo);
}

void BreakReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, breakpointInfo);
}

DeleteRequest::DeleteRequest()
     : DeleteRequest::DeleteRequest(bmpDeleteRequestId)
{
}

DeleteRequest::DeleteRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), breakpointId()
{
}

std::string DeleteRequest::ClassName()
{
    return "db::DeleteRequest";
}

void DeleteRequest::Register()
{
    cmajor::bmp::RegisterMessage<DeleteRequest>();
}

cmajor::bmp::BinaryMessage* DeleteRequest::Create(uint32_t messageId)
{
    return new DeleteRequest(messageId);
}

uint32_t DeleteRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(breakpointId);
    return length;
}

void DeleteRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, breakpointId);
}

void DeleteRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, breakpointId);
}

DeleteReply::DeleteReply()
     : DeleteReply::DeleteReply(bmpDeleteReplyId)
{
}

DeleteReply::DeleteReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), error()
{
}

std::string DeleteReply::ClassName()
{
    return "db::DeleteReply";
}

void DeleteReply::Register()
{
    cmajor::bmp::RegisterMessage<DeleteReply>();
}

cmajor::bmp::BinaryMessage* DeleteReply::Create(uint32_t messageId)
{
    return new DeleteReply(messageId);
}

uint32_t DeleteReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(error);
    return length;
}

void DeleteReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, error);
}

void DeleteReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, error);
}

DepthRequest::DepthRequest()
     : DepthRequest::DepthRequest(bmpDepthRequestId)
{
}

DepthRequest::DepthRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string DepthRequest::ClassName()
{
    return "db::DepthRequest";
}

void DepthRequest::Register()
{
    cmajor::bmp::RegisterMessage<DepthRequest>();
}

cmajor::bmp::BinaryMessage* DepthRequest::Create(uint32_t messageId)
{
    return new DepthRequest(messageId);
}

uint32_t DepthRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void DepthRequest::Write(util::MemoryWriter& writer) const
{
}

void DepthRequest::Read(util::MemoryReader& reader)
{
}

DepthReply::DepthReply()
     : DepthReply::DepthReply(bmpDepthReplyId)
{
}

DepthReply::DepthReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), depth(), error()
{
}

std::string DepthReply::ClassName()
{
    return "db::DepthReply";
}

void DepthReply::Register()
{
    cmajor::bmp::RegisterMessage<DepthReply>();
}

cmajor::bmp::BinaryMessage* DepthReply::Create(uint32_t messageId)
{
    return new DepthReply(messageId);
}

uint32_t DepthReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(depth);
    length += cmajor::bmp::Length(error);
    return length;
}

void DepthReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, depth);
    cmajor::bmp::Write(writer, error);
}

void DepthReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, depth);
    cmajor::bmp::Read(reader, error);
}

FramesRequest::FramesRequest()
     : FramesRequest::FramesRequest(bmpFramesRequestId)
{
}

FramesRequest::FramesRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), lowFrame(), highFrame()
{
}

std::string FramesRequest::ClassName()
{
    return "db::FramesRequest";
}

void FramesRequest::Register()
{
    cmajor::bmp::RegisterMessage<FramesRequest>();
}

cmajor::bmp::BinaryMessage* FramesRequest::Create(uint32_t messageId)
{
    return new FramesRequest(messageId);
}

uint32_t FramesRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(lowFrame);
    length += cmajor::bmp::Length(highFrame);
    return length;
}

void FramesRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, lowFrame);
    cmajor::bmp::Write(writer, highFrame);
}

void FramesRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, lowFrame);
    cmajor::bmp::Read(reader, highFrame);
}

FramesReply::FramesReply()
     : FramesReply::FramesReply(bmpFramesReplyId)
{
}

FramesReply::FramesReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), frames(), error()
{
}

std::string FramesReply::ClassName()
{
    return "db::FramesReply";
}

void FramesReply::Register()
{
    cmajor::bmp::RegisterMessage<FramesReply>();
}

cmajor::bmp::BinaryMessage* FramesReply::Create(uint32_t messageId)
{
    return new FramesReply(messageId);
}

uint32_t FramesReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(frames);
    length += cmajor::bmp::Length(error);
    return length;
}

void FramesReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, frames);
    cmajor::bmp::Write(writer, error);
}

void FramesReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, frames);
    cmajor::bmp::Read(reader, error);
}

EvaluateRequest::EvaluateRequest()
     : EvaluateRequest::EvaluateRequest(bmpEvaluateRequestId)
{
}

EvaluateRequest::EvaluateRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), expression()
{
}

std::string EvaluateRequest::ClassName()
{
    return "db::EvaluateRequest";
}

void EvaluateRequest::Register()
{
    cmajor::bmp::RegisterMessage<EvaluateRequest>();
}

cmajor::bmp::BinaryMessage* EvaluateRequest::Create(uint32_t messageId)
{
    return new EvaluateRequest(messageId);
}

uint32_t EvaluateRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(expression);
    return length;
}

void EvaluateRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, expression);
}

void EvaluateRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, expression);
}

EvaluateReply::EvaluateReply()
     : EvaluateReply::EvaluateReply(bmpEvaluateReplyId)
{
}

EvaluateReply::EvaluateReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), result(), error()
{
}

std::string EvaluateReply::ClassName()
{
    return "db::EvaluateReply";
}

void EvaluateReply::Register()
{
    cmajor::bmp::RegisterMessage<EvaluateReply>();
}

cmajor::bmp::BinaryMessage* EvaluateReply::Create(uint32_t messageId)
{
    return new EvaluateReply(messageId);
}

uint32_t EvaluateReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(result);
    length += cmajor::bmp::Length(error);
    return length;
}

void EvaluateReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, result);
    cmajor::bmp::Write(writer, error);
}

void EvaluateReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, result);
    cmajor::bmp::Read(reader, error);
}

CountRequest::CountRequest()
     : CountRequest::CountRequest(bmpCountRequestId)
{
}

CountRequest::CountRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), expression()
{
}

std::string CountRequest::ClassName()
{
    return "db::CountRequest";
}

void CountRequest::Register()
{
    cmajor::bmp::RegisterMessage<CountRequest>();
}

cmajor::bmp::BinaryMessage* CountRequest::Create(uint32_t messageId)
{
    return new CountRequest(messageId);
}

uint32_t CountRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(expression);
    return length;
}

void CountRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, expression);
}

void CountRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, expression);
}

CountReply::CountReply()
     : CountReply::CountReply(bmpCountReplyId)
{
}

CountReply::CountReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), count(), error()
{
}

std::string CountReply::ClassName()
{
    return "db::CountReply";
}

void CountReply::Register()
{
    cmajor::bmp::RegisterMessage<CountReply>();
}

cmajor::bmp::BinaryMessage* CountReply::Create(uint32_t messageId)
{
    return new CountReply(messageId);
}

uint32_t CountReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(count);
    length += cmajor::bmp::Length(error);
    return length;
}

void CountReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, count);
    cmajor::bmp::Write(writer, error);
}

void CountReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, count);
    cmajor::bmp::Read(reader, error);
}

EvaluateChildRequest::EvaluateChildRequest()
     : EvaluateChildRequest::EvaluateChildRequest(bmpEvaluateChildRequestId)
{
}

EvaluateChildRequest::EvaluateChildRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), expression(), start(), count()
{
}

std::string EvaluateChildRequest::ClassName()
{
    return "db::EvaluateChildRequest";
}

void EvaluateChildRequest::Register()
{
    cmajor::bmp::RegisterMessage<EvaluateChildRequest>();
}

cmajor::bmp::BinaryMessage* EvaluateChildRequest::Create(uint32_t messageId)
{
    return new EvaluateChildRequest(messageId);
}

uint32_t EvaluateChildRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(expression);
    length += cmajor::bmp::Length(start);
    length += cmajor::bmp::Length(count);
    return length;
}

void EvaluateChildRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, expression);
    cmajor::bmp::Write(writer, start);
    cmajor::bmp::Write(writer, count);
}

void EvaluateChildRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, expression);
    cmajor::bmp::Read(reader, start);
    cmajor::bmp::Read(reader, count);
}

EvaluateChildReply::EvaluateChildReply()
     : EvaluateChildReply::EvaluateChildReply(bmpEvaluateChildReplyId)
{
}

EvaluateChildReply::EvaluateChildReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), success(), results(), error()
{
}

std::string EvaluateChildReply::ClassName()
{
    return "db::EvaluateChildReply";
}

void EvaluateChildReply::Register()
{
    cmajor::bmp::RegisterMessage<EvaluateChildReply>();
}

cmajor::bmp::BinaryMessage* EvaluateChildReply::Create(uint32_t messageId)
{
    return new EvaluateChildReply(messageId);
}

uint32_t EvaluateChildReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(success);
    length += cmajor::bmp::Length(results);
    length += cmajor::bmp::Length(error);
    return length;
}

void EvaluateChildReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, success);
    cmajor::bmp::Write(writer, results);
    cmajor::bmp::Write(writer, error);
}

void EvaluateChildReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, success);
    cmajor::bmp::Read(reader, results);
    cmajor::bmp::Read(reader, error);
}

TargetRunningRequest::TargetRunningRequest()
     : TargetRunningRequest::TargetRunningRequest(bmpTargetRunningRequestId)
{
}

TargetRunningRequest::TargetRunningRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string TargetRunningRequest::ClassName()
{
    return "db::TargetRunningRequest";
}

void TargetRunningRequest::Register()
{
    cmajor::bmp::RegisterMessage<TargetRunningRequest>();
}

cmajor::bmp::BinaryMessage* TargetRunningRequest::Create(uint32_t messageId)
{
    return new TargetRunningRequest(messageId);
}

uint32_t TargetRunningRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void TargetRunningRequest::Write(util::MemoryWriter& writer) const
{
}

void TargetRunningRequest::Read(util::MemoryReader& reader)
{
}

TargetRunningReply::TargetRunningReply()
     : TargetRunningReply::TargetRunningReply(bmpTargetRunningReplyId)
{
}

TargetRunningReply::TargetRunningReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string TargetRunningReply::ClassName()
{
    return "db::TargetRunningReply";
}

void TargetRunningReply::Register()
{
    cmajor::bmp::RegisterMessage<TargetRunningReply>();
}

cmajor::bmp::BinaryMessage* TargetRunningReply::Create(uint32_t messageId)
{
    return new TargetRunningReply(messageId);
}

uint32_t TargetRunningReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void TargetRunningReply::Write(util::MemoryWriter& writer) const
{
}

void TargetRunningReply::Read(util::MemoryReader& reader)
{
}

TargetInputRequest::TargetInputRequest()
     : TargetInputRequest::TargetInputRequest(bmpTargetInputRequestId)
{
}

TargetInputRequest::TargetInputRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string TargetInputRequest::ClassName()
{
    return "db::TargetInputRequest";
}

void TargetInputRequest::Register()
{
    cmajor::bmp::RegisterMessage<TargetInputRequest>();
}

cmajor::bmp::BinaryMessage* TargetInputRequest::Create(uint32_t messageId)
{
    return new TargetInputRequest(messageId);
}

uint32_t TargetInputRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void TargetInputRequest::Write(util::MemoryWriter& writer) const
{
}

void TargetInputRequest::Read(util::MemoryReader& reader)
{
}

TargetInputReply::TargetInputReply()
     : TargetInputReply::TargetInputReply(bmpTargetInputReplyId)
{
}

TargetInputReply::TargetInputReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), line(), eof()
{
}

std::string TargetInputReply::ClassName()
{
    return "db::TargetInputReply";
}

void TargetInputReply::Register()
{
    cmajor::bmp::RegisterMessage<TargetInputReply>();
}

cmajor::bmp::BinaryMessage* TargetInputReply::Create(uint32_t messageId)
{
    return new TargetInputReply(messageId);
}

uint32_t TargetInputReply::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(line);
    length += cmajor::bmp::Length(eof);
    return length;
}

void TargetInputReply::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, line);
    cmajor::bmp::Write(writer, eof);
}

void TargetInputReply::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, line);
    cmajor::bmp::Read(reader, eof);
}

TargetOutputRequest::TargetOutputRequest()
     : TargetOutputRequest::TargetOutputRequest(bmpTargetOutputRequestId)
{
}

TargetOutputRequest::TargetOutputRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), handle(), output()
{
}

std::string TargetOutputRequest::ClassName()
{
    return "db::TargetOutputRequest";
}

void TargetOutputRequest::Register()
{
    cmajor::bmp::RegisterMessage<TargetOutputRequest>();
}

cmajor::bmp::BinaryMessage* TargetOutputRequest::Create(uint32_t messageId)
{
    return new TargetOutputRequest(messageId);
}

uint32_t TargetOutputRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(handle);
    length += cmajor::bmp::Length(output);
    return length;
}

void TargetOutputRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, handle);
    cmajor::bmp::Write(writer, output);
}

void TargetOutputRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, handle);
    cmajor::bmp::Read(reader, output);
}

TargetOutputReply::TargetOutputReply()
     : TargetOutputReply::TargetOutputReply(bmpTargetOutputReplyId)
{
}

TargetOutputReply::TargetOutputReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string TargetOutputReply::ClassName()
{
    return "db::TargetOutputReply";
}

void TargetOutputReply::Register()
{
    cmajor::bmp::RegisterMessage<TargetOutputReply>();
}

cmajor::bmp::BinaryMessage* TargetOutputReply::Create(uint32_t messageId)
{
    return new TargetOutputReply(messageId);
}

uint32_t TargetOutputReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void TargetOutputReply::Write(util::MemoryWriter& writer) const
{
}

void TargetOutputReply::Read(util::MemoryReader& reader)
{
}

LogDebugMessageRequest::LogDebugMessageRequest()
     : LogDebugMessageRequest::LogDebugMessageRequest(bmpLogDebugMessageRequestId)
{
}

LogDebugMessageRequest::LogDebugMessageRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_), logMessage()
{
}

std::string LogDebugMessageRequest::ClassName()
{
    return "db::LogDebugMessageRequest";
}

void LogDebugMessageRequest::Register()
{
    cmajor::bmp::RegisterMessage<LogDebugMessageRequest>();
}

cmajor::bmp::BinaryMessage* LogDebugMessageRequest::Create(uint32_t messageId)
{
    return new LogDebugMessageRequest(messageId);
}

uint32_t LogDebugMessageRequest::Length() const
{
    uint32_t length = 0;
    length += cmajor::bmp::Length(logMessage);
    return length;
}

void LogDebugMessageRequest::Write(util::MemoryWriter& writer) const
{
    cmajor::bmp::Write(writer, logMessage);
}

void LogDebugMessageRequest::Read(util::MemoryReader& reader)
{
    cmajor::bmp::Read(reader, logMessage);
}

LogDebugMessageReply::LogDebugMessageReply()
     : LogDebugMessageReply::LogDebugMessageReply(bmpLogDebugMessageReplyId)
{
}

LogDebugMessageReply::LogDebugMessageReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string LogDebugMessageReply::ClassName()
{
    return "db::LogDebugMessageReply";
}

void LogDebugMessageReply::Register()
{
    cmajor::bmp::RegisterMessage<LogDebugMessageReply>();
}

cmajor::bmp::BinaryMessage* LogDebugMessageReply::Create(uint32_t messageId)
{
    return new LogDebugMessageReply(messageId);
}

uint32_t LogDebugMessageReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void LogDebugMessageReply::Write(util::MemoryWriter& writer) const
{
}

void LogDebugMessageReply::Read(util::MemoryReader& reader)
{
}

KillRequest::KillRequest()
     : KillRequest::KillRequest(bmpKillRequestId)
{
}

KillRequest::KillRequest(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string KillRequest::ClassName()
{
    return "db::KillRequest";
}

void KillRequest::Register()
{
    cmajor::bmp::RegisterMessage<KillRequest>();
}

cmajor::bmp::BinaryMessage* KillRequest::Create(uint32_t messageId)
{
    return new KillRequest(messageId);
}

uint32_t KillRequest::Length() const
{
    uint32_t length = 0;
    return length;
}

void KillRequest::Write(util::MemoryWriter& writer) const
{
}

void KillRequest::Read(util::MemoryReader& reader)
{
}

KillReply::KillReply()
     : KillReply::KillReply(bmpKillReplyId)
{
}

KillReply::KillReply(uint32_t id_)
     : cmajor::bmp::BinaryMessage(id_)
{
}

std::string KillReply::ClassName()
{
    return "db::KillReply";
}

void KillReply::Register()
{
    cmajor::bmp::RegisterMessage<KillReply>();
}

cmajor::bmp::BinaryMessage* KillReply::Create(uint32_t messageId)
{
    return new KillReply(messageId);
}

uint32_t KillReply::Length() const
{
    uint32_t length = 0;
    return length;
}

void KillReply::Write(util::MemoryWriter& writer) const
{
}

void KillReply::Read(util::MemoryReader& reader)
{
}


} // namespace db

namespace {

struct Reg
{
    Reg()
    {
        db::Location::Register();
        db::Type::Register();
        db::Result::Register();
        db::ChildResult::Register();
        db::TargetState::Register();
        db::BreakpointInfo::Register();
        db::StartDebugRequest::Register();
        db::StartDebugReply::Register();
        db::StopDebugRequest::Register();
        db::StopDebugReply::Register();
        db::GenericDebugErrorReply::Register();
        db::ContinueRequest::Register();
        db::ContinueReply::Register();
        db::NextRequest::Register();
        db::NextReply::Register();
        db::StepRequest::Register();
        db::StepReply::Register();
        db::FinishRequest::Register();
        db::FinishReply::Register();
        db::UntilRequest::Register();
        db::UntilReply::Register();
        db::BreakRequest::Register();
        db::BreakReply::Register();
        db::DeleteRequest::Register();
        db::DeleteReply::Register();
        db::DepthRequest::Register();
        db::DepthReply::Register();
        db::FramesRequest::Register();
        db::FramesReply::Register();
        db::EvaluateRequest::Register();
        db::EvaluateReply::Register();
        db::CountRequest::Register();
        db::CountReply::Register();
        db::EvaluateChildRequest::Register();
        db::EvaluateChildReply::Register();
        db::TargetRunningRequest::Register();
        db::TargetRunningReply::Register();
        db::TargetInputRequest::Register();
        db::TargetInputReply::Register();
        db::TargetOutputRequest::Register();
        db::TargetOutputReply::Register();
        db::LogDebugMessageRequest::Register();
        db::LogDebugMessageReply::Register();
        db::KillRequest::Register();
        db::KillReply::Register();
    }
};

Reg reg;

} // namespace

