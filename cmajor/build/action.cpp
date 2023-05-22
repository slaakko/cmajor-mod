// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.action;

import std.filesystem;
import soul.rex;
import soul.xml.dom;
import soul.xml.xpath;
import soul.xml.dom.parser;
import util;

namespace cmajor::build {

Variable::Variable(const std::string& name_, const std::string& value_) : name(name_), value(value_)
{
}

Variables::Variables()
{
}

void Variables::AddVariable(Variable* variable)
{
    Variable* prev = GetVariable(variable->Name());
    if (prev)
    {
        throw std::runtime_error("build variable name '" + prev->Name() + "' not unique");
    }
    variableMap[variable->Name()] = variable;
    variables.push_back(std::unique_ptr<Variable>(variable));
}

Variable* Variables::GetVariable(const std::string& name) const
{
    auto it = variableMap.find(name);
    if (it != variableMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

std::string Expand(const std::string& str, const Variables& variables)
{
    std::string result;
    std::string variableName;
    int state = 0;
    for (char c : str)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '$')
            {
                variableName.clear();
                state = 1;
            }
            else
            {
                result.append(1, c);
            }
            break;
        }
        case 1:
        {
            if (c == '$')
            {
                Variable* variable = variables.GetVariable(variableName);
                if (variable)
                {
                    result.append(variable->Value());
                }
                else
                {
                    throw std::runtime_error("build variable '" + variableName + "' not found");
                }
                state = 0;
            }
            else
            {
                variableName.append(1, c);
            }
            break;
        }
        }
    }
    if (state == 1)
    {
        throw std::runtime_error("build variable '" + variableName + "' has no ending '$' character");
    }
    return result;
}

void Copy(const std::vector<std::string>& sourceFilePaths, const std::string& targetDir)
{
    for (const auto& sourceFilePath : sourceFilePaths)
    {
        std::string dest = util::GetFullPath(util::Path::Combine(targetDir, util::Path::GetFileName(sourceFilePath)));
        util::CopyFile(sourceFilePath, dest, true, false);
        util::LogMessage(-1, sourceFilePath + " -> " + dest);
    }
}

std::vector<std::string> GetSourceFilePaths(const std::string& sourceDir, const std::string& sourceFileMask, soul::rex::context::Context& context)
{
    std::vector<std::string> sourceFilePaths;
    soul::rex::nfa::Nfa nfa = soul::rex::CompileFilePattern(context, util::ToUtf32(sourceFileMask));
    std::error_code ec;
    std::filesystem::directory_iterator it(std::filesystem::path(sourceDir), ec);
    if (ec)
    {
        throw std::runtime_error("could not iterate directory '" + sourceDir + "': " + util::PlatformStringToUtf8(ec.message()));
    }
    while (it != std::filesystem::directory_iterator())
    {
        if (std::filesystem::is_regular_file(it->path()))
        {
            std::string fileName = it->path().filename().generic_string();
            if (soul::rex::PatternMatch(util::ToUtf32(fileName), nfa))
            {
                sourceFilePaths.push_back(util::Path::Combine(sourceDir, fileName));
            }
        }
        ++it;
    }
    return sourceFilePaths;
}

void RunBuildActions(soul::xml::Document* actionDoc, const std::string& rootDir, soul::rex::context::Context& context, const Variables& variables)
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/actions/copy", actionDoc);
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string sourceFileAttribute = element->GetAttribute("sourceFile");
            if (sourceFileAttribute.empty())
            {
                throw std::runtime_error("'actions/copy' element has no 'sourceFile' attribute");
            }
            std::string targetDirAttribute = element->GetAttribute("targetDir");
            if (targetDirAttribute.empty())
            {
                throw std::runtime_error("'actions/copy' element has no 'targetDir' attribute");
            }
            std::string sourceFilePath = Expand(util::ToUtf8(sourceFileAttribute), variables);
            std::string targetDirPath = Expand(util::ToUtf8(targetDirAttribute), variables);
            sourceFilePath = util::GetFullPath(util::Path::Combine(rootDir, sourceFilePath));
            std::string sourceDir;
            std::string sourceFileMask;
            sourceDir = util::Path::GetDirectoryName(sourceFilePath);
            sourceFileMask = util::Path::GetFileName(sourceFilePath);
            std::vector<std::string> sourceFilePaths = GetSourceFilePaths(sourceDir, sourceFileMask, context);
            Copy(sourceFilePaths, targetDirPath);
        }
    }
}

void RunBuildActions(const cmajor::ast::Project& project, const Variables& variables)
{
    soul::rex::context::Context context;
    for (const auto& actionFilePath : project.ActionFilePaths())
    {
        std::string rootDir = util::Path::GetDirectoryName(actionFilePath);
        std::unique_ptr<soul::xml::Document> actionDoc = soul::xml::ParseXmlFile(actionFilePath);
        RunBuildActions(actionDoc.get(), rootDir, context, variables);
    }
}

} // namespace cmajor::build
