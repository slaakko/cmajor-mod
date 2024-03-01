// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.compile.unit;

namespace cmajor::systemx::intermediate {

CompileUnit::CompileUnit() : context(nullptr), id(), metadataRef()
{
}

void CompileUnit::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

void CompileUnit::SetInfo(const std::string& id_, MetadataRef* metadataRef_)
{
    id = id_;
    metadataRef = metadataRef_;
}

} // cmajor::systemx::intermediate
