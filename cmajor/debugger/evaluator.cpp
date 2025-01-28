// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debugger.evaluator;

import cmajor.debugger;
import cmajor.debugger.container.subscript.translator;
import cmajor.debugger.debug.expr.evaluator;
import cmajor.debug.expression.parser;
import cmajor.debug.debug.expr.binder;
import soul.xml.xpath;
import util;

namespace cmajor::debugger {

class EvaluationContext
{
public:
    static EvaluationContext& Instance();
    void Clear();
    Container* GetOrInsertContainer(cmajor::debug::ContainerClassTemplateKind containerKind, const std::string& containerVarExpr, Debugger* debugger);
private:
    EvaluationContext();
    std::vector<std::unique_ptr<Container>> containers;
    std::map<uint64_t, Container*> containerMap;
};

EvaluationContext::EvaluationContext()
{
}

EvaluationContext& EvaluationContext::Instance()
{
    static EvaluationContext instance;
    return instance;
}

void EvaluationContext::Clear()
{
    containers.clear();
    containerMap.clear();
}

void ClearBrowsingData()
{
    EvaluationContext::Instance().Clear();
}

Container* EvaluationContext::GetOrInsertContainer(cmajor::debug::ContainerClassTemplateKind containerKind, const std::string& containerVarExpr, Debugger* debugger)
{
    uint64_t address = GetContainerAddress(containerKind, containerVarExpr, debugger);
    if (address != 0)
    {
        auto it = containerMap.find(address);
        if (it != containerMap.cend())
        {
            Container* container = it->second;
            if (container->GetKind() == containerKind)
            {
                return container;
            }
        }
    }
    Container* container = CreateContainer(containerKind, address, debugger);
    if (address != 0)
    {
        containerMap[address] = container;
    }
    containers.push_back(std::unique_ptr<Container>(container));
    return container;
}

Container* GetOrInsertContainer(cmajor::debug::ContainerClassTemplateKind containerKind, const std::string& containerVarExpr, Debugger* debugger)
{
    return EvaluationContext::Instance().GetOrInsertContainer(containerKind, containerVarExpr, debugger);
}

cmajor::info::db::Type ToTypeObject(soul::xml::Element* element)
{
    cmajor::info::db::Type type;
    type.name = element->GetAttribute("name");
    type.id = element->GetAttribute("id");
    return type;
}

cmajor::info::db::Result ToResultObject(soul::xml::Element* element)
{
    cmajor::info::db::Result result;
    std::string status = element->GetAttribute("status");
    if (status == "initialized")
    {
        result.initialized = true;
    }
    else
    {
        result.initialized = false;
        result.value = "<not initialized>";
    }
    if (result.initialized)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> valueNodeSet = soul::xml::xpath::EvaluateToNodeSet("value", element);
        if (valueNodeSet->Count() == 1)
        {
            soul::xml::Node* node = valueNodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* elem = static_cast<soul::xml::Element*>(node);
                result.value = elem->GetAttribute("value");
            }
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> staticTypeNodeSet = soul::xml::xpath::EvaluateToNodeSet("static_type", element);
    if (staticTypeNodeSet->Count() == 1)
    {
        soul::xml::Node* node = staticTypeNodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* elem = static_cast<soul::xml::Element*>(node);
            result.staticType = ToTypeObject(elem);
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> dynamicTypeNodeSet = soul::xml::xpath::EvaluateToNodeSet("dynamic_type", element);
    if (dynamicTypeNodeSet->Count() == 1)
    {
        soul::xml::Node* node = dynamicTypeNodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* elem = static_cast<soul::xml::Element*>(node);
            result.dynamicType = ToTypeObject(elem);
        }
    }
    return result;
}

cmajor::info::db::EvaluateReply Evaluate(const std::string& expression, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply reply;
    try
    {
        std::unique_ptr<soul::xml::Element> result;
        std::unique_ptr<cmajor::debug::DebugExprNode> node = cmajor::debug::Parse(expression);
        cmajor::debug::DebugExprBinder binder(debugger->StoppedInstruction(), debugger->GetDebugInfo(), debugger->StoppedInstruction()->GetScope(), false);
        node->Accept(binder);
        cmajor::debug::BoundDebugExpression* boundExpression = binder.BoundExpression(node.get());
        if (boundExpression->HasContainerSubscript())
        {
            ContainerSubscriptTranslator translator(debugger);
            boundExpression->Accept(translator);
            return Evaluate(translator.Expression(), debugger);
        }
        else
        {
            DebugExprEvaluator evaluator(debugger);
            boundExpression->Accept(evaluator);
            result.reset(evaluator.ReleaseResult());
        }
        reply.result = ToResultObject(result.get());
        reply.success = true;
    }
    catch (const std::exception& ex)
    {
        reply.success = false;
        reply.error = ex.what();
    }
    return reply;
}

std::unique_ptr<soul::xml::Element> EvaluateToXml(const std::string& expression, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply reply = Evaluate(expression, debugger);
    return reply.ToXml();
}

std::string GetValue(const std::string& expression, cmajor::debug::DIType* type, int64_t& count, cmajor::debug::DIType*& dynType, std::set<uint64_t>& printedPointers,
    int& level, int maxLevel, Debugger* debugger)
{
    dynType = nullptr;
    cmajor::info::db::EvaluateReply evaluateReply = Evaluate(expression, debugger);
    if (evaluateReply.success)
    {
        switch (type->GetKind())
        {
            case cmajor::debug::DIType::Kind::primitiveType:
            {
                return evaluateReply.result.value;
            }
            case cmajor::debug::DIType::Kind::enumType:
            {
                if (evaluateReply.result.initialized)
                {
                    try
                    {
                        uint64_t value = std::stoull(evaluateReply.result.value);
                        return GetEnumeratedTypeValue(value, static_cast<cmajor::debug::DIEnumType*>(type));
                    }
                    catch (...)
                    {
                        return evaluateReply.result.value;
                    }
                }
                else
                {
                    return evaluateReply.result.value;
                }
            }
            case cmajor::debug::DIType::Kind::specializationType:
            {
                return GetSpecializationValue(evaluateReply.result.initialized, expression, static_cast<cmajor::debug::DIClassTemplateSpecializationType*>(type), count,
                    printedPointers, level, maxLevel, debugger);
            }
            case cmajor::debug::DIType::Kind::classType:
            {
                return GetClassValue(expression, static_cast<cmajor::debug::DIClassType*>(type), count, printedPointers, level, maxLevel, debugger);
            }
            case cmajor::debug::DIType::Kind::pointerType:
            {
                std::string value = evaluateReply.result.value;
                uint64_t addr = util::ParseHexULong(value);
                if (addr != 0)
                {
                    if (printedPointers.find(addr) == printedPointers.cend())
                    {
                        printedPointers.insert(addr);
                        cmajor::debug::DIType* dynamicType = nullptr;
                        if (!evaluateReply.result.dynamicType.id.empty())
                        {
                            try
                            {
                                util::uuid dynamicTypeId = util::ParseUuid(evaluateReply.result.dynamicType.id);
                                dynamicType = type->GetProject()->GetType(dynamicTypeId);
                                dynType = dynamicType;
                            }
                            catch (...)
                            {
                            }
                        }
                        std::string pointedValue = GetPointedValue(expression, static_cast<cmajor::debug::DIPointerType*>(type)->PointedToType(), dynamicType, count, 
                            printedPointers, level, maxLevel, debugger);
                        if (!pointedValue.empty())
                        {
                            value.append(" *=").append(pointedValue);
                        }
                    }
                }
                return value;
            }
            case cmajor::debug::DIType::Kind::referenceType:
            {
                std::string value = evaluateReply.result.value;
                uint64_t addr = util::ParseHexULong(value);
                if (printedPointers.find(addr) == printedPointers.cend())
                {
                    printedPointers.insert(addr);
                    cmajor::debug::DIType* dynamicType = nullptr;
                    if (!evaluateReply.result.dynamicType.id.empty())
                    {
                        try
                        {
                            util::uuid dynamicTypeId = util::ParseUuid(evaluateReply.result.dynamicType.id);
                            dynamicType = type->GetProject()->GetType(dynamicTypeId);
                            dynType = dynamicType;
                        }
                        catch (...)
                        {
                        }
                    }
                    std::string pointedValue = GetPointedValue(expression, static_cast<cmajor::debug::DIReferenceType*>(type)->BaseType(), dynamicType, count, printedPointers, 
                        level, maxLevel, debugger);
                    if (!pointedValue.empty())
                    {
                        value.append(" *=").append(pointedValue);
                    }
                }
                return value;
            }
            case cmajor::debug::DIType::Kind::constType:
            {
                cmajor::debug::DIConstType* constType = static_cast<cmajor::debug::DIConstType*>(type);
                return GetValue(expression, constType->BaseType(), count, dynType, printedPointers, level, maxLevel, debugger);
            }
            case cmajor::debug::DIType::Kind::delegateType:
            {
                return evaluateReply.result.value;
            }
            case cmajor::debug::DIType::Kind::classDelegateType:
            {
                return GetClassDelegateValue(expression, static_cast<cmajor::debug::DIClassDelegateType*>(type), printedPointers, level, maxLevel, debugger);
            }
        }
    }
    return std::string();
}

std::string GetValue(const std::string& parentExpr, cmajor::debug::DIVariable* variable, int64_t& count, cmajor::debug::DIType*& dynType, std::set<uint64_t>& printedPointers, 
    int& level, int maxLevel, Debugger* debugger)
{
    std::string expression;
    if (!parentExpr.empty())
    {
        expression.append(parentExpr).append(".");
    }
    expression.append(variable->Name());
    return GetValue(expression, variable->GetType(), count, dynType, printedPointers, level, maxLevel, debugger);
}

void EvaluateChildRequest(cmajor::debug::Project* project, const std::string& expression, int start, int count, cmajor::info::db::EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply evaluateReply = Evaluate(expression, debugger);
    if (evaluateReply.success)
    {
        const cmajor::info::db::Result& result = evaluateReply.result;
        const cmajor::info::db::Type& staticType = result.staticType;
        const cmajor::info::db::Type& dynamicType = result.dynamicType;
        util::uuid staticTypeId = util::ParseUuid(staticType.id);
        cmajor::debug::DIType* type = project->GetType(staticTypeId);
        while (type)
        {
            switch (type->GetKind())
            {
                case cmajor::debug::DIType::Kind::specializationType:
                {
                    cmajor::debug::DIClassTemplateSpecializationType* specializationType = static_cast<cmajor::debug::DIClassTemplateSpecializationType*>(type);
                    EvaluateSpecializationTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel, debugger);
                    return;
                }
                case cmajor::debug::DIType::Kind::classType:
                {
                    cmajor::debug::DIClassType* classType = static_cast<cmajor::debug::DIClassType*>(type);
                    EvaluateClassTypeChildRequest(classType, expression, start, count, reply, printedPointers, level, maxLevel, debugger);
                    return;
                }
                case cmajor::debug::DIType::Kind::pointerType:
                {
                    if (!dynamicType.id.empty())
                    {
                        EvaluateChildRequest(project, "(*cast<typeid(\"" + dynamicType.id + "\")>(" + expression + "))", start, count, reply, printedPointers, level, maxLevel, debugger);
                    }
                    else
                    {
                        EvaluateChildRequest(project, "(*" + expression + ")", start, count, reply, printedPointers, level, maxLevel, debugger);
                    }
                    return;
                }
                case cmajor::debug::DIType::Kind::referenceType:
                {
                    if (!dynamicType.id.empty())
                    {
                        EvaluateChildRequest(project, "(*cast<typeid(\"" + dynamicType.id + "\")>(" + expression + "))", start, count, reply, printedPointers, level, maxLevel, debugger);
                    }
                    else
                    {
                        EvaluateChildRequest(project, "(*" + expression + ")", start, count, reply, printedPointers, level, maxLevel, debugger);
                    }
                    return;
                }
                case cmajor::debug::DIType::Kind::constType:
                {
                    cmajor::debug::DIConstType* constType = static_cast<cmajor::debug::DIConstType*>(type);
                    type = constType->BaseType();
                    break;
                }
            }
        }
    }
    else
    {
        reply.success = false;
        reply.error = evaluateReply.error;
    }
}

