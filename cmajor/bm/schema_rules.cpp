module cmajor.binary.message.schema.rules;

namespace cmajor::binary::message::schema::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 1324476289549074433, "BinaryMessageSchemaParser.SourceFile" },
        { 1324476289549074434, "BinaryMessageSchemaParser.NamespaceContent" },
        { 1324476289549074435, "BinaryMessageSchemaParser.Declaration" },
        { 1324476289549074436, "BinaryMessageSchemaParser.ForwardClassDeclaration" },
        { 1324476289549074437, "BinaryMessageSchemaParser.UsingAliasDeclaration" },
        { 1324476289549074438, "BinaryMessageSchemaParser.Definition" },
        { 1324476289549074439, "BinaryMessageSchemaParser.Class" },
        { 1324476289549074440, "BinaryMessageSchemaParser.ClassContent" },
        { 1324476289549074441, "BinaryMessageSchemaParser.MemberVariable" },
        { 1324476289549074442, "BinaryMessageSchemaParser.Type" },
        { 1324476289549074443, "BinaryMessageSchemaParser.ScalarType" },
        { 1324476289549074444, "BinaryMessageSchemaParser.ClassType" },
        { 1324476289549074445, "BinaryMessageSchemaParser.Enum" },
        { 1324476289549074446, "BinaryMessageSchemaParser.EnumContent" },
        { 1324476289549074447, "BinaryMessageSchemaParser.EnumConstant" },
        { 1324476289549074448, "BinaryMessageSchemaParser.Namespace" },
        { 1324476289549074449, "BinaryMessageSchemaParser.QualifiedId" },
        { 1324476289549074450, "BinaryMessageSchemaParser.QualifiedCppId" },
        { 1324476289549074451, "BinaryMessageSchemaParser.ExportModule" },
        { 1324476289549074452, "BinaryMessageSchemaParser.Import" },
        { 1324476289549074453, "BinaryMessageSchemaParser.ImportPrefix" },
        { 1324476289549074454, "BinaryMessageSchemaParser.ExportKeyword" },
        { 1324476289549074455, "BinaryMessageSchemaParser.ModuleKeyword" },
        { 1324476289549074456, "BinaryMessageSchemaParser.ImportKeyword" },
        { 1324476289549074457, "BinaryMessageSchemaParser.ImplementationPrefix" },
        { 1324476289549074458, "BinaryMessageSchemaParser.InterfacePrefix" },
        { 1324476289549074459, "BinaryMessageSchemaParser.ImplementationKeyword" },
        { 1324476289549074460, "BinaryMessageSchemaParser.InterfaceKeyword" }
    };
    return &ruleNameMap;
}

} // cmajor::binary::message::schema::rules
