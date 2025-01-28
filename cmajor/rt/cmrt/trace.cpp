// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/trace.hpp>
#include <util/binary_stream_reader.hpp>
#include <util/path.hpp>
#include <util/system.hpp>
#include <util/binary_resource_ptr.hpp>
#include <util/memory_stream.hpp>
#include <util/text_util.hpp>
#include <mutex>
#include <map>

struct TraceEntry
{
    TraceEntry* next;
    int64_t fn;
    int32_t line;
};

thread_local TraceEntry* traceList = nullptr;

void RtmPushFunction(void* traceEntry)
{
    TraceEntry* entry = static_cast<TraceEntry*>(traceEntry);
    entry->next = traceList;
    traceList = entry;
}

void RtmPopFunction(void* traceEntry)
{
    TraceEntry* entry = static_cast<TraceEntry*>(traceEntry);
    traceList = entry->next;
}

namespace cmajor::rt {

    class SourceFileInfo
    {
    public:
        SourceFileInfo();
        SourceFileInfo(int64_t sourceFileId_, const std::string& sourceFilePath_);
        int64_t SourceFileId() const { return sourceFileId; }
        const std::string& SourceFilePath() const { return sourceFilePath; }
        void Read(util::BinaryStreamReader& reader);
    private:
        int64_t sourceFileId;
        std::string sourceFilePath;
    };

    SourceFileInfo::SourceFileInfo() : sourceFileId(-1), sourceFilePath()
    {
    }

    SourceFileInfo::SourceFileInfo(int64_t sourceFileId_, const std::string& sourceFilePath_) : sourceFileId(sourceFileId_), sourceFilePath(sourceFilePath_)
    {
    }

    void SourceFileInfo::Read(util::BinaryStreamReader& reader)
    {
        sourceFileId = reader.ReadLong();
        sourceFilePath = reader.ReadUtf8String();
    }

    class FunctionTraceInfo
    {
    public:
        FunctionTraceInfo();
        FunctionTraceInfo(int64_t functionId_, const std::string& functionFullName_, int64_t sourceFileId_);
        int64_t FunctionId() const { return functionId; }
        const std::string& FunctionFullName() const { return functionFullName; }
        int64_t SourceFileId() const { return sourceFileId; }
        void Read(util::BinaryStreamReader& reader);
    private:
        int64_t functionId;
        std::string functionFullName;
        int64_t sourceFileId;
    };

    FunctionTraceInfo::FunctionTraceInfo() : functionId(-1), functionFullName(), sourceFileId(-1)
    {
    }

    FunctionTraceInfo::FunctionTraceInfo(int64_t functionId_, const std::string& functionFullName_, int64_t sourceFileId_) :
        functionId(functionId_), functionFullName(functionFullName_), sourceFileId(sourceFileId_)
    {
    }

    void FunctionTraceInfo::Read(util::BinaryStreamReader& reader)
    {
        functionId = reader.ReadLong();
        functionFullName = reader.ReadUtf8String();
        sourceFileId = reader.ReadLong();
    }

    std::mutex runtimeInfoMtx;

    class RuntimeInfo
    {
    public:
        RuntimeInfo();
        SourceFileInfo* GetSourceFileInfo(int64_t sourceFileId) const;
        FunctionTraceInfo* GetFunctionTraceInfo(int64_t functionId) const;
        void Read(util::BinaryStreamReader& reader);
    private:
        std::vector<std::unique_ptr<SourceFileInfo>> sourceFileInfoVec;
        std::map<int64_t, SourceFileInfo*> sourceFileInfoMap;
        std::vector<std::unique_ptr<FunctionTraceInfo>> traceInfoVec;
        std::map<int64_t, FunctionTraceInfo*> traceInfoMap;
    };

    void RuntimeInfo::Read(util::BinaryStreamReader& reader)
    {
        int32_t ns = reader.ReadInt();
        for (int32_t i = 0; i < ns; ++i)
        {
            SourceFileInfo* sourceFileInfo = new SourceFileInfo();
            sourceFileInfo->Read(reader);
            sourceFileInfoVec.push_back(std::unique_ptr<SourceFileInfo>(sourceFileInfo));
            sourceFileInfoMap[sourceFileInfo->SourceFileId()] = sourceFileInfo;
        }
        int32_t nf = reader.ReadInt();
        for (int32_t i = 0; i < nf; ++i)
        {
            FunctionTraceInfo* functionTraceInfo = new FunctionTraceInfo();
            functionTraceInfo->Read(reader);
            traceInfoVec.push_back(std::unique_ptr<FunctionTraceInfo>(functionTraceInfo));
            traceInfoMap[functionTraceInfo->FunctionId()] = functionTraceInfo;
        }
    }

    SourceFileInfo* RuntimeInfo::GetSourceFileInfo(int64_t sourceFileId) const
    {
        auto it = sourceFileInfoMap.find(sourceFileId);
        if (it != sourceFileInfoMap.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }

    FunctionTraceInfo* RuntimeInfo::GetFunctionTraceInfo(int64_t functionId) const
    {
        auto it = traceInfoMap.find(functionId);
        if (it != traceInfoMap.end())
        {
            return it->second;
        }
        else
        {
            return nullptr;
        }
    }

    RuntimeInfo* runtimeInfo = nullptr;

    void ReadRuntimeInfo()
    {
        std::lock_guard<std::mutex> lock(runtimeInfoMtx);
        if (runtimeInfo) return;
        runtimeInfo = new RuntimeInfo();
        std::string currentExecutableName = util::Path::GetFileName(util::GetFullPath(util::GetPathToExecutable()));
        util::BinaryResourcePtr resource(currentExecutableName, "TRACE_DATA");
        util::MemoryStream memoryStream(resource.Data(), resource.Size());
        util::BinaryStreamReader reader(memoryStream);
        runtimeInfo->Read(reader);
    }

    RuntimeInfo::RuntimeInfo()
    {
    }

    StackTrace::StackTrace()
    {
    }

    void StackTrace::AddFunctionLine(const FunctionLine& fnLine)
    {
        fnLines.push_back(fnLine);
    }

    std::string StackTrace::ToString()
    {
        std::string stackTrace;
        try
        {
            ReadRuntimeInfo();
            for (const auto& fnLine : fnLines)
            {
                FunctionTraceInfo* functionTraceInfo = runtimeInfo->GetFunctionTraceInfo(fnLine.fn);
                if (functionTraceInfo)
                {
                    std::string entryStr = functionTraceInfo->FunctionFullName();
                    int64_t sourceFileId = functionTraceInfo->SourceFileId();
                    SourceFileInfo* sourceFileInfo = runtimeInfo->GetSourceFileInfo(sourceFileId);
                    if (sourceFileInfo)
                    {
                        entryStr.append(" ").append(sourceFileInfo->SourceFilePath()).append(":").append(std::to_string(fnLine.line));
                    }
                    stackTrace.append(entryStr).append("\n");
                }
            }
        }
        catch (const std::exception& ex)
        {
            stackTrace.append("internal error: could not read runtime_info for generating stack trace: " + util::PlatformStringToUtf8(ex.what()));
        }
        return stackTrace;
    }

    StackTrace GetStackTrace()
    {
        StackTrace stackTrace;
        TraceEntry* entry = traceList;
        while (entry)
        {
            stackTrace.AddFunctionLine(FunctionLine(entry->fn, entry->line));
            entry = entry->next;
        }
        return stackTrace;
    }

} // cmajor::rt