void EvaluateSpecializationTypeChildRequest(cmajor::debug::DIClassTemplateSpecializationType* specializationType, const std::string& expression, int start, int count,
    cmajor::info::db::EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    if (specializationType->GetContainerClassTemplateKind() != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
    {
        EvaluateContainerTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel, debugger);
    }
    else
    {
        EvaluateClassTypeChildRequest(specializationType, expression, start, count, reply, printedPointers, level, maxLevel, debugger);
    }
}

void EvaluateContainerTypeChildRequest(cmajor::debug::DIClassTemplateSpecializationType* containerType, const std::string& expression, int start, int count,
    cmajor::info::db::EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    Container* container = EvaluationContext::Instance().GetOrInsertContainer(containerType->GetContainerClassTemplateKind(), expression, debugger);
    int64_t childCount = container->Count(expression);
    if (start >= 0 && start < childCount)
    {
        if (start + count >= 0 && start + count <= childCount)
        {
            cmajor::debug::DIType* valueType = containerType->ValueType();
            for (int i = start; i < start + count; ++i)
            {
                cmajor::info::db::ChildResult result;
                result.expr = expression + "[" + std::to_string(i) + "]";
                result.name = "[" + std::to_string(i) + "]";
                result.type = valueType->Name();
                int64_t count = 0;
                cmajor::debug::DIType* dynType = nullptr;
                result.value = GetValue(result.expr, valueType, count, dynType, printedPointers, level, maxLevel, debugger);
                if (dynType != nullptr)
                {
                    result.dynType = dynType->Name();
                }
                result.count = count;
                reply.results.push_back(std::move(result));
            }
            reply.success = true;
        }
        else
        {
            reply.success = false;
            reply.error = "start/count not valid";
        }
    }
    else
    {
        reply.success = false;
        reply.error = "start not valid";
    }
}

