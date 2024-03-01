// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xpath.expr;

import std.core;
import soul.xml.xpath.object;
import soul.xml.xpath.context;
import soul.xml.axis;
import soul.xml.element;

export namespace soul::xml::xpath::expr {

enum class Operator
{
    or_, and_, equal, notEqual, less, greater, lessOrEqual, greaterOrEqual, plus, minus, mul, div, mod, union_, slash, slashSlash, parens
};

std::string OperatorStr(Operator op);

enum class ExprKind
{
    unaryExpr, binaryExpr, root, filterExpr, locationStepExpr, variableReference, literal, numberExpr, functionCall
};

std::string ExprKindStr(ExprKind exprKind);

class Expr
{
public:
    Expr(ExprKind kind_);
    virtual ~Expr();
    ExprKind Kind() const { return kind; }
    const std::string& Str() const { return str; }
    void SetStr(const std::string& str_);
    virtual std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const = 0;
    virtual soul::xml::Element* ToXmlElement() const;
private:
    ExprKind kind;
    std::string str;
};

class UnaryExpr : public Expr
{
public:
    UnaryExpr(Operator op_, Expr* operand_);
    Operator Op() const { return op; }
    Expr* Operand() const { return operand.get(); }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    Operator op;
    std::unique_ptr<Expr> operand;
};

class BinaryExpr : public Expr
{
public:
    BinaryExpr(Operator op_, Expr* left_, Expr* right_);
    Operator Op() const { return op; }
    Expr* Left() const { return left.get(); }
    Expr* Right() const { return right.get(); }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    Operator op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

class Root : public Expr
{
public:
    Root();
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
};

class FilterExpr : public Expr
{
public:
    FilterExpr(Expr* subject_, Expr* predicate_);
    Expr* Subject() const { return subject.get(); }
    Expr* Predicate() const { return predicate.get(); }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::unique_ptr<Expr> subject;
    std::unique_ptr<Expr> predicate;
};

enum class NodeTestKind
{
    piLiteralTest, commentNodeTest, textNodeTest, piNodeTest, anyNodeTest, principalNodeTest, prefixTest, nameTest
};

std::string NodeTestStr(NodeTestKind nodeTest);

class NodeTest
{
public:
    NodeTest(NodeTestKind kind_);
    virtual ~NodeTest();
    NodeTestKind Kind() const { return kind; }
    virtual bool Select(soul::xml::Node* node, soul::xml::Axis axis) const;
    virtual soul::xml::Element* ToXmlElement() const;
private:
    NodeTestKind kind;
};

class Literal;

class PILiteralNodeTest : public NodeTest
{
public:
    PILiteralNodeTest(Literal* literal_);
    Literal* Lit() const { return literal.get(); }
    bool Select(soul::xml::Node* node, soul::xml::Axis axis) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::unique_ptr<Literal> literal;
};

class PrefixNodeTest : public NodeTest
{
public:
    PrefixNodeTest(const std::string& prefix_);
    const std::string& Prefix() const { return prefix; }
    bool Select(soul::xml::Node* node, soul::xml::Axis axis) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string prefix;
};

class NameNodeTest : public NodeTest
{
public:
    NameNodeTest(const std::string& name_);
    const std::string& Name() const { return name; }
    bool Select(soul::xml::Node* node, soul::xml::Axis axis) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string name;
};

class LocationStepExpr : public Expr
{
public:
    LocationStepExpr(soul::xml::Axis axis_, NodeTest* nodeTest_);
    NodeTest* GetNodeTest() const { return nodeTest.get(); }
    void AddPredicate(Expr* predicate);
    const std::vector<std::unique_ptr<Expr>>& Predicates() const { return predicates; }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    soul::xml::Axis axis;
    std::unique_ptr<NodeTest> nodeTest;
    std::vector<std::unique_ptr<Expr>> predicates;
};

class VariableReference : public Expr
{
public:
    VariableReference(const std::string& variableName_);
    const std::string& VariableName() const { return variableName; }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string variableName;
};

class Literal : public Expr
{
public:
    Literal(const std::string& value_);
    const std::string& Value() const { return value; }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string value;
};

class NumberExpr : public Expr
{
public:
    NumberExpr(double value_);
    double Value() const { return value; }
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    double value;
};

class FunctionCall : public Expr
{
public:
    FunctionCall(const std::string& functionName_);
    const std::string& FunctionName() const { return functionName; }
    void AddArgument(Expr* argument);
    std::unique_ptr<soul::xml::xpath::Object> Evaluate(Context& context) const override;
    soul::xml::Element* ToXmlElement() const override;
private:
    std::string functionName;
    std::vector<std::unique_ptr<Expr>> arguments;
};

Expr* MakeSlashSlashExpr(Expr* left, Expr* right);

} // namespace soul::xml::xpath::expr
