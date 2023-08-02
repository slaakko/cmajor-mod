// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.util;

import cmajor.debugger.reply;
import cmajor.debugger.record;
import cmajor.debugger.value;
import util;

namespace cmajor::debugger {

std::string ToExternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix)
{
    if (externalRootPrefix != internalRootPrefix)
    {
        if (path.starts_with(internalRootPrefix))
        {
            std::string suffix = path.substr(internalRootPrefix.length() + 1);
            return util::Path::Combine(externalRootPrefix, suffix);
        }
    }
    return path;
}

std::string ToInternalFilePath(const std::string& path, const std::string& externalRootPrefix, const std::string& internalRootPrefix)
{
    if (externalRootPrefix != internalRootPrefix)
    {
        if (path.starts_with(externalRootPrefix))
        {
            std::string suffix = path.substr(externalRootPrefix.length() + 1);
            return util::Path::Combine(internalRootPrefix, suffix);
        }
    }
    return path;
}

cmajor::debug::Frame GetCppFrame(Results* results, cmajor::debug::DebugInfo* debugInfo)
{
    cmajor::debug::Frame frame;
    if (results && !results->IsEmpty())
    {
        Value* value = results->Get("frame");
        if (value && value->IsTuple())
        {
            Tuple* tuple = static_cast<Tuple*>(value);
            frame.level = tuple->GetInt("level");
            frame.func = tuple->GetString("func");
            frame.file = tuple->GetString("file");
            if (!frame.file.empty())
            {
                std::string currentCmajorRootPrefix = cmajor::debug::GetCurrentCmajorRootPrefix();
                if (!frame.file.starts_with(currentCmajorRootPrefix))
                {
                    cmajor::debug::Project* project = debugInfo->GetProjectByInternalPath(frame.file);
                    if (project)
                    {
                        frame.file = ToExternalFilePath(frame.file, currentCmajorRootPrefix, project->CmajorRootPrefix());
                    }
                }
            }
            frame.line = tuple->GetInt("line");
        }
    }
    return frame;
}

Tuple* MakeFrameTuple(const cmajor::debug::Frame& frame)
{
    Tuple* tuple = new Tuple();
    if (frame.level != 0)
    {
        tuple->Add("level", new String(std::to_string(frame.level)));
    }
    if (!frame.func.empty())
    {
        tuple->Add("func", new String(frame.func));
    }
    if (!frame.file.empty())
    {
        tuple->Add("file", new String(frame.file));
    }
    if (frame.line != 0)
    {
        tuple->Add("line", new String(std::to_string(frame.line)));
    }
    if (frame.scol != 0)
    {
        tuple->Add("scol", new String(std::to_string(frame.scol)));
    }
    if (frame.ecol != 0)
    {
        tuple->Add("ecol", new String(std::to_string(frame.ecol)));
    }
    return tuple;
}

bool StackFrameHasLine(Reply* reply)
{
    ResultRecord* resultRec = reply->GetResultRecord();
    if (resultRec && resultRec->IsDone())
    {
        Results* results = resultRec->GetResults();
        if (results)
        {
            Value* stack = results->Get("stack");
            if (stack && stack->IsList())
            {
                List* frameList = static_cast<List*>(stack);
                if (!frameList->Items().empty())
                {
                    Value* frame = frameList->Items()[0]->GetValue();
                    if (frame && frame->IsTuple())
                    {
                        Tuple* frameTuple = static_cast<Tuple*>(frame);
                        int line = frameTuple->GetInt("line");
                        if (line != 0)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

} // namespace cmajor::debugger
