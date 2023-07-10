// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/db.schema' by cmajor binary protocol message generator version 5.0.0

export module cmajor.debug.message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;
import cmajor.common.message;


using SourceLoc = common::SourceLoc;
export namespace db {

const uint32_t bmpLocationId = 1547390544;

class Location : public cmajor::bmp::BinaryMessage
{
public:
    Location();
    Location(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string func;
    std::string file;
    int32_t line;
    int32_t scol;
    int32_t ecol;
    int32_t level;
};

const uint32_t bmpTypeId = 2914857150;

class Type : public cmajor::bmp::BinaryMessage
{
public:
    Type();
    Type(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string name;
    std::string id;
};

const uint32_t bmpResultId = 508923823;

class Result : public cmajor::bmp::BinaryMessage
{
public:
    Result();
    Result(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    Type staticType;
    Type dynamicType;
    bool initialized;
    std::string value;
};

const uint32_t bmpChildResultId = 1327160344;

class ChildResult : public cmajor::bmp::BinaryMessage
{
public:
    ChildResult();
    ChildResult(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string expr;
    std::string name;
    std::string type;
    std::string dynType;
    std::string value;
    int32_t count;
};

const uint32_t bmpTargetStateId = 3897194628;

class TargetState : public cmajor::bmp::BinaryMessage
{
public:
    TargetState();
    TargetState(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string stopReason;
    std::string exitCode;
    std::string breakpointId;
    std::string signalName;
    std::string signalMeaning;
};

const uint32_t bmpBreakpointInfoId = 4191602890;

class BreakpointInfo : public cmajor::bmp::BinaryMessage
{
public:
    BreakpointInfo();
    BreakpointInfo(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    std::string breakpointId;
    int32_t numInsts;
    Location location;
    std::string error;
};

const uint32_t bmpStartDebugRequestId = 2113606929;

class StartDebugRequest : public cmajor::bmp::BinaryMessage
{
public:
    StartDebugRequest();
    StartDebugRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::vector<SourceLoc> breakpointLocations;
};

const uint32_t bmpStartDebugReplyId = 161505241;

class StartDebugReply : public cmajor::bmp::BinaryMessage
{
public:
    StartDebugReply();
    StartDebugReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::vector<BreakpointInfo> breakpointInfos;
    std::string error;
};

const uint32_t bmpStopDebugRequestId = 4171664306;

class StopDebugRequest : public cmajor::bmp::BinaryMessage
{
public:
    StopDebugRequest();
    StopDebugRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStopDebugReplyId = 1968648994;

class StopDebugReply : public cmajor::bmp::BinaryMessage
{
public:
    StopDebugReply();
    StopDebugReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpGenericDebugErrorReplyId = 2447478980;

class GenericDebugErrorReply : public cmajor::bmp::BinaryMessage
{
public:
    GenericDebugErrorReply();
    GenericDebugErrorReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string errorMessage;
};

const uint32_t bmpContinueRequestId = 1471952156;

class ContinueRequest : public cmajor::bmp::BinaryMessage
{
public:
    ContinueRequest();
    ContinueRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpContinueReplyId = 1546153786;

class ContinueReply : public cmajor::bmp::BinaryMessage
{
public:
    ContinueReply();
    ContinueReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::string error;
};

const uint32_t bmpNextRequestId = 2834535409;

class NextRequest : public cmajor::bmp::BinaryMessage
{
public:
    NextRequest();
    NextRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpNextReplyId = 25267456;

class NextReply : public cmajor::bmp::BinaryMessage
{
public:
    NextReply();
    NextReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::string error;
};

const uint32_t bmpStepRequestId = 4084241166;

class StepRequest : public cmajor::bmp::BinaryMessage
{
public:
    StepRequest();
    StepRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStepReplyId = 1581941352;

class StepReply : public cmajor::bmp::BinaryMessage
{
public:
    StepReply();
    StepReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::string error;
};

const uint32_t bmpFinishRequestId = 3858226593;

class FinishRequest : public cmajor::bmp::BinaryMessage
{
public:
    FinishRequest();
    FinishRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpFinishReplyId = 3633362252;

class FinishReply : public cmajor::bmp::BinaryMessage
{
public:
    FinishReply();
    FinishReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::string error;
};

const uint32_t bmpUntilRequestId = 2025501188;

class UntilRequest : public cmajor::bmp::BinaryMessage
{
public:
    UntilRequest();
    UntilRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    SourceLoc sourceLoc;
};

const uint32_t bmpUntilReplyId = 1252995174;

class UntilReply : public cmajor::bmp::BinaryMessage
{
public:
    UntilReply();
    UntilReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Location location;
    TargetState state;
    std::string error;
};

const uint32_t bmpBreakRequestId = 3479099848;

class BreakRequest : public cmajor::bmp::BinaryMessage
{
public:
    BreakRequest();
    BreakRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    SourceLoc breakpointLocation;
};

const uint32_t bmpBreakReplyId = 954686659;

class BreakReply : public cmajor::bmp::BinaryMessage
{
public:
    BreakReply();
    BreakReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    BreakpointInfo breakpointInfo;
};

const uint32_t bmpDeleteRequestId = 1442107473;

class DeleteRequest : public cmajor::bmp::BinaryMessage
{
public:
    DeleteRequest();
    DeleteRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string breakpointId;
};

const uint32_t bmpDeleteReplyId = 553028847;

class DeleteReply : public cmajor::bmp::BinaryMessage
{
public:
    DeleteReply();
    DeleteReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    std::string error;
};

const uint32_t bmpDepthRequestId = 1044126982;

class DepthRequest : public cmajor::bmp::BinaryMessage
{
public:
    DepthRequest();
    DepthRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpDepthReplyId = 412587117;

class DepthReply : public cmajor::bmp::BinaryMessage
{
public:
    DepthReply();
    DepthReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    int32_t depth;
    std::string error;
};

const uint32_t bmpFramesRequestId = 3705985587;

class FramesRequest : public cmajor::bmp::BinaryMessage
{
public:
    FramesRequest();
    FramesRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    int32_t lowFrame;
    int32_t highFrame;
};

const uint32_t bmpFramesReplyId = 4261454144;

class FramesReply : public cmajor::bmp::BinaryMessage
{
public:
    FramesReply();
    FramesReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    std::vector<Location> frames;
    std::string error;
};

const uint32_t bmpEvaluateRequestId = 909672462;

class EvaluateRequest : public cmajor::bmp::BinaryMessage
{
public:
    EvaluateRequest();
    EvaluateRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string expression;
};

const uint32_t bmpEvaluateReplyId = 3174785700;

class EvaluateReply : public cmajor::bmp::BinaryMessage
{
public:
    EvaluateReply();
    EvaluateReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    Result result;
    std::string error;
};

const uint32_t bmpCountRequestId = 2829641100;

class CountRequest : public cmajor::bmp::BinaryMessage
{
public:
    CountRequest();
    CountRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string expression;
};

const uint32_t bmpCountReplyId = 2901505741;

class CountReply : public cmajor::bmp::BinaryMessage
{
public:
    CountReply();
    CountReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    int32_t count;
    std::string error;
};

const uint32_t bmpEvaluateChildRequestId = 4110851001;

class EvaluateChildRequest : public cmajor::bmp::BinaryMessage
{
public:
    EvaluateChildRequest();
    EvaluateChildRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string expression;
    int32_t start;
    int32_t count;
};

const uint32_t bmpEvaluateChildReplyId = 679420226;

class EvaluateChildReply : public cmajor::bmp::BinaryMessage
{
public:
    EvaluateChildReply();
    EvaluateChildReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool success;
    std::vector<ChildResult> results;
    std::string error;
};

const uint32_t bmpTargetRunningRequestId = 3354602395;

class TargetRunningRequest : public cmajor::bmp::BinaryMessage
{
public:
    TargetRunningRequest();
    TargetRunningRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpTargetRunningReplyId = 2676018855;

class TargetRunningReply : public cmajor::bmp::BinaryMessage
{
public:
    TargetRunningReply();
    TargetRunningReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpTargetInputRequestId = 591991911;

class TargetInputRequest : public cmajor::bmp::BinaryMessage
{
public:
    TargetInputRequest();
    TargetInputRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpTargetInputReplyId = 383692291;

class TargetInputReply : public cmajor::bmp::BinaryMessage
{
public:
    TargetInputReply();
    TargetInputReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string line;
    bool eof;
};

const uint32_t bmpTargetOutputRequestId = 3116278524;

class TargetOutputRequest : public cmajor::bmp::BinaryMessage
{
public:
    TargetOutputRequest();
    TargetOutputRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    int32_t handle;
    std::string output;
};

const uint32_t bmpTargetOutputReplyId = 624821920;

class TargetOutputReply : public cmajor::bmp::BinaryMessage
{
public:
    TargetOutputReply();
    TargetOutputReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpLogDebugMessageRequestId = 3890811415;

class LogDebugMessageRequest : public cmajor::bmp::BinaryMessage
{
public:
    LogDebugMessageRequest();
    LogDebugMessageRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string logMessage;
};

const uint32_t bmpLogDebugMessageReplyId = 219991085;

class LogDebugMessageReply : public cmajor::bmp::BinaryMessage
{
public:
    LogDebugMessageReply();
    LogDebugMessageReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpKillRequestId = 1995387795;

class KillRequest : public cmajor::bmp::BinaryMessage
{
public:
    KillRequest();
    KillRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpKillReplyId = 1283001734;

class KillReply : public cmajor::bmp::BinaryMessage
{
public:
    KillReply();
    KillReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};


} // namespace db
