module debug.parsers.rules;

namespace debug::parsers::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 1594533928926969857, "DebugExprParser.DebugExpr" },
        { 1594533928926969858, "DebugExprParser.DisjunctiveDebugExpr" },
        { 1594533928926969859, "DebugExprParser.ConjunctiveDebugExpr" },
        { 1594533928926969860, "DebugExprParser.BitOrDebugExpr" },
        { 1594533928926969861, "DebugExprParser.BitXorDebugExpr" },
        { 1594533928926969862, "DebugExprParser.BitAndDebugExpr" },
        { 1594533928926969863, "DebugExprParser.EqualityDebugExpr" },
        { 1594533928926969864, "DebugExprParser.RelationalDebugExpr" },
        { 1594533928926969865, "DebugExprParser.ShiftDebugExpr" },
        { 1594533928926969866, "DebugExprParser.AdditiveDebugExpr" },
        { 1594533928926969867, "DebugExprParser.MultiplicativeDebugExpr" },
        { 1594533928926969868, "DebugExprParser.PrefixDebugExpr" },
        { 1594533928926969869, "DebugExprParser.PostfixDebugExpr" },
        { 1594533928926969870, "DebugExprParser.PrimaryDebugExpr" },
        { 1594533928926969871, "DebugExprParser.CastDebugExpr" },
        { 1594533928926969872, "DebugExprParser.TypeIdExpr" },
        { 7022048444063678465, "DebugCommandParser.DebuggerCommand" },
        { 7022048444063678466, "DebugCommandParser.DebuggerExitCommand" },
        { 7022048444063678467, "DebugCommandParser.DebuggerHelpCommand" },
        { 7022048444063678468, "DebugCommandParser.DebuggerNextCommand" },
        { 7022048444063678469, "DebugCommandParser.DebuggerStepCommand" },
        { 7022048444063678470, "DebugCommandParser.DebuggerContinueCommand" },
        { 7022048444063678471, "DebugCommandParser.DebuggerFinishCommand" },
        { 7022048444063678472, "DebugCommandParser.DebuggerUntilCommand" },
        { 7022048444063678473, "DebugCommandParser.DebuggerBreakCommand" },
        { 7022048444063678474, "DebugCommandParser.DebuggerDeleteCommand" },
        { 7022048444063678475, "DebugCommandParser.DebuggerDepthCommand" },
        { 7022048444063678476, "DebugCommandParser.DebuggerFramesCommand" },
        { 7022048444063678477, "DebugCommandParser.DebuggerShowBreakpointCommand" },
        { 7022048444063678478, "DebugCommandParser.DebuggerShowBreakpointsCommand" },
        { 7022048444063678479, "DebugCommandParser.DebuggerListCommand" },
        { 7022048444063678480, "DebugCommandParser.DebuggerPrintCommand" },
        { 7022048444063678481, "DebugCommandParser.DebuggerSetConditionCommand" },
        { 7022048444063678482, "DebugCommandParser.DebuggerSetBreakOnThrowCommand" },
        { 7022048444063678483, "DebugCommandParser.DebuggerRepeatLatestCommand" },
        { 7022048444063678484, "DebugCommandParser.SourceLocation" },
        { 7022048444063678485, "DebugCommandParser.Line" },
        { 1509825976870109185, "GdbReplyParser.GdbReplyRecord" },
        { 1509825976870109186, "GdbReplyParser.GdbResultRecord" },
        { 1509825976870109187, "GdbReplyParser.GdbDoneRecord" },
        { 1509825976870109188, "GdbReplyParser.GdbRunningRecord" },
        { 1509825976870109189, "GdbReplyParser.GdbConnectedRecord" },
        { 1509825976870109190, "GdbReplyParser.GdbErrorRecord" },
        { 1509825976870109191, "GdbReplyParser.GdbExitRecord" },
        { 1509825976870109192, "GdbReplyParser.GdbAsyncRecord" },
        { 1509825976870109193, "GdbReplyParser.GdbExecAsyncRecord" },
        { 1509825976870109194, "GdbReplyParser.GdbExecRunningRecord" },
        { 1509825976870109195, "GdbReplyParser.GdbExecStoppedRecord" },
        { 1509825976870109196, "GdbReplyParser.GdbNotifyAsyncRecord" },
        { 1509825976870109197, "GdbReplyParser.GdbStreamRecord" },
        { 1509825976870109198, "GdbReplyParser.GdbPrompt" },
        { 1509825976870109199, "GdbReplyParser.GdbResults" },
        { 1509825976870109200, "GdbReplyParser.GdbResult" },
        { 1509825976870109201, "GdbReplyParser.GdbVariable" },
        { 1509825976870109202, "GdbReplyParser.GdbValue" },
        { 1509825976870109203, "GdbReplyParser.GdbStringValue" },
        { 1509825976870109204, "GdbReplyParser.GdbTupleValue" },
        { 1509825976870109205, "GdbReplyParser.GdbListValue" }
    };
    return &ruleNameMap;
}

} // debug::parsers::rules
