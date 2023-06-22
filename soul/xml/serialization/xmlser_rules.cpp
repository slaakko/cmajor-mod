module soul.xml.serialization.spg.rules;

namespace soul::xml::serialization::spg::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 9203395559845527553, "XmlSerParser.SourceFile" },
        { 9203395559845527554, "XmlSerParser.NamespaceContent" },
        { 9203395559845527555, "XmlSerParser.Declaration" },
        { 9203395559845527556, "XmlSerParser.ForwardClassDeclaration" },
        { 9203395559845527557, "XmlSerParser.UsingAliasDeclaration" },
        { 9203395559845527558, "XmlSerParser.Definition" },
        { 9203395559845527559, "XmlSerParser.Class" },
        { 9203395559845527560, "XmlSerParser.Inheritance" },
        { 9203395559845527561, "XmlSerParser.ClassContent" },
        { 9203395559845527562, "XmlSerParser.MemberVariable" },
        { 9203395559845527563, "XmlSerParser.Type" },
        { 9203395559845527564, "XmlSerParser.ScalarType" },
        { 9203395559845527565, "XmlSerParser.ClassType" },
        { 9203395559845527566, "XmlSerParser.CppBlock" },
        { 9203395559845527567, "XmlSerParser.Enum" },
        { 9203395559845527568, "XmlSerParser.EnumContent" },
        { 9203395559845527569, "XmlSerParser.EnumConstant" },
        { 9203395559845527570, "XmlSerParser.Namespace" },
        { 9203395559845527571, "XmlSerParser.QualifiedId" },
        { 9203395559845527572, "XmlSerParser.QualifiedCppId" },
        { 9203395559845527573, "XmlSerParser.ExportModule" },
        { 9203395559845527574, "XmlSerParser.Import" },
        { 9203395559845527575, "XmlSerParser.ImportPrefix" },
        { 9203395559845527576, "XmlSerParser.ExportKeyword" },
        { 9203395559845527577, "XmlSerParser.ModuleKeyword" },
        { 9203395559845527578, "XmlSerParser.ImportKeyword" },
        { 9203395559845527579, "XmlSerParser.ImplementationPrefix" },
        { 9203395559845527580, "XmlSerParser.InterfacePrefix" },
        { 9203395559845527581, "XmlSerParser.ImplementationKeyword" },
        { 9203395559845527582, "XmlSerParser.InterfaceKeyword" }
    };
    return &ruleNameMap;
}

} // soul::xml::serialization::spg::rules
