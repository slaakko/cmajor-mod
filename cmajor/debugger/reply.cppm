// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.reply;

import cmajor.debugger.record;
import cmajor.debug;
import cmajor.info;
import soul.xml.dom;
import soul.xml.xpath;
import std.core;

export namespace cmajor::debugger {

export namespace reply {}

class Request;

class Reply
{
public:
    Reply();
    void Add(Record* record);
    const std::vector<std::unique_ptr<Record>>& Records() const { return records; }
    void Merge(Reply& that);
    AsyncRecord* GetExecRecord() const { return execRecord; }
    ResultRecord* GetResultRecord() const { return resultRecord; }
    std::string ToString() const;
    bool IsFinal(Request* request) const;
    void SetStopReason(StopReason stopReason_) { stopReason = stopReason_; }
    StopReason GetStopReason() const { return stopReason; }
    const cmajor::info::db::Location& GetLocation() const { return location; }
    void SetLocation(const cmajor::info::db::Location& location_) { location = location_; }
    std::unique_ptr<soul::xml::xpath::NodeSet> Evaluate(const std::string& keyPath);
    cmajor::debug::Instruction* StoppedInstruction() const { return stoppedInstruction; }
    void SetStoppedInstruction(cmajor::debug::Instruction* stoppedInstruction_) { stoppedInstruction = stoppedInstruction_; }
    int ExitCode() const { return exitCode; }
    void SetExitCode(int exitCode_) { exitCode = exitCode_; }
    std::unique_ptr<soul::xml::Element> ToXml();
private:
    std::vector<std::unique_ptr<Record>> records;
    std::unique_ptr<soul::xml::Element> xml;
    Prompt* prompt;
    AsyncRecord* execRecord;
    ResultRecord* resultRecord;
    StopReason stopReason;
    cmajor::info::db::Location location;
    int exitCode;
    cmajor::debug::Instruction* stoppedInstruction;
};

} // namespace cmajor::debugger
