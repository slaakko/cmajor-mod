module cmajor.sbin.assembly.parser.rules;

namespace cmajor::sbin::assembly::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 4519251244111364097, "AsmParser.AsmFile" },
        { 4519251244111364098, "AsmParser.EmptyLine" },
        { 4519251244111364099, "AsmParser.End" },
        { 4519251244111364100, "AsmParser.DeclarationSection" },
        { 4519251244111364101, "AsmParser.Declaration" },
        { 4519251244111364102, "AsmParser.SymbolKind" },
        { 4519251244111364103, "AsmParser.Symbol" },
        { 4519251244111364104, "AsmParser.TypeField" },
        { 4519251244111364105, "AsmParser.Type" },
        { 4519251244111364106, "AsmParser.DataSection" },
        { 4519251244111364107, "AsmParser.DataSectionHeader" },
        { 4519251244111364108, "AsmParser.DataDefinitionLine" },
        { 4519251244111364109, "AsmParser.MacroAssignmentLine" },
        { 4519251244111364110, "AsmParser.DataDefinition" },
        { 4519251244111364111, "AsmParser.Label" },
        { 4519251244111364112, "AsmParser.DataInst" },
        { 4519251244111364113, "AsmParser.OperandList" },
        { 4519251244111364114, "AsmParser.Operand" },
        { 4519251244111364115, "AsmParser.Expr" },
        { 4519251244111364116, "AsmParser.BinaryExpr" },
        { 4519251244111364117, "AsmParser.BinaryOp" },
        { 4519251244111364118, "AsmParser.UnaryExpr" },
        { 4519251244111364119, "AsmParser.UnaryOp" },
        { 4519251244111364120, "AsmParser.PrimaryExpr" },
        { 4519251244111364121, "AsmParser.ContentExpr" },
        { 4519251244111364122, "AsmParser.SizeExpr" },
        { 4519251244111364123, "AsmParser.SizePrefix" },
        { 4519251244111364124, "AsmParser.ParenthesizedExpr" },
        { 4519251244111364125, "AsmParser.Register" },
        { 4519251244111364126, "AsmParser.Value" },
        { 4519251244111364127, "AsmParser.CodeSection" },
        { 4519251244111364128, "AsmParser.CodeSectionHeader" },
        { 4519251244111364129, "AsmParser.FunctionDefinition" },
        { 4519251244111364130, "AsmParser.FunctionHeader" },
        { 4519251244111364131, "AsmParser.FunctionTrailer" },
        { 4519251244111364132, "AsmParser.FunctionBody" },
        { 4519251244111364133, "AsmParser.InstructionLine" },
        { 4519251244111364134, "AsmParser.Instruction" },
        { 4519251244111364135, "AsmParser.OpCode" }
    };
    return &ruleNameMap;
}

} // cmajor::sbin::assembly::parser::rules
