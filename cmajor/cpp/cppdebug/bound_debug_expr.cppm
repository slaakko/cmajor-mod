// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.bound.debug.expr;

import std.core;

export namespace cmajor::debug {

class BoundDebugNodeVisitor;
class DebugExprNode;
class DIType;
class DIVariable;
class DebugInfo;
class Project;
class DebuggerVariable;

enum class InitializationStatus : int8_t
{
    unknown, initialized, uninitialized
};

std::string InitializationStatusStr(InitializationStatus status);

class BoundDebugNode
{
public:
    enum class Kind : int8_t
    {
        variableReferenceNode, integerLiteralNode,
        disjunctionNode, conjunctionNode, bitOrNode, bitXorNode, bitAndNode, equalNode, notEqualNode, lessEqualNode, greaterEqualNode, lessNode, greaterNode,
        shiftLeftNode, shiftRightNode, addNode, subNode, mulNode, divNode, modNode, derefNode, addrOfNode, dotNode, subscriptNode, rangeNode, parenExprNode,
        typeNode, castNode, debugExpression
    };
    BoundDebugNode(Kind kind_, DIType* type_, DebugExprNode* sourceNode_);
    Kind GetKind() const { return kind; }
    virtual ~BoundDebugNode();
    virtual void Accept(BoundDebugNodeVisitor& visitor) = 0;
    virtual bool IsDebuggerVarNode() const { return false; }
    virtual BoundDebugNode* Clone() const = 0;
    DIType* Type() const { return type; }
    void SetType(DIType* type_) { type = type_; }
    virtual bool IsTypeNode() const { return false; }
    virtual std::string ToString() const = 0;
    virtual std::string GdbExprString() const = 0;
    DebugExprNode* SourceNode() const { return sourceNode; }
private:
    Kind kind;
    DIType* type;
    DebugExprNode* sourceNode;
};

class BoundVariableReferenceNode : public BoundDebugNode
{
public:
    BoundVariableReferenceNode(DIType* type, DIVariable* variable_, InitializationStatus status_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    DIVariable* Variable() const { return variable; }
    std::string GdbExprString() const override;
    InitializationStatus Status() const { return status; }
private:
    DIVariable* variable;
    InitializationStatus status;
};

class BoundIntegerLiteralNode : public BoundDebugNode
{
public:
    BoundIntegerLiteralNode(DIType* type, int64_t value_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    std::string GdbExprString() const override;
    int64_t Value() const { return value; }
private:
    int64_t value;
};

class BoundDisjunctionNode : public BoundDebugNode
{
public:
    BoundDisjunctionNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundConjunctionNode : public BoundDebugNode
{
public:
    BoundConjunctionNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundBitOrNode : public BoundDebugNode
{
public:
    BoundBitOrNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundBitXorNode : public BoundDebugNode
{
public:
    BoundBitXorNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundBitAndNode : public BoundDebugNode
{
public:
    BoundBitAndNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundEqualNode : public BoundDebugNode
{
public:
    BoundEqualNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundNotEqualNode : public BoundDebugNode
{
public:
    BoundNotEqualNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundLessEqualNode : public BoundDebugNode
{
public:
    BoundLessEqualNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundGreaterEqualNode : public BoundDebugNode
{
public:
    BoundGreaterEqualNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundLessNode : public BoundDebugNode
{
public:
    BoundLessNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundGreaterNode : public BoundDebugNode
{
public:
    BoundGreaterNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundShiftLeftNode : public BoundDebugNode
{
public:
    BoundShiftLeftNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundShiftRightNode : public BoundDebugNode
{
public:
    BoundShiftRightNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundAddNode : public BoundDebugNode
{
public:
    BoundAddNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundSubNode : public BoundDebugNode
{
public:
    BoundSubNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundMulNode : public BoundDebugNode
{
public:
    BoundMulNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundDivNode : public BoundDebugNode
{
public:
    BoundDivNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundModNode : public BoundDebugNode
{
public:
    BoundModNode(DIType* type, BoundDebugNode* left_, BoundDebugNode* right_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Left() const { return left.get(); }
    BoundDebugNode* Right() const { return right.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> left;
    std::unique_ptr<BoundDebugNode> right;
};

class BoundDerefNode : public BoundDebugNode
{
public:
    BoundDerefNode(DIType* type, BoundDebugNode* subject_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> subject;
};

class BoundAddrOfNode : public BoundDebugNode
{
public:
    BoundAddrOfNode(DIType* type, BoundDebugNode* subject_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> subject;
};

class BoundDotNode : public BoundDebugNode
{
public:
    BoundDotNode(DIType* type_, BoundDebugNode* subject_, const std::string& member_, const std::string& gdbExprString_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    const std::string& Member() const { return member; }
    std::string GdbExprString() const override { return gdbExprString; }
private:
    DIType* type;
    std::unique_ptr<BoundDebugNode> subject;
    std::string member;
    std::string gdbExprString;
};

class BoundSubscriptNode : public BoundDebugNode
{
public:
    BoundSubscriptNode(DIType* type, BoundDebugNode* subject_, BoundDebugNode* index_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    BoundDebugNode* Index() const { return index.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> subject;
    std::unique_ptr<BoundDebugNode> index;
};

class BoundRangeNode : public BoundDebugNode
{
public:
    BoundRangeNode(DIType* elementType, BoundDebugNode* subject_, BoundDebugNode* rangeStart_, BoundDebugNode* rangeEnd_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    BoundDebugNode* RangeStart() const { return rangeStart.get(); }
    BoundDebugNode* RangeEnd() const { return rangeEnd.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> subject;
    std::unique_ptr<BoundDebugNode> rangeStart;
    std::unique_ptr<BoundDebugNode> rangeEnd;
};

class BoundParenExprNode : public BoundDebugNode
{
public:
    BoundParenExprNode(BoundDebugNode* subject_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    BoundDebugNode* Subject() const { return subject.get(); }
    std::string GdbExprString() const override;
private:
    std::unique_ptr<BoundDebugNode> subject;
};

class BoundTypeNode : public BoundDebugNode
{
public:
    BoundTypeNode(DIType* type_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    bool IsTypeNode() const override { return true; }
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    std::string GdbExprString() const override;
};

class BoundCastNode : public BoundDebugNode
{
public:
    BoundCastNode(BoundDebugNode* typeIdNode_, BoundDebugNode* exprNode_, const std::string& gdbExprString_, DebugExprNode* sourceNode);
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    std::string GdbExprString() const override;
    BoundDebugNode* TypeIdNode() const { return typeIdNode.get(); }
    BoundDebugNode* ExprNode() const { return exprNode.get(); }
private:
    std::unique_ptr<BoundDebugNode> typeIdNode;
    std::unique_ptr<BoundDebugNode> exprNode;
    std::string gdbExprString;
};

class BoundDebugExpression : public BoundDebugNode
{
public:
    BoundDebugExpression(BoundDebugNode* node_, DebugExprNode* sourceNode, bool hasContainerSubscript_, InitializationStatus status_);
    bool HasContainerSubscript() const { return hasContainerSubscript; }
    void Accept(BoundDebugNodeVisitor& visitor) override;
    std::string ToString() const override;
    BoundDebugNode* Clone() const override;
    std::string GdbExprString() const override;
    BoundDebugNode* Node() const { return node.get(); }
    InitializationStatus Status() const { return status; }
private:
    std::unique_ptr<BoundDebugNode> node;
    bool hasContainerSubscript;
    InitializationStatus status;
};

} // namespace cmajor::debug
