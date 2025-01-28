// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.ir.compile.unit;

import cmajor.masm.ir.context;

namespace cmajor::masm::ir {

CompileUnit::CompileUnit(const std::string& filePath_) : filePath(filePath_), context()
{
}

void CompileUnit::SetId(const std::string& id_)
{
    id = id_;
}

void CompileUnit::SetSourceFilePath(const std::string& sourceFilePath_)
{
    sourceFilePath = sourceFilePath_;
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
        //Function* function = new Function(name, type, context);
        Function* function = new Function(name, context.GetPtrType(type), context); // the type of a function is now a pointer-to-function type
        functions.push_back(std::unique_ptr<Function>(function));
        functionMap[name] = function;
        return function;
    }
}

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    std::ofstream linesFile(filePath + ".lns");
    util::CodeFormatter formatter(file);
    util::CodeFormatter linesFormatter(linesFile);
    formatter.SetIndentSize(8);
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
        f->Write(formatter, context, linesFormatter);
    }
    context.GetMetadata().Write(formatter);
}

} // namespace cmajor::masm::ir