void EvaluateClassTypeChildRequest(cmajor::debug::DIClassType* classType, const std::string& expression, int start, int count, cmajor::info::db::EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    int index = 0;
    int nb = NumBaseClasses(classType);
    for (int i = 0; i < nb; ++i)
    {
        if (index >= start && index < start + count)
        {
            int nc = i + 1;
            std::string expr = expression;
            cmajor::debug::DIClassType* type = classType;
            for (int j = 0; j < nc; ++j)
            {
                expr.append(".base");
                type = static_cast<cmajor::debug::DIClassType*>(type->BaseClassType());
            }
            cmajor::info::db::ChildResult childResult;
            childResult.expr = expr;
            childResult.name = "[" + type->Name() + "]";
            childResult.type = type->Name();
            int64_t count = 0;
            cmajor::debug::DIType* dynType = nullptr;
            childResult.value = GetValue(expr, type, count, dynType, printedPointers, level, maxLevel, debugger);
            if (dynType != nullptr)
            {
                childResult.dynType = dynType->Name();
            }
            childResult.count = count;
            reply.results.push_back(childResult);
        }
        ++index;
    }
    int nm = classType->MemberVariables().size();
    for (int i = 0; i < nm; ++i)
    {
        if (index >= start && index < start + count)
        {
            cmajor::debug::DIVariable* memberVar = classType->MemberVariables()[i].get();
            std::string expr = expression;
            expr.append(".").append(memberVar->Name());
            cmajor::info::db::ChildResult childResult;
            childResult.expr = expr;
            childResult.name = memberVar->Name();
            cmajor::debug::DIType* type = memberVar->GetType();
            childResult.type = type->Name();
            int64_t count = 0;
            cmajor::debug::DIType* dynType = nullptr;
            childResult.value = GetValue(expr, type, count, dynType, printedPointers, level, maxLevel, debugger);
            if (dynType != nullptr)
            {
                childResult.dynType = dynType->Name();
            }
            childResult.count = count;
            reply.results.push_back(childResult);
        }
        ++index;
    }
    reply.success = true;
}

std::string GetEnumeratedTypeValue(uint64_t value, cmajor::debug::DIEnumType* enumType)
{
    uint64_t wholeValue = value;
    std::string strValue;
    bool first = true;
    while (true)
    {
        cmajor::debug::DIEnumConstant* enumConstant = enumType->GetEnumConstant(value);
        if (enumConstant)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                strValue.append(" | ");
            }
            strValue.append(enumType->Name()).append(".").append(enumConstant->Name());
            if (value == 0)
            {
                break;
            }
        }
        else
        {
            return std::to_string(wholeValue);
        }
    }
    return strValue;
}

std::string GetSpecializationValue(bool initialized, const std::string& expression, cmajor::debug::DIClassTemplateSpecializationType* specializationType, int64_t& count,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    if (specializationType->GetContainerClassTemplateKind() != cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate)
    {
        std::string value;
        value.append("{ ").append("container=<").append(ContainerName(specializationType->GetContainerClassTemplateKind())).append(">");
        if (initialized)
        {
            Container* container = EvaluationContext::Instance().GetOrInsertContainer(specializationType->GetContainerClassTemplateKind(), expression, debugger);
            count = container->Count(expression);
            value.append(", count=").append(std::to_string(count));
        }
        else
        {
            value.append(", value=<uninitialized>");
        }
        value.append(" }");
        return value;
    }
    else
    {
        if (specializationType->Name() == "String<char>")
        {
            return GetStringValue(expression, debugger);
        }
        else if (specializationType->Name() == "String<wchar>")
        {
            return GetWStringValue(expression, debugger);
        }
        else if (specializationType->Name() == "String<uchar>")
        {
            return GetUStringValue(expression, debugger);
        }
        else
        {
            return GetClassValue(expression, specializationType, count, printedPointers, level, maxLevel, debugger);
        }
    }
}

std::string GetClassValue(const std::string& expression, cmajor::debug::DIClassType* classType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel, 
    Debugger* debugger)
{
    bool first = true;
    std::string value = "{ ";
    if (!classType->BaseClassId().is_nil())
    {
        cmajor::debug::DIType* baseClassType = classType->BaseClassType();
        if (baseClassType && (baseClassType->GetKind() == cmajor::debug::DIType::Kind::classType || baseClassType->GetKind() == cmajor::debug::DIType::Kind::specializationType))
        {
            int64_t cnt = 0;
            std::string baseClassValue = GetClassValue(expression + ".base", static_cast<cmajor::debug::DIClassType*>(baseClassType), cnt, printedPointers, level, maxLevel,
                debugger);
            value.append(baseClassValue);
        }
        if (first)
        {
            first = false;
        }
    }
    for (const std::unique_ptr<cmajor::debug::DIVariable>& memberVar : classType->MemberVariables())
    {
        if (first)
        {
            first = false;
        }
        else
        {
            value.append(", ");
        }
        int64_t count = 0;
        cmajor::debug::DIType* dynType = nullptr;
        value.append(memberVar->Name()).append("=").append(GetValue(expression, memberVar.get(), count, dynType, printedPointers, level, maxLevel, debugger));
    }
    value.append(" }");
    count = NumBaseClasses(classType) + classType->MemberVariables().size();
    return value;
}

