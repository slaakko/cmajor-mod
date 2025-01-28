// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.record;

import util;

namespace cmajor::debugger {

Record::Record(RecordKind kind_) : kind(kind_)
{
}

Record::~Record()
{
}

Prompt::Prompt() : Record(RecordKind::prompt)
{
}

std::string ResultRecordKindStr(ResultRecordKind kind)
{
    switch (kind)
    {
        case ResultRecordKind::done: return "done";
        case ResultRecordKind::running: return "running";
        case ResultRecordKind::connected: return "connected";
        case ResultRecordKind::error: return "error";
        case ResultRecordKind::exit: return "exit";
    }
    return std::string();
}

ResultRecord::ResultRecord(ResultRecordKind resultRecordKind_, Results* results_) : Record(RecordKind::result), resultRecordKind(resultRecordKind_), results(results_)
{
}

std::string ResultRecord::ToString() const
{
    std::string str = "^";
    str.append(ResultRecordKindStr(resultRecordKind));
    if (results && !results->IsEmpty())
    {
        str.append(", ").append(results->ToString());
    }
    return str;
}

std::string ResultRecord::ErrorMessage() const
{
    if (IsError())
    {
        if (results)
        {
            return results->GetString("msg");
        }
        else
        {
            return std::string();
        }
    }
    else
    {
        return std::string();
    }
}

std::string StreamRecordKindStr(StreamRecordKind kind)
{
    switch (kind)
    {
        case StreamRecordKind::console: return "~";
        case StreamRecordKind::target: return "@";
        case StreamRecordKind::log: return "&";
    }
    return std::string();
}

StreamRecord::StreamRecord(StreamRecordKind streamRecordKind_, const std::string& output_) : Record(RecordKind::stream), streamRecordKind(streamRecordKind_), output(output_)
{
}

std::string StreamRecord::ToString() const
{
    std::string str = StreamRecordKindStr(streamRecordKind);
    str.append(util::StringStr(output));
    return str;
}

std::string AsyncRecordKindStr(AsyncRecordKind kind)
{
    switch (kind)
    {
        case AsyncRecordKind::exec: return "*";
        case AsyncRecordKind::status: return "+";
        case AsyncRecordKind::notify: return "=";
    }
    return std::string();
}

class AsyncClasses
{
public:
    static AsyncClasses& Instance();
    AsyncClass GetAsyncClass(const std::string& className) const;
private:
    AsyncClasses();
    std::map<std::string, AsyncClass> classMap;
};

AsyncClasses& AsyncClasses::Instance()
{
    static AsyncClasses instance;
    return instance;
}

AsyncClass AsyncClasses::GetAsyncClass(const std::string& className) const
{
    auto it = classMap.find(className);
    if (it != classMap.end())
    {
        return it->second;
    }
    return AsyncClass::unknown;
}

AsyncClasses::AsyncClasses()
{
    classMap["thread-group-added"] = AsyncClass::threadGroupAdded;
    classMap["thread-group-removed"] = AsyncClass::threadGroupRemoved;
    classMap["thread-group-started"] = AsyncClass::threadGroupStarted;
    classMap["thread-group-exited"] = AsyncClass::threadGroupExited;
    classMap["thread-created"] = AsyncClass::threadCreated;
    classMap["thread-exited"] = AsyncClass::threadExited;
    classMap["thread-selected"] = AsyncClass::threadSelected;
    classMap["library-loaded"] = AsyncClass::libraryLoaded;
    classMap["library-unloaded"] = AsyncClass::libraryUnloaded;
    classMap["trace-frame-changed"] = AsyncClass::traceframeChanged;
    classMap["tsv-created"] = AsyncClass::tsvCreated;
    classMap["tsv-deleted"] = AsyncClass::tsvDeleted;
    classMap["tsv-modified"] = AsyncClass::tsvModified;
    classMap["breakpoint-created"] = AsyncClass::breakpointCreated;
    classMap["breakpoint-modified"] = AsyncClass::breakpointModified;
    classMap["breakpoint-deleted"] = AsyncClass::breakpointDeleted;
    classMap["record-started"] = AsyncClass::recordStarted;
    classMap["record-stopped"] = AsyncClass::recordStopped;
    classMap["cmd-param-changed"] = AsyncClass::cmdParamChanged;
    classMap["memory-changed"] = AsyncClass::memoryChanged;
}

AsyncClass ParseAsyncClass(const std::string& cls)
{
    return AsyncClasses::Instance().GetAsyncClass(cls);
}

std::string AsyncClassStr(AsyncClass cls)
{
    switch (cls)
    {
        case AsyncClass::running: return "running";
        case AsyncClass::stopped: return "stopped";
        case AsyncClass::threadGroupAdded: return "thread-group-added";
        case AsyncClass::threadGroupRemoved: return "thread-group-removed";
        case AsyncClass::threadGroupStarted: return "thread-group-started";
        case AsyncClass::threadGroupExited: return "thread-group-exited";
        case AsyncClass::threadCreated: return "thread-created";
        case AsyncClass::threadExited: return "thread-exited";
        case AsyncClass::threadSelected: return "thread-selected";
        case AsyncClass::libraryLoaded: return "library-loaded";
        case AsyncClass::libraryUnloaded: return "library-unloaded";
        case AsyncClass::traceframeChanged: return "trace-frame-changed";
        case AsyncClass::tsvCreated: return "tsv-created";
        case AsyncClass::tsvDeleted: return "tsv-deleted";
        case AsyncClass::tsvModified: return "tsv-modified";
        case AsyncClass::breakpointCreated: return "breakpoint-created";
        case AsyncClass::breakpointModified: return "breakpoint-modified";
        case AsyncClass::breakpointDeleted: return "breakpoint-deleted";
        case AsyncClass::recordStarted: return "record-started";
        case AsyncClass::recordStopped: return "record-stopped";
        case AsyncClass::cmdParamChanged: return "cmd-param-changed";
        case AsyncClass::memoryChanged: return "memory-changed";
    }
    return std::string();
}

AsyncRecord::AsyncRecord(AsyncRecordKind asyncRecordKind_, AsyncClass asyncClass_, Results* results_) :
    Record(RecordKind::async), asyncRecordKind(asyncRecordKind_), asyncClass(asyncClass_), results(results_)
{
}

std::string AsyncRecord::ToString() const
{
    std::string str = AsyncRecordKindStr(asyncRecordKind);
    str.append(AsyncClassStr(asyncClass));
    if (results && !results->IsEmpty())
    {
        str.append(1, ',').append(results->ToString());
    }
    return str;
}

class StopReasons
{
public:
    static StopReasons& Instance();
    StopReason GetStopReason(const std::string& reasonText) const;
private:
    StopReasons();
    std::map<std::string, StopReason> reasonMap;
};

StopReasons& StopReasons::Instance()
{
    static StopReasons instance;
    return instance;
}

StopReasons::StopReasons()
{
    reasonMap["breakpoint-hit"] = StopReason::breakpointHit;
    reasonMap["watchpoint-trigger"] = StopReason::watchPointTrigger;
    reasonMap["read-watchpoint-trigger"] = StopReason::readWatchPointTrigger;
    reasonMap["access-watchpoint-trigger"] = StopReason::accessWatchPointTrigger;
    reasonMap["function-finished"] = StopReason::functionFinished;
    reasonMap["location-reached"] = StopReason::locationReached;
    reasonMap["watchpoint-scope"] = StopReason::watchPointScope;
    reasonMap["end-stepping-range"] = StopReason::endSteppingRange;
    reasonMap["exited-signalled"] = StopReason::exitedSignaled;
    reasonMap["exited"] = StopReason::exited;
    reasonMap["exited-normally"] = StopReason::exitedNormally;
    reasonMap["signal-received"] = StopReason::signalReceived;
    reasonMap["solib-event"] = StopReason::solibEvent;
    reasonMap["fork"] = StopReason::fork;
    reasonMap["vfork"] = StopReason::vfork;
    reasonMap["syscall-entry"] = StopReason::syscallEntry;
    reasonMap["syscall-return"] = StopReason::syscallReturn;
    reasonMap["exec"] = StopReason::exec;
    reasonMap["no-history"] = StopReason::noHistory;
}

StopReason StopReasons::GetStopReason(const std::string& reasonText) const
{
    auto it = reasonMap.find(reasonText);
    if (it != reasonMap.end())
    {
        return it->second;
    }
    return StopReason::unknown;
}

StopReason ParseStopReason(const std::string& reasonText)
{
    return StopReasons::Instance().GetStopReason(reasonText);
}

std::string StopReasonStr(StopReason reason)
{
    switch (reason)
    {
        case StopReason::breakpointHit: return "breakpoint-hit";
        case StopReason::watchPointTrigger: return "watchpoint-trigger";
        case StopReason::readWatchPointTrigger: return "read-watchpoint-trigger";
        case StopReason::accessWatchPointTrigger: return "access-watchpoint-trigger";
        case StopReason::functionFinished: return "function-finished";
        case StopReason::locationReached: return "location-reached";
        case StopReason::watchPointScope: return "watchpoint-scope";
        case StopReason::endSteppingRange: return "end-stepping-range";
        case StopReason::exitedSignaled: return "exited-signalled";
        case StopReason::exited: return "exited";
        case StopReason::exitedNormally: return "exited-normally";
        case StopReason::signalReceived: return "signal-received";
        case StopReason::solibEvent: return "solib-event";
        case StopReason::fork: return "fork";
        case StopReason::vfork: return "vfork";
        case StopReason::syscallEntry: return "syscall-entry";
        case StopReason::syscallReturn: return "syscall-return";
        case StopReason::exec: return "exec";
        case StopReason::noHistory: return "no-history";
    }
    return std::string();
}

} // namespace cmajor::debugger
