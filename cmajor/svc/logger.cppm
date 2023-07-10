// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.service.logger;

import bpm.client;

export namespace cmajor::service {

class ServiceLogger : public bpm::Logger
{
public:
    void LogMessage(const std::string& message) override;
};

} // namespace cmajor::service