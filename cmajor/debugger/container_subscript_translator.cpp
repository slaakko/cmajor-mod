// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.container.subscript.translator;

import cmajor.debugger.evaluator;

namespace cmajor::debugger {

ContainerSubscriptTranslator::ContainerSubscriptTranslator(Debugger* debugger_) : debugger(debugger_), evaluateIndex(false), index(-1)
{
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundDebugExpression& expr)
{
    expr.Node()->Accept(*this);
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundVariableReferenceNode& node)
{
    expression = node.SourceNode()->ToString();
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundIntegerLiteralNode& node)
{
    if (evaluateIndex)
    {
        index = node.Value();
    }
    else
    {
        expression = node.SourceNode()->ToString();
    }
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundAddNode& node)
{
    node.Left()->Accept(*this);
    std::string left = expression;
    node.Right()->Accept(*this);
    std::string right = expression;
    expression = left + " + " + right;
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundSubNode& node)
{
    node.Left()->Accept(*this);
    std::string left = expression;
    node.Right()->Accept(*this);
    std::string right = expression;
    expression = left + " - " + right;
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundDerefNode& node)
{
    node.Subject()->Accept(*this);
    std::string subject = expression;
    expression = "(*" + subject + ")";
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundAddrOfNode& node)
{
    node.Subject()->Accept(*this);
    std::string subject = expression;
    expression = "(&" + subject + ")";
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundDotNode& node)
{
    node.Subject()->Accept(*this);
    std::string subject = expression;
    expression = subject + "." + node.Member();
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundSubscriptNode& node)
{
    cmajor::debug::BoundDebugNode* subject = node.Subject();
    cmajor::debug::DIType* subjectType = subject->Type();
    bool translated = false;
    if (subjectType->GetKind() == cmajor::debug::DIType::Kind::specializationType)
    {
        cmajor::debug::ContainerClassTemplateKind containerKind = GetContainerKind(subjectType);
        if (containerKind != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
        {
            index = -1;
            bool prevEvaluateIndex = evaluateIndex;
            evaluateIndex = true;
            node.Index()->Accept(*this);
            evaluateIndex = prevEvaluateIndex;
            if (index == -1)
            {
                throw std::runtime_error(node.ToString() + ": invalid subscript");
            }
            subject->Accept(*this);
            std::string containerExpr = expression;
            Container* container = GetOrInsertContainer(containerKind, containerExpr, debugger);
            expression = container->SubscriptExpressionString(containerExpr, index);
            translated = true;
        }
    }
    if (!translated)
    {
        node.Index()->Accept(*this);
        std::string indexStr = expression;
        node.Subject()->Accept(*this);
        std::string subject = expression;
        expression = subject + "[" + indexStr + "]";
    }
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundRangeNode& node)
{
    node.Subject()->Accept(*this);
    std::string subject = expression;
    node.RangeStart()->Accept(*this);
    std::string rangeStart = expression;
    node.RangeEnd()->Accept(*this);
    std::string rangeEnd = expression;
    expression = subject + "[" + rangeStart + ", " + rangeEnd + "]";
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundParenExprNode& node)
{
    node.Subject()->Accept(*this);
    std::string subject = expression;
    expression = "(" + subject + ")";
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundTypeNode& node)
{
    expression = node.SourceNode()->ToString();
}

void ContainerSubscriptTranslator::Visit(cmajor::debug::BoundCastNode& node)
{
    node.TypeIdNode()->Accept(*this);
    std::string typeId = expression;
    node.ExprNode()->Accept(*this);
    std::string expr = expression;
    expression = "cast<" + typeId + ">(" + expr + ")";
}

cmajor::debug::ContainerClassTemplateKind ContainerSubscriptTranslator::GetContainerKind(cmajor::debug::DIType* type)
{
    if (type->GetKind() == cmajor::debug::DIType::Kind::specializationType)
    {
        cmajor::debug::DIClassTemplateSpecializationType* specialization = static_cast<cmajor::debug::DIClassTemplateSpecializationType*>(type);
        return specialization->GetContainerClassTemplateKind();
    }
    return cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate;
}

} // namespace cmajor::debug
