// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.context;

import cmajor.masm.assembly.literal;
import cmajor.masm.assembly.symbol;

namespace cmajor::masm::assembly {

void CheckSize(int64_t size, const std::string& message)
{
    if (size != 1 && size != 2 && size != 4 && size != 8)
    {
        throw std::runtime_error(message);
    }
}

Context::Context() : registerPool(new RegisterPool(registers))
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
    CheckSize(size, "cmajor.masm.assembly.GetGlobalReg: invalid size: " + std::to_string(size));
    RegisterGroup* regGroup = registerPool->GetGlobalRegisterGroup(regGroupKind);
    return regGroup->GetReg(static_cast<int>(size));
}

NumericLiteral* Context::MakeNumericLiteral(int64_t value, int size)
{
    NumericLiteral* literal = new NumericLiteral(value, size);
    values.push_back(std::unique_ptr<Value>(literal));
    return literal;
}

Symbol* Context::MakeSymbol(const std::string& symbolName)
{
    Symbol* symbol = new Symbol(symbolName);
    values.push_back(std::unique_ptr<Value>(symbol));
    return symbol;
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
