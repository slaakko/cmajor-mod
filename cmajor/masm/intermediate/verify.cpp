// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.verify;

import cmajor.masm.intermediate.visitor;
import cmajor.masm.intermediate.error;
import cmajor.masm.intermediate.util;
import cmajor.masm.intermediate.types;
import cmajor.masm.intermediate.code;
import cmajor.symbols;
import soul.ast.span;
import util;

namespace cmajor::masm::intermediate {

class VerifierVisitor : public Visitor
{
public:
    VerifierVisitor(Context* context_);
    void Visit(Function& function) override;
    void Visit(BasicBlock& basicBlock) override;
    void Visit(StoreInstruction& inst) override;
    void Visit(ArgInstruction& inst) override;
    void Visit(JmpInstruction& inst) override;
    void Visit(BranchInstruction& inst) override;
    void Visit(ProcedureCallInstruction& inst) override;
    void Visit(RetInstruction& inst) override;
    void Visit(SwitchInstruction& inst) override;
    void Visit(NotInstruction& inst) override;
    void Visit(NegInstruction& inst) override;
    void Visit(SignExtendInstruction& inst) override;
    void Visit(ZeroExtendInstruction& inst) override;
    void Visit(TruncateInstruction& inst) override;
    void Visit(BitcastInstruction& inst) override;
    void Visit(IntToFloatInstruction& inst) override;
    void Visit(FloatToIntInstruction& inst) override;
    void Visit(IntToPtrInstruction& inst) override;
    void Visit(PtrToIntInstruction& inst) override;
    void Visit(AddInstruction& inst) override;
    void Visit(SubInstruction& inst) override;
    void Visit(MulInstruction& inst) override;
    void Visit(DivInstruction& inst) override;
    void Visit(ModInstruction& inst) override;
    void Visit(AndInstruction& inst) override;
    void Visit(OrInstruction& inst) override;
    void Visit(XorInstruction& inst) override;
    void Visit(ShlInstruction& inst) override;
    void Visit(ShrInstruction& inst) override;
    void Visit(EqualInstruction& inst) override;
    void Visit(LessInstruction& inst) override;
    void Visit(ParamInstruction& inst) override;
    void Visit(LocalInstruction& inst) override;
    void Visit(LoadInstruction& inst) override;
    void Visit(ElemAddrInstruction& inst) override;
    void Visit(PtrOffsetInstruction& inst) override;
    void Visit(PtrDiffInstruction& inst) override;
    void Visit(FunctionCallInstruction& inst) override;
    void Visit(NoOperationInstruction& inst) override;
private:
    void CheckSameType(const std::string& typeDescription, Type* type, const std::string& expectedTypeDescription, Type* expected, const soul::ast::Span& span);
    void CheckType(const std::string& typeDescription, Type* type, Type* assertedType, const std::string& expectedTypeDescription, Type* expected, const soul::ast::Span& span);
    void CheckBinaryInstructionTypes(BinaryInstruction& inst);
    void CheckBooleanInstructionTypes(BinaryInstruction& inst);
    void CheckArithmeticType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckArithmeticOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckArithmeticOrPointerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckIntegerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckIntegerOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckFloatingPointType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckPointerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckArithmeticPointerOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckArithmeticPointerFunctionOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span);
    void CheckRegValue(Value* value, const soul::ast::Span& span);
    void CheckValueInstruction(ValueInstruction* valueInstruction);
    void CheckUnaryInstuction(UnaryInstruction* unaryInstruction);
    void CheckBinaryInstuction(BinaryInstruction* binaryInstruction);
    Function* currentFunction;
    BasicBlock* currentBasicBlock;
    int numParams;
    std::vector<Value*> arguments;
    int32_t regNumber;
    int index;
};

VerifierVisitor::VerifierVisitor(Context* context_) : Visitor(context_), currentFunction(nullptr), currentBasicBlock(nullptr), numParams(0), regNumber(0), index(0)
{
}

void VerifierVisitor::CheckSameType(const std::string& typeDescription, Type* type, const std::string& expectedTypeDescription, Type* expected, const soul::ast::Span& span)
{
    if (type != expected)
    {
        Error("type check error: " + typeDescription + " is '" + type->Name() + "' but " + expectedTypeDescription + " '" + expected->Name() + "' expected", span, GetContext());
    }
}

