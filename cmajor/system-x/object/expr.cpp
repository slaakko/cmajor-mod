// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.expr;

import cmajor.systemx.object.type;
import cmajor.systemx.object.type.table;

namespace cmajor::systemx::object {

int64_t ParseInteger(const std::string& s)
{
    return std::stoll(s);
}

Expr::Expr(ExprKind kind_) : kind(kind_)
{
}

Expr::~Expr()
{
}

ValueExpr::ValueExpr(TypedValue* value_) : Expr(ExprKind::valueExpr), value(value_)
{
}

void ValueExpr::Evaluate(EvaluationContext& context)
{
    context.Push(new ValueRef(value));
}

DollarExpr::DollarExpr(int64_t index_) : Expr(ExprKind::dollarExpr), index(index_)
{
}

void DollarExpr::Evaluate(EvaluationContext& context)
{
    auto& indexedValues = *context.indexedValues;
    if (index >= 0 && index < static_cast<int64_t>(indexedValues.size()))
    {
        TypedValue* value = indexedValues[index].get();
        if (!value->Content() && context.content || value->Start() != context.start)
        {
            Type* type = value->GetType();
            context.address = value->Address();
            indexedValues[index] = std::move(type->Evaluate(context));
            value = indexedValues[index].get();
        }
        context.Push(new ValueRef(value));
    }
    else
    {
        throw std::runtime_error("index out of bounds");
    }
}

DerefExpr::DerefExpr(Expr* subject_) : Expr(ExprKind::derefExpr), subject(subject_), topLevel(false)
{
}

void DerefExpr::Evaluate(EvaluationContext& context)
{
    subject->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    TypedValue* ptrValue = value->Get();
    if (ptrValue->IsPointerValue())
    {
        PointerValue* pointerValue = static_cast<PointerValue*>(ptrValue);
        Type* type = pointerValue->GetType();
        if (type && type->IsPointerType())
        {
            PointerType* pointerType = static_cast<PointerType*>(type);
            if (pointerType->PointerCount() > 1)
            {
                uint64_t address = pointerValue->Address();
                uint64_t addr = context.memory.ReadOcta(context.rv, address, cmajor::systemx::machine::Protection::read);
                PointerValue* resultPtrValue = new PointerValue();
                resultPtrValue->SetAddress(addr);
                int32_t ptrTypeId = MakePointerTypeId(pointerType->BaseType()->Id(), pointerType->PointerCount() - 1);
                Type* type = context.typeTable->GetType(ptrTypeId, context.rv, context.memory, context.stringTable, context.symbolTable);
                if (type->IsPointerType())
                {
                    PointerType* ptrType = static_cast<PointerType*>(type);
                    resultPtrValue->SetType(ptrType);
                    context.Push(resultPtrValue);
                }
                else
                {
                    throw std::runtime_error("pointer type expected");
                }
            }
            else
            {
                EvaluationContext derefContext = context;
                uint64_t address = pointerValue->Address();
                if (topLevel)
                {
                    uint64_t addr = context.memory.ReadOcta(context.rv, address, cmajor::systemx::machine::Protection::read);
                    derefContext.address = addr;
                }
                else
                {
                    derefContext.address = address;
                }
                std::unique_ptr<TypedValue> value = pointerType->BaseType()->Evaluate(derefContext);
                context.Push(value.release());
            }
        }
        else
        {
            throw std::runtime_error("pointer type expected");
        }
    }
    else
    {
        throw std::runtime_error("pointer value expected");
    }
}

MemberExpr::MemberExpr(Expr* subject_, const std::string& id_) : Expr(ExprKind::memberExpr), subject(subject_), id(id_)
{
}

void MemberExpr::Evaluate(EvaluationContext& context)
{
    subject->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    TypedValue* svalue = value->Get();
    if (svalue->IsStructureValue())
    {
        StructureValue* structValue = static_cast<StructureValue*>(svalue);
        const FieldValue& fieldValue = structValue->GetFieldValue(id);
        context.Push(new ValueRef(fieldValue.Value()));
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

ArrowExpr::ArrowExpr(Expr* subject_, const std::string& id_) : Expr(ExprKind::arrowExpr), subject(subject_), id(id_)
{
}

void ArrowExpr::Evaluate(EvaluationContext& context)
{
    subject->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    TypedValue* val = value->Get();
    MemberExpr member(new DerefExpr(new ValueExpr(val)), id);
    member.Evaluate(context);
}

AddExpr::AddExpr(Expr* left_, Expr* right_) : Expr(ExprKind::addExpr), left(left_), right(right_)
{
}

void AddExpr::Evaluate(EvaluationContext& context)
{
    left->Evaluate(context);
    std::unique_ptr<TypedValue> leftValue = context.Pop();
    TypedValue* lvalue = leftValue->Get();
    if (lvalue->IsPointerValue())
    {
        PointerValue* lptrValue = static_cast<PointerValue*>(lvalue);
        Type* type = lptrValue->GetType();
        if (type && type->IsPointerType())
        {
            PointerType* pointerType = static_cast<PointerType*>(type);
            right->Evaluate(context);
            std::unique_ptr<TypedValue> rightValue = context.Pop();
            TypedValue* rvalue = rightValue->Get();
            if (rvalue->IsLongValue())
            {
                LongValue* offsetValue = static_cast<LongValue*>(rvalue);
                int64_t offset = offsetValue->Value();
                PointerValue* resultValue = new PointerValue();
                resultValue->SetType(pointerType);
                int64_t scale = 8;
                if (pointerType->PointerCount() == 1)
                {
                    scale = pointerType->BaseType()->Size();
                }
                resultValue->SetAddress(lptrValue->Address() + offset * scale);
                context.Push(resultValue);
            }
            else
            {
                throw std::runtime_error("long value expected");
            }
        }
        else
        {
            throw std::runtime_error("pointer type expected");
        }
    }
    else
    {
        throw std::runtime_error("pointer value expected");
    }
}

IndexExpr::IndexExpr(Expr* subject_, int64_t index_) : Expr(ExprKind::indexExpr), subject(subject_), index(index_)
{
}

void IndexExpr::Evaluate(EvaluationContext& context)
{
    context.start = index;
    context.count = 1;
    subject->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    TypedValue* val = value->Get();
    if (val->GetType()->IsPointerType())
    {
        LongValue indexValue(index);
        DerefExpr derefExpr(new AddExpr(new ValueExpr(val), new ValueExpr(&indexValue)));
        derefExpr.Evaluate(context);
    }
    else
    {
        context.Push(value.release());
    }
}

RangeExpr::RangeExpr(Expr* subject_, int64_t index_, int64_t count_) : Expr(ExprKind::rangeExpr), subject(subject_), index(index_), count(count_)
{
}

void RangeExpr::Evaluate(EvaluationContext& context)
{
    context.start = index;
    context.count = count;
    subject->Evaluate(context);
}

CastExpr::CastExpr(Expr* subject_, Type* destType_) : Expr(ExprKind::castExpr), subject(subject_), destType(destType_)
{
}

void CastExpr::Evaluate(EvaluationContext& context)
{
    subject->Evaluate(context);
    std::unique_ptr<TypedValue> value = context.Pop();
    value->SetType(destType);
    context.Push(value.release());
}

} // namespace cmajor::systemx::object
