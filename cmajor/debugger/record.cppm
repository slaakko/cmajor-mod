// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.record;

import cmajor.debugger.value;
import std.core;

export namespace cmajor::debugger {

export namespace record {}

enum class RecordKind
{
    result, stream, async, prompt
};

class Record
{
public:
    Record(RecordKind kind_);
    virtual ~Record();
    virtual std::string ToString() const = 0;
    RecordKind Kind() const { return kind; }
    bool IsPrompt() const { return kind == RecordKind::prompt; }
    bool IsResultRecord() const { return kind == RecordKind::result; }
    bool IsStreamRecord() const { return kind == RecordKind::stream; }
    bool IsAsyncRecord() const { return kind == RecordKind::async; }
private:
    RecordKind kind;
};

class Prompt : public Record
{
public:
    Prompt();
    std::string ToString() const override { return "(gdb)"; }
};

enum class ResultRecordKind
{
    done, running, connected, error, exit
};

std::string ResultRecordKindStr(ResultRecordKind kind);

class ResultRecord : public Record
{
public:
    ResultRecord(ResultRecordKind resultRecordKind_, Results* results_);
    ResultRecordKind GetResultRecordKind() const { return resultRecordKind; }
    bool IsDone() const { return resultRecordKind == ResultRecordKind::done; }
    bool IsRunning() const { return resultRecordKind == ResultRecordKind::running; }
    bool IsConnected() const { return resultRecordKind == ResultRecordKind::connected; }
    bool IsError() const { return resultRecordKind == ResultRecordKind::error; }
    bool IsExit() const { return resultRecordKind == ResultRecordKind::exit; }
    Results* GetResults() const { return results.get(); }
    std::string ToString() const override;
    std::string ErrorMessage() const;
private:
    ResultRecordKind resultRecordKind;
    std::unique_ptr<Results> results;
};

enum class StreamRecordKind
{
    console, target, log
};

std::string StreamRecordKindStr(StreamRecordKind kind);

class StreamRecord : public Record
{
public:
    StreamRecord(StreamRecordKind streamRecordKind_, const std::string& output_);
    StreamRecordKind GetStreamRecordKind() const { return streamRecordKind; }
    bool IsConsole() const { return streamRecordKind == StreamRecordKind::console; }
    bool IsTarget() const { return streamRecordKind == StreamRecordKind::target; }
    bool IsLog() const { return streamRecordKind == StreamRecordKind::log; }
    const std::string& Output() const { return output; }
    std::string ToString() const override;
private:
    StreamRecordKind streamRecordKind;
    std::string output;
};

enum class AsyncRecordKind
{
    exec, status, notify
};

std::string AsyncRecordKindStr(AsyncRecordKind kind);

enum class AsyncClass
{
    running, stopped, 
    threadGroupAdded, threadGroupRemoved, threadGroupStarted, threadGroupExited, threadCreated, threadExited, threadSelected, 
    libraryLoaded, libraryUnloaded, traceframeChanged, tsvCreated, tsvDeleted, tsvModified, 
    breakpointCreated, breakpointModified, breakpointDeleted, recordStarted, recordStopped, cmdParamChanged, memoryChanged,
    unknown
};

AsyncClass ParseAsyncClass(const std::string& cls);
std::string AsyncClassStr(AsyncClass cls);

enum class StopReason
{
    breakpointHit, watchPointTrigger, readWatchPointTrigger, accessWatchPointTrigger, functionFinished, locationReached, watchPointScope,
    endSteppingRange, exitedSignaled, exited, exitedNormally, signalReceived, solibEvent, fork, vfork, syscallEntry, syscallReturn, exec, noHistory, 
    unknown
};

StopReason ParseStopReason(const std::string& reasonText);
std::string StopReasonStr(StopReason reason);

class AsyncRecord : public Record
{
public:
    AsyncRecord(AsyncRecordKind asyncRecordKind_, AsyncClass asyncClass_, Results* results_);
    AsyncRecordKind GetAsyncRecordKind() const { return asyncRecordKind; }
    bool IsExecRecord() const { return asyncRecordKind == AsyncRecordKind::exec; }
    bool IsStatusRecord() const { return asyncRecordKind == AsyncRecordKind::status; }
    bool IsNotifyRecord() const { return asyncRecordKind == AsyncRecordKind::notify; }
    bool IsRunningRecord() const { return asyncClass == AsyncClass::running; }
    bool IsStoppedRecord() const { return asyncClass == AsyncClass::stopped; }
    AsyncClass GetAsyncClass() const { return asyncClass; }
    Results* GetResults() const { return results.get(); }
    std::string ToString() const override;
private:
    AsyncRecordKind asyncRecordKind;
    AsyncClass asyncClass;
    std::unique_ptr<Results> results;
};

} // namespace cmajor::debugger
