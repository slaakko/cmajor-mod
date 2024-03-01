// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.debug.expr.evaluator;

import cmajor.debugger.container;
import cmajor.debugger.evaluator;
import cmajor.debugger;
import cmajor.debugger.request;
import cmajor.debugger.util;

namespace cmajor::debugger {

DebugExprEvaluator::DebugExprEvaluator(Debugger* debugger_) : debugger(debugger_), integer(-1), rangeStart(-1), rangeEnd(-1), status(cmajor::debug::InitializationStatus::unknown)
{
}

void DebugExprEvaluator::Visit(cmajor::debug::BoundDebugExpression& expr)
{
    status = expr.Status();
    cmajor::debug::BoundDebugNode* node = expr.Node();
    switch (node->GetKind())
    {
        case cmajor::debug::BoundDebugNode::Kind::typeNode:
        {
            cmajor::debug::DIType* type = node->Type();
            std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("result"));
            element->AppendChild(type->ToXml().release());
            element->SetAttribute("success", "true");
            result.reset(element.release());
            break;
        }
        case cmajor::debug::BoundDebugNode::Kind::subscriptNode:
        {
            node->Accept(*this);
            break;
        }
        case cmajor::debug::BoundDebugNode::Kind::rangeNode:
        {
            node->Accept(*this);
            break;
        }
        default:
        {
            Evaluate(node);
            break;
        }
    }
}

void DebugExprEvaluator::Visit(cmajor::debug::BoundIntegerLiteralNode& node)
{
    integer = node.Value();
}

