// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.xpath.expr;

import soul.xml.xpath.function;
import soul.xml.node.operation;
import soul.xml.dom;

namespace soul::xml::xpath::expr {

std::string OperatorStr(Operator op)
{
    switch (op)
    {
        case Operator::or_: return "or";
        case Operator::and_: return "and";
        case Operator::equal: return "equal";
        case Operator::notEqual: return "not-equal";
        case Operator::less: return "less";
        case Operator::greater: return "greater";
        case Operator::lessOrEqual: return "less-or-equal";
        case Operator::greaterOrEqual: return "greater-or-equal";
        case Operator::plus: return "plus";
        case Operator::minus: return "minus";
        case Operator::mul: return "mul";
        case Operator::div: return "div";
        case Operator::mod: return "mod";
        case Operator::union_: return "union";
        case Operator::slash: return "slash";
        case Operator::slashSlash: return "slash-slash";
        case Operator::parens: return "parens";
    }
    return "<unknown operator>";
}

std::string ExprKindStr(ExprKind exprKind)
{
    switch (exprKind)
    {
        case ExprKind::unaryExpr:  return "unary-expr";
        case ExprKind::binaryExpr:  return "binary-expr";
        case ExprKind::root:  return "root";
        case ExprKind::filterExpr:  return "filter";
        case ExprKind::locationStepExpr:  return "location-step";
        case ExprKind::variableReference:  return "variable-reference";
        case ExprKind::literal:  return "literal";
        case ExprKind::numberExpr:  return "number";
        case ExprKind::functionCall:  return "function-call";
    }
    return "<unknown expression>";
}

class NodeSelectionOperation : public soul::xml::NodeOperation
{
public:
    NodeSelectionOperation(NodeTest* nodeTest_, NodeSet& nodeSet_, soul::xml::Axis axis_);
    void Apply(soul::xml::Node* node) override;
private:
    NodeTest* nodeTest;
    NodeSet& nodeSet;
    soul::xml::Axis axis;
};

NodeSelectionOperation::NodeSelectionOperation(NodeTest* nodeTest_, NodeSet& nodeSet_, soul::xml::Axis axis_) : nodeTest(nodeTest_), nodeSet(nodeSet_), axis(axis_)
{
}

void NodeSelectionOperation::Apply(soul::xml::Node* node)
{
    if (nodeTest->Select(node, axis))
    {
        nodeSet.Add(node);
    }
}

NodeSet* NodeSetCast(Object* object)
{
    if (object->IsNodeSet())
    {
        return static_cast<NodeSet*>(object);
    }
    else
    {
        throw std::runtime_error("node-set object expected");
    }
}

Boolean* BooleanCast(Object* object)
{
    if (object->IsBoolean())
    {
        return static_cast<Boolean*>(object);
    }
    else
    {
        throw std::runtime_error("Boolean object expected");
    }
}

Number* NumberCast(Object* object)
{
    if (object->IsNumber())
    {
        return static_cast<Number*>(object);
    }
    else
    {
        throw std::runtime_error("number object expected");
    }
}

bool Compare(const std::string& leftStr, const std::string& rightStr, Operator op)
{
    switch (op)
    {
        case Operator::equal:
        {
            return leftStr == rightStr;
        }
        case Operator::notEqual:
        {
            return leftStr != rightStr;
        }
        case Operator::less:
        {
            return leftStr < rightStr;
        }
        case Operator::greater:
        {
            return leftStr > rightStr;
        }
        case Operator::lessOrEqual:
        {
            return leftStr <= rightStr;
        }
        case Operator::greaterOrEqual:
        {
            return leftStr >= rightStr;
        }
    }
    return false;
}

bool Compare(Number* left, Number* right, Operator op)
{
    switch (op)
    {
        case Operator::equal:
        {
            return left->Value() == right->Value();
        }
        case Operator::notEqual:
        {
            return left->Value() != right->Value();
        }
        case Operator::less:
        {
            return left->Value() < right->Value();
        }
        case Operator::greater:
        {
            return left->Value() > right->Value();
        }
        case Operator::lessOrEqual:
        {
            return left->Value() <= right->Value();
        }
        case Operator::greaterOrEqual:
        {
            return left->Value() >= right->Value();
        }
    }
    return false;
}

bool Compare(Boolean* left, Boolean* right, Operator op)
{
    switch (op)
    {
        case Operator::equal:
        {
            return left->Value() == right->Value();
        }
        case Operator::notEqual:
        {
            return left->Value() != right->Value();
        }
        case Operator::less:
        {
            return static_cast<int>(left->Value()) < static_cast<int>(right->Value());
        }
        case Operator::greater:
        {
            return static_cast<int>(left->Value()) > static_cast<int>(right->Value());
        }
        case Operator::lessOrEqual:
        {
            return static_cast<int>(left->Value()) <= static_cast<int>(right->Value());
        }
        case Operator::greaterOrEqual:
        {
            return static_cast<int>(left->Value()) >= static_cast<int>(right->Value());
        }
    }
    return false;
}

std::unique_ptr<Object> ToNumber(soul::xml::xpath::Object* object, Context& context)
{
    Function* numberFunction = GetFunction(FunctionKind::number);
    std::vector<Object*> args(1, object);
    return numberFunction->Evaluate(context, args);
}

std::unique_ptr<Object> ToNumber(soul::xml::Node* node, Context& context)
{
    std::unique_ptr<Object> stringValue(new String(StringValue(node)));
    return ToNumber(stringValue.get(), context);
}

std::unique_ptr<Object> ToBoolean(soul::xml::xpath::Object* object, Context& context)
{
    Function* booleanFunction = GetFunction(FunctionKind::boolean);
    std::vector<Object*> args(1, object);
    return booleanFunction->Evaluate(context, args);
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateUnaryMinusExpr(Expr* operand, Context& context)
{
    std::unique_ptr<Object> operandResult = operand->Evaluate(context);
    std::unique_ptr<Object> numberResult = ToNumber(operandResult.get(), context);
    Number* number = NumberCast(numberResult.get());
    return std::unique_ptr<Object>(new Number(-number->Value()));
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateParenExpr(Expr* operand, Context& context)
{
    return operand->Evaluate(context);
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateOrExpr(Expr* left, Expr* right, Context& context)
{
    Function* booleanFunction = GetFunction(FunctionKind::boolean);
    std::unique_ptr<soul::xml::xpath::Object> leftResult = left->Evaluate(context);
    std::vector<Object*> leftArgs(1, leftResult.get());
    std::unique_ptr<soul::xml::xpath::Object> leftBooleanResult = booleanFunction->Evaluate(context, leftArgs);
    Boolean* leftAsBoolean = BooleanCast(leftBooleanResult.get());
    if (leftAsBoolean->Value())
    {
        return leftBooleanResult;
    }
    std::unique_ptr<soul::xml::xpath::Object> rightResult = right->Evaluate(context);
    std::vector<Object*> rightArgs(1, rightResult.get());
    return booleanFunction->Evaluate(context, rightArgs);
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateAndExpr(Expr* left, Expr* right, Context& context)
{
    Function* booleanFunction = GetFunction(FunctionKind::boolean);
    std::unique_ptr<soul::xml::xpath::Object> leftResult = left->Evaluate(context);
    std::vector<Object*> leftArgs(1, leftResult.get());
    std::unique_ptr<soul::xml::xpath::Object> leftBooleanResult = booleanFunction->Evaluate(context, leftArgs);
    Boolean* leftAsBoolean = BooleanCast(leftBooleanResult.get());
    if (!leftAsBoolean->Value())
    {
        return leftBooleanResult;
    }
    std::unique_ptr<soul::xml::xpath::Object> rightResult = right->Evaluate(context);
    std::vector<Object*> rightArgs(1, rightResult.get());
    return booleanFunction->Evaluate(context, rightArgs);
}

std::unique_ptr<soul::xml::xpath::Object> Compare(NodeSet* left, NodeSet* right, Operator op)
{
    int n = left->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* leftNode = left->GetNode(i);
        std::string leftStringValue = StringValue(leftNode);
        int m = right->Count();
        for (int j = 0; j < m; ++j)
        {
            soul::xml::Node* rightNode = right->GetNode(j);
            std::string rightStringValue = StringValue(rightNode);
            if (Compare(leftStringValue, rightStringValue, op))
            {
                return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(true));
            }
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(NodeSet* left, Boolean* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftAsBoolean = ToBoolean(left, context);
    Boolean* leftBoolean = BooleanCast(leftAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(leftBoolean, right, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Boolean* left, NodeSet* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> rightAsBoolean = ToBoolean(right, context);
    Boolean* rightBoolean = BooleanCast(rightAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(left, rightBoolean, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Boolean* left, Number* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> rightAsBoolean = ToBoolean(right, context);
    Boolean* rightBoolean = BooleanCast(rightAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(left, rightBoolean, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Number* left, Boolean* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftAsBoolean = ToBoolean(left, context);
    Boolean* leftBoolean = BooleanCast(leftAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(leftBoolean, right, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Boolean* left, String* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> rightAsBoolean = ToBoolean(right, context);
    Boolean* rightBoolean = BooleanCast(rightAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(left, rightBoolean, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(String* left, Boolean* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftAsBoolean = ToBoolean(left, context);
    Boolean* leftBoolean = BooleanCast(leftAsBoolean.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(leftBoolean, right, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(NodeSet* left, Number* right, Operator op, Context& context)
{
    int n = left->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* leftNode = left->GetNode(i);
        std::unique_ptr<soul::xml::xpath::Object> leftAsNumber = ToNumber(leftNode, context);
        Number* leftNumber = NumberCast(leftAsNumber.get());
        if (Compare(leftNumber, right, op))
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(true));
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Number* left, NodeSet* right, Operator op, Context& context)
{
    int n = right->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* rightNode = right->GetNode(i);
        std::unique_ptr<soul::xml::xpath::Object> rightAsNumber = ToNumber(rightNode, context);
        Number* rightNumber = NumberCast(rightAsNumber.get());
        if (Compare(left, rightNumber, op))
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(true));
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(NodeSet* left, String* right, Operator op, Context& context)
{
    int n = left->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* leftNode = left->GetNode(i);
        std::string leftStringValue = StringValue(leftNode);
        if (Compare(leftStringValue, right->Value(), op))
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(true));
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(String* left, NodeSet* right, Operator op, Context& context)
{
    int n = right->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* rightNode = right->GetNode(i);
        std::string rightStringValue = StringValue(rightNode);
        if (Compare(left->Value(), rightStringValue, op))
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(true));
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Number* left, String* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> rightAsNumber = ToNumber(right, context);
    Number* rightNumber = NumberCast(rightAsNumber.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(left, rightNumber, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(String* left, Number* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftAsNumber = ToNumber(left, context);
    Number* leftNumber = NumberCast(leftAsNumber.get());
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(Compare(leftNumber, right, op)));
}

std::unique_ptr<soul::xml::xpath::Object> Compare(Expr* left, Expr* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftOperand = left->Evaluate(context);
    std::unique_ptr<soul::xml::xpath::Object> rightOperand = right->Evaluate(context);
    switch (leftOperand->Kind())
    {
        case ObjectKind::nodeSet:
        {
            NodeSet* leftNodeSet = static_cast<NodeSet*>(leftOperand.get());
            switch (rightOperand->Kind())
            {
                case ObjectKind::nodeSet:
                {
                    NodeSet* rightNodeSet = static_cast<NodeSet*>(rightOperand.get());
                    return Compare(leftNodeSet, rightNodeSet, op);
                }
                case ObjectKind::boolean:
                {
                    Boolean* rightBoolean = static_cast<Boolean*>(rightOperand.get());
                    return Compare(leftNodeSet, rightBoolean, op, context);
                }
                case ObjectKind::number:
                {
                    Number* rightNumber = static_cast<Number*>(rightOperand.get());
                    return Compare(leftNodeSet, rightNumber, op, context);
                }
                case ObjectKind::string:
                {
                    String* rightString = static_cast<String*>(rightOperand.get());
                    return Compare(leftNodeSet, rightString, op, context);
                }
            }
            break;
        }
        case ObjectKind::boolean:
        {
            Boolean* leftBoolean = static_cast<Boolean*>(leftOperand.get());
            switch (rightOperand->Kind())
            {
                case ObjectKind::nodeSet:
                {
                    NodeSet* rightNodeSet = static_cast<NodeSet*>(rightOperand.get());
                    return Compare(leftBoolean, rightNodeSet, op, context);
                }
                case ObjectKind::boolean:
                {
                    Boolean* rightBoolean = static_cast<Boolean*>(rightOperand.get());
                    return std::unique_ptr<Object>(new Boolean(Compare(leftBoolean, rightBoolean, op)));
                }
                case ObjectKind::number:
                {
                    Number* rightNumber = static_cast<Number*>(rightOperand.get());
                    return Compare(leftBoolean, rightNumber, op, context);
                }
                case ObjectKind::string:
                {
                    String* rightString = static_cast<String*>(rightOperand.get());
                    return Compare(leftBoolean, rightString, op, context);
                }
            }
            break;
        }
        case ObjectKind::number:
        {
            Number* leftNumber = static_cast<Number*>(leftOperand.get());
            switch (rightOperand->Kind())
            {
                case ObjectKind::nodeSet:
                {
                    NodeSet* rightNodeSet = static_cast<NodeSet*>(rightOperand.get());
                    return Compare(leftNumber, rightNodeSet, op, context);
                }
                case ObjectKind::boolean:
                {
                    Boolean* rightBoolean = static_cast<Boolean*>(rightOperand.get());
                    return Compare(leftNumber, rightBoolean, op, context);
                }
                case ObjectKind::number:
                {
                    Number* rightNumber = static_cast<Number*>(rightOperand.get());
                    return std::unique_ptr<Object>(new Boolean(Compare(leftNumber, rightNumber, op)));
                }
                case ObjectKind::string:
                {
                    String* rightString = static_cast<String*>(rightOperand.get());
                    return Compare(leftNumber, rightString, op, context);
                }
            }
            break;
        }
        case ObjectKind::string:
        {
            String* leftString = static_cast<String*>(leftOperand.get());
            switch (rightOperand->Kind())
            {
                case ObjectKind::nodeSet:
                {
                    NodeSet* rightNodeSet = static_cast<NodeSet*>(rightOperand.get());
                    return Compare(leftString, rightNodeSet, op, context);
                }
                case ObjectKind::boolean:
                {
                    Boolean* rightBoolean = static_cast<Boolean*>(rightOperand.get());
                    return Compare(leftString, rightBoolean, op, context);
                }
                case ObjectKind::number:
                {
                    Number* rightNumber = static_cast<Number*>(rightOperand.get());
                    return Compare(leftString, rightNumber, op, context);
                }
                case ObjectKind::string:
                {
                    String* rightString = static_cast<String*>(rightOperand.get());
                    return std::unique_ptr<Object>(new Boolean(Compare(leftString->Value(), rightString->Value(), op)));
                }
            }
            break;
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(new Boolean(false));
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateArithmeticOp(Expr* left, Expr* right, Operator op, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftOperand = left->Evaluate(context);
    std::unique_ptr<soul::xml::xpath::Object> leftAsNumber = ToNumber(leftOperand.get(), context);
    Number* leftNumber = NumberCast(leftAsNumber.get());
    std::unique_ptr<soul::xml::xpath::Object> rightOperand = right->Evaluate(context);
    std::unique_ptr<soul::xml::xpath::Object> rightAsNumber = ToNumber(rightOperand.get(), context);
    Number* rightNumber = NumberCast(rightAsNumber.get());
    switch (op)
    {
        case Operator::plus:
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Number(leftNumber->Value() + rightNumber->Value()));
        }
        case Operator::minus:
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Number(leftNumber->Value() - rightNumber->Value()));
        }
        case Operator::mul:
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Number(leftNumber->Value() * rightNumber->Value()));
        }
        case Operator::div:
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Number(leftNumber->Value() / rightNumber->Value()));
        }
        case Operator::mod:
        {
            return std::unique_ptr<soul::xml::xpath::Object>(new Number(std::remainder(leftNumber->Value(), rightNumber->Value())));
        }
    }
    throw std::runtime_error("arithmetic binary operator expected");
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateUnionExpr(Expr* left, Expr* right, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftOperand = left->Evaluate(context);
    NodeSet* leftNodeSet = NodeSetCast(leftOperand.get());
    std::unique_ptr<soul::xml::xpath::Object> rightOperand = right->Evaluate(context);
    NodeSet* rightNodeSet = NodeSetCast(rightOperand.get());
    std::unique_ptr<soul::xml::xpath::NodeSet> result(new soul::xml::xpath::NodeSet());
    int n = leftNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        result->Add(leftNodeSet->GetNode(i));
    }
    int m = rightNodeSet->Count();
    for (int j = 0; j < m; ++j)
    {
        result->Add(rightNodeSet->GetNode(j));
    }
    return std::unique_ptr<soul::xml::xpath::Object>(result.release()); 
}

std::unique_ptr<soul::xml::xpath::Object> EvaluateCombineStepExpr(Expr* left, Expr* right, Context& context)
{
    std::unique_ptr<soul::xml::xpath::Object> leftOperand = left->Evaluate(context);
    NodeSet* leftNodeSet = NodeSetCast(leftOperand.get());
    std::unique_ptr<soul::xml::xpath::NodeSet> result(new soul::xml::xpath::NodeSet());
    int n = leftNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* leftNode = leftNodeSet->GetNode(i);
        Context rightContext(leftNode, i + 1, n);
        std::unique_ptr<soul::xml::xpath::Object> rightOperand = right->Evaluate(rightContext);
        NodeSet* rightNodeSet = NodeSetCast(rightOperand.get());
        int m = rightNodeSet->Count();
        for (int j = 0; j < m; ++j)
        {
            soul::xml::Node* rightNode = rightNodeSet->GetNode(j);
            result->Add(rightNode);
        }
    }
    return std::unique_ptr<soul::xml::xpath::Object>(result.release()); 
}

Expr::Expr(ExprKind kind_) : kind(kind_)
{
}

Expr::~Expr()
{
}

void Expr::SetStr(const std::string& str_)
{
    str = str_;
}

soul::xml::Element* Expr::ToXmlElement() const
{
    soul::xml::Element* element = soul::xml::MakeElement("expr");
    element->SetAttribute("kind", ExprKindStr(kind));
    return element;
}

UnaryExpr::UnaryExpr(Operator op_, Expr* operand_) : Expr(ExprKind::unaryExpr), op(op_), operand(operand_)
{
}

std::unique_ptr<soul::xml::xpath::Object> UnaryExpr::Evaluate(Context& context) const
{
    switch (op)
    {
        case Operator::minus:
        {
            return EvaluateUnaryMinusExpr(operand.get(), context);
        }
        case Operator::parens:
        {
            return EvaluateParenExpr(operand.get(), context);
        }
    }
    throw std::runtime_error("unary minus or parenthesis operator expected");
}

soul::xml::Element* UnaryExpr::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("operator", OperatorStr(op));
    soul::xml::Element* child = operand->ToXmlElement();
    element->AppendChild(child);
    return element;
}

BinaryExpr::BinaryExpr(Operator op_, Expr* left_, Expr* right_) : Expr(ExprKind::binaryExpr), op(op_), left(left_), right(right_)
{
}

std::unique_ptr<soul::xml::xpath::Object> BinaryExpr::Evaluate(Context& context) const
{
    switch (op)
    {
        case Operator::or_:
        {
            return EvaluateOrExpr(left.get(), right.get(), context);
        }
        case Operator::and_:
        {
            return EvaluateAndExpr(left.get(), right.get(), context);
        }
        case Operator::equal:
        {
            return Compare(left.get(), right.get(), Operator::equal, context);
        }
        case Operator::notEqual:
        {
            return Compare(left.get(), right.get(), Operator::notEqual, context);
        }
        case Operator::less:
        {
            return Compare(left.get(), right.get(), Operator::less, context);
        }
        case Operator::greater:
        {
            return Compare(left.get(), right.get(), Operator::greater, context);
        }
        case Operator::lessOrEqual:
        {
            return Compare(left.get(), right.get(), Operator::lessOrEqual, context);
        }
        case Operator::greaterOrEqual:
        {
            return Compare(left.get(), right.get(), Operator::greaterOrEqual, context);
        }
        case Operator::plus:
        {
            return EvaluateArithmeticOp(left.get(), right.get(), Operator::plus, context);
        }
        case Operator::minus:
        {
            return EvaluateArithmeticOp(left.get(), right.get(), Operator::minus, context);
        }
        case Operator::mul:
        {
            return EvaluateArithmeticOp(left.get(), right.get(), Operator::mul, context);
        }
        case Operator::div:
        {
            return EvaluateArithmeticOp(left.get(), right.get(), Operator::div, context);
        }
        case Operator::mod:
        {
            return EvaluateArithmeticOp(left.get(), right.get(), Operator::mod, context);
        }
        case Operator::union_:
        {
            return EvaluateUnionExpr(left.get(), right.get(), context);
        }
        case Operator::slash:
        {
            return EvaluateCombineStepExpr(left.get(), right.get(), context);
        }
    }
    throw std::runtime_error("binary operator expected");
}

soul::xml::Element* BinaryExpr::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("operator", OperatorStr(op));
    soul::xml::Element* leftElement = left->ToXmlElement();
    element->AppendChild(leftElement);
    soul::xml::Element* rightElement = right->ToXmlElement();
    element->AppendChild(rightElement);
    return element;
}

Root::Root() : Expr(ExprKind::root)
{
}

std::unique_ptr<soul::xml::xpath::Object> Root::Evaluate(Context& context) const
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet(new soul::xml::xpath::NodeSet());
    if (context.Node()->IsDocumentNode())
    {
        nodeSet->Add(context.Node());
    }
    else if (context.Node()->OwnerDocument())
    {
        nodeSet->Add(context.Node()->OwnerDocument());
    }
    return std::unique_ptr<soul::xml::xpath::Object>(nodeSet.release());
}

FilterExpr::FilterExpr(Expr* subject_, Expr* predicate_) : Expr(ExprKind::filterExpr), subject(subject_), predicate(predicate_)
{
}

std::unique_ptr<soul::xml::xpath::Object> FilterExpr::Evaluate(Context& context) const
{
    std::unique_ptr<soul::xml::xpath::Object> subjectResult = subject->Evaluate(context);
    if (subjectResult->IsNodeSet())
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet(static_cast<soul::xml::xpath::NodeSet*>(subjectResult.release()));
        std::unique_ptr<soul::xml::xpath::NodeSet> filteredNodeSet(new soul::xml::xpath::NodeSet());
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            Context filterContext(node, i + 1, n);
            std::unique_ptr<soul::xml::xpath::Object> result = predicate->Evaluate(filterContext);
            bool include = false;
            if (result->IsNumber())
            {
                Number* number = NumberCast(result.get());
                if (number->Value() == filterContext.Pos())
                {
                    include = true;
                }
            }
            else
            {
                std::unique_ptr<soul::xml::xpath::Object> booleanResult = ToBoolean(result.get(), filterContext);
                Boolean* boolean = BooleanCast(booleanResult.get());
                include = boolean->Value();
            }
            if (include)
            {
                filteredNodeSet->Add(node);
            }
        }
        std::swap(nodeSet, filteredNodeSet);
        return std::unique_ptr<soul::xml::xpath::Object>(nodeSet.release());
    }
    else
    {
        throw std::runtime_error("node-set expected");
    }
}

soul::xml::Element* FilterExpr::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    soul::xml::Element* subjectElement = subject->ToXmlElement();
    element->AppendChild(subjectElement);
    soul::xml::Element* predicateElement = predicate->ToXmlElement();
    element->AppendChild(predicateElement);
    return element;
}

std::string NodeTestStr(NodeTestKind nodeTest)
{
    switch (nodeTest)
    {
        case NodeTestKind::piLiteralTest: return "pi-literal-test";
        case NodeTestKind::commentNodeTest: return "comment-node-test";
        case NodeTestKind::textNodeTest: return "text-node-test";
        case NodeTestKind::piNodeTest: return "pi-node-test";
        case NodeTestKind::anyNodeTest: return "any-node-test";
        case NodeTestKind::principalNodeTest: return "principal-node-test";
        case NodeTestKind::prefixTest: return "prefix-node-test";
        case NodeTestKind::nameTest: return "name-node-test";
    }
    return "<unknown node-test>";
}

NodeTest::NodeTest(NodeTestKind kind_) : kind(kind_)
{
}

NodeTest::~NodeTest()
{
}

bool NodeTest::Select(soul::xml::Node* node, soul::xml::Axis axis) const
{
    switch (kind)
    {
        case NodeTestKind::commentNodeTest:
        {
            return node->IsCommentNode();
        }
        case NodeTestKind::textNodeTest:
        {
            return node->IsTextNode();
        }
        case NodeTestKind::piNodeTest:
        {
            return node->IsProcessingInstructionNode();
        }
        case NodeTestKind::anyNodeTest:
        {
            return true;
        }
        case NodeTestKind::principalNodeTest:
        {
            if (axis == soul::xml::Axis::attribute)
            {
                return node->IsAttributeNode();
            }
            else if (axis == soul::xml::Axis::ns)
            {
                return false; // todo namespace axis not supported
            }
            else
            {
                return node->IsElementNode();
            }
        }
    }
    return false;
}

soul::xml::Element* NodeTest::ToXmlElement() const
{
    soul::xml::Element* element = soul::xml::MakeElement("node-test");
    element->SetAttribute("kind", NodeTestStr(kind));
    return element;
}

PILiteralNodeTest::PILiteralNodeTest(Literal* literal_) : NodeTest(NodeTestKind::piLiteralTest), literal(literal_)
{
}

bool PILiteralNodeTest::Select(soul::xml::Node* node, soul::xml::Axis axis) const
{
    if (node->IsProcessingInstructionNode())
    {
        soul::xml::ProcessingInstruction* pi = static_cast<soul::xml::ProcessingInstruction*>(node);
        if (pi->Target() == literal->Value())
        {
            return true;
        }
    }
    return false;
}

soul::xml::Element* PILiteralNodeTest::ToXmlElement() const
{
    soul::xml::Element* element = NodeTest::ToXmlElement();
    element->AppendChild(literal->ToXmlElement());
    return element;
}

PrefixNodeTest::PrefixNodeTest(const std::string& prefix_) : NodeTest(NodeTestKind::prefixTest), prefix(prefix_)
{
}

bool PrefixNodeTest::Select(soul::xml::Node* node, soul::xml::Axis axis) const
{
    if (axis == soul::xml::Axis::attribute)
    {
        if (node->IsAttributeNode())
        {
            if (node->Prefix() == prefix)
            {
                return true;
            }
        }
    }
    else if (axis == soul::xml::Axis::ns)
    {
        return false; // todo namespace axis not supported
    }
    else if (node->IsElementNode())
    {
        if (node->Prefix() == prefix)
        {
            return true; 
        }
    }
    return false;
}

soul::xml::Element* PrefixNodeTest::ToXmlElement() const
{
    soul::xml::Element* element = NodeTest::ToXmlElement();
    element->SetAttribute("prefix", prefix);
    return element;
}

NameNodeTest::NameNodeTest(const std::string& name_) : NodeTest(NodeTestKind::nameTest), name(name_)
{
}

bool NameNodeTest::Select(soul::xml::Node* node, soul::xml::Axis axis) const
{
    if (axis == soul::xml::Axis::attribute)
    {
        if (node->IsAttributeNode())
        {
            if (node->Name() == name)
            {
                return true;
            }
        }
    }
    else if (axis == soul::xml::Axis::ns)
    {
        return false; // todo namespace axis not supported
    }
    else
    {
        if (node->IsElementNode())
        {
            if (node->Name() == name)
            {
                return true;
            }
        }
    }
    return false;
}

soul::xml::Element* NameNodeTest::ToXmlElement() const
{
    soul::xml::Element* element = NodeTest::ToXmlElement();
    element->SetAttribute("name", name);
    return element;
}

LocationStepExpr::LocationStepExpr(soul::xml::Axis axis_, NodeTest* nodeTest_) : Expr(ExprKind::locationStepExpr), axis(axis_), nodeTest(nodeTest_)
{
}

void LocationStepExpr::AddPredicate(Expr* predicate)
{
    predicates.push_back(std::unique_ptr<Expr>(predicate));
}

std::unique_ptr<soul::xml::xpath::Object> LocationStepExpr::Evaluate(Context& context) const
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet(new soul::xml::xpath::NodeSet());
    NodeSelectionOperation selectNodes(nodeTest.get(), *nodeSet, axis);
    context.Node()->Walk(selectNodes, axis);
    for (const auto& predicate : predicates)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> filteredNodeSet(new soul::xml::xpath::NodeSet());
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            Context filterContext(node, i + 1, n);
            std::unique_ptr<soul::xml::xpath::Object> predicateResult = predicate->Evaluate(filterContext);
            bool include = false;
            if (predicateResult->IsNumber())
            {
                Number* number = static_cast<Number*>(predicateResult.get());
                if (number->Value() == filterContext.Pos())
                {
                    include = true;
                }
            }
            else
            {
                std::unique_ptr<Object> booleanResult = ToBoolean(predicateResult.get(), filterContext);
                Boolean* boolean = BooleanCast(booleanResult.get());
                include = boolean->Value();
            }
            if (include)
            {
                filteredNodeSet->Add(node);
            }
        }
        std::swap(nodeSet, filteredNodeSet);
    }
    return std::unique_ptr<soul::xml::xpath::Object>(nodeSet.release());
}

soul::xml::Element* LocationStepExpr::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("axis", soul::xml::AxisName(axis));
    element->AppendChild(nodeTest->ToXmlElement());
    soul::xml::Element* predicatesElement = MakeElement("predicates");
    for (const auto& predicate : predicates)
    {
        predicatesElement->AppendChild(predicate->ToXmlElement());
    }
    element->AppendChild(predicatesElement);
    return element;
}

VariableReference::VariableReference(const std::string& variableName_) : Expr(ExprKind::variableReference), variableName(variableName_)
{
}

std::unique_ptr<soul::xml::xpath::Object> VariableReference::Evaluate(Context& context) const
{
    throw std::runtime_error("error: variable references not implemented");
}

soul::xml::Element* VariableReference::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("variable-name", variableName);
    return element;
}

Literal::Literal(const std::string& value_) : Expr(ExprKind::literal), value(value_)
{
}

std::unique_ptr<soul::xml::xpath::Object> Literal::Evaluate(Context& context) const
{
    return std::unique_ptr<soul::xml::xpath::Object>(new String(value));
}

soul::xml::Element* Literal::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("value", value);
    return element;
}

NumberExpr::NumberExpr(double value_) : Expr(ExprKind::numberExpr), value(value_)
{
}

std::unique_ptr<soul::xml::xpath::Object> NumberExpr::Evaluate(Context& context) const
{
    return std::unique_ptr<soul::xml::xpath::Object>(new Number(value));
}

soul::xml::Element* NumberExpr::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("value", std::to_string(value));
    return element;
}

FunctionCall::FunctionCall(const std::string& functionName_) : Expr(ExprKind::functionCall), functionName(functionName_)
{
}

void FunctionCall::AddArgument(Expr* argument)
{
    arguments.push_back(std::unique_ptr<Expr>(argument));
}

std::unique_ptr<soul::xml::xpath::Object> FunctionCall::Evaluate(Context& context) const
{
    Function* function = GetFunction(functionName);
    std::vector<std::unique_ptr<Object>> ownedArgs;
    std::vector<Object*> args;
    for (const auto& arg : arguments)
    {
        auto argValue = arg->Evaluate(context);
        args.push_back(argValue.get());
        ownedArgs.push_back(std::move(argValue));
    }
    return function->Evaluate(context, args);
}

soul::xml::Element* FunctionCall::ToXmlElement() const
{
    soul::xml::Element* element = Expr::ToXmlElement();
    element->SetAttribute("function-name", functionName);
    soul::xml::Element* argumentsElement = soul::xml::MakeElement("arguments");
    for (const auto& arg : arguments)
    {
        argumentsElement->AppendChild(arg->ToXmlElement());
    }
    element->AppendChild(argumentsElement);
    return element;
}

Expr* MakeSlashSlashExpr(Expr* left, Expr* right)
{
    return new BinaryExpr(Operator::slash, left,
        new BinaryExpr(Operator::slash, 
            new LocationStepExpr(soul::xml::Axis::descendantOrSelf, new NodeTest(NodeTestKind::anyNodeTest)),
            right));
}

} // namespace soul::xml::xpath::expr