void VerifierVisitor::CheckType(const std::string& typeDescription, Type* type, Type* assertedType, const std::string& expectedTypeDescription, Type* expected, const soul::ast::Span& span)
{
    if (assertedType != expected)
    {
        Error("type check error: " + typeDescription + " is '" + type->Name() + "', and " + expectedTypeDescription + " is '" + assertedType->Name() +
            "' but '" + expected->Name() + "' expected", span, GetContext());
    }
}

void VerifierVisitor::CheckBinaryInstructionTypes(BinaryInstruction& inst)
{
    if (inst.Result()->GetType() != inst.Left()->GetType() || inst.Result()->GetType() != inst.Right()->GetType())
    {
        Error("type check error: equal types expected: types are '" + inst.Result()->GetType()->Name() + "', '" + inst.Left()->GetType()->Name() + +"' and '" +
            inst.Right()->GetType()->Name() + "'", inst.Span(), GetContext());
    }
}

void VerifierVisitor::CheckBooleanInstructionTypes(BinaryInstruction& inst)
{
    CheckSameType("result type", inst.Result()->GetType(), "Boolean type", GetContext()->GetTypes().GetBoolType(), inst.Span());
    if (inst.Left()->GetType() != inst.Right()->GetType())
    {
        Error("type check error: operand types '" + inst.Left()->GetType()->Name() + "' and '" + inst.Right()->GetType()->Name() + "' differ", inst.Span(), GetContext());
    }
}

