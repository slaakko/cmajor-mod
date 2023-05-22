module soul.xml.xpath.parser.rules;

namespace soul::xml::xpath::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 6774979419569127425, "XPathParser.Expr" },
        { 6774979419569127426, "XPathParser.OrExpr" },
        { 6774979419569127427, "XPathParser.AndExpr" },
        { 6774979419569127428, "XPathParser.EqualityExpr" },
        { 6774979419569127429, "XPathParser.RelationalExpr" },
        { 6774979419569127430, "XPathParser.AdditiveExpr" },
        { 6774979419569127431, "XPathParser.MultiplicativeExpr" },
        { 6774979419569127432, "XPathParser.UnaryExpr" },
        { 6774979419569127433, "XPathParser.UnionExpr" },
        { 6774979419569127434, "XPathParser.PathExpr" },
        { 6774979419569127435, "XPathParser.LocationPath" },
        { 6774979419569127436, "XPathParser.AbsoluteLocationPath" },
        { 6774979419569127437, "XPathParser.AbbreviatedAbsoluteLocationPath" },
        { 6774979419569127438, "XPathParser.RelativeLocationPath" },
        { 6774979419569127439, "XPathParser.Step" },
        { 6774979419569127440, "XPathParser.AxisSpecifier" },
        { 6774979419569127441, "XPathParser.AxisName" },
        { 6774979419569127442, "XPathParser.AbbreviatedAxisSpecifier" },
        { 6774979419569127443, "XPathParser.NodeTest" },
        { 6774979419569127444, "XPathParser.Literal" },
        { 6774979419569127445, "XPathParser.NodeType" },
        { 6774979419569127446, "XPathParser.NameTest" },
        { 6774979419569127447, "XPathParser.NCName" },
        { 6774979419569127448, "XPathParser.QName" },
        { 6774979419569127449, "XPathParser.PrefixedName" },
        { 6774979419569127450, "XPathParser.Prefix" },
        { 6774979419569127451, "XPathParser.UnprefixedName" },
        { 6774979419569127452, "XPathParser.LocalPart" },
        { 6774979419569127453, "XPathParser.Predicate" },
        { 6774979419569127454, "XPathParser.AbbreviatedStep" },
        { 6774979419569127455, "XPathParser.FunctionCall" },
        { 6774979419569127456, "XPathParser.FunctionName" },
        { 6774979419569127457, "XPathParser.Argument" },
        { 6774979419569127458, "XPathParser.FilterExpr" },
        { 6774979419569127459, "XPathParser.PrimaryExpr" },
        { 6774979419569127460, "XPathParser.VariableReference" },
        { 6774979419569127461, "XPathParser.Number" },
        { 6774979419569127462, "XPathParser.EqualityOp" },
        { 6774979419569127463, "XPathParser.RelationalOp" },
        { 6774979419569127464, "XPathParser.AdditiveOp" },
        { 6774979419569127465, "XPathParser.MultiplicativeOp" },
        { 6774979419569127466, "XPathParser.CombinePathOp" },
        { 6774979419569127467, "XPathParser.OrKeyword" },
        { 6774979419569127468, "XPathParser.AndKeyword" },
        { 6774979419569127469, "XPathParser.DivKeyword" },
        { 6774979419569127470, "XPathParser.ModKeyword" },
        { 6774979419569127471, "XPathParser.ProcessingInstructionKeyword" }
    };
    return &ruleNameMap;
}

} // soul::xml::xpath::parser::rules
