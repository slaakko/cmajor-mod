// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.arithmetics;

namespace cmajor::masm::optimizer {

bool OptimizeNot(cmajor::masm::intermediate::NotInstruction* notInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    switch (notInst->Operand()->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::boolValue:
        {
            cmajor::masm::intermediate::BoolValue* boolValue = static_cast<cmajor::masm::intermediate::BoolValue*>(notInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(!boolValue->GetValue(), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(notInst, value);
            optimized = true;
            break;
        }
    }
    return optimized;
}

bool OptimizeNeg(cmajor::masm::intermediate::NegInstruction* negInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    switch (negInst->Operand()->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            cmajor::masm::intermediate::SByteValue* sbyteValue = static_cast<cmajor::masm::intermediate::SByteValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(-sbyteValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            cmajor::masm::intermediate::ShortValue* shortValue = static_cast<cmajor::masm::intermediate::ShortValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(-shortValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            cmajor::masm::intermediate::IntValue* intValue = static_cast<cmajor::masm::intermediate::IntValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(-intValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            cmajor::masm::intermediate::LongValue* longValue = static_cast<cmajor::masm::intermediate::LongValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(-longValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
        case cmajor::masm::intermediate::ValueKind::floatValue:
        {
            cmajor::masm::intermediate::FloatValue* floatValue = static_cast<cmajor::masm::intermediate::FloatValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<float>(-floatValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
        case cmajor::masm::intermediate::ValueKind::doubleValue:
        {
            cmajor::masm::intermediate::DoubleValue* doubleValue = static_cast<cmajor::masm::intermediate::DoubleValue*>(negInst->Operand());
            cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<double>(-doubleValue->GetValue()), context->GetTypes());
            intermediate::ReplaceInstructionWithValue(negInst, value);
            optimized = true;
            break;
        }
    }
    return optimized;
}

bool OptimizeSignExtend(cmajor::masm::intermediate::SignExtendInstruction* signExtendInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    switch (signExtendInst->Result()->GetType()->Id())
    {
        case cmajor::masm::intermediate::shortTypeId:
        {
            switch (signExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    cmajor::masm::intermediate::SByteValue* sbyteValue = static_cast<cmajor::masm::intermediate::SByteValue*>(signExtendInst->Operand());
                    int16_t val = sbyteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::intTypeId:
        {
            switch (signExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    cmajor::masm::intermediate::SByteValue* sbyteValue = static_cast<cmajor::masm::intermediate::SByteValue*>(signExtendInst->Operand());
                    int32_t val = sbyteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    cmajor::masm::intermediate::ShortValue* shortValue = static_cast<cmajor::masm::intermediate::ShortValue*>(signExtendInst->Operand());
                    int32_t val = shortValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::longTypeId:
        {
            switch (signExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    cmajor::masm::intermediate::SByteValue* sbyteValue = static_cast<cmajor::masm::intermediate::SByteValue*>(signExtendInst->Operand());
                    int64_t val = sbyteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    cmajor::masm::intermediate::ShortValue* shortValue = static_cast<cmajor::masm::intermediate::ShortValue*>(signExtendInst->Operand());
                    int64_t val = shortValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    cmajor::masm::intermediate::IntValue* intValue = static_cast<cmajor::masm::intermediate::IntValue*>(signExtendInst->Operand());
                    int64_t val = intValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(signExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeZeroExtend(cmajor::masm::intermediate::ZeroExtendInstruction* zeroExtendInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    switch (zeroExtendInst->Result()->GetType()->Id())
    {
        case cmajor::masm::intermediate::ushortTypeId:
        {
            switch (zeroExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    cmajor::masm::intermediate::ByteValue* byteValue = static_cast<cmajor::masm::intermediate::ByteValue*>(zeroExtendInst->Operand());
                    uint16_t val = byteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::uintTypeId:
        {
            switch (zeroExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    cmajor::masm::intermediate::ByteValue* byteValue = static_cast<cmajor::masm::intermediate::ByteValue*>(zeroExtendInst->Operand());
                    uint32_t val = byteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    cmajor::masm::intermediate::UShortValue* ushortValue = static_cast<cmajor::masm::intermediate::UShortValue*>(zeroExtendInst->Operand());
                    uint32_t val = ushortValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ulongTypeId:
        {
            switch (zeroExtendInst->Operand()->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    cmajor::masm::intermediate::ByteValue* byteValue = static_cast<cmajor::masm::intermediate::ByteValue*>(zeroExtendInst->Operand());
                    uint64_t val = byteValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    cmajor::masm::intermediate::UShortValue* ushortValue = static_cast<cmajor::masm::intermediate::UShortValue*>(zeroExtendInst->Operand());
                    uint64_t val = ushortValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    cmajor::masm::intermediate::UIntValue* uintValue = static_cast<cmajor::masm::intermediate::UIntValue*>(zeroExtendInst->Operand());
                    uint64_t val = uintValue->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(val), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(zeroExtendInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeAdd(cmajor::masm::intermediate::AddInstruction* addInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = addInst->Left();
    cmajor::masm::intermediate::Value* right = addInst->Right();
    if (left->IsZero())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, right);
        optimized = true;
    }
    else if (right->IsZero())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, left);
        optimized = true;
    }
    if (!optimized)
    {
        switch (left->Kind())
        {
            case cmajor::masm::intermediate::ValueKind::sbyteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::sbyteValue:
                    {
                        int8_t sum = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() + 
                            static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::byteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::byteValue:
                    {
                        uint8_t sum = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::shortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::shortValue:
                    {
                        int16_t sum = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ushortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ushortValue:
                    {
                        uint16_t sum = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::intValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::intValue:
                    {
                        int32_t sum = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::uintValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::uintValue:
                    {
                        uint32_t sum = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::longValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::longValue:
                    {
                        int64_t sum = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ulongValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ulongValue:
                    {
                        uint64_t sum = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::floatValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::floatValue:
                    {
                        float sum = static_cast<cmajor::masm::intermediate::FloatValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::FloatValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<float>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::doubleValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::doubleValue:
                    {
                        double sum = static_cast<cmajor::masm::intermediate::DoubleValue*>(left)->GetValue() +
                            static_cast<cmajor::masm::intermediate::DoubleValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<double>(sum), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(addInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }

        }
    }
    return optimized;
}

bool OptimizeMul(cmajor::masm::intermediate::MulInstruction* mulInst, cmajor::masm::intermediate::Function* fn, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = mulInst->Left();
    cmajor::masm::intermediate::Value* right = mulInst->Right();
    if (left->IsZero())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, left);
        optimized = true;
    }
    else if (right->IsZero())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, right);
        optimized = true;
    }
    else if (left->IsOne())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, right);
        optimized = true;
    }
    else if (right->IsOne())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, left);
        optimized = true;
    }
    else if (left->IsTwo())
    {
        cmajor::masm::intermediate::AddInstruction* addInst = new cmajor::masm::intermediate::AddInstruction(soul::ast::Span(), 
            fn->MakeNextRegValue(soul::ast::Span(), right->GetType(), context), right, right);
        cmajor::masm::intermediate::ReplaceInstructionWithInstruction(mulInst, addInst);
        optimized = true;
    }
    else if (right->IsTwo())
    {
        cmajor::masm::intermediate::AddInstruction* addInst = new cmajor::masm::intermediate::AddInstruction(soul::ast::Span(), 
            fn->MakeNextRegValue(soul::ast::Span(), left->GetType(), context), left, left);
        cmajor::masm::intermediate::ReplaceInstructionWithInstruction(mulInst, addInst);
        optimized = true;
    }
    else
    {
        cmajor::masm::intermediate::Value* leftLog2 = left->Log2(context);
        if (leftLog2)
        {
            cmajor::masm::intermediate::ShlInstruction* shlInst = new cmajor::masm::intermediate::ShlInstruction(soul::ast::Span(), 
                fn->MakeNextRegValue(soul::ast::Span(), left->GetType(), context), right, leftLog2);
            cmajor::masm::intermediate::ReplaceInstructionWithInstruction(mulInst, shlInst);
            optimized = true;
        }
        else
        {
            cmajor::masm::intermediate::Value* rightLog2 = right->Log2(context);
            if (rightLog2)
            {
                cmajor::masm::intermediate::ShlInstruction* shlInst = new cmajor::masm::intermediate::ShlInstruction(soul::ast::Span(),
                    fn->MakeNextRegValue(soul::ast::Span(), right->GetType(), context), left, rightLog2);
                cmajor::masm::intermediate::ReplaceInstructionWithInstruction(mulInst, shlInst);
                optimized = true;
            }
        }
    }
    if (!optimized)
    {
        switch (left->Kind())
        {
            case cmajor::masm::intermediate::ValueKind::sbyteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::sbyteValue:
                    {
                        int8_t product = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::byteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::byteValue:
                    {
                        uint8_t product = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::shortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::shortValue:
                    {
                        int16_t product = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ushortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ushortValue:
                    {
                        uint16_t product = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::intValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::intValue:
                    {
                        int32_t product = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::uintValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::uintValue:
                    {
                        uint32_t product = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::longValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::longValue:
                    {
                        int64_t product = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ulongValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ulongValue:
                    {
                        uint64_t product = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::floatValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::floatValue:
                    {
                        float product = static_cast<cmajor::masm::intermediate::FloatValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::FloatValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<float>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::doubleValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::doubleValue:
                    {
                        double product = static_cast<cmajor::masm::intermediate::DoubleValue*>(left)->GetValue() *
                            static_cast<cmajor::masm::intermediate::DoubleValue*>(right)->GetValue();
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<double>(product), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(mulInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    return optimized;
}

bool OptimizeDiv(cmajor::masm::intermediate::DivInstruction* divInst, cmajor::masm::intermediate::Function* fn, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = divInst->Left();
    cmajor::masm::intermediate::Value* right = divInst->Right();
    if (right->IsOne())
    {
        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, left);
        optimized = true;
    }
    else
    {
        cmajor::masm::intermediate::Value* rightLog2 = right->Log2(context);
        if (rightLog2)
        {
            cmajor::masm::intermediate::ShrInstruction* shrInst = new cmajor::masm::intermediate::ShrInstruction(soul::ast::Span(),
                fn->MakeNextRegValue(soul::ast::Span(), right->GetType(), context), left, rightLog2);
            cmajor::masm::intermediate::ReplaceInstructionWithInstruction(divInst, shrInst);
            optimized = true;
        }
    }
    if (!optimized)
    {
        switch (left->Kind())
        {
            case cmajor::masm::intermediate::ValueKind::sbyteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::sbyteValue:
                    {
                        int8_t r = static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int8_t quotient = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::byteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::byteValue:
                    {
                        uint8_t r = static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint8_t quotient = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::shortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::shortValue:
                    {
                        int16_t r = static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int16_t quotient = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ushortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ushortValue:
                    {
                        uint16_t r = static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint16_t quotient = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::intValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::intValue:
                    {
                        int32_t r = static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int32_t quotient = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::uintValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::uintValue:
                    {
                        uint32_t r = static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint32_t quotient = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::longValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::longValue:
                    {
                        int64_t r = static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int64_t quotient = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ulongValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ulongValue:
                    {
                        uint64_t r = static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint64_t quotient = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::floatValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::floatValue:
                    {
                        float r = static_cast<cmajor::masm::intermediate::FloatValue*>(right)->GetValue();
                        if (r == 0.0f)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        float quotient = static_cast<cmajor::masm::intermediate::FloatValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<float>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::doubleValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::doubleValue:
                    {
                        double r = static_cast<cmajor::masm::intermediate::DoubleValue*>(right)->GetValue();
                        if (r == 0.0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        double quotient = static_cast<cmajor::masm::intermediate::DoubleValue*>(left)->GetValue() / r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<double>(quotient), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(divInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    return optimized;
}

bool OptimizeMod(cmajor::masm::intermediate::ModInstruction* modInst, cmajor::masm::intermediate::Function* fn, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = modInst->Left();
    cmajor::masm::intermediate::Value* right = modInst->Right();
    cmajor::masm::intermediate::Value* rightModPowerOfTwo = right->ModPowerOfTwo(context);
    if (rightModPowerOfTwo)
    {
        cmajor::masm::intermediate::AndInstruction* andInst = new cmajor::masm::intermediate::AndInstruction(soul::ast::Span(),
            fn->MakeNextRegValue(soul::ast::Span(), right->GetType(), context), left, rightModPowerOfTwo);
        cmajor::masm::intermediate::ReplaceInstructionWithInstruction(modInst, andInst);
        optimized = true;
    }
    if (!optimized)
    {
        switch (left->Kind())
        {
            case cmajor::masm::intermediate::ValueKind::sbyteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::sbyteValue:
                    {
                        int8_t r = static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int8_t remainder = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::byteValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::byteValue:
                    {
                        uint8_t r = static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint8_t remainder = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::shortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::shortValue:
                    {
                        int16_t r = static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int16_t remainder = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ushortValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ushortValue:
                    {
                        uint16_t r = static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint16_t remainder = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::intValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::intValue:
                    {
                        int32_t r = static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int32_t remainder = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::uintValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::uintValue:
                    {
                        uint32_t r = static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint32_t remainder = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::longValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::longValue:
                    {
                        int64_t r = static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        int64_t remainder = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
            case cmajor::masm::intermediate::ValueKind::ulongValue:
            {
                switch (right->Kind())
                {
                    case cmajor::masm::intermediate::ValueKind::ulongValue:
                    {
                        uint64_t r = static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                        if (r == 0)
                        {
                            cmajor::masm::intermediate::Error("division by zero", right->Span(), context);
                        }
                        uint64_t remainder = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() % r;
                        cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(remainder), context->GetTypes());
                        cmajor::masm::intermediate::ReplaceInstructionWithValue(modInst, value);
                        optimized = true;
                        break;
                    }
                }
                break;
            }
        }
    }
    return optimized;
}

bool OptimizeShl(cmajor::masm::intermediate::ShlInstruction* shlInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = shlInst->Left();
    cmajor::masm::intermediate::Value* right = shlInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    int8_t shiftResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    uint8_t shiftResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    int16_t shiftResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    uint16_t shiftResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    int32_t shiftResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    uint32_t shiftResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    int64_t shiftResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    uint64_t shiftResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() <<
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shlInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeShr(cmajor::masm::intermediate::ShrInstruction* shrInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = shrInst->Left();
    cmajor::masm::intermediate::Value* right = shrInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    int8_t shiftResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    uint8_t shiftResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    int16_t shiftResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    uint16_t shiftResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    int32_t shiftResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    uint32_t shiftResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    int64_t shiftResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    uint64_t shiftResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() >>
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(shiftResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(shrInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeAnd(cmajor::masm::intermediate::AndInstruction* andInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = andInst->Left();
    cmajor::masm::intermediate::Value* right = andInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    int8_t bitwiseResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    uint8_t bitwiseResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    int16_t bitwiseResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    uint16_t bitwiseResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    int32_t bitwiseResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    uint32_t bitwiseResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    int64_t bitwiseResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    uint64_t bitwiseResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() &
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(andInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeOr(cmajor::masm::intermediate::OrInstruction* orInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = orInst->Left();
    cmajor::masm::intermediate::Value* right = orInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    int8_t bitwiseResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    uint8_t bitwiseResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    int16_t bitwiseResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    uint16_t bitwiseResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    int32_t bitwiseResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    uint32_t bitwiseResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    int64_t bitwiseResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    uint64_t bitwiseResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() |
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(orInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeXor(cmajor::masm::intermediate::XorInstruction* xorInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = xorInst->Left();
    cmajor::masm::intermediate::Value* right = xorInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    int8_t bitwiseResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    uint8_t bitwiseResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint8_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    int16_t bitwiseResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    uint16_t bitwiseResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint16_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    int32_t bitwiseResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    uint32_t bitwiseResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint32_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    int64_t bitwiseResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<int64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    uint64_t bitwiseResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() ^
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().MakeValue(static_cast<uint64_t>(bitwiseResult), context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(xorInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeEqual(cmajor::masm::intermediate::EqualInstruction* equalInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = equalInst->Left();
    cmajor::masm::intermediate::Value* right = equalInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::boolValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::boolValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::BoolValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::BoolValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::floatValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::floatValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::FloatValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::FloatValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::doubleValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::doubleValue:
                {
                    bool equalResult = static_cast<cmajor::masm::intermediate::DoubleValue*>(left)->GetValue() ==
                        static_cast<cmajor::masm::intermediate::DoubleValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(equalResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(equalInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

bool OptimizeLess(cmajor::masm::intermediate::LessInstruction* lessInst, cmajor::masm::intermediate::Context* context)
{
    bool optimized = false;
    cmajor::masm::intermediate::Value* left = lessInst->Left();
    cmajor::masm::intermediate::Value* right = lessInst->Right();
    switch (left->Kind())
    {
        case cmajor::masm::intermediate::ValueKind::sbyteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::sbyteValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::SByteValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::SByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::byteValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::byteValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::ByteValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::ByteValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::shortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::shortValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::ShortValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::ShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ushortValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ushortValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::UShortValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::UShortValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::intValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::intValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::IntValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::IntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::uintValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::uintValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::UIntValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::UIntValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::longValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::longValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::LongValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::LongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::ulongValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::ulongValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::ULongValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::ULongValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::floatValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::floatValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::FloatValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::FloatValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
        case cmajor::masm::intermediate::ValueKind::doubleValue:
        {
            switch (right->Kind())
            {
                case cmajor::masm::intermediate::ValueKind::doubleValue:
                {
                    bool lessResult = static_cast<cmajor::masm::intermediate::DoubleValue*>(left)->GetValue() <
                        static_cast<cmajor::masm::intermediate::DoubleValue*>(right)->GetValue();
                    cmajor::masm::intermediate::Value* value = context->GetData().GetBoolValue(lessResult, context->GetTypes());
                    cmajor::masm::intermediate::ReplaceInstructionWithValue(lessInst, value);
                    optimized = true;
                    break;
                }
            }
            break;
        }
    }
    return optimized;
}

void OptimizeArithmetics(cmajor::masm::intermediate::Function* fn, cmajor::masm::intermediate::Context* context)
{
    bool optimized = true;
    int count = 0;
    int maxCount = context->MaxArithmeticOptimizationCount();
    while (optimized && count < maxCount)
    {
        optimized = false;
        cmajor::masm::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
        while (bb)
        {
            cmajor::masm::intermediate::Instruction* inst = bb->FirstInstruction();
            while (inst)
            {
                cmajor::masm::intermediate::Instruction* next = inst->Next();
                switch (inst->GetOpCode())
                {
                    case cmajor::masm::intermediate::OpCode::not_:
                    {
                        cmajor::masm::intermediate::NotInstruction* notInst = static_cast<cmajor::masm::intermediate::NotInstruction*>(inst);
                        if (OptimizeNot(notInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::neg:
                    {
                        cmajor::masm::intermediate::NegInstruction* negInst = static_cast<cmajor::masm::intermediate::NegInstruction*>(inst);
                        if (OptimizeNeg(negInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::signextend:
                    {
                        cmajor::masm::intermediate::SignExtendInstruction* signExtendInst = static_cast<cmajor::masm::intermediate::SignExtendInstruction*>(inst);
                        if (OptimizeSignExtend(signExtendInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::zeroextend:
                    {
                        cmajor::masm::intermediate::ZeroExtendInstruction* zeroExtendInst = static_cast<cmajor::masm::intermediate::ZeroExtendInstruction*>(inst);
                        if (OptimizeZeroExtend(zeroExtendInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::add:
                    {
                        cmajor::masm::intermediate::AddInstruction* addInst = static_cast<cmajor::masm::intermediate::AddInstruction*>(inst);
                        if (OptimizeAdd(addInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::mul:
                    {
                        cmajor::masm::intermediate::MulInstruction* mulInst = static_cast<cmajor::masm::intermediate::MulInstruction*>(inst);
                        if (OptimizeMul(mulInst, fn, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::div_:
                    {
                        cmajor::masm::intermediate::DivInstruction* divInst = static_cast<cmajor::masm::intermediate::DivInstruction*>(inst);
                        if (OptimizeDiv(divInst, fn, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::mod:
                    {
                        cmajor::masm::intermediate::ModInstruction* modInst = static_cast<cmajor::masm::intermediate::ModInstruction*>(inst);
                        if (OptimizeMod(modInst, fn, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::shl:
                    {
                        cmajor::masm::intermediate::ShlInstruction* shlInst = static_cast<cmajor::masm::intermediate::ShlInstruction*>(inst);
                        if (OptimizeShl(shlInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::shr:
                    {
                        cmajor::masm::intermediate::ShrInstruction* shrInst = static_cast<cmajor::masm::intermediate::ShrInstruction*>(inst);
                        if (OptimizeShr(shrInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::and_:
                    {
                        cmajor::masm::intermediate::AndInstruction* andInst = static_cast<cmajor::masm::intermediate::AndInstruction*>(inst);
                        if (OptimizeAnd(andInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::or_:
                    {
                        cmajor::masm::intermediate::OrInstruction* orInst = static_cast<cmajor::masm::intermediate::OrInstruction*>(inst);
                        if (OptimizeOr(orInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::xor_:
                    {
                        cmajor::masm::intermediate::XorInstruction* xorInst = static_cast<cmajor::masm::intermediate::XorInstruction*>(inst);
                        if (OptimizeXor(xorInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::equal:
                    {
                        cmajor::masm::intermediate::EqualInstruction* equalInst = static_cast<cmajor::masm::intermediate::EqualInstruction*>(inst);
                        if (OptimizeEqual(equalInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                    case cmajor::masm::intermediate::OpCode::less:
                    {
                        cmajor::masm::intermediate::LessInstruction* lessInst = static_cast<cmajor::masm::intermediate::LessInstruction*>(inst);
                        if (OptimizeLess(lessInst, context))
                        {
                            optimized = true;
                        }
                        break;
                    }
                }
                inst = next;
            }
            bb = bb->Next();
        }
        if (optimized)
        {
            ++count;
            fn->SetNumbers();
        }
    }
}

} // cmajor::masm::optimizer