std::string GetPointedValue(const std::string& expression, cmajor::debug::DIType* derefType, cmajor::debug::DIType* dynamicType, int64_t& count, 
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger)
{
    if (level >= maxLevel)
    {
        return "...";
    }
    ++level;
    cmajor::debug::DIType* dynType = nullptr;
    if (dynamicType)
    {
        return GetValue("(*cast<typeid(\"" + util::ToString(dynamicType->Id()) + "\")>(" + expression + "))", dynamicType->DerefType(), count, dynType,
            printedPointers, level, maxLevel, debugger);
    }
    else
    {
        if (derefType->GetKind() == cmajor::debug::DIType::Kind::primitiveType)
        {
            cmajor::debug::DIPrimitiveType* primitiveType = static_cast<cmajor::debug::DIPrimitiveType*>(derefType);
            if (primitiveType->GetPrimitiveTypeKind() == cmajor::debug::DIPrimitiveType::Kind::charType ||
                primitiveType->GetPrimitiveTypeKind() == cmajor::debug::DIPrimitiveType::Kind::wcharType ||
                primitiveType->GetPrimitiveTypeKind() == cmajor::debug::DIPrimitiveType::Kind::ucharType ||
                primitiveType->GetPrimitiveTypeKind() == cmajor::debug::DIPrimitiveType::Kind::voidType)
            {
                return std::string();
            }
        }
        return GetValue("(*" + expression + ")", derefType, count, dynType, printedPointers, level, maxLevel, debugger);
    }
}

std::string GetClassDelegateValue(const std::string& expression, cmajor::debug::DIClassDelegateType* classDelegateType, std::set<uint64_t>& printedPointers, int& level, int maxLevel,
    Debugger* debugger)
{
    int64_t count = 0;
    std::string value = GetClassValue(expression, static_cast<cmajor::debug::DIClassType*>(classDelegateType->GetClassType()), count, printedPointers, level, maxLevel, debugger);
    return value;
}

std::string GetStringValue(const std::string& expression, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply evaluateReply = Evaluate(expression + ".chars", debugger);
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type firstDoubleQuotePos = value.find('"');
        if (firstDoubleQuotePos != std::string::npos)
        {
            std::string::size_type start = firstDoubleQuotePos + 1;
            while (start != std::string::npos)
            {
                std::string::size_type secondDoubleQuotePos = value.find('"', start);
                if (secondDoubleQuotePos > firstDoubleQuotePos + 1 && value[secondDoubleQuotePos - 1] == '\\')
                {
                    start = secondDoubleQuotePos + 1;
                }
                else if (secondDoubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(firstDoubleQuotePos, secondDoubleQuotePos - firstDoubleQuotePos + 1);
                    if (str.length() > 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

std::string GetWStringValue(const std::string& expression, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply evaluateReply = Evaluate(expression + ".chars", debugger);
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type uPos = value.find('u');
        if (uPos != std::string::npos)
        {
            std::string::size_type start = uPos + 2;
            while (start != std::string::npos)
            {
                std::string::size_type doubleQuotePos = value.find('"', start);
                if (doubleQuotePos > uPos + 2 && value[doubleQuotePos - 1] == '\\')
                {
                    start = doubleQuotePos + 1;
                }
                else if (doubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(uPos, doubleQuotePos - uPos + 1);
                    if (str.length() > 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

std::string GetUStringValue(const std::string& expression, Debugger* debugger)
{
    cmajor::info::db::EvaluateReply evaluateReply = Evaluate(expression + ".chars", debugger);
    if (evaluateReply.success)
    {
        std::string value = evaluateReply.result.value;
        std::string::size_type uPos = value.find('U');
        if (uPos != std::string::npos)
        {
            std::string::size_type start = uPos + 2;
            while (start != std::string::npos)
            {
                std::string::size_type doubleQuotePos = value.find('"', start);
                if (doubleQuotePos > uPos + 2 && value[doubleQuotePos - 1] == '\\')
                {
                    start = doubleQuotePos + 1;
                }
                else if (doubleQuotePos != std::string::npos)
                {
                    std::string str = value.substr(uPos, doubleQuotePos - uPos + 1);
                    if (str.length() > 1024)
                    {
                        str = str.substr(0, 1024) + "...";
                    }
                    return str;
                }
                else
                {
                    start = std::string::npos;
                }
            }
        }
    }
    return std::string();
}

} // namespace cmajor::debugger
