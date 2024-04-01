// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.compile_unit;

import cmajor.masm.intermediate.context;
import util;

namespace cmajor::masm::intermediate {

CompileUnit::CompileUnit() : context(nullptr)
{
}

void CompileUnit::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
}

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(8);
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
}

} // cmajor::systemx::intermediate
