// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.reply;

import cmajor.debugger.record;
import cmajor.debugger.location;
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
    const Location& GetLocation() const { return location; }
    void SetLocation(const Location& location_) { location = location_; }
    soul::xml::Element* Xml();
    std::unique_ptr<soul::xml::xpath::NodeSet> Evaluate(const std::string& keyPath);
private:
    std::vector<std::unique_ptr<Record>> records;
    std::unique_ptr<soul::xml::Element> xml;
    Prompt* prompt;
    AsyncRecord* execRecord;
    ResultRecord* resultRecord;
    StopReason stopReason;
    Location location;
};

} // namespace cmajor::debugger
