// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.init.done;

import cmajor.service.message;
import cmajor.service.request;
import cmajor.service.request.dispatcher;
import cmajor.logger.service;
import cmajor.build.service;
import cmajor.debug.service;
import cmajor.run.service;
import cmajor.symbols;
import util;
// #include <cmajor/cmsvc/CodeCompletionService.hpp>

namespace cmajor::service {

void Init()
{
    cmajor::symbols::SetCompilerVersion(util::CmajorVersionStr());
    StartLoggerService();
    StartBuildService();
    InitRunService();
    //InitCodeCompletionService();
}

void Done()
{
    //DoneCodeCompletionService();
    DoneRunService();
    StopBuildService();
    StopLoggerService();
}

} // namespace cmajor::service
