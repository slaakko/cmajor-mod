// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.cpp.ir.compile.unit;

import cmajor.cpp.ir.function;
import cmajor.debug;
import util;

namespace cmajor::cpp::ir {

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

void CompileUnit::Write()
{
    std::ofstream file(filePath);
    std::string cudiFilePath = util::Path::ChangeExtension(filePath, ".cudi");
    int32_t numFunctions = 0;
    util::FileStream fileStream(cudiFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamWriter binaryStreamWriter(bufferedStream);
    int64_t numFunctionsPos = binaryStreamWriter.GetStream().Tell();
    cmajor::debug::WriteNumberOfCompileUnitFunctionRecords(binaryStreamWriter, numFunctions);
    util::CodeFormatter formatter(file);
    formatter.SetLine(1);
    formatter.SetIndentSize(4);
    formatter.WriteLine("// " + sourceFilePath);
    formatter.WriteLine();
    context.GetTypeRepository().Write(formatter);
    formatter.WriteLine();
    if (!functions.empty())
    {
        formatter.WriteLine("extern \"C\" {");
        formatter.WriteLine();
        for (const auto& f : functions)
        {
            f->WriteDeclaration(formatter, context);
        }
        formatter.WriteLine();
        formatter.WriteLine("} // extern \"C\"");
        formatter.WriteLine();
    }
    context.GetDataRepository().Write(context, formatter);
    if (!functions.empty())
    {
        bool first = true;
        for (const auto& f : functions)
        {
            if (f->BasicBlocks().empty())
            {
                continue;
            }
            formatter.WriteLine();
            if (first)
            {
                formatter.WriteLine("extern \"C\" {");
                formatter.WriteLine();
                first = false;
            }
            f->Write(formatter, context, binaryStreamWriter, numFunctions);
        }
        formatter.WriteLine();
        formatter.WriteLine("} // extern \"C\"");
    }
    int64_t currentPos = binaryStreamWriter.GetStream().Tell();;
    binaryStreamWriter.GetStream().Seek(numFunctionsPos, util::Origin::seekSet);
    cmajor::debug::WriteNumberOfCompileUnitFunctionRecords(binaryStreamWriter, numFunctions);
    binaryStreamWriter.GetStream().Seek(currentPos, util::Origin::seekSet);
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

} // namespace cmajor::cpp::ir

