// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.msg.reg;

import cmajor.msg.build.server.message;
import cmajor.msg.cmdb.message;
import cmajor.msg.code.completion.message;

namespace cmajor::msg {

void Register()
{
    StopBuildRequest::Register();
    StopBuildReply::Register();
    KeepAliveBuildRequest::Register();
    KeepAliveBuildReply::Register();
    BuildRequest::Register();
    CompileError::Register();
    BuildReply::Register();
    LogBuildMessageRequest::Register();
    LogBuildMessageReply::Register();
    BuildProgressMessage::Register();
    GenericBuildErrorReply::Register();
    CacheModuleRequest::Register();
    CacheModuleReply::Register();
    DefinitionSourceLocation::Register();
    GetDefinitionRequest::Register();
    GetDefinitionReply::Register();

    Location::Register();
    Type::Register();
    Result::Register();
    ChildResult::Register();
    TargetState::Register();
    SourceLoc::Register();
    BreakpointInfo::Register();
    StartDebugRequest::Register();
    StartDebugReply::Register();
    StopDebugRequest::Register();
    StopDebugReply::Register();
    GenericDebugErrorReply::Register();
    ContinueRequest::Register();
    ContinueReply::Register();
    NextRequest::Register();
    NextReply::Register();
    StepRequest::Register();
    StepReply::Register();
    FinishRequest::Register();
    FinishReply::Register();
    UntilRequest::Register();
    UntilReply::Register();
    BreakRequest::Register();
    BreakReply::Register();
    DeleteRequest::Register();
    DeleteReply::Register();
    DepthRequest::Register();
    DepthReply::Register();
    FramesRequest::Register();
    FramesReply::Register();
    EvaluateRequest::Register();
    EvaluateReply::Register();
    CountRequest::Register();
    CountReply::Register();
    EvaluateChildRequest::Register();
    EvaluateChildReply::Register();
    TargetRunningRequest::Register();
    TargetRunningReply::Register();
    TargetInputRequest::Register();
    TargetInputReply::Register();
    TargetOutputRequest::Register();
    TargetOutputReply::Register();
    LogDebugMessageRequest::Register();
    LogDebugMessageReply::Register();
    KillRequest::Register();
    KillReply::Register();

    CodeCompletionRequest::Register();
    CodeCompletionReply::Register();
    LoadEditModuleRequest::Register();
    LoadEditModuleReply::Register();
    ResetEditModuleCacheRequest::Register();
    ResetEditModuleCacheReply::Register();
    ParseSourceRequest::Register();
    ParseSourceReply::Register();
    GetCCListRequest::Register();
    GetCCListReply::Register();
    GetParamHelpListRequest::Register();
    GetParamHelpListReply::Register();
    StopCCRequest::Register();
    StopCCReply::Register();
    KeepAliveCCRequest::Register();
    KeepAliveCCReply::Register();
}

namespace {

    struct Reg 
    {
        Reg()
        {
            Register();
        }
    };

    Reg reg;
}

} // namespace cmajor::msg
