// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.config;

namespace soul::json::rpc {

Config& Config::Instance()
{
    static Config instance;
    return instance;
}

Config::Config() : flags(Flags::none)
{
}

void Config::SetLogDir(const std::string& logDir_)
{
    logDir = logDir_;
}

} // namespace soul::json::rpc
