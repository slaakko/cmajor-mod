// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.io;

import cmajor.debug.debug.info;
import util;
import std.core;

export namespace cmajor::debug {

const uint8_t debugInfoFormat_1 = uint8_t('1');
const uint8_t debugInfoFormat_2 = uint8_t('2');
const uint8_t debugInfoFormat_3 = uint8_t('3');
const uint8_t currentDebugInfoFormat = debugInfoFormat_3;

void WriteCmdbFileTag(util::BinaryStreamWriter& writer);
void ReadCmdbFileTag(util::BinaryStreamReader& reader, const std::string& filePath);
void WriteMainProjectName(util::BinaryStreamWriter& writer, const std::string& mainProjectName);
void ReadMainProjectName(util::BinaryStreamReader& reader, std::string& mainProjectName);
void WriteNumberOfProjects(util::BinaryStreamWriter& writer, int32_t numProjects);
void ReadNumberOfProjects(util::BinaryStreamReader& reader, int32_t& numProjects);
void WriteProjectTableHeader(util::BinaryStreamWriter& writer, const std::string& projectName, const std::string& projectDirectoryPath, const std::string& cmajorRootPrefix,
    const util::uuid& moduleId, int32_t numCompileUnits, const util::uuid& mainFunctionId);
void ReadProjectTableHeader(util::BinaryStreamReader& reader, std::string& projectName, std::string& projectDirectoryPath, std::string& cmajorRootPrefix, 
    util::uuid& moduleId, int32_t& numCompileUnits, util::uuid& mainFunctionId);
void WriteProjectTableRecord(util::BinaryStreamWriter& writer, const std::string& compileUnitBaseName);
void ReadProjectTableRecord(util::BinaryStreamReader& reader, std::string& compileUnitBaseName);
void WriteNumberOfCompileUnitFunctionRecords(util::BinaryStreamWriter& writer, int32_t numFunctionRecords);
void ReadNumberOfCompileUnitFunctionRecords(util::BinaryStreamReader& reader, int32_t& numFunctionRecords);
void WriteCompileUnitFunctionRecord(util::BinaryStreamWriter& writer, int32_t fileIndex, const util::uuid& sourceModuleId, const util::uuid& functionId);
void ReadCompileUnitFunctionRecord(util::BinaryStreamReader& reader, int32_t& fileIndex, util::uuid& sourceModuleId, util::uuid& functionId);
void WriteNumberOfInstructionRecords(util::BinaryStreamWriter& writer, int32_t numInstructionRecords);
void ReadNumberOfInstructionRecords(util::BinaryStreamReader& reader, int32_t& numInstructionRecods);
void WriteInstructionRecord(util::BinaryStreamWriter& writer, int32_t cppLineNumber, const SourceSpan& span, int32_t cppLineIndex, int16_t scopeId, int16_t flags);
void ReadInstructionRecord(util::BinaryStreamReader& reader, int32_t& cppLineNumber, SourceSpan& span, int32_t& cppLineIndex, int16_t& scopeId, int16_t& flags);
void WriteControlFlowGraphNodeCount(util::BinaryStreamWriter& writer, int32_t nodeCount);
void ReadControlFlowGraphNodeCount(util::BinaryStreamReader& reader, int32_t& nodeCount);
void WriteControlFlowGraphNode(util::BinaryStreamWriter& writer, int32_t nodeId, const SourceSpan& span, int32_t cppLineIndex, int32_t cppLineNumber);
void ReadControlFlowGraphNode(util::BinaryStreamReader& reader, int32_t& nodeId, SourceSpan& span, int32_t& cppLineIndex, int32_t& cppLineNumber);
void WriteControlFlowGraphNodeEdgeCount(util::BinaryStreamWriter& writer, int32_t edgeCount);
void ReadControlFlowGraphNodeEdgeCount(util::BinaryStreamReader& reader, int32_t& edgeCount);
void WriteControlFlowGraphNodeEdge(util::BinaryStreamWriter& writer, int32_t endNodeId);
void ReadControlFlowGraphNodeEdge(util::BinaryStreamReader& reader, int32_t& endNodeId);
void WriteNumberOfScopes(util::BinaryStreamWriter& writer, int32_t numScopes);
void ReadNumberOfScopes(util::BinaryStreamReader& reader, int32_t& numScopes);
void WriteScopeRecord(util::BinaryStreamWriter& writer, int16_t scopeId, int16_t parentScopeId, int32_t numLocalVariables);
void ReadScopeRecord(util::BinaryStreamReader& reader, int16_t& scopeId, int16_t& parentScopeId, int32_t& numLocalVariables);
void WriteNumberOfFileIndexRecords(util::BinaryStreamWriter& writer, int32_t numFileIndexRecords);
void ReadNumberOfFileIndexRecords(util::BinaryStreamReader& reader, int32_t& numFileIndexRecords);
void WriteFileIndexRecord(util::BinaryStreamWriter& writer, int32_t fileIndex, const std::string& sourceFilePath);
void ReadFileIndexRecord(util::BinaryStreamReader& reader, int32_t& fileIndex, std::string& sourceFilePath);
void WriteNumberOfFunctionIndexFunctionRecords(util::BinaryStreamWriter& writer, int32_t numFunctionIndexFunctionRecords);
void ReadNumberOfFunctionIndexFunctionRecords(util::BinaryStreamReader& reader, int32_t& numFunctionIndexFunctionRecords);
void WriteFunctionIndexFunctionRecord(util::BinaryStreamWriter& writer, const util::uuid& functionId, const std::string& fullFunctionName,
    const std::string& mangledFunctionName);
void ReadFunctionIndexFunctionRecord(util::BinaryStreamReader& reader, util::uuid& functionId, std::string& fullFunctionName,
    std::string& mangledFunctionName);

} // namespace cmajor::debug
