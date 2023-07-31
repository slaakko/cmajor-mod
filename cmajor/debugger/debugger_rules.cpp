module cmajor.debugger.spg.rules;

namespace cmajor::debugger::spg::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 811220426297442305, "ReplyParser.Reply" },
        { 811220426297442306, "ReplyParser.Record" },
        { 811220426297442307, "ReplyParser.Prompt" },
        { 811220426297442308, "ReplyParser.ResultRecord" },
        { 811220426297442309, "ReplyParser.StreamRecord" },
        { 811220426297442310, "ReplyParser.AsyncRecord" },
        { 811220426297442311, "ReplyParser.ExecRecord" },
        { 811220426297442312, "ReplyParser.StatusRecord" },
        { 811220426297442313, "ReplyParser.NotifyRecord" },
        { 811220426297442314, "ReplyParser.Class" },
        { 811220426297442315, "ReplyParser.Results" },
        { 811220426297442316, "ReplyParser.Item" },
        { 811220426297442317, "ReplyParser.Value" },
        { 811220426297442318, "ReplyParser.StringValue" },
        { 811220426297442319, "ReplyParser.TupleValue" },
        { 811220426297442320, "ReplyParser.ListValue" },
        { 811220426297442321, "ReplyParser.Output" }
    };
    return &ruleNameMap;
}

} // cmajor::debugger::spg::rules
