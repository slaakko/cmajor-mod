// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.logger;

import cmajor.service.message;

namespace cmajor::service {

void ServiceLogger::LogMessage(const std::string& message) 
{
    PutOutputServiceMessage(message);
}

} // namespace cmajor::service
