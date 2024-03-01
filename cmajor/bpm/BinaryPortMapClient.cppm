// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module bpm.client;

import std.core;

export namespace bpm {

void StartPortMapClient(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid);
void StopPortMapClient();

class Logger
{
public:
    virtual ~Logger();
    virtual void LogMessage(const std::string& message);
};

int GetPortMapServicePortNumberFromConfig();
bool StartPortMapServer(Logger* logger);
bool StopPortMapServer(Logger* logger);
int GetPortMapServicePortNumber(Logger* logger, bool& portMapServerStarted);
int GetFreePortNumber(Logger* logger, const std::string& processName);
bool IsPortMapServerRunning(Logger* logger);

} // namespace bpm