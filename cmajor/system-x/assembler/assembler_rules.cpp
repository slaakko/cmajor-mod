module cmajor.systemx.assembler.parser.rules;

namespace cmajor::systemx::assembler::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 7507127981823754241, "AssemblyParser.AssemblyFile" },
        { 7507127981823754242, "AssemblyParser.AssemblyLine" },
        { 7507127981823754243, "AssemblyParser.CommentLine" },
        { 7507127981823754244, "AssemblyParser.Comment" },
        { 7507127981823754245, "AssemblyParser.EmptyLine" },
        { 7507127981823754246, "AssemblyParser.ModeLine" },
        { 7507127981823754247, "AssemblyParser.InstructionLine" },
        { 7507127981823754248, "AssemblyParser.Instruction" },
        { 7507127981823754249, "AssemblyParser.Label" },
        { 7507127981823754250, "AssemblyParser.SplitLine" },
        { 7507127981823754251, "AssemblyParser.OpCode" },
        { 7507127981823754252, "AssemblyParser.Operands" },
        { 7507127981823754253, "AssemblyParser.Expression" },
        { 7507127981823754254, "AssemblyParser.WeakOperator" },
        { 7507127981823754255, "AssemblyParser.Term" },
        { 7507127981823754256, "AssemblyParser.StrongOperator" },
        { 7507127981823754257, "AssemblyParser.PrimaryExpression" },
        { 7507127981823754258, "AssemblyParser.UnaryOperator" },
        { 7507127981823754259, "AssemblyParser.LocalSymbol" },
        { 7507127981823754260, "AssemblyParser.Constant" },
        { 7507127981823754261, "AssemblyParser.Symbol" },
        { 7507127981823754262, "AssemblyParser.At" }
    };
    return &ruleNameMap;
}

} // cmajor::systemx::assembler::parser::rules