void DebugExprEvaluator::Visit(cmajor::debug::BoundSubscriptNode& node)
{
    integer = -1;
    node.Index()->Accept(*this);
    int64_t index = integer;
    if (index == -1)
    {
        throw std::runtime_error(node.ToString() + ": invalid subscript");
    }
    cmajor::debug::BoundDebugNode* subject = node.Subject();
    cmajor::debug::DIType* subjectType = subject->Type();
    switch (subjectType->GetKind())
    {
        case cmajor::debug::DIType::Kind::pointerType:
        case cmajor::debug::DIType::Kind::arrayType:
        {
            Evaluate(&node);
            break;
        }
        case cmajor::debug::DIType::Kind::specializationType:
        {
            cmajor::debug::ContainerClassTemplateKind containerKind = GetContainerKind(subjectType);
            if (containerKind != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
            {
                Container* container = GetOrInsertContainer(containerKind, subject->SourceNode()->ToString(), debugger);
                result = container->Subscript(subject->SourceNode()->ToString(), index);
            }
            else
            {
                throw std::runtime_error(node.ToString() + ": unknown container type");
            }
            break;
        }
    }
}

void DebugExprEvaluator::Visit(cmajor::debug::BoundRangeNode& node)
{
    integer = -1;
    node.RangeStart()->Accept(*this);
    rangeStart = integer;
    integer = -1;
    node.RangeEnd()->Accept(*this);
    rangeEnd = integer;
    cmajor::debug::BoundDebugNode* subject = node.Subject();
    cmajor::debug::DIType* subjectType = subject->Type();
    switch (subjectType->GetKind())
    {
        case cmajor::debug::DIType::Kind::pointerType:
        {
            EvaluatePointerRange(&node, subject, rangeStart, rangeEnd);
            break;
        }
        case cmajor::debug::DIType::Kind::arrayType:
        {
            EvaluateArrayRange(&node, subject, rangeStart, rangeEnd);
            break;
        }
        case cmajor::debug::DIType::Kind::specializationType:
        {
            cmajor::debug::ContainerClassTemplateKind containerKind = GetContainerKind(subjectType);
            if (containerKind != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
            {
                Container* container = GetOrInsertContainer(containerKind, subject->SourceNode()->ToString(), debugger);
                result = container->Range(subject->SourceNode()->ToString(), rangeStart, rangeEnd);
            }
            else
            {
                throw std::runtime_error(node.ToString() + ": unknown container type");
            }
            break;
        }
    }
}

void DebugExprEvaluator::Evaluate(cmajor::debug::BoundDebugNode* node)
{
    cmajor::debug::DebuggerVariable variable = debugger->GetNextDebuggerVariable();
    std::string gdbExprString = node->GdbExprString();
    VarCreateRequest request(variable.GdbVarName(), "*", gdbExprString);
    std::unique_ptr<Reply> reply = debugger->Execute(&request);
    result.reset(reply->ToXml().release());
    result->SetAttribute("status", InitializationStatusStr(status));
    AddTypes(node);
    cmajor::debug::ContainerClassTemplateKind containerKind = GetContainerKind(node->Type());
    if (containerKind != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
    {
        Container* container = GetOrInsertContainer(containerKind, node->SourceNode()->ToString(), debugger);
        result->SetAttribute("container", ContainerName(container->GetKind()));
        if (status != cmajor::debug::InitializationStatus::uninitialized)
        {
            int64_t count = container->Count(node->SourceNode()->ToString());
            if (count != -1)
            {
                result->SetAttribute("count", std::to_string(count));
            }
        }
    }
    VarDeleteRequest varDeleteRequest(variable.GdbVarName());
    std::unique_ptr<Reply> deleteReply = debugger->Execute(&varDeleteRequest);
}

void DebugExprEvaluator::EvaluatePointerRange(cmajor::debug::BoundDebugNode* node, cmajor::debug::BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd)
{
    if (rangeEnd == -1)
    {
        throw std::runtime_error("must specify range end explicitly for pointer type range expression: " + node->ToString());
    }
    std::unique_ptr<soul::xml::Element> rangeResult(soul::xml::MakeElement("result"));
    for (int64_t index = rangeStart; index < rangeEnd; ++index)
    {
        cmajor::debug::IntegerDebugExprNode integerLiteralNode(index);
        cmajor::debug::BoundIntegerLiteralNode boundIntegerLiteralNode(debugger->GetDebugInfo()->GetMainProject()->GetLongType(), index, &integerLiteralNode);
        cmajor::debug::SubscriptDebugExprNode subscriptNode(subject->SourceNode()->Clone(), integerLiteralNode.Clone());
        cmajor::debug::BoundSubscriptNode boundSubscriptNode(node->Type(), subject->Clone(), boundIntegerLiteralNode.Clone(), &subscriptNode);
        boundSubscriptNode.Accept(*this);
        std::unique_ptr<soul::xml::Element> subscriptResult(result.release());
        rangeResult->AppendChild(subscriptResult.release());
    }
    result.reset(rangeResult.release());
}

void DebugExprEvaluator::EvaluateArrayRange(cmajor::debug::BoundDebugNode* node, cmajor::debug::BoundDebugNode* subject, int64_t rangeStart, int64_t rangeEnd)
{
    cmajor::debug::DIType* subjectType = subject->Type();
    if (rangeEnd == -1)
    {
        if (subjectType->GetKind() == cmajor::debug::DIType::Kind::arrayType)
        {
            cmajor::debug::DIArrayType* arrayType = static_cast<cmajor::debug::DIArrayType*>(subjectType);
            if (arrayType->Size() != -1)
            {
                rangeEnd = arrayType->Size();
            }
        }
    }
    if (rangeEnd == -1)
    {
        throw std::runtime_error("must specify range end explicitly for array type without size range expression: " + node->ToString());
    }
    std::unique_ptr<soul::xml::Element> rangeResult(soul::xml::MakeElement("result"));
    for (int64_t index = rangeStart; index < rangeEnd; ++index)
    {
        cmajor::debug::IntegerDebugExprNode integerLiteralNode(index);
        cmajor::debug::BoundIntegerLiteralNode boundIntegerLiteralNode(debugger->GetDebugInfo()->GetMainProject()->GetLongType(), index, &integerLiteralNode);
        cmajor::debug::SubscriptDebugExprNode subscriptNode(subject->SourceNode()->Clone(), integerLiteralNode.Clone());
        cmajor::debug::BoundSubscriptNode boundSubscriptNode(node->Type(), subject->Clone(), boundIntegerLiteralNode.Clone(), &subscriptNode);
        boundSubscriptNode.Accept(*this);
        std::unique_ptr<soul::xml::Element> subscriptResult(result.release());
        rangeResult->AppendChild(subscriptResult.release());
    }
    result.reset(rangeResult.release());
}

void DebugExprEvaluator::AddTypes(cmajor::debug::BoundDebugNode* node)
{
    if (result)
    {
        cmajor::debug::DITypeRef staticTypeRef(node->Type());
        result->AppendChild(staticTypeRef.ToXml("static_type").release());
        cmajor::debug::DIType* dynamicType = GetDynamicType(node, node->Type(), debugger); 
        if (dynamicType)
        {
            cmajor::debug::DITypeRef dynamicTypeRef(dynamicType);
            result->AppendChild(dynamicTypeRef.ToXml("dynamic_type").release());
        }
    }
}

cmajor::debug::ContainerClassTemplateKind DebugExprEvaluator::GetContainerKind(cmajor::debug::DIType* type)
{
    if (type->GetKind() == cmajor::debug::DIType::Kind::specializationType)
    {
        cmajor::debug::DIClassTemplateSpecializationType* specialization = static_cast<cmajor::debug::DIClassTemplateSpecializationType*>(type);
        return specialization->GetContainerClassTemplateKind();
    }
    return cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate;
}

} // namespace cmajor::debugger
