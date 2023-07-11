// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.cmdb.session;

import cmajor.debug.debugger.output.writer;
import std.core;

export namespace cmajor::debug {

class RemoveCmdbSessionFileGuard
{
public:
    RemoveCmdbSessionFileGuard(const std::string& cmdbSessionFilePath_);
    ~RemoveCmdbSessionFileGuard();
private:
    std::string cmdbSessionFilePath;
};

class CmdbSessionClient
{
public:
    virtual ~CmdbSessionClient();
    virtual std::string GetTargetInputBytes() = 0;
    virtual void WriteTargetOuput(int handle, const std::string& s) = 0;
};

void SetCmdbSessionPort(int port_);
void StartCmdbSession(const std::string& cmdbSessionFilePath, DebuggerOutputWriter* outputWriter, CmdbSessionClient* client, bool verbose);
void StopCmdbSession();

} // namespace cmajor::debug
