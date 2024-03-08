// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.compile_unit;

namespace cmajor::masm::intermediate {

CompileUnit::CompileUnit() : context(nullptr)
{
}

void CompileUnit::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

} // cmajor::systemx::intermediate
