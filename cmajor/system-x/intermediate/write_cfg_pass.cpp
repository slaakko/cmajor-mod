// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.write.cfg.pass;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.code;
import util;

namespace cmajor::systemx::intermediate {

WriteCfgPass::WriteCfgPass() : FunctionPass("write-cfg")
{
}

void WriteCfgPass::Run(Function& function)
{
    std::string libDir = util::Path::GetDirectoryName(function.Parent()->GetContext()->FilePath());
    std::string functionXmlFilePath = util::Path::Combine(libDir, function.Name() + ".xml");
    function.WriteXmlDocument(functionXmlFilePath);
}

} // cmajor::systemx::intermediate
