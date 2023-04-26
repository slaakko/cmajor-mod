module cmajor.rules;

namespace cmajor::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 6496561291342118913, "BasicTypeParser.BasicType" },
        { 3562699651482124289, "IdentifierParser.Identifier" },
        { 3562699651482124290, "IdentifierParser.QualifiedId" }
    };
    return &ruleNameMap;
}

} // cmajor::rules
