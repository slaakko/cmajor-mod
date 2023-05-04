module soul.rex.spg.rules;

namespace soul::rex::spg::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 7261371920241655809, "RexParser.RegularExpression" },
        { 7261371920241655810, "RexParser.Alternative" },
        { 7261371920241655811, "RexParser.Catenation" },
        { 7261371920241655812, "RexParser.Repetition" },
        { 7261371920241655813, "RexParser.Primary" },
        { 7261371920241655814, "RexParser.Class" },
        { 7261371920241655815, "RexParser.Range" },
        { 7261371920241655816, "RexParser.Char" }
    };
    return &ruleNameMap;
}

} // soul::rex::spg::rules
