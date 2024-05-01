module common.parser.rules;

namespace common.parser.rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 7440832391356088321, "CommonParser.TokensKeyword" },
        { 7440832391356088322, "CommonParser.KeywordsKeyword" },
        { 7440832391356088323, "CommonParser.ExpressionsKeyword" },
        { 7440832391356088324, "CommonParser.LexerKeyword" },
        { 7440832391356088325, "CommonParser.ProjectKeyword" },
        { 7440832391356088326, "CommonParser.QualifiedUtf8Id" },
        { 1044754359574855681, "ContainerFileParser.QualifiedId" },
        { 1044754359574855682, "ContainerFileParser.FilePath" },
        { 1044754359574855683, "ContainerFileParser.ProjectKeyword" },
        { 1044754359574855684, "ContainerFileParser.SolutionKeyword" }
    };
    return &ruleNameMap;
}

} // common.parser.rules
