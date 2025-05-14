// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.expr;

import cmajor.systemx.object.value;
import std.core;

export namespace cmajor::systemx::object {

namespace expr {}

class TypedValue;
struct EvaluationContext;

enum class ExprKind : uint8_t
{
    valueExpr = 0, dollarExpr = 1, derefExpr = 2, memberExpr = 3, arrowExpr = 4, addExpr = 5, indexExpr = 6, rangeExpr = 7, castExpr = 8
};

int64_t ParseInteger(const std::string& s);

class Expr
{
public:
    Expr(ExprKind kind_);
    virtual ~Expr();
    ExprKind Kind() const { return kind; }
    virtual void Evaluate(EvaluationContext& context) = 0;
private:
    ExprKind kind;
};

class ValueExpr : public Expr
{
public:
    ValueExpr(TypedValue* value_);
    void Evaluate(EvaluationContext& context) override;
private:
    TypedValue* value;
};

class DollarExpr : public Expr
{
public:
    DollarExpr(int64_t index_);
    void Evaluate(EvaluationContext& context) override;
private:
    int64_t index;
};

class DerefExpr : public Expr
{
public:
    DerefExpr(Expr* subject_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
};

class MemberExpr : public Expr
{
public:
    MemberExpr(Expr* subject_, const std::string& id_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
    std::string id;
};

class ArrowExpr : public Expr
{
public:
    ArrowExpr(Expr* subject_, const std::string& id_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
    std::string id;
};

class AddExpr : public Expr
{
public:
    AddExpr(Expr* left_, Expr* right_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

class IndexExpr : public Expr
{
public:
    IndexExpr(Expr* subject_, int64_t index_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
    int64_t index;
};

class RangeExpr : public Expr
{
public:
    RangeExpr(Expr* subject_, int64_t index_, int64_t count_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
    int64_t index;
    int64_t count;
};

class CastExpr : public Expr
{
public:
    CastExpr(Expr* subject_, Type* destType_);
    void Evaluate(EvaluationContext& context) override;
private:
    std::unique_ptr<Expr> subject;
    Type* destType;
};

} // namespace cmajor::systemx::object
