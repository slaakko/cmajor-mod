// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.xpath.function;

import util;
import soul.xml.dom;

namespace soul::xml::xpath {

class StringValueComputerVisitor : public soul::xml::Visitor
{
public:
    void Visit(soul::xml::Text& text) override;
    std::string GetStringValue() { return std::move(stringValue); }
private:
    std::string stringValue;
};

void StringValueComputerVisitor::Visit(soul::xml::Text& text)
{
    stringValue.append(text.Data());
}

std::string GetStringValue(soul::xml::ParentNode* node)
{
    StringValueComputerVisitor visitor;
    node->Accept(visitor);
    return visitor.GetStringValue();
}

std::string StringValue(soul::xml::AttributeNode* attributeNode)
{
    return attributeNode->Value();
}

std::string StringValue(soul::xml::Document* documentNode)
{
    return GetStringValue(documentNode);
}

std::string StringValue(soul::xml::Element* elementNode)
{
    return GetStringValue(elementNode);
}

std::string StringValue(soul::xml::ProcessingInstruction* processingInstructionNode)
{
    return std::string(processingInstructionNode->Data());
}

std::string StringValue(soul::xml::Text* textNode)
{
    return std::string(textNode->Data());
}

std::string StringValue(soul::xml::CDataSection* cdataSectionNode)
{
    return std::string(cdataSectionNode->Data());
}

std::string StringValue(soul::xml::Comment* commentNode)
{
    return std::string(commentNode->Data());
}

std::string StringValue(soul::xml::Node* node)
{
    switch (node->Kind())
    {
        case soul::xml::NodeKind::attributeNode:
        {
            soul::xml::AttributeNode* attributeNode = static_cast<soul::xml::AttributeNode*>(node);
            return StringValue(attributeNode);
        }
        case soul::xml::NodeKind::documentNode:
        {
            soul::xml::Document* documentNode = static_cast<soul::xml::Document*>(node);
            return StringValue(documentNode);
        }
        case soul::xml::NodeKind::elementNode:
        {
            soul::xml::Element* elementNode = static_cast<soul::xml::Element*>(node);
            return StringValue(elementNode);
        }
        case soul::xml::NodeKind::processingInstructionNode:
        {
            soul::xml::ProcessingInstruction* processingInstructionNode = static_cast<soul::xml::ProcessingInstruction*>(node);
            return StringValue(processingInstructionNode);
        }
        case soul::xml::NodeKind::textNode:
        {
            soul::xml::Text* textNode = static_cast<soul::xml::Text*>(node);
            return StringValue(textNode);
        }
        case soul::xml::NodeKind::cdataSectionNode:
        {
            soul::xml::CDataSection* cdataSectionNode = static_cast<soul::xml::CDataSection*>(node);
            return StringValue(cdataSectionNode);
        }
        case soul::xml::NodeKind::commentNode:
        {
            soul::xml::Comment* commentNode = static_cast<soul::xml::Comment*>(node);
            return StringValue(commentNode);
        }
    }
    return std::string();
}

std::unique_ptr<Object> EvaluateToString(NodeSet* nodeSet)
{
    if (nodeSet->Count() == 0)
    {
        return std::unique_ptr<Object>(new String());
    }
    else
    {
        return std::unique_ptr<Object>(new String(StringValue(nodeSet->GetNode(0))));
    }
}

std::unique_ptr<Object> EvaluateToString(Boolean* boolean)
{
    if (boolean->Value())
    {
        return std::unique_ptr<Object>(new String("true"));
    }
    else
    {
        return std::unique_ptr<Object>(new String("false"));
    }
}

std::unique_ptr<Object> EvaluateToString(Number* number)
{
    return std::unique_ptr<Object>(new String(std::to_string(number->Value())));
}

std::unique_ptr<Object> EvaluateToString(String* string)
{
    return std::unique_ptr<Object>(new String(string->Value()));
}

std::unique_ptr<Object> EvaluateToBoolean(NodeSet* nodeSet)
{
    return std::unique_ptr<Object>(new Boolean(nodeSet->Count() != 0));
}

std::unique_ptr<Object> EvaluateToBoolean(Boolean* boolean)
{
    return std::unique_ptr<Object>(new Boolean(boolean->Value()));
}

std::unique_ptr<Object> EvaluateToBoolean(Number* number)
{
    return std::unique_ptr<Object>(new Boolean(number->Value() != 0));
}

std::unique_ptr<Object> EvaluateToBoolean(String* string)
{
    return std::unique_ptr<Object>(new Boolean(!string->Value().empty()));
}

std::unique_ptr<Object> EvaluateToNumber(String* string);

std::unique_ptr<Object> EvaluateToNumber(NodeSet* nodeSet)
{
    std::unique_ptr<Object> stringResult = EvaluateToString(nodeSet);
    if (stringResult->IsString())
    {
        String* string = static_cast<String*>(stringResult.get());
        return EvaluateToNumber(string);
    }
    else
    {
        throw std::runtime_error("string expected");
    }
}

std::unique_ptr<Object> EvaluateToNumber(Boolean* boolean)
{
    if (boolean->Value())
    {
        return std::unique_ptr<Object>(new Number(1));
    }
    else
    {
        return std::unique_ptr<Object>(new Number(0));
    }
}

std::unique_ptr<Object> EvaluateToNumber(Number* number)
{
    return std::unique_ptr<Object>(new Number(number->Value()));
}

std::unique_ptr<Object> EvaluateToNumber(String* string)
{
    return std::unique_ptr<Object>(new Number(std::stod(util::Trim(string->Value()))));
}

constexpr const char* functionNames[static_cast<int>(FunctionKind::max)] =
{
    "boolean", "number", "string", "last", "position", "count"
};

NodeSet* NodeSetCast(Object* object, Function* function)
{
    if (object->IsNodeSet())
    {
        return static_cast<NodeSet*>(object);
    }
    else
    {
        throw std::runtime_error("error: '" + function->Name() + "()' function requires a node-set, " + ObjectKindStr(object->Kind()) + " provided");
    }
}

std::string FunctionName(FunctionKind kind)
{
    return functionNames[static_cast<int>(kind)];
}

Function::Function(FunctionKind kind_) : kind(kind_), name(FunctionName(kind_)), minArity(1), maxArity(1)
{
}

Function::Function(FunctionKind kind_, int minArity_, int maxArity_) : kind(kind_), name(FunctionName(kind_)), minArity(minArity_), maxArity(maxArity_)
{
}

Function::~Function()
{
}

std::string Function::ArityStr() const
{
    if (minArity == maxArity)
    {
        return std::to_string(minArity);
    }
    else if (minArity == 0)
    {
        return "at most " + std::to_string(maxArity);
    }
    else 
    {
        return "at least " + std::to_string(minArity) + " and at most " + std::to_string(maxArity);
    }
}

std::unique_ptr<Object> Function::Evaluate(Context& context, const std::vector<Object*>& arguments)
{
    int n = arguments.size();
    if (n < minArity || n > maxArity)
    {
        throw std::runtime_error("wrong number of arguments for '" + name + "() function: " + ArityStr() + " arguments expected, " + std::to_string(n) + " arguments provided");
    }
    return DoEvaluate(context, arguments);
}

class BooleanFunction : public Function
{
public:
    BooleanFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

BooleanFunction::BooleanFunction() : Function(FunctionKind::boolean)
{
}

std::unique_ptr<Object> BooleanFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    Object* arg = arguments[0];
    switch (arg->Kind())
    {
        case ObjectKind::nodeSet:
        {
            NodeSet* nodeSet = static_cast<NodeSet*>(arg);
            return EvaluateToBoolean(nodeSet);
        }
        case ObjectKind::boolean:
        {
            Boolean* boolean = static_cast<Boolean*>(arg);
            return EvaluateToBoolean(boolean);
        }
        case ObjectKind::number:
        {
            Number* number = static_cast<Number*>(arg);
            return EvaluateToBoolean(number);
        }
        case ObjectKind::string:
        {
            String* string = static_cast<String*>(arg);
            return EvaluateToBoolean(string);
        }
    }
    return std::unique_ptr<Object>(new Boolean());
}

class NumberFunction : public Function
{
public:
    NumberFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

NumberFunction::NumberFunction() : Function(FunctionKind::number, 0, 1)
{
}

std::unique_ptr<Object> NumberFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    Object* arg = nullptr;
    NodeSet nodeSet;
    if (arguments.empty())
    {
        nodeSet.Add(context.Node());
        arg = &nodeSet;
    }
    else
    {
        arg = arguments[0];
    }
    switch (arg->Kind())
    {
        case ObjectKind::nodeSet:
        {
            NodeSet* nodeSet = static_cast<NodeSet*>(arg);
            return EvaluateToNumber(nodeSet);
        }
        case ObjectKind::boolean:
        {
            Boolean* boolean = static_cast<Boolean*>(arg);
            return EvaluateToNumber(boolean);
        }
        case ObjectKind::number:
        {
            Number* number = static_cast<Number*>(arg);
            return EvaluateToNumber(number);
        }
        case ObjectKind::string:
        {
            String* string = static_cast<String*>(arg);
            return EvaluateToNumber(string);
        }
    }
    throw std::runtime_error("error: invalid argument to '" + Name() + "()' function");
}

class StringFunction : public Function
{
public:
    StringFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

StringFunction::StringFunction() : Function(FunctionKind::string, 0, 1)
{
}

std::unique_ptr<Object> StringFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    Object* arg = nullptr;
    NodeSet nodeSet;
    if (arguments.empty())
    {
        nodeSet.Add(context.Node());
        arg = &nodeSet;
    }
    else
    {
        arg = arguments[0];
    }
    switch (arg->Kind())
    {
        case ObjectKind::nodeSet:
        {
            NodeSet* nodeSet = static_cast<NodeSet*>(arg);
            return EvaluateToString(nodeSet);
        }
        case ObjectKind::boolean:
        {
            Boolean* boolean = static_cast<Boolean*>(arg);
            return EvaluateToString(boolean);
        }
        case ObjectKind::number:
        {
            Number* number = static_cast<Number*>(arg);
            return EvaluateToString(number);
        }
        case ObjectKind::string:
        {
            String* string = static_cast<String*>(arg);
            return EvaluateToString(string);
        }
    }
    throw std::runtime_error("error: invalid argument to '" + Name() + "()' function");
}

class LastFunction : public Function
{
public:
    LastFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

LastFunction::LastFunction() : Function(FunctionKind::last, 0, 0)
{
}

std::unique_ptr<Object> LastFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    return std::unique_ptr<Object>(new Number(context.Size()));
}

class PositionFunction : public Function
{
public:
    PositionFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

PositionFunction::PositionFunction() : Function(FunctionKind::position, 0, 0)
{
}

std::unique_ptr<Object> PositionFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    return std::unique_ptr<Object>(new Number(context.Pos()));
}

class CountFunction : public Function
{
public:
    CountFunction();
protected:
    std::unique_ptr<Object> DoEvaluate(Context& context, const std::vector<Object*>& arguments) override;
};

CountFunction::CountFunction() : Function(FunctionKind::count)
{
}

std::unique_ptr<Object> CountFunction::DoEvaluate(Context& context, const std::vector<Object*>& arguments)
{
    NodeSet* nodeSet = NodeSetCast(arguments[0], this);
    return std::unique_ptr<Object>(new Number(nodeSet->Count()));
}

class FunctionLibrary
{
public:
    static FunctionLibrary& Instance();
    Function* GetFunction(FunctionKind kind) const;
    Function* GetFunction(const std::string& name) const;
    void Install(Function* function);
private:
    FunctionLibrary();
    std::vector<std::unique_ptr<Function>> functions;
    std::map<std::string, Function*> functionMap;
};

FunctionLibrary& FunctionLibrary::Instance()
{
    static FunctionLibrary instance;
    return instance;
}

FunctionLibrary::FunctionLibrary()
{
    functions.resize(static_cast<int>(FunctionKind::max));
    Install(new BooleanFunction());
    Install(new NumberFunction());
    Install(new StringFunction());
    Install(new LastFunction());
    Install(new PositionFunction());
    Install(new CountFunction());
}

void FunctionLibrary::Install(Function* function)
{
    functions[static_cast<int>(function->Kind())].reset(function);
    functionMap[function->Name()] = function;
}

Function* FunctionLibrary::GetFunction(FunctionKind kind) const
{
    Function* function = functions[static_cast<int>(kind)].get();
    if (!function)
    {
        throw std::runtime_error("error: soul::xml::xpath::FunctionLibrary::GetFunction: function '" + FunctionName(kind) + "()' not installed");;
    }
    return function;
}

Function* FunctionLibrary::GetFunction(const std::string& name) const
{
    auto it = functionMap.find(name);
    if (it != functionMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("error: soul::xml::xpath::FunctionLibrary::GetFunction: function '" + name + "()' not found");
    }
}

Function* GetFunction(FunctionKind kind)
{
    return FunctionLibrary::Instance().GetFunction(kind);
}

Function* GetFunction(const std::string& name)
{
    return FunctionLibrary::Instance().GetFunction(name);
}

} // namespace soul::xml::xpath
