// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.context;

import cmajor.masm.assembly.literal;
import cmajor.masm.assembly.symbol;
import cmajor.masm.assembly.function;
import cmajor.masm.assembly.macro;
import cmajor.masm.assembly.data;
import cmajor.masm.assembly.instruction;
import cmajor.masm.assembly.file;

namespace cmajor::masm::assembly {

void CheckSize(int64_t size, const std::string& message)
{
    if (size != 1 && size != 2 && size != 4 && size != 8 && size != highByteRegSize && size != 16)
    {
        throw std::runtime_error(message);
    }
}

Context::Context() : file(nullptr), registerPool(new RegisterPool(registers)), currentFunction(nullptr), floatingLiteralCounter(0)
{
}

void Context::ResetRegisterPool()
{
    registerPool.reset(new RegisterPool(registers));
}

Register* Context::GetLocalReg(int64_t size)
{
    CheckSize(size, "cmajor.masm.assembly.GetLocalReg: invalid size: " + std::to_string(size));
    RegisterGroup* regGroup = registerPool->GetLocalRegisterGroup();
    return regGroup->GetReg(static_cast<int>(size));
}

Register* Context::GetGlobalReg(int64_t size, RegisterGroupKind regGroupKind)
{
    return GetGlobalReg(size, regGroupKind, true);
}

Register* Context::GetGlobalReg(int64_t size, RegisterGroupKind regGroupKind, bool used)
{
    CheckSize(size, "cmajor.masm.assembly.GetGlobalReg: invalid size: " + std::to_string(size));
    RegisterGroup* regGroup = registerPool->GetRegisterGroup(regGroupKind, used);
    if (regGroup->IsLocal())
    {
        throw std::runtime_error("global reg group kind expected");
    }
    return regGroup->GetReg(static_cast<int>(size));
}

IntegerLiteral* Context::MakeIntegerLiteral(int64_t value, int size)
{
    IntegerLiteral* literal = new IntegerLiteral(value, size);
    values.push_back(std::unique_ptr<Value>(literal));
    return literal;
}

Value* Context::MakeFloatLiteralSymbol(float value)
{
    FloatLiteral* literal = new FloatLiteral(value);
    values.push_back(std::unique_ptr<Value>(literal));
    Symbol* symbol = MakeSymbol("floating_" + std::to_string(floatingLiteralCounter++));
    Value* expr = MakeSizePrefix(4, MakeContent(symbol));
    Data* data = new Data();
    Instruction* inst = new Instruction(OpCode::REAL4);
    inst->SetNoColon();
    inst->SetLabel(symbol->Name());
    inst->AddOperand(literal);
    data->AddInstruction(std::unique_ptr<Instruction>(inst));
    file->GetDataSection().AddData(data);
    return expr;
}

Value* Context::MakeDoubleLiteralSymbol(double value)
{
    DoubleLiteral* literal = new DoubleLiteral(value);
    values.push_back(std::unique_ptr<Value>(literal));
    Symbol* symbol = MakeSymbol("floating_" + std::to_string(floatingLiteralCounter++));
    Value* expr = MakeSizePrefix(8, MakeContent(symbol));
    Data* data = new Data();
    Instruction* inst = new Instruction(OpCode::REAL8);
    inst->SetNoColon();
    inst->SetLabel(symbol->Name());
    inst->AddOperand(literal);
    data->AddInstruction(std::unique_ptr<Instruction>(inst));
    file->GetDataSection().AddData(data);
    return expr;
}

Symbol* Context::MakeSymbol(const std::string& symbolName)
{
    Symbol* symbol = new Symbol(symbolName);
    values.push_back(std::unique_ptr<Value>(symbol));
    return symbol;
}

Macro* Context::MakeMacro(const std::string& name)
{
    Macro* macro = new Macro(name);
    values.push_back(std::unique_ptr<Macro>(macro));
    if (currentFunction)
    {
        currentFunction->AddMacro(macro);
    }
    return macro;
}

Value* Context::MakeContent(Value* value)
{
    Value* content = new Content(value);
    values.push_back(std::unique_ptr<Value>(content));
    return content;
}

Value* Context::MakeSizePrefix(int64_t size, Value* value)
{
    CheckSize(size, "cmajor.masm.assembly.MakeSizePrefix: invalid size");
    Value* sizePrefix = new SizePrefix(static_cast<int>(size), value);
    values.push_back(std::unique_ptr<Value>(sizePrefix));
    return sizePrefix;
}

Value* Context::MakeBinaryExpr(Value* left, Value* right, Operator op)
{
    Value* value = new BinaryExpr(left, right, op);
    values.push_back(std::unique_ptr<Value>(value));
    return value;
}

} // cmajor::masm::assembly
