// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.init.done;

import cmajor.service.message;
import cmajor.service.request;
import cmajor.service.request.dispatcher;
import cmajor.logger.service;
//import cmajor.service.build.service;
//import cmajor.service.debug.service;
// #include <cmajor/cmsvc/RunService.hpp>
// #include <cmajor/cmsvc/CodeCompletionService.hpp>

namespace cmajor::service {

void Init()
{
    StartLoggerService();
    //InitDebugService();
    //InitRunService();
    //InitCodeCompletionService();
    //cmajor::debug::CmdbSessionInit();
}

void Done()
{
    //cmajor::debug::CmdbSessionDone();
    //DoneCodeCompletionService();
    //DoneRunService();
    //DoneDebugService();
    StopLoggerService();
}

} // namespace cmajor::service
