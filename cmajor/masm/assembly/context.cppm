// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.context;

import cmajor.masm.assembly.reg;
import cmajor.masm.assembly.expr;
import std.core;

export namespace cmajor::masm::assembly {

class IntegerLiteral;
class Symbol;
class Macro;
class Function;
class File;

class Context
{
public:
    Context();
    void SetFile(File* file_) { file = file_; }
    File* GetFile() const { return file; }
    RegisterPool* GetRegisterPool() { return registerPool.get(); }
    void ResetRegisterPool();
    void SetCurrentFunction(Function* function) { currentFunction = function; }
    Register* GetLocalReg(int64_t size);
    Register* GetGlobalReg(int64_t size, RegisterGroupKind regGroupKind);
    Register* GetGlobalReg(int64_t size, RegisterGroupKind regGroupKind, bool used);
    IntegerLiteral* MakeIntegerLiteral(int64_t value, int size);
    Value* MakeFloatLiteralSymbol(float value);
    Value* MakeDoubleLiteralSymbol(double value);
    Symbol* MakeSymbol(const std::string& symbolName);
    Macro* MakeMacro(const std::string& name);
    Value* MakeContent(Value* value);
    Value* MakeSizePrefix(int64_t size, Value* value);
    Value* MakeBinaryExpr(Value* left, Value* right, Operator op);
private:
    Registers registers;
    File* file;
    std::unique_ptr<RegisterPool> registerPool;
    std::vector<std::unique_ptr<Value>> values;
    Function* currentFunction;
    int floatingLiteralCounter;
};

} // cmajor::masm::assembly
