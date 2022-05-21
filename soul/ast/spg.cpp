// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.spg;

import util;

using namespace util;

namespace soul::ast::spg {

std::string ToNamespaceName(const std::string& qualifiedId)
{
    std::string namespaceName;
    std::vector<std::string> components = Split(qualifiedId, '.');
    bool first = true;
    for (const auto& component : components)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            namespaceName.append("::");
        }
        namespaceName.append(component);
    }
    return namespaceName;
}

ExportModule::ExportModule(const std::string& moduleName_) : moduleName(moduleName_)
{
}

Import::Import(const std::string& moduleName_, ImportPrefix prefix_) : moduleName(moduleName_), prefix(prefix_)
{
}

} // namespace soul::ast::spg
