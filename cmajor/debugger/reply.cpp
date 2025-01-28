// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.reply;

import cmajor.debugger.request;

namespace cmajor::debugger {

Reply::Reply() : prompt(nullptr), execRecord(nullptr), resultRecord(nullptr), stopReason(StopReason::unknown), stoppedInstruction(nullptr), exitCode(0)
{
}

void Reply::Add(Record* record)
{
    records.push_back(std::unique_ptr<Record>(record));
    if (record->IsPrompt())
    {
        prompt = static_cast<Prompt*>(record);
    }
    else if (record->IsAsyncRecord())
    {
        AsyncRecord* asyncRecord = static_cast<AsyncRecord*>(record);
        if (asyncRecord->IsExecRecord())
        {
            execRecord = asyncRecord;
        }
    }
    else if (record->IsResultRecord())
    {
        resultRecord = static_cast<ResultRecord*>(record);
    }
}

void Reply::Merge(Reply& that)
{
    for (auto& record : that.records)
    {
        Add(record.release());
    }
}

std::string Reply::ToString() const
{
    std::string str;
    for (const auto& record : records)
    {
        str.append(record->ToString());
    }
    return str;
}

bool Reply::IsFinal(Request* request) const
{
    if (!request)
    {
        if (prompt)
        {
            return true;
        }
    }
    else if (request->IsExecRequest())
    {
        if (resultRecord && resultRecord->IsError())
        {
            return true;
        }
        if (execRecord)
        {
            return execRecord->IsStoppedRecord();
        }
    }
    else
    {
        if (resultRecord)
        {
            return true;
        }
    }
    return false;
}

std::unique_ptr<soul::xml::Element> Reply::ToXml()
{
    if (execRecord)
    {
        return execRecord->GetResults()->ToXml();
    }
    else if (resultRecord)
    {
        return resultRecord->GetResults()->ToXml();
    }
    return std::unique_ptr<soul::xml::Element>();
}

std::unique_ptr<soul::xml::xpath::NodeSet> Reply::Evaluate(const std::string& keyPath)
{
    std::unique_ptr<soul::xml::Element> xml = ToXml();
    return soul::xml::xpath::EvaluateToNodeSet(keyPath, xml.get());
}

} // namespace cmajor::debugger
