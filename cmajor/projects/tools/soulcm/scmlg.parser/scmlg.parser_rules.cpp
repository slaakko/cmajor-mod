module scmlg.parser.rules;

namespace scmlg.parser.rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 89331080624078849, "ExpressionFileParser.ExpressionFile" },
        { 89331080624078850, "ExpressionFileParser.Expression" },
        { 89331080624078851, "ExpressionFileParser.ExprString" },
        { 713474246315081729, "KeywordFileParser.KeywordFile" },
        { 713474246315081730, "KeywordFileParser.Usings" },
        { 713474246315081731, "KeywordFileParser.Keyword" },
        { 753471983957573633, "ScmlgFileParser.ScmlgFile" },
        { 753471983957573634, "ScmlgFileParser.ScmlgFileDeclaration" },
        { 753471983957573635, "ScmlgFileParser.TokenFileDeclaration" },
        { 753471983957573636, "ScmlgFileParser.KeywordFileDeclaration" },
        { 753471983957573637, "ScmlgFileParser.ExpressionFileDeclaration" },
        { 753471983957573638, "ScmlgFileParser.LexerFileDeclaration" },
        { 1158510743117627393, "TokenFileParser.TokenFile" },
        { 1158510743117627394, "TokenFileParser.Token" },
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

} // scmlg.parser.rules
