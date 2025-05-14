module cmajor.systemx.object.expr.parser.rules;

namespace cmajor::systemx::object::expr::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 8093789852685828097, "ExprParser.Expr" },
        { 8093789852685828098, "ExprParser.UnaryExpr" },
        { 8093789852685828099, "ExprParser.PostfixExpr" },
        { 8093789852685828100, "ExprParser.PrimaryExpr" },
        { 8093789852685828101, "ExprParser.DollarExpr" }
    };
    return &ruleNameMap;
}

} // cmajor::systemx::object::expr::parser::rules
