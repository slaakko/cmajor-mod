// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.ir.data;

import cmajor.masm.ir.context;

namespace cmajor::masm::ir {

GlobalVariable::GlobalVariable(Type* type_, const std::string& name_) : type(type_), name(name_), initializer(nullptr)
{
}

std::string GlobalVariable::Name(Context& context)
{
    return name; 
}

Type* GlobalVariable::GetType(Context& context)
{
    return context.GetPtrType(type);
}

void GlobalVariable::Write(Context& context, util::CodeFormatter& formatter)
{
    formatter.Write(type->Name());
    formatter.Write(" ");
    formatter.Write(name);
    if (initializer)
    {
        formatter.Write(" = ");
        if (initializer->IsAggregateValue() || initializer->IsStringValue())
        {
            formatter.Write(initializer->Name(context));
        }
        else
        {
            formatter.Write(initializer->GetType(context)->Name());
            formatter.Write(" ");
            formatter.Write(initializer->Name(context));
        }
    }
    else
    {
        formatter.Write(";");
    }
}

DataRepository::DataRepository() : globalVariableDefinitions(), globalVariableMap(), nextStringId(0)
{
}

GlobalVariable* DataRepository::GetOrInsertGlobal(const std::string& name, Type* type)
{
    auto it = globalVariableMap.find(name);
    if (it != globalVariableMap.cend())
    {
        return it->second;
    }
    GlobalVariable* globalVariable = new GlobalVariable(type, name);
    globalVariableDefinitions.push_back(std::unique_ptr<GlobalVariable>(globalVariable));
    globalVariableMap[name] = globalVariable;
    return globalVariable;
}

GlobalVariable* DataRepository::CreateGlobalStringPtr(Context& context, const std::string& stringValue)
{
    GlobalVariable* globalVariable = new GlobalVariable(context.GetByteType(), "string" + std::to_string(nextStringId++) + "_" + compileUnitId);
    globalVariable->SetInitializer(context.GetStringValue(globalVariable->GetType(context), stringValue));
    globalVariableDefinitions.push_back(std::unique_ptr<GlobalVariable>(globalVariable));
    return globalVariable;
}

void DataRepository::Write(Context& context, util::CodeFormatter& formatter)
{
    if (globalVariableDefinitions.empty()) return;
    formatter.WriteLine("data");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& globalVariable : globalVariableDefinitions)
    {
        globalVariable->Write(context, formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

void DataRepository::SetCompileUnitId(const std::string& compileUnitId_)
{
    compileUnitId = compileUnitId_;
}

} // namespace cmajor::masm::ir
