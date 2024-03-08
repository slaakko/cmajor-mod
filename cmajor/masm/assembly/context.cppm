// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.context;

import cmajor.masm.assembly.reg;
import cmajor.masm.assembly.expr;
import std.core;

export namespace cmajor::masm::assembly {

class NumericLiteral;
class Symbol;

class Context
{
public:
    Context();
    RegisterPool* GetRegisterPool() { return registerPool.get(); }
    void ResetRegisterPool();
    Register* GetLocalReg(int64_t size);
    Register* GetGlobalReg(int64_t size, RegisterGroupKind regGroupKind);
    NumericLiteral* MakeNumericLiteral(int64_t value, int size);
    Symbol* MakeSymbol(const std::string& symbolName);
    Value* MakeContent(Value* value);
    Value* MakeSizePrefix(int64_t size, Value* value);
    Value* MakeBinaryExpr(Value* left, Value* right, Operator op);
private:
    Registers registers;
    std::unique_ptr<RegisterPool> registerPool;
    std::vector<std::unique_ptr<Value>> values;
};

} // cmajor::masm::assembly