void VerifierVisitor::CheckArithmeticType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsArithmeticType())
    {
        Error("type check error: arithmetic type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckArithmeticOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsArithmeticType() && !type->IsBooleanType())
    {
        Error("type check error: arithmetic or Boolean type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckArithmeticOrPointerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsArithmeticType() && !type->IsPointerType())
    {
        Error("type check error: arithmetic or pointer type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckIntegerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsIntegerType())
    {
        Error("type check error: integer type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckIntegerOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsIntegerType() && !type->IsBooleanType())
    {
        Error("type check error: integer or Boolean type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckFloatingPointType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsFloatingPointType())
    {
        Error("type check error: floating-point type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckPointerType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsPointerType())
    {
        Error("type check error: pointer type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckArithmeticPointerOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsBooleanType() && !type->IsArithmeticType() && !type->IsPointerType())
    {
        Error("type check exception: Boolean, arithmetic or pointer type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckArithmeticPointerFunctionOrBooleanType(Type* type, const std::string& typeDescription, const soul::ast::Span& span)
{
    if (!type->IsBooleanType() && !type->IsArithmeticType() && !type->IsPointerType() && !type->IsFunctionType())
    {
        Error("type check exception: Boolean, arithmetic, pointer or function type expected, note: " + typeDescription + " is '" + type->Name() + "'", span, GetContext());
    }
}

void VerifierVisitor::CheckRegValue(Value* value, const soul::ast::Span& span)
{
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        if (regValue->Reg() >= 0 && regValue->Reg() < regNumber)
        {
            Instruction* inst = currentFunction->GetInstruction(regValue->Reg());
            if (inst)
            {
                regValue->SetInst(inst);
            }
            else
            {
                Error("code verification error: instruction '" + std::to_string(regValue->Reg()) + " not found from function '" + currentFunction->Name() + "'",
                    span, GetContext());
            }
        }
        else
        {
            Error("code verification error: invalid register value " + std::to_string(regValue->Reg()) +
                ": note: value must be greater than or equal to zero and less than " + std::to_string(regNumber), span, GetContext());
        }
    }
}

void VerifierVisitor::CheckValueInstruction(ValueInstruction* valueInstruction)
{
    RegValue* regValue = valueInstruction->Result();
    if (regValue->Reg() == regNumber)
    {
        regValue->SetInst(valueInstruction);
        ++regNumber;
    }
    else
    {
        Error("code verification error: result register value " + std::to_string(regNumber) + " expected", valueInstruction->Span(), GetContext());
    }
}

void VerifierVisitor::CheckUnaryInstuction(UnaryInstruction* unaryInstruction)
{
    CheckValueInstruction(unaryInstruction);
    CheckRegValue(unaryInstruction->Operand(), unaryInstruction->Span());
}

void VerifierVisitor::CheckBinaryInstuction(BinaryInstruction* binaryInstruction)
{
    CheckValueInstruction(binaryInstruction);
    CheckRegValue(binaryInstruction->Left(), binaryInstruction->Span());
    CheckRegValue(binaryInstruction->Right(), binaryInstruction->Span());
}

void VerifierVisitor::Visit(Function& function)
{
    if (!function.IsDefined()) return;
    index = 0;
    numParams = 0;
    regNumber = 0;
    currentFunction = &function;
    function.VisitBasicBlocks(*this);
    if (numParams != function.Arity())
    {
        Error("code verification error: function '" + function.Name() + "' has wrong number of parameters: " + std::to_string(numParams) +
            " parameters, function arity=" + std::to_string(function.Arity()), function.Span(), GetContext());
    }
    function.SetNextRegNumber(regNumber);
}

void VerifierVisitor::Visit(BasicBlock& basicBlock)
{
    currentBasicBlock = &basicBlock;
    if (basicBlock.IsEmpty())
    {
        Error("code verification error: basic block " + std::to_string(basicBlock.Id()) + " is empty", basicBlock.Span(), GetContext());
    }
    if (!basicBlock.LastInstruction()->IsTerminator())
    {
        Error("code verification error: the last instruction '" + basicBlock.LastInstruction()->Name() + "' of basic block " + std::to_string(basicBlock.Id()) +
            " is not a terminator", basicBlock.LastInstruction()->Span(), GetContext());
    }
    basicBlock.VisitInstructions(*this);
}

void VerifierVisitor::Visit(StoreInstruction& inst)
{
    CheckArithmeticPointerFunctionOrBooleanType(inst.GetValue()->GetType(), "type of value", inst.Span());
    //CheckArithmeticPointerOrBooleanType(inst.GetValue()->GetType(), "type of value", inst.Span());
    CheckType("type of value", inst.GetValue()->GetType(), inst.GetValue()->GetType()->AddPointer(GetContext()), "pointer type", inst.GetPtr()->GetType(), inst.Span());
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ArgInstruction& inst)
{
    CheckArithmeticPointerOrBooleanType(inst.Arg()->GetType(), "type of argument", inst.Span());
    arguments.push_back(inst.Arg());
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(JmpInstruction& inst)
{
    int32_t target = inst.TargetLabelId();
    BasicBlock* basicBlock = inst.Parent();
    Function* function = basicBlock->Parent();
    BasicBlock* targetBasicBlock = function->GetBasicBlock(target);
    if (!targetBasicBlock)
    {
        Error("code verification error: jump target basic block " + std::to_string(target) + " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
    }
    inst.SetTargetBasicBlock(targetBasicBlock);
    if (&inst != basicBlock->LastInstruction())
    {
        Error("code verification error: terminator in the middle of basic block " + std::to_string(basicBlock->Id()), inst.Span(), GetContext());
    }
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(BranchInstruction& inst)
{
    CheckSameType("condition type", inst.Cond()->GetType(), "Boolean type", GetContext()->GetTypes().GetBoolType(), inst.Span());
    int32_t trueTarget = inst.TrueTargetLabelId();
    BasicBlock* trueBasicBlock = inst.Parent();
    Function* trueFunction = trueBasicBlock->Parent();
    BasicBlock* trueTargetBasicBlock = trueFunction->GetBasicBlock(trueTarget);
    if (!trueTargetBasicBlock)
    {
        Error("code verification error: branch true target basic block " + std::to_string(trueTarget) + " not found from function '" + trueFunction->Name() + "'",
            inst.Span(), GetContext());
    }
    inst.SetTrueTargetBasicBlock(trueTargetBasicBlock);
    int32_t falseTarget = inst.FalseTargetLabelId();
    BasicBlock* falseBasicBlock = inst.Parent();
    Function* falseFunction = falseBasicBlock->Parent();
    BasicBlock* falseTargetBasicBlock = falseFunction->GetBasicBlock(falseTarget);
    if (!falseTargetBasicBlock)
    {
        Error("code verification error: branch false target basic block " + std::to_string(falseTarget) + " not found from function '" + falseFunction->Name() + "'",
            inst.Span(), GetContext());
    }
    inst.SetFalseTargetBasicBlock(falseTargetBasicBlock);
    if (&inst != inst.Parent()->LastInstruction())
    {
        Error("code verification error: terminator in the middle of basic block " + std::to_string(inst.Parent()->Id()), inst.Span(), GetContext());
    }
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ProcedureCallInstruction& inst)
{
    Type* calleeType = inst.Callee()->GetType();
    if (calleeType->IsPointerType())
    {
        calleeType = calleeType->RemovePointer(inst.Span(), GetContext());
    }
    if (calleeType->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(calleeType);
        if (functionType->Arity() != arguments.size())
        {
            Error("code verification error: function call has wrong number of arguments: " + std::to_string(functionType->Arity()) + " arguments expected: note: " +
                std::to_string(arguments.size()) + " arguments passed", inst.Span(), GetContext());
        }
        if (inst.Callee()->IsSymbolValue())
        {
            SymbolValue* calleeSymbolValue = static_cast<SymbolValue*>(inst.Callee());
            Function* function = GetContext()->AddFunctionDeclaration(inst.Span(), calleeType, calleeSymbolValue->Symbol());
            if (function)
            {
                calleeSymbolValue->SetFunction(function);
            }
            else
            {
                Error("internal error: could not add function declaration", inst.Span(), GetContext());
            }
        }
    }
    else
    {
        Error("type check error: callee in function call has invalid type: function type or function pointer type expected: note: type is " + inst.Callee()->GetType()->Name(),
            inst.Span(), GetContext());
    }
    inst.SetArgs(std::move(arguments));
    arguments.clear();
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(RetInstruction& inst)
{
    FunctionType* functionType = currentFunction->GetType();
    if (inst.ReturnValue())
    {
        Type* returnValueType = inst.ReturnValue()->GetType();
        CheckSameType("instruction return type", returnValueType, "function return type", functionType->ReturnType(), inst.Span());
        CheckArithmeticPointerOrBooleanType(returnValueType, "instruction return type", inst.Span());
    }
    else
    {
        Type* returnValueType = GetContext()->GetTypes().GetVoidType();
        CheckSameType("instruction return type", returnValueType, "function return type", functionType->ReturnType(), inst.Span());
    }
    if (&inst != inst.Parent()->LastInstruction())
    {
        Error("code verification error: terminator in the middle of basic block " + std::to_string(inst.Parent()->Id()), inst.Span(), GetContext());
    }
    currentFunction->AddRetBlock(inst.Parent());
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(SwitchInstruction& inst)
{
    CheckIntegerOrBooleanType(inst.Cond()->GetType(), "condition type", inst.Span());
    if (&inst != inst.Parent()->LastInstruction())
    {
        Error("code verification error: terminator in the middle of basic block " + std::to_string(inst.Parent()->Id()), inst.Span(), GetContext());
    }
    int32_t defaultTarget = inst.DefaultTargetLabelId();
    BasicBlock* parent = inst.Parent();
    Function* function = parent->Parent();
    BasicBlock* defaultTargetBlock = function->GetBasicBlock(defaultTarget);
    if (!defaultTargetBlock)
    {
        Error("code verification error: switch default target basic block " + std::to_string(defaultTarget) +
            " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
    }
    inst.SetDefaultTargetBlock(defaultTargetBlock);
    for (CaseTarget& caseTarget : inst.CaseTargets())
    {
        BasicBlock* caseTargetBlock = function->GetBasicBlock(caseTarget.targetLabelId);
        if (!caseTargetBlock)
        {
            Error("code verification error: switch case target basic block " + std::to_string(caseTarget.targetLabelId) +
                " not found from function '" + function->Name() + "'", inst.Span(), GetContext());
        }
        caseTarget.targetBlock = caseTargetBlock;
    }
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(NotInstruction& inst)
{
    CheckIntegerOrBooleanType(inst.Result()->GetType(), "result type", inst.Span());
    CheckIntegerOrBooleanType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(NegInstruction& inst)
{
    CheckSameType("result type", inst.Result()->GetType(), "operand type", inst.Operand()->GetType(), inst.Span());
    CheckArithmeticType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(SignExtendInstruction& inst)
{
    CheckArithmeticType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckArithmeticType(inst.Result()->GetType(), "result type", inst.Span());
    if (inst.Result()->GetType()->Size() <= inst.Operand()->GetType()->Size())
    {
        Error("code verification error: result type width expected to be greater than operand type width", inst.Span(), GetContext());
    }
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ZeroExtendInstruction& inst)
{
    CheckArithmeticOrBooleanType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckArithmeticType(inst.Result()->GetType(), "result type", inst.Span());
    if (!inst.Operand()->GetType()->IsBooleanType() && inst.Result()->GetType()->Size() <= inst.Operand()->GetType()->Size())
    {
        Error("code verification error: result type width expected to be greater than operand type width", inst.Span(), GetContext());
    }
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(TruncateInstruction& inst)
{
    CheckArithmeticType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckArithmeticOrBooleanType(inst.Result()->GetType(), "result type", inst.Span());
    if (inst.Result()->GetType()->Size() >= inst.Operand()->GetType()->Size() && !inst.Result()->GetType()->IsBooleanType())
    {
        Error("code verification error: result type width expected to be less than operand type width", inst.Span(), GetContext());
    }
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(BitcastInstruction& inst)
{
    CheckArithmeticPointerFunctionOrBooleanType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckArithmeticPointerFunctionOrBooleanType(inst.Result()->GetType(), "result type", inst.Span());
    if (inst.Result()->GetType()->Size() != inst.Operand()->GetType()->Size())
    {
        Error("code verification error: result type width expected to be same as operand type width", inst.Span(), GetContext());
    }
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(IntToFloatInstruction& inst)
{
    CheckIntegerType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckFloatingPointType(inst.Result()->GetType(), "result type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(FloatToIntInstruction& inst)
{
    CheckFloatingPointType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckIntegerType(inst.Result()->GetType(), "result type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(IntToPtrInstruction& inst)
{
    CheckIntegerType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckPointerType(inst.Result()->GetType(), "result type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(PtrToIntInstruction& inst)
{
    CheckPointerType(inst.Operand()->GetType(), "operand type", inst.Span());
    CheckIntegerType(inst.Result()->GetType(), "result type", inst.Span());
    CheckUnaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(AddInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckArithmeticType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(SubInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckArithmeticType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(MulInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckArithmeticType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(DivInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckArithmeticType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ModInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(AndInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerOrBooleanType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerOrBooleanType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(OrInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerOrBooleanType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerOrBooleanType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(XorInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerOrBooleanType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerOrBooleanType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ShlInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ShrInstruction& inst)
{
    CheckBinaryInstructionTypes(inst);
    CheckIntegerType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckIntegerType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(EqualInstruction& inst)
{
    CheckBooleanInstructionTypes(inst);
    CheckArithmeticPointerOrBooleanType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticPointerOrBooleanType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(LessInstruction& inst)
{
    CheckBooleanInstructionTypes(inst);
    CheckArithmeticOrPointerType(inst.Left()->GetType(), "left operand type", inst.Span());
    CheckArithmeticOrPointerType(inst.Right()->GetType(), "right operand type", inst.Span());
    CheckBinaryInstuction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ParamInstruction& inst)
{
    CheckArithmeticPointerOrBooleanType(inst.Result()->GetType(), "parameter type", inst.Span());
    CheckValueInstruction(&inst);
    ++numParams;
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(LocalInstruction& inst)
{
    CheckType("type of local", inst.LocalType(), inst.LocalType()->AddPointer(GetContext()), "instruction result type", inst.Result()->GetType(), inst.Span());
    CheckValueInstruction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(LoadInstruction& inst)
{
    CheckPointerType(inst.Ptr()->GetType(), "operand type", inst.Span());
    CheckType("result type", inst.Result()->GetType(), inst.Ptr()->GetType(), "pointer to result type", inst.Result()->GetType()->AddPointer(GetContext()), inst.Span());
    CheckArithmeticPointerOrBooleanType(inst.Result()->GetType(), "result type", inst.Span());
    CheckValueInstruction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(ElemAddrInstruction& inst)
{
    Type* ptrType = inst.Ptr()->GetType();
    CheckPointerType(ptrType, "pointer type", inst.Span());
    PointerType* pointerType = static_cast<PointerType*>(ptrType);
    Type* baseType = pointerType->BaseType();
    if (baseType->IsStructureType())
    {
        StructureType* structureType = static_cast<StructureType*>(baseType);
        CheckIntegerType(inst.Index()->GetType(), "integer type index", inst.Span());
        CheckValueInstruction(&inst);
        if (inst.Index()->IsIntegerValue())
        {
            int64_t index = inst.Index()->GetIntegerValue();
            if (index < 0 || index >= structureType->FieldCount())
            {
                Error("code verification error: invalid index", inst.Span(), GetContext());
            }
        }
    }
    else if (baseType->IsArrayType())
    {
        ArrayType* arrayType = static_cast<ArrayType*>(baseType);
        CheckIntegerType(inst.Index()->GetType(), "integer type index", inst.Span());
        CheckValueInstruction(&inst);
        if (inst.Index()->IsIntegerValue())
        {
            int64_t index = inst.Index()->GetIntegerValue();
            if (index < 0 || index > arrayType->ElementCount())
            {
                Error("code verification error: invalid index", inst.Span(), GetContext());
            }
        }
    }
    else
    {
        Error("type check error: pointer to structure or array type expected", inst.Span(), GetContext());
    }
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(PtrOffsetInstruction& inst)
{
    CheckPointerType(inst.Ptr()->GetType(), "pointer operand type", inst.Span());
    CheckIntegerType(inst.Offset()->GetType(), "offset type", inst.Span());
    CheckValueInstruction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(PtrDiffInstruction& inst)
{
    CheckIntegerType(inst.Result()->GetType(), "result type", inst.Span());
    CheckPointerType(inst.LeftPtr()->GetType(), "left pointer operand type", inst.Span());
    CheckPointerType(inst.RightPtr()->GetType(), "right pointer operand type", inst.Span());
    CheckSameType("left pointer operand type", inst.LeftPtr()->GetType(), "right pointer operand type", inst.RightPtr()->GetType(), inst.Span());
    CheckValueInstruction(&inst);
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(FunctionCallInstruction& inst)
{
    Type* calleeType = inst.Callee()->GetType();
    if (calleeType->IsPointerType())
    {
        calleeType = calleeType->RemovePointer(inst.Span(), GetContext());
    }
    if (calleeType->IsFunctionType())
    {
        FunctionType* functionType = static_cast<FunctionType*>(calleeType);
        if (functionType->Arity() != arguments.size())
        {
            Error("code verification error: function call has wrong number of arguments: " + std::to_string(functionType->Arity()) + " arguments expected: note: " +
                std::to_string(arguments.size()) + " arguments passed", inst.Span(), GetContext());
        }
        if (inst.Callee()->IsSymbolValue())
        {
            SymbolValue* calleeSymbolValue = static_cast<SymbolValue*>(inst.Callee());
            Function* function = GetContext()->AddFunctionDeclaration(inst.Span(), calleeType, calleeSymbolValue->Symbol());
            if (function)
            {
                calleeSymbolValue->SetFunction(function);
            }
            else
            {
                Error("internal error: could not add function declaration", inst.Span(), GetContext());
            }
        }
    }
    else
    {
        Error("type check error: callee in function call has invalid type: function type or function pointer type expected: note: type is " + inst.Callee()->GetType()->Name(),
            inst.Span(), GetContext());
    }
    CheckValueInstruction(&inst);
    inst.SetArgs(std::move(arguments));
    arguments.clear();
    inst.AddToUses();
    inst.SetIndex(index++);
}

void VerifierVisitor::Visit(NoOperationInstruction& inst)
{
    inst.AddToUses();
    inst.SetIndex(index++);
}

void Verify(Context& context)
{
    VerifierVisitor visitor(&context);
    context.GetCode().VisitFunctions(visitor);
}

} // cmajor::masm::intermediate
