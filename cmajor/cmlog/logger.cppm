// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.log.logger;

import util;
import std.core;

export namespace cmajor::log {

class Logger
{
public:
    Logger(util::TcpSocket&& socket_);
    void Run();
    void Stop();
private:
    util::TcpSocket socket;
    bool exiting;
};

void RunLogger(Logger* logger);

} // namespace cmajor::log
