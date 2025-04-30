// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.compile.unit;

import cmajor.systemx.intermediate.context;

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

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(8);
    formatter.Write("cu(");
    formatter.Write(id);
    formatter.Write(",");
    metadataRef->Write(formatter);
    formatter.WriteLine(")");
    formatter.WriteLine();
    context->GetTypes().Write(formatter);
    context->GetData().Write(formatter);
    bool first = true;
    Function* fn = context->GetCode().FirstFunction();
    while (fn)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        fn->Write(formatter);
        fn = fn->Next();
    }
    context->GetMetadata().Write(formatter);
}

} // cmajor::systemx::intermediate
