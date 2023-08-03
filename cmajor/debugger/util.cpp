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

cmajor::info::db::Location ToLocation(const cmajor::debug::Frame& frame)
{
    cmajor::info::db::Location location;
    location.level = frame.level;
    location.func = frame.func;
    location.addr = frame.addr;
    location.file = frame.file;
    location.line = frame.line;
    location.scol = frame.scol;
    location.ecol = frame.ecol;
    return location;
}

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

std::string MakeExternalFilePath(const std::string& path, cmajor::debug::DebugInfo* debugInfo)
{
    if (!path.empty())
    {
        std::string currentCmajorRootPrefix = cmajor::debug::GetCurrentCmajorRootPrefix();
        if (!path.starts_with(currentCmajorRootPrefix))
        {
            cmajor::debug::Project* project = debugInfo->GetProjectByInternalPath(path);
            if (project)
            {
                return ToExternalFilePath(path, currentCmajorRootPrefix, project->CmajorRootPrefix());
            }
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
            frame.addr = tuple->GetString("addr");
            frame.file = MakeExternalFilePath(tuple->GetString("file"), debugInfo);
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
    if (!frame.addr.empty())
    {
        tuple->Add("addr", new String(frame.addr));
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

std::vector<cmajor::info::db::Location> GetFrames(cmajor::debugger::Results* results, cmajor::debug::DebugInfo* debugInfo, cmajor::debug::DebuggerOutputWriter* outputWriter)
{
    std::vector<cmajor::info::db::Location> frames;
    Value* stack = results->Get("stack");
    if (stack && stack->IsList())
    {
        List* frameList = static_cast<List*>(stack);
        int n = frameList->Items().size();
        for (int i = 0; i < n; ++i)
        {
            Value* frame = frameList->Items()[i]->GetValue();
            if (frame && frame->IsTuple())
            {
                Tuple* frameTuple = static_cast<Tuple*>(frame);
                cmajor::debug::Frame cppFrame;
                cppFrame.level = frameTuple->GetInt("level");
                cppFrame.addr = frameTuple->GetString("addr");
                cppFrame.func = frameTuple->GetString("func");
                cppFrame.file = MakeExternalFilePath(frameTuple->GetString("file"), debugInfo);
                cppFrame.line = frameTuple->GetInt("line");
                cmajor::debug::Instruction* inst = debugInfo->GetNearestInstruction(cppFrame);
                cmajor::debug::Frame cmajorFrame;
                if (inst)
                {
                    cmajorFrame = inst->GetCmajorFrame();
                }
                cmajorFrame.level = cppFrame.level;
                cmajorFrame.addr = cppFrame.addr;
                cmajor::info::db::Location loc = ToLocation(cmajorFrame);
                frames.push_back(loc);
            }
        }
    }
    return frames;
}

} // namespace cmajor::debugger
