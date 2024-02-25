module json.parser.rules;

namespace json::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 8988793984142802945, "JsonParser.Value" },
        { 8988793984142802946, "JsonParser.Object" },
        { 8988793984142802947, "JsonParser.Field" },
        { 8988793984142802948, "JsonParser.Array" }
    };
    return &ruleNameMap;
}

} // json::parser::rules
