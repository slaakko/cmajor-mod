module cmajor.command.line.rules;

namespace cmajor::command::line::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 7881572280185126913, "CommandLineParser.CommandLine" },
        { 7881572280185126914, "CommandLineParser.Spaces" },
        { 7881572280185126915, "CommandLineParser.Argument" },
        { 7881572280185126916, "CommandLineParser.ArgElement" },
        { 7881572280185126917, "CommandLineParser.OddBackslashesAndLiteralQuotationMark" },
        { 7881572280185126918, "CommandLineParser.EvenBackslashesAndQuotationMark" },
        { 7881572280185126919, "CommandLineParser.StringChar" }
    };
    return &ruleNameMap;
}

} // cmajor::command::line::rules
