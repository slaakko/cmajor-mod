module soul.xml.parser.rules;

namespace soul::xml::parser::rules {

std::mutex ruleMtx;

std::map<int64_t, std::string>* GetRuleNameMapPtr()
{
    std::lock_guard<std::mutex> lock(ruleMtx);
    static std::map<int64_t, std::string> ruleNameMap = {
        { 4556688588539953153, "XmlParser.Document" },
        { 4556688588539953154, "XmlParser.Prolog" },
        { 4556688588539953155, "XmlParser.XmlDecl" },
        { 4556688588539953156, "XmlParser.VersionInfo" },
        { 4556688588539953157, "XmlParser.S" },
        { 4556688588539953158, "XmlParser.Eq" },
        { 4556688588539953159, "XmlParser.VersionNumber" },
        { 4556688588539953160, "XmlParser.VersionNumDQ" },
        { 4556688588539953161, "XmlParser.VersionNumSQ" },
        { 4556688588539953162, "XmlParser.VersionNum" },
        { 4556688588539953163, "XmlParser.EncodingDecl" },
        { 4556688588539953164, "XmlParser.EncodingName" },
        { 4556688588539953165, "XmlParser.EncNameDQ" },
        { 4556688588539953166, "XmlParser.EncNameSQ" },
        { 4556688588539953167, "XmlParser.EncName" },
        { 4556688588539953168, "XmlParser.StandaloneDecl" },
        { 4556688588539953169, "XmlParser.YesNo" },
        { 4556688588539953170, "XmlParser.Misc" },
        { 4556688588539953171, "XmlParser.Comment" },
        { 4556688588539953172, "XmlParser.Char" },
        { 4556688588539953173, "XmlParser.PI" },
        { 4556688588539953174, "XmlParser.PITarget" },
        { 4556688588539953175, "XmlParser.Name" },
        { 4556688588539953176, "XmlParser.NameStartChar" },
        { 4556688588539953177, "XmlParser.NameChar" },
        { 4556688588539953178, "XmlParser.Xml" },
        { 4556688588539953179, "XmlParser.Element" },
        { 4556688588539953180, "XmlParser.Attribute" },
        { 4556688588539953181, "XmlParser.AttValue" },
        { 4556688588539953182, "XmlParser.AttValueDQ" },
        { 4556688588539953183, "XmlParser.AttValueSQ" },
        { 4556688588539953184, "XmlParser.Reference" },
        { 4556688588539953185, "XmlParser.CharRef" },
        { 4556688588539953186, "XmlParser.DecCodePoint" },
        { 4556688588539953187, "XmlParser.HexCodePoint" },
        { 4556688588539953188, "XmlParser.EntityRef" },
        { 4556688588539953189, "XmlParser.Content" },
        { 4556688588539953190, "XmlParser.CharData" },
        { 4556688588539953191, "XmlParser.CharDataChar" },
        { 4556688588539953192, "XmlParser.CDSect" },
        { 4556688588539953193, "XmlParser.ETag" },
        { 4556688588539953194, "XmlParser.DocTypeDecl" }
    };
    return &ruleNameMap;
}

} // soul::xml::parser::rules
