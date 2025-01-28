// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.io;

import cmajor.ast;

namespace cmajor::debug {

void WriteCmdbFileTag(util::BinaryStreamWriter& writer)
{
    writer.Write("CMDB", false);
    writer.Write(currentDebugInfoFormat);
}

void ReadCmdbFileTag(util::BinaryStreamReader& reader, const std::string& filePath)
{
    char c;
    char m;
    char d;
    char b;
    uint8_t format;
    try
    {
        c = reader.ReadChar();
        m = reader.ReadChar();
        d = reader.ReadChar();
        b = reader.ReadChar();
        format = reader.ReadByte();
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("invalid CMDB file tag read from '" + filePath + "': not a CMDB debug information file: " + ex.what());
    }
    if (c != 'C' || m != 'M' || d != 'D' || b != 'B')
    {
        throw std::runtime_error("invalid CMDB file tag read from '" + filePath + "': not a CMDB debug information file");
    }
    if (format != currentDebugInfoFormat)
    {
        throw std::runtime_error("wrong CMDB file format version number: format version " + std::string(1, format) + " read, format version " +
            std::string(1, currentDebugInfoFormat) + " expected. Please rebuild project(s) from sources.");
    }
}

void WriteCmajorRootPrefix(util::BinaryStreamWriter& writer, const std::string& cmajorRootPrefix)
{
    writer.Write(cmajorRootPrefix);
}

void ReadCmajorRootPrefix(util::BinaryStreamReader& reader, std::string& cmajorRootPrefix)
{
    cmajorRootPrefix = reader.ReadUtf8String();
}

void WriteMainProjectName(util::BinaryStreamWriter& writer, const std::string& mainProjectName)
{
    writer.Write(mainProjectName);
}

void ReadMainProjectName(util::BinaryStreamReader& reader, std::string& mainProjectName)
{
    mainProjectName = reader.ReadUtf8String();
}

void WriteNumberOfProjects(util::BinaryStreamWriter& writer, int32_t numProjects)
{
    writer.Write(numProjects);
}

void ReadNumberOfProjects(util::BinaryStreamReader& reader, int32_t& numProjects)
{
    numProjects = reader.ReadInt();
}

void WriteProjectTableHeader(util::BinaryStreamWriter& writer, const std::string& projectName, const std::string& projectDirectoryPath, const std::string& cmajorRootPrefix, 
    const util::uuid& moduleId, int32_t numCompileUnits, const util::uuid& mainFunctionId)
{
    writer.Write(projectName);
    std::string cmajorRootRelativeDirectoryPath = cmajor::ast::MakeCmajorRootRelativeFilePath(projectDirectoryPath);
    writer.Write(cmajorRootRelativeDirectoryPath);
    writer.Write(cmajorRootPrefix);
    writer.Write(moduleId);
    writer.Write(numCompileUnits);
    writer.Write(mainFunctionId);
}

void ReadProjectTableHeader(util::BinaryStreamReader& reader, std::string& projectName, std::string& projectDirectoryPath, std::string& cmajorRootPrefix, 
    util::uuid& moduleId, int32_t& numCompileUnits, util::uuid& mainFunctionId)
{
    projectName = reader.ReadUtf8String();
    std::string cmajorRootRelativeDirectoryPath = reader.ReadUtf8String();
    projectDirectoryPath = cmajor::ast::ExpandCmajorRootRelativeFilePath(cmajorRootRelativeDirectoryPath);
    cmajorRootPrefix = reader.ReadUtf8String();
    reader.ReadUuid(moduleId);
    numCompileUnits = reader.ReadInt();
    reader.ReadUuid(mainFunctionId);
}

void WriteProjectTableRecord(util::BinaryStreamWriter& writer, const std::string& compileUnitBaseName)
{
    writer.Write(compileUnitBaseName);
}

void ReadProjectTableRecord(util::BinaryStreamReader& reader, std::string& compileUnitBaseName)
{
    compileUnitBaseName = reader.ReadUtf8String();
}

void WriteNumberOfFileIndexRecords(util::BinaryStreamWriter& writer, int32_t numFileIndexRecords)
{
    writer.Write(numFileIndexRecords);
}

void ReadNumberOfFileIndexRecords(util::BinaryStreamReader& reader, int32_t& numFileIndexRecords)
{
    numFileIndexRecords = reader.ReadInt();
}

void WriteFileIndexRecord(util::BinaryStreamWriter& writer, int32_t fileIndex, const std::string& sourceFilePath)
{
    writer.Write(fileIndex);
    std::string cmajorRootRelativeFilePath = cmajor::ast::MakeCmajorRootRelativeFilePath(sourceFilePath);
    writer.Write(cmajorRootRelativeFilePath);
}

void ReadFileIndexRecord(util::BinaryStreamReader& reader, int32_t& fileIndex, std::string& sourceFilePath)
{
    fileIndex = reader.ReadInt();
    std::string cmajorRootRelativeFilePath = reader.ReadUtf8String();
    sourceFilePath = cmajor::ast::ExpandCmajorRootRelativeFilePath(cmajorRootRelativeFilePath);
}

void WriteNumberOfCompileUnitFunctionRecords(util::BinaryStreamWriter& writer, int32_t numFunctionRecords)
{
    writer.Write(numFunctionRecords);
}

void ReadNumberOfCompileUnitFunctionRecords(util::BinaryStreamReader& reader, int32_t& numFunctionRecords)
{
    numFunctionRecords = reader.ReadInt();
}

void WriteCompileUnitFunctionRecord(util::BinaryStreamWriter& writer, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId)
{
    writer.Write(fileIndex);
    writer.Write(sourceModuleId);
    writer.Write(functionId);
}

void ReadCompileUnitFunctionRecord(util::BinaryStreamReader& reader, int32_t& fileIndex, util::uuid& sourceModuleId, util::uuid& functionId)
{
    fileIndex = reader.ReadInt();
    reader.ReadUuid(sourceModuleId);
    reader.ReadUuid(functionId);
}

void WriteNumberOfInstructionRecords(util::BinaryStreamWriter& writer, int32_t numInstructionRecords)
{
    writer.Write(numInstructionRecords);
}

void ReadNumberOfInstructionRecords(util::BinaryStreamReader& reader, int32_t& numInstructionRecords)
{
    numInstructionRecords = reader.ReadInt();
}

void WriteInstructionRecord(util::BinaryStreamWriter& writer, int32_t cppLineNumber, const SourceSpan& span, int32_t cppLineIndex, int16_t scopeId, int16_t flags)
{
    writer.Write(cppLineNumber);
    writer.Write(span.line);
    writer.Write(span.scol);
    writer.Write(span.ecol);
    writer.Write(cppLineIndex);
    writer.Write(scopeId);
    writer.Write(flags);
}

void ReadInstructionRecord(util::BinaryStreamReader& reader, int32_t& cppLineNumber, SourceSpan& span, int32_t& cppLineIndex, int16_t& scopeId, int16_t& flags)
{
    cppLineNumber = reader.ReadInt();
    span.line = reader.ReadInt();
    span.scol = reader.ReadShort();
    span.ecol = reader.ReadShort();
    cppLineIndex = reader.ReadInt();
    scopeId = reader.ReadShort();
    flags = reader.ReadShort();
}

void WriteControlFlowGraphNodeCount(util::BinaryStreamWriter& writer, int32_t nodeCount)
{
    writer.Write(nodeCount);
}

void ReadControlFlowGraphNodeCount(util::BinaryStreamReader& reader, int32_t& nodeCount)
{
    nodeCount = reader.ReadInt();
}

void WriteControlFlowGraphNode(util::BinaryStreamWriter& writer, int32_t nodeId, const SourceSpan& span, int32_t cppLineIndex, int32_t cppLineNumber)
{
    writer.Write(nodeId);
    writer.Write(span.line);
    writer.Write(span.scol);
    writer.Write(span.ecol);
    writer.Write(cppLineIndex);
    writer.Write(cppLineNumber);
}

void ReadControlFlowGraphNode(util::BinaryStreamReader& reader, int32_t& nodeId, SourceSpan& span, int32_t& cppLineIndex, int32_t& cppLineNumber)
{
    nodeId = reader.ReadInt();
    span.line = reader.ReadInt();
    span.scol = reader.ReadShort();
    span.ecol = reader.ReadShort();
    cppLineIndex = reader.ReadInt();
    cppLineNumber = reader.ReadInt();
}

void WriteControlFlowGraphNodeEdgeCount(util::BinaryStreamWriter& writer, int32_t edgeCount)
{
    writer.Write(edgeCount);
}

void ReadControlFlowGraphNodeEdgeCount(util::BinaryStreamReader& reader, int32_t& edgeCount)
{
    edgeCount = reader.ReadInt();
}

void WriteControlFlowGraphNodeEdge(util::BinaryStreamWriter& writer, int32_t endNodeId)
{
    writer.Write(endNodeId);
}

void ReadControlFlowGraphNodeEdge(util::BinaryStreamReader& reader, int32_t& endNodeId)
{
    endNodeId = reader.ReadInt();
}

void WriteNumberOfScopes(util::BinaryStreamWriter& writer, int32_t numScopes)
{
    writer.Write(numScopes);
}

void ReadNumberOfScopes(util::BinaryStreamReader& reader, int32_t& numScopes)
{
    numScopes = reader.ReadInt();
}

void WriteScopeRecord(util::BinaryStreamWriter& writer, int16_t scopeId, int16_t parentScopeId, int32_t numLocalVariables)
{
    writer.Write(scopeId);
    writer.Write(parentScopeId);
    writer.Write(numLocalVariables);
}

void ReadScopeRecord(util::BinaryStreamReader& reader, int16_t& scopeId, int16_t& parentScopeId, int32_t& numLocalVariables)
{
    scopeId = reader.ReadShort();
    parentScopeId = reader.ReadShort();
    numLocalVariables = reader.ReadInt();
}

void WriteNumberOfFunctionIndexFunctionRecords(util::BinaryStreamWriter& writer, int32_t numFunctionIndexFunctionRecords)
{
    writer.Write(numFunctionIndexFunctionRecords);
}

void ReadNumberOfFunctionIndexFunctionRecords(util::BinaryStreamReader& reader, int32_t& numFunctionIndexFunctionRecords)
{
    numFunctionIndexFunctionRecords = reader.ReadInt();
}

void WriteFunctionIndexFunctionRecord(util::BinaryStreamWriter& writer, const util::uuid& functionId, const std::string& fullFunctionName,
    const std::string& mangledFunctionName)
{
    writer.Write(functionId);
    writer.Write(fullFunctionName);
    writer.Write(mangledFunctionName);
}

void ReadFunctionIndexFunctionRecord(util::BinaryStreamReader& reader, util::uuid& functionId, std::string& fullFunctionName,
    std::string& mangledFunctionName)
{
    reader.ReadUuid(functionId);
    fullFunctionName = reader.ReadUtf8String();
    mangledFunctionName = reader.ReadUtf8String();
}

} // namespace cmajor::debug
