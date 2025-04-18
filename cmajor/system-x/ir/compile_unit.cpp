// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.ir.compile.unit;

import cmajor.systemx.ir.context;

namespace cmajor::systemx::ir {

CompileUnit::CompileUnit(const std::string& filePath_) : filePath(filePath_), context(), cu(nullptr)
{
}

void CompileUnit::SetId(const std::string& id_)
{
    id = id_;
}

void CompileUnit::SetSourceFilePath(const std::string& sourceFilePath_)
{
    sourceFilePath = sourceFilePath_;
    cu = context.GetMDStructRefForSourceFile(sourceFilePath);
}

Function* CompileUnit::GetOrInsertFunction(const std::string& name, FunctionType* type)
{
    auto it = functionMap.find(name);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        Function* function = new Function(name, type, context);
        functions.push_back(std::unique_ptr<Function>(function));
        functionMap[name] = function;
        return function;
    }
}

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(8);
    formatter.Write("cu(");
    formatter.Write(id);
    formatter.Write(",");
    cu->Write(formatter);
    formatter.WriteLine(")");
    formatter.WriteLine();
    context.GetTypeRepository().Write(formatter);
    context.GetDataRepository().Write(context, formatter);
    bool first = true;
    for (const auto& f : functions)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.WriteLine();
        }
        f->Write(formatter, context);
    }
    context.GetMetadata().Write(formatter);
}

} // namespace cmajor::systemx::ir
