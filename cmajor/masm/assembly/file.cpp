// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.file;

import cmajor.masm.assembly.function;
import cmajor.masm.assembly.instruction;
import cmajor.masm.assembly.data;

namespace cmajor::masm::assembly {

void DeclarationSection::AddFunctionDeclaration(FunctionDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<Declaration>(declaration));
}

void DeclarationSection::AddPublicDataDeclaration(PublicDataDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<Declaration>(declaration));
}

void DeclarationSection::AddExternalDataDeclaration(ExternalDataDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<Declaration>(declaration));
}

void DeclarationSection::Write(util::CodeFormatter& formatter)
{
    for (const auto& declaration : declarations)
    {
        declaration->Write(formatter);
    }
    formatter.WriteLine();
}

void DataSection::AddData(Data* data)
{
    dataVec.push_back(std::unique_ptr<Data>(data));
}

void DataSection::Write(util::CodeFormatter& formatter)
{
    if (dataVec.empty()) return;
    formatter.WriteLine(".DATA");
    formatter.WriteLine();
    for (const auto& data : dataVec)
    {
        data->Write(formatter);
    }
    formatter.WriteLine();
}

Function* CodeSection::CreateFunction(const std::string& name)
{
    Function* function = new Function(name);
    functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

void CodeSection::Write(util::CodeFormatter& formatter)
{
    formatter.WriteLine(".CODE");
    formatter.WriteLine();
    for (const auto& function : functions)
    {
        function->Write(formatter);
        formatter.WriteLine();
    }
}

File::File(const std::string& filePath_) : filePath(filePath_), file(filePath), formatter(file)
{
    formatter.SetIndentSize(8);
}

void File::Write()
{
    declarationSection.Write(formatter);
    dataSection.Write(formatter);
    codeSection.Write(formatter);
    formatter.WriteLine("END");
}

} // cmajor::masm::assembly
