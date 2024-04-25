module common.parser.rules;

namespace common.parser.rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 2028434430229479425, "CommonParser.QualifiedId" },
        { 2028434430229479426, "CommonParser.Import" },
        { 2028434430229479427, "CommonParser.ExprString" },
        { 2028434430229479428, "CommonParser.FilePath" },
        { 2028434430229479429, "CommonParser.ExportKeyword" },
        { 2028434430229479430, "CommonParser.ModuleKeyword" },
        { 2028434430229479431, "CommonParser.ImportKeyword" },
        { 2028434430229479432, "CommonParser.ParserKeyword" },
        { 2028434430229479433, "CommonParser.LexerKeyword" },
        { 2028434430229479434, "CommonParser.TokensKeyword" },
        { 2028434430229479435, "CommonParser.KeywordsKeyword" },
        { 2028434430229479436, "CommonParser.ExpressionsKeyword" },
        { 2028434430229479437, "CommonParser.RulesKeyword" },
        { 2028434430229479438, "CommonParser.VariablesKeyword" },
        { 2028434430229479439, "CommonParser.ActionsKeyword" },
        { 2028434430229479440, "CommonParser.MainKeyword" },
        { 2028434430229479441, "CommonParser.StartKeyword" },
        { 2028434430229479442, "CommonParser.EmptyKeyword" },
        { 2028434430229479443, "CommonParser.AnyKeyword" },
        { 2028434430229479444, "CommonParser.VarKeyword" },
        { 2028434430229479445, "CommonParser.ProjectKeyword" }
    };
    return &ruleNameMap;
}

} // common.parser.rules
