module re.parser.rules;

namespace re.parser.rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 7493594346600529921, "RegExParser.RegularExpression" },
        { 7493594346600529922, "RegExParser.Alternative" },
        { 7493594346600529923, "RegExParser.Catenation" },
        { 7493594346600529924, "RegExParser.Repetition" },
        { 7493594346600529925, "RegExParser.Primary" },
        { 7493594346600529926, "RegExParser.Class" },
        { 7493594346600529927, "RegExParser.Range" },
        { 7493594346600529928, "RegExParser.Char" },
        { 7493594346600529929, "RegExParser.ExpressionReference" }
    };
    return &ruleNameMap;
}

} // re.parser.rules
