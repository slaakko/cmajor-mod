module cmajor.masm.intermediate.parser.rules;

namespace cmajor::masm::intermediate::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 5911887668215545857, "IntermediateParser.IntermediateFile" },
        { 5911887668215545858, "IntermediateParser.TypeDeclarations" },
        { 5911887668215545859, "IntermediateParser.TypeDeclaration" },
        { 5911887668215545860, "IntermediateParser.TypeId" },
        { 5911887668215545861, "IntermediateParser.FundamentalTypeId" },
        { 5911887668215545862, "IntermediateParser.TypeRef" },
        { 5911887668215545863, "IntermediateParser.StructureType" },
        { 5911887668215545864, "IntermediateParser.ArrayType" },
        { 5911887668215545865, "IntermediateParser.FunctionType" },
        { 5911887668215545866, "IntermediateParser.DataDefinitions" },
        { 5911887668215545867, "IntermediateParser.DataDefinition" },
        { 5911887668215545868, "IntermediateParser.Constant" },
        { 5911887668215545869, "IntermediateParser.BoolConstant" },
        { 5911887668215545870, "IntermediateParser.SByteConstant" },
        { 5911887668215545871, "IntermediateParser.ByteConstant" },
        { 5911887668215545872, "IntermediateParser.ShortConstant" },
        { 5911887668215545873, "IntermediateParser.UShortConstant" },
        { 5911887668215545874, "IntermediateParser.IntConstant" },
        { 5911887668215545875, "IntermediateParser.UIntConstant" },
        { 5911887668215545876, "IntermediateParser.LongConstant" },
        { 5911887668215545877, "IntermediateParser.ULongConstant" },
        { 5911887668215545878, "IntermediateParser.FloatConstant" },
        { 5911887668215545879, "IntermediateParser.DoubleConstant" },
        { 5911887668215545880, "IntermediateParser.AddressConstant" },
        { 5911887668215545881, "IntermediateParser.ArrayConstant" },
        { 5911887668215545882, "IntermediateParser.StructureConstant" },
        { 5911887668215545883, "IntermediateParser.StringConstant" },
        { 5911887668215545884, "IntermediateParser.StringArrayConstant" },
        { 5911887668215545885, "IntermediateParser.StringArrayPrefix" },
        { 5911887668215545886, "IntermediateParser.ConversionConstant" },
        { 5911887668215545887, "IntermediateParser.ClsIdConstant" },
        { 5911887668215545888, "IntermediateParser.SymbolConstant" },
        { 5911887668215545889, "IntermediateParser.Value" },
        { 5911887668215545890, "IntermediateParser.RegValue" },
        { 5911887668215545891, "IntermediateParser.ResultRegValue" },
        { 5911887668215545892, "IntermediateParser.SymbolValue" },
        { 5911887668215545893, "IntermediateParser.LiteralValue" },
        { 5911887668215545894, "IntermediateParser.FunctionDefinitions" },
        { 5911887668215545895, "IntermediateParser.FunctionDeclaration" },
        { 5911887668215545896, "IntermediateParser.FunctionDefinition" },
        { 5911887668215545897, "IntermediateParser.FunctionHeader" },
        { 5911887668215545898, "IntermediateParser.BasicBlock" },
        { 5911887668215545899, "IntermediateParser.Label" },
        { 5911887668215545900, "IntermediateParser.Operand" },
        { 5911887668215545901, "IntermediateParser.Instructions" },
        { 5911887668215545902, "IntermediateParser.Instruction" },
        { 5911887668215545903, "IntermediateParser.StoreInstruction" },
        { 5911887668215545904, "IntermediateParser.ArgInstruction" },
        { 5911887668215545905, "IntermediateParser.JmpInstruction" },
        { 5911887668215545906, "IntermediateParser.BranchInstruction" },
        { 5911887668215545907, "IntermediateParser.ProcedureCallInstruction" },
        { 5911887668215545908, "IntermediateParser.RetInstruction" },
        { 5911887668215545909, "IntermediateParser.SwitchInstruction" },
        { 5911887668215545910, "IntermediateParser.ValueInstruction" },
        { 5911887668215545911, "IntermediateParser.Operation" },
        { 5911887668215545912, "IntermediateParser.UnaryInstruction" },
        { 5911887668215545913, "IntermediateParser.BinaryInstruction" },
        { 5911887668215545914, "IntermediateParser.ParamInstruction" },
        { 5911887668215545915, "IntermediateParser.LocalInstruction" },
        { 5911887668215545916, "IntermediateParser.LoadInstruction" },
        { 5911887668215545917, "IntermediateParser.ElemAddrInstruction" },
        { 5911887668215545918, "IntermediateParser.PtrOffsetInstruction" },
        { 5911887668215545919, "IntermediateParser.PtrDiffInstruction" },
        { 5911887668215545920, "IntermediateParser.FunctionCallInstruction" },
        { 5911887668215545921, "IntermediateParser.BlockValue" },
        { 5911887668215545922, "IntermediateParser.NoOperationInstruction" }
    };
    return &ruleNameMap;
}

} // cmajor::masm::intermediate::parser::rules
