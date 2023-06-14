// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.resources;

import cmajor.symbols;
import cmajor.systemx.object;

namespace cmajor::build {

void AddResourcesSystemX(cmajor::ast::Project* project, std::vector<std::string>& objectFilePaths)
{
    for (const auto& resourceFilePath : project->ResourceFilePaths())
    {
        std::string objectFilePath = cmajor::systemx::object::GenerateResourceUnit(resourceFilePath, project->LibraryDir());
        objectFilePaths.push_back(objectFilePath);
    }
}

void AddResources(cmajor::ast::Project* project, std::vector<std::string>& objectFilePaths)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        AddResourcesSystemX(project, objectFilePaths);
    }
}

} // namespace cmajor::build
