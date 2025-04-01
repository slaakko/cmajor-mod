// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.bound.expression;

import std.core;
import cmajor.binder.bound.node;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundCompileUnit;

enum class BoundExpressionFlags : uint8_t
{
    none = 0,
    argIsExplicitThisOrBasePtr = 1 << 0,
    bindToRvalueReference = 1 << 1,
    virtualCall = 1 << 2,
    deref = 1 << 3,
    exceptionCapture = 1 << 4
};

inline BoundExpressionFlags operator|(BoundExpressionFlags left, BoundExpressionFlags right)
{
    return BoundExpressionFlags(uint8_t(left) | uint8_t(right));
}

inline BoundExpressionFlags operator&(BoundExpressionFlags left, BoundExpressionFlags right)
{
    return BoundExpressionFlags(uint8_t(left) & uint8_t(right));
}

class BoundFunctionCall;
class BoundFunction;

class BoundExpression : public BoundNode
{
public:
    BoundExpression(const soul::ast::Span& span_, BoundNodeType boundNodeType_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression(const BoundExpression&) = delete;
    BoundExpression& operator=(const BoundExpression&) = delete;
    virtual BoundExpression* Clone() = 0;
    virtual bool IsComplete() const { return true; }
    virtual bool IsLvalueExpression() const { return false; }
    virtual bool HasValue() const { return false; }
    virtual std::string TypeString() const { return "expression"; }
    virtual std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const { return std::unique_ptr<cmajor::symbols::Value>(); }
    virtual bool ContainsExceptionCapture() const { return GetFlag(BoundExpressionFlags::exceptionCapture); }
    virtual const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const { return nullptr; }
    virtual cmajor::symbols::FunctionSymbol* GetFunctionSymbol() { return nullptr; }
    const cmajor::symbols::TypeSymbol* GetType() const { return type; }
    cmajor::symbols::TypeSymbol* GetType() { return type; }
    bool GetFlag(BoundExpressionFlags flag) const { return (flags & flag) != BoundExpressionFlags::none; }
    void SetFlag(BoundExpressionFlags flag) { flags = flags | flag; }
    void AddTemporaryDestructorCall(std::unique_ptr<BoundFunctionCall>&& destructorCall);
    void MoveTemporaryDestructorCallsTo(BoundExpression& expression);
    void DestroyTemporaries(cmajor::ir::Emitter& emitter);
private:
    cmajor::symbols::TypeSymbol* type;
    BoundExpressionFlags flags;
    std::vector<std::unique_ptr<BoundFunctionCall>> temporaryDestructorCalls;
};

class BoundParameter : public BoundExpression
{
public:
    BoundParameter(const soul::ast::Span& span_, cmajor::symbols::ParameterSymbol* parameterSymbol_);
    BoundExpression* Clone() override;
    cmajor::symbols::ParameterSymbol* GetParameterSymbol() { return parameterSymbol; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    std::string TypeString() const override { return "parameter"; }
private:
    cmajor::symbols::ParameterSymbol* parameterSymbol;
};

class BoundLocalVariable : public BoundExpression
{
public:
    BoundLocalVariable(const soul::ast::Span& span_, cmajor::symbols::LocalVariableSymbol* localVariableSymbol_);
    BoundExpression* Clone() override;
    cmajor::symbols::LocalVariableSymbol* GetLocalVariableSymbol() { return localVariableSymbol; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    std::string TypeString() const override { return "local variable"; }
private:
    cmajor::symbols::LocalVariableSymbol* localVariableSymbol;
};

class BoundMemberVariable : public BoundExpression
{
public:
    BoundMemberVariable(const soul::ast::Span& span_, cmajor::symbols::MemberVariableSymbol* memberVariableSymbol_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    void SetClassPtr(std::unique_ptr<BoundExpression>&& classPtr_);
    void SetStaticInitNeeded() { staticInitNeeded = true; }
    std::string TypeString() const override { return "member variable"; }
    cmajor::symbols::MemberVariableSymbol* GetMemberVariableSymbol() { return memberVariableSymbol; }
private:
    cmajor::symbols::MemberVariableSymbol* memberVariableSymbol;
    std::unique_ptr<BoundExpression> classPtr;
    bool staticInitNeeded;
};

class BoundConstant : public BoundExpression
{
public:
    BoundConstant(const soul::ast::Span& span_, cmajor::symbols::ConstantSymbol* constantSymbol_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    std::string TypeString() const override { return "constant"; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override 
    { 
        return std::unique_ptr<cmajor::symbols::Value>(constantSymbol->GetValue()->Clone()); 
    }
    cmajor::symbols::ConstantSymbol* GetConstantSymbol() { return constantSymbol; }
private:
    cmajor::symbols::ConstantSymbol* constantSymbol;
};

class BoundEnumConstant : public BoundExpression
{
public:
    BoundEnumConstant(const soul::ast::Span& span_, cmajor::symbols::EnumConstantSymbol* enumConstantSymbol_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    std::string TypeString() const override { return "enumeration constant"; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override 
    { 
        return std::unique_ptr<cmajor::symbols::Value>(enumConstantSymbol->GetValue()->Clone());
    }
    cmajor::symbols::EnumConstantSymbol* GetEnumConstantSymbol() { return enumConstantSymbol; }
private:
    cmajor::symbols::EnumConstantSymbol* enumConstantSymbol;
};

class BoundLiteral : public BoundExpression
{
public:
    BoundLiteral(std::unique_ptr<cmajor::symbols::Value>&& value_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "literal"; }
    bool HasValue() const override { return true; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override;
    cmajor::symbols::Value* GetValue() { return value.get(); }
private:
    std::unique_ptr<cmajor::symbols::Value> value;
};

class BoundGlobalVariable : public BoundExpression
{
public:
    BoundGlobalVariable(const soul::ast::Span& span_, cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol_);
    cmajor::symbols::GlobalVariableSymbol* GetGlobalVariableSymbol() const { return globalVariableSymbol; }
    BoundExpression* Clone() override;
    bool IsLvalueExpression() const override { return true; }
    bool HasValue() const override { return true; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "global variable"; }
private:
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol;
};


class BoundTemporary : public BoundExpression
{
public:
    BoundTemporary(std::unique_ptr<BoundExpression>&& rvalueExpr_, std::unique_ptr<BoundLocalVariable>&& backingStore_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    std::string TypeString() const override { return "temporary"; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override;
    bool ContainsExceptionCapture() const override;
    BoundExpression* RvalueExpr() { return rvalueExpr.get(); }
    BoundLocalVariable* BackingStore() { return backingStore.get(); }
private:
    std::unique_ptr<BoundExpression> rvalueExpr;
    std::unique_ptr<BoundLocalVariable> backingStore;
};

class BoundSizeOfExpression : public BoundExpression
{
public:
    BoundSizeOfExpression(const soul::ast::Span& span_, cmajor::symbols::TypeSymbol* type_, cmajor::symbols::TypeSymbol* pointerType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "sizeof"; }
    cmajor::symbols::TypeSymbol* PointerType() { return pointerType; }
private:
    cmajor::symbols::TypeSymbol* pointerType;
};

class BoundAddressOfExpression : public BoundExpression
{
public:
    BoundAddressOfExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "address of expression"; }
    std::unique_ptr<BoundExpression>& Subject() { return subject; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override;
    bool ContainsExceptionCapture() const override;
private:
    std::unique_ptr<BoundExpression> subject;
};

class BoundDereferenceExpression : public BoundExpression
{
public:
    BoundDereferenceExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "deference expression"; }
    bool IsLvalueExpression() const override { return true; }
    std::unique_ptr<BoundExpression>& Subject() { return subject; }
    bool ContainsExceptionCapture() const override;
private:
    std::unique_ptr<BoundExpression> subject;
};

class BoundReferenceToPointerExpression : public BoundExpression
{
public:
    BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>&& subject_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "reference to pointer expression"; }
    bool ContainsExceptionCapture() const override;
    std::unique_ptr<BoundExpression>& Subject() { return subject; }
private:
    std::unique_ptr<BoundExpression> subject;
};

class BoundFunctionCall : public BoundExpression
{
public:
    BoundFunctionCall(const soul::ast::Span& span_, cmajor::symbols::FunctionSymbol* functionSymbol_);
    BoundFunctionCall(const BoundFunctionCall&) = delete;
    BoundFunctionCall& operator=(const BoundFunctionCall&) = delete;
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override;
    std::string TypeString() const override { return "function call"; }
    bool IsLvalueExpression() const override;
    const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const override  { return functionSymbol; }
    cmajor::symbols::FunctionSymbol* GetFunctionSymbol() override { return functionSymbol; }
    void SetFunctionSymbol(cmajor::symbols::FunctionSymbol* functionSymbol_) { functionSymbol = functionSymbol_; }
    void AddArgument(std::unique_ptr<BoundExpression>&& argument);
    void SetArguments(std::vector<std::unique_ptr<BoundExpression>>&& arguments_);
    const std::vector<std::unique_ptr<BoundExpression>>& Arguments() const { return arguments; }
    void AddTemporary(std::unique_ptr<BoundLocalVariable>&& temporary);
    bool ContainsExceptionCapture() const override;
    std::vector<std::unique_ptr<GenObject>> ReleaseTemporaries();
private:
    cmajor::symbols::FunctionSymbol* functionSymbol;
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    std::vector<std::unique_ptr<BoundLocalVariable>> temporaries;
};

class BoundDelegateCall : public BoundExpression
{
public:
    BoundDelegateCall(const soul::ast::Span& span_, cmajor::symbols::DelegateTypeSymbol* delegateType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override;
    std::string TypeString() const override { return "delegate call"; }
    bool IsLvalueExpression() const override;
    cmajor::symbols::DelegateTypeSymbol* GetDelegateSymbol() { return delegateTypeSymbol; }
    void AddArgument(std::unique_ptr<BoundExpression>&& argument);
    const std::vector<std::unique_ptr<BoundExpression>>& Arguments() const { return arguments; }
    bool ContainsExceptionCapture() const override;
private:
    cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol;
    std::vector<std::unique_ptr<BoundExpression>> arguments;
};

class BoundClassDelegateCall : public BoundExpression
{
public:
    BoundClassDelegateCall(const soul::ast::Span& span_, cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override;
    std::string TypeString() const override { return "class delegate call"; }
    bool IsLvalueExpression() const override;
    cmajor::symbols::ClassDelegateTypeSymbol* GetClassDelegateSymbol() { return classDelegateTypeSymbol; }
    void AddArgument(std::unique_ptr<BoundExpression>&& argument);
    const std::vector<std::unique_ptr<BoundExpression>>& Arguments() const { return arguments; }
    bool ContainsExceptionCapture() const override;
private:
    cmajor::symbols::ClassDelegateTypeSymbol* classDelegateTypeSymbol;
    std::vector<std::unique_ptr<BoundExpression>> arguments;
};

class BoundConstructExpression : public BoundExpression
{
public:
    BoundConstructExpression(std::unique_ptr<BoundExpression>&& constructorCall_, cmajor::symbols::TypeSymbol* resultType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    std::string TypeString() const override { return "construct expression"; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* ConstructorCall() { return constructorCall.get(); }
    const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const override { return constructorCall->GetFunctionSymbol(); }
    cmajor::symbols::FunctionSymbol* GetFunctionSymbol() override { return constructorCall->GetFunctionSymbol(); }
private:
    std::unique_ptr<BoundExpression> constructorCall;
};

class BoundConstructAndReturnTemporaryExpression : public BoundExpression
{
public:
    BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>&& constructorCall_, std::unique_ptr<BoundExpression>&& boundTemporary_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    std::string TypeString() const override { return "construct and return temporary expression"; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* ConstructorCall() { return constructorCall.get(); }
    const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const override { return constructorCall->GetFunctionSymbol(); }
    cmajor::symbols::FunctionSymbol* GetFunctionSymbol() override { return constructorCall->GetFunctionSymbol(); }
    BoundExpression* BoundTemporary() { return boundTemporary.get(); }
private:
    std::unique_ptr<BoundExpression> constructorCall;
    std::unique_ptr<BoundExpression> boundTemporary;
};

class BoundClassOrClassDelegateConversionResult : public BoundExpression
{
public:
    BoundClassOrClassDelegateConversionResult(std::unique_ptr<BoundExpression>&& conversionResult_, std::unique_ptr<BoundFunctionCall>&& conversionFunctionCall_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override { return true; }
    std::string TypeString() const override { return "class conversion result"; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* ConversionResult() { return conversionResult.get(); }
    BoundFunctionCall* ConversionFunctionCall() { return conversionFunctionCall.get(); }
private:
    std::unique_ptr<BoundExpression> conversionResult;
    std::unique_ptr<BoundFunctionCall> conversionFunctionCall;
};

class BoundConversion : public BoundExpression
{
public:
    BoundConversion(std::unique_ptr<BoundExpression>&& sourceExpr_, cmajor::symbols::FunctionSymbol* conversionFun_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool IsLvalueExpression() const override;
    std::string TypeString() const override { return "conversion"; }
    BoundExpression* SourceExpr() { return sourceExpr.get(); }
    cmajor::symbols::FunctionSymbol* ConversionFun() { return conversionFun; }
    std::unique_ptr<cmajor::symbols::Value> ToValue(BoundCompileUnit& boundCompileUnit) const override;
    bool ContainsExceptionCapture() const override;
    void AddTemporary(std::unique_ptr<BoundLocalVariable>&& temporary);
private:
    std::unique_ptr<BoundExpression> sourceExpr;
    cmajor::symbols::FunctionSymbol* conversionFun;
    std::vector<std::unique_ptr<BoundLocalVariable>> temporaries;
};

class BoundIsExpression : public BoundExpression
{
public:
    BoundIsExpression(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::ClassTypeSymbol* rightClassType_, cmajor::symbols::TypeSymbol* boolType_,
        std::unique_ptr<BoundLocalVariable>&& leftClassIdVar_, std::unique_ptr<BoundLocalVariable>&& rightClassIdVar_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    std::string TypeString() const override { return "is expression"; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* Expr() { return expr.get(); }
    cmajor::symbols::ClassTypeSymbol* RightClassType() { return rightClassType; }
private:
    std::unique_ptr<BoundExpression> expr;
    cmajor::symbols::ClassTypeSymbol* rightClassType;
    std::unique_ptr<BoundLocalVariable> leftClassIdVar;
    std::unique_ptr<BoundLocalVariable> rightClassIdVar;
};

class BoundAsExpression : public BoundExpression
{
public:
    BoundAsExpression(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::ClassTypeSymbol* rightClassType_, std::unique_ptr<BoundLocalVariable>&& variable_,
        std::unique_ptr<BoundLocalVariable>&& leftClassIdVar_, std::unique_ptr<BoundLocalVariable>&& rightClassIdVar_, cmajor::symbols::Context* context_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    std::string TypeString() const override { return "as expression"; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* Expr() { return expr.get(); }
    BoundLocalVariable* Variable() { return variable.get(); }
    cmajor::symbols::ClassTypeSymbol* RightClassType() { return rightClassType; }
private:
    std::unique_ptr<BoundExpression> expr;
    cmajor::symbols::ClassTypeSymbol* rightClassType;
    std::unique_ptr<BoundLocalVariable> variable;
    std::unique_ptr<BoundLocalVariable> leftClassIdVar;
    std::unique_ptr<BoundLocalVariable> rightClassIdVar;
    cmajor::symbols::Context* context;
};

class BoundTypeNameExpression : public BoundExpression
{
public:
    BoundTypeNameExpression(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::TypeSymbol* constCharPtrType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* ClassPtr() { return classPtr.get(); }
private:
    std::unique_ptr<BoundExpression> classPtr;
};

class BoundTypeIdExpression : public BoundExpression
{
public:
    BoundTypeIdExpression(std::unique_ptr<BoundExpression>&& classPtr_, cmajor::symbols::TypeSymbol* ulongType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* ClassPtr() { return classPtr.get(); }
private:
    std::unique_ptr<BoundExpression> classPtr;
};

class BoundBitCast : public BoundExpression
{
public:
    BoundBitCast(std::unique_ptr<BoundExpression>&& expr_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    bool ContainsExceptionCapture() const override;
    BoundExpression* Expr() { return expr.get(); }
private:
    std::unique_ptr<BoundExpression> expr;
};

class BoundFunctionPtr : public BoundExpression
{
public:
    BoundFunctionPtr(const soul::ast::Span& span_, cmajor::symbols::FunctionSymbol* function_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    cmajor::symbols::FunctionSymbol* Function() { return function; }
private:
    cmajor::symbols::FunctionSymbol* function;
};

class BoundDisjunction : public BoundExpression
{
public:
    BoundDisjunction(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& left_, std::unique_ptr<BoundExpression>&& right_, cmajor::symbols::TypeSymbol* boolType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    BoundExpression* Left() { return left.get(); }
    BoundExpression* Right() { return right.get(); }
    void SetTemporary(BoundLocalVariable* temporary_);
    bool ContainsExceptionCapture() const override;
private:
    std::unique_ptr<BoundExpression> left;
    std::unique_ptr<BoundExpression> right;
    std::unique_ptr<BoundLocalVariable> temporary;
};

class BoundConjunction : public BoundExpression
{
public:
    BoundConjunction(const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& left_, std::unique_ptr<BoundExpression>&& right_, cmajor::symbols::TypeSymbol* boolType_);
    BoundExpression* Clone() override;
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    bool HasValue() const override { return true; }
    BoundExpression* Left() { return left.get(); }
    BoundExpression* Right() { return right.get(); }
    void SetTemporary(BoundLocalVariable* temporary_);
    bool ContainsExceptionCapture() const override;
private:
    std::unique_ptr<BoundExpression> left;
    std::unique_ptr<BoundExpression> right;
    std::unique_ptr<BoundLocalVariable> temporary;
};

class BoundTypeExpression : public BoundExpression
{
public:
    BoundTypeExpression(const soul::ast::Span& span_, cmajor::symbols::TypeSymbol* type_);
    BoundExpression* Clone() override;
    bool IsComplete() const override { return false; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "type expression"; }
};

class BoundNamespaceExpression : public BoundExpression
{
public:
    BoundNamespaceExpression(const soul::ast::Span& span_, cmajor::symbols::NamespaceSymbol* ns_);
    BoundExpression* Clone() override;
    bool IsComplete() const override { return false; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "namespace expression"; }
    cmajor::symbols::NamespaceSymbol* Ns() { return ns; }
private:
    cmajor::symbols::NamespaceSymbol* ns;
    std::unique_ptr<cmajor::symbols::TypeSymbol> nsType;
};

class BoundFunctionGroupExpression : public BoundExpression
{
public:
    BoundFunctionGroupExpression(const soul::ast::Span& span_, int fileIndex_, const util::uuid& moduleId_, cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol_);
    BoundExpression* Clone() override;
    bool IsComplete() const override { return false; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "function group expression"; }
    const cmajor::symbols::FunctionGroupSymbol* FunctionGroup() const { return functionGroupSymbol; }
    cmajor::symbols::FunctionGroupSymbol* FunctionGroup() { return functionGroupSymbol; }
    void SetClassPtr(std::unique_ptr<BoundExpression>&& classPtr_);
    BoundExpression* ClassPtr() { return classPtr.get(); }
    BoundExpression* ReleaseClassPtr() { return classPtr.release(); }
    bool IsScopeQualified() const { return scopeQualified; }
    void SetScopeQualified() { scopeQualified = true; }
    cmajor::symbols::ContainerScope* QualifiedScope() const { return qualifiedScope; }
    void SetQualifiedScope(cmajor::symbols::ContainerScope* qualifiedScope_) { qualifiedScope = qualifiedScope_; }
    void SetTemplateArgumentTypes(const std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes_);
    const std::vector<cmajor::symbols::TypeSymbol*>& TemplateArgumentTypes() const { return templateArgumentTypes; }
private:
    cmajor::symbols::FunctionGroupSymbol* functionGroupSymbol;
    std::unique_ptr<cmajor::symbols::TypeSymbol> functionGroupType;
    std::unique_ptr<BoundExpression> classPtr;
    bool scopeQualified;
    cmajor::symbols::ContainerScope* qualifiedScope;
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
};

class BoundMemberExpression : public BoundExpression
{
public:
    BoundMemberExpression(cmajor::symbols::Context* context_, const soul::ast::Span& span_, std::unique_ptr<BoundExpression>&& classPtr_, 
        std::unique_ptr<BoundExpression>&& member_);
    BoundExpression* Clone() override;
    bool IsComplete() const override { return false; }
    void Load(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Store(cmajor::ir::Emitter& emitter, cmajor::ir::OperationFlags flags) override;
    void Accept(BoundNodeVisitor& visitor) override;
    std::string TypeString() const override { return "member expression"; }
    BoundExpression* ClassPtr() { return classPtr.get(); }
    void ResetClassPtr() { classPtr.reset(); }
    BoundExpression* ReleaseClassPtr() { return classPtr.release(); }
    BoundExpression* Member() { return member.get(); }
    const cmajor::symbols::FunctionSymbol* GetFunctionSymbol() const override { return member->GetFunctionSymbol(); }
    cmajor::symbols::FunctionSymbol* GetFunctionSymbol() override { return member->GetFunctionSymbol(); }
private:
    cmajor::symbols::Context* context;
    std::unique_ptr<BoundExpression> classPtr;
    std::unique_ptr<BoundExpression> member;
    std::unique_ptr<cmajor::symbols::TypeSymbol> memberExpressionType;
};

} // namespace cmajor::binder
