// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.pass.manager;

import cmajor.systemx.intermediate.passes;
import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.function.pass.driver;
import cmajor.systemx.intermediate.module_pass_driver;
import soul.xml.dom;
import soul.xml.xpath;
import soul.xml.dom.parser;
import util;

namespace cmajor::systemx::intermediate {

namespace {

    std::string CmajorConfigDir()
    {
        return util::Path::Combine(util::CmajorRoot(), "config");
    }

} // namespace

PassKind GetPassKind(const std::string& passKindStr)
{
    if (passKindStr == "function")
    {
        return PassKind::function;
    }
    else if (passKindStr == "module")
    {
        return PassKind::module;
    }
    else if (passKindStr == "composite")
    {
        return PassKind::composite;
    }
    else
    {
        throw std::runtime_error("invalid pass kind '" + passKindStr + "'");
    }
}

void PassManager::Init()
{
    instance.reset(new PassManager());
}

void PassManager::Done()
{
    instance.reset();
}

std::unique_ptr<PassManager> PassManager::instance;

PassManager::PassManager()
{
}

void PassManager::ReadPassesXml()
{
    std::string passesXmlFilePath = util::Path::Combine(CmajorConfigDir(), "passes.xml");
    std::unique_ptr<soul::xml::Document> passesDoc = soul::xml::ParseXmlFile(passesXmlFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/passes/pass", passesDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string passNameAttr = element->GetAttribute("name");
            if (passNameAttr.empty())
            {
                throw std::runtime_error("pass name attribute is empty");
            }
            std::string passName = passNameAttr;
            std::string passKindAttr = element->GetAttribute("kind");
            if (passKindAttr.empty())
            {
                throw std::runtime_error("pass kind attribute is empty");
            }
            PassKind kind = GetPassKind(passKindAttr);
            if (kind == PassKind::function || kind == PassKind::module)
            {
                Pass* pass = GetPass(passName);
                if (pass)
                {
                    if (pass->Kind() != kind)
                    {
                        throw std::runtime_error("pass '" + passName + "' kind conflict");
                    }
                    ReadPassContent(element, pass);
                }
                else
                {
                    throw std::runtime_error("pass '" + passName + "' not found");
                }
            }
            else if (kind == PassKind::composite)
            {
                Pass* compositePass = new CompositePass(passName);
                AddPass(compositePass);
                ReadPassContent(element, compositePass);
            }
        }
    }
}

void PassManager::ReadPassContent(soul::xml::Element* passElement, Pass* pass)
{
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("require", passElement);
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string passAttr = element->GetAttribute("pass");
            if (passAttr.empty())
            {
                throw std::runtime_error("pass attribute is empty");
            }
            std::string passName = passAttr;
            Pass* requiredPass = GetPass(passName);
            if (requiredPass)
            {
                pass->AddRequiredPass(requiredPass);
            }
            else
            {
                throw std::runtime_error("required pass '" + passName + "' not found");
            }
        }
    }
}

void PassManager::Initialize()
{
    CreatePasses();
    ReadPassesXml();
}

void PassManager::AddPass(Pass* pass)
{
    Pass* prev = GetPass(pass->Name());
    if (prev)
    {
        throw std::runtime_error("pass name '" + prev->Name() + "' not unique");
    }
    passes.push_back(std::unique_ptr<Pass>(pass));
    passMap[pass->Name()] = pass;
}

Pass* PassManager::GetPass(const std::string& passName) const
{
    auto it = passMap.find(passName);
    if (it != passMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void Visit(Pass* pass, std::vector<Pass*>& order, std::set<Pass*>& visited)
{
    visited.insert(pass);
    for (Pass* requiredPass : pass->RequiredPasses())
    {
        if (visited.find(requiredPass) == visited.cend())
        {
            Visit(requiredPass, order, visited);
        }
    }
    order.push_back(pass);
}

std::vector<Pass*> TopologicalSort(Pass* pass)
{
    std::vector<Pass*> order;
    std::set<Pass*> visited;
    Visit(pass, order, visited);
    return order;
}

std::vector<Pass*> PassManager::CollectPasses(const std::string& passName) const
{
    Pass* pass = GetPass(passName);
    if (!pass)
    {
        throw std::runtime_error("pass '" + passName + "' not found");
    }
    std::vector<Pass*> passOrder = TopologicalSort(pass);
    return passOrder;
}

std::vector<FunctionPass*> PassManager::GetFunctionPasses(const std::vector<Pass*>& passes)
{
    std::vector<FunctionPass*> functionPasses;
    for (Pass* pass : passes)
    {
        if (pass->Kind() == PassKind::function)
        {
            functionPasses.push_back(static_cast<FunctionPass*>(pass));
        }
    }
    return functionPasses;
}

std::vector<ModulePass*> PassManager::GetModuleAndCompositePasses(const std::vector<Pass*>& passes)
{
    std::vector<ModulePass*> moduleAndCompositePasses;
    for (Pass* pass : passes)
    {
        if (pass->Kind() == PassKind::module || pass->Kind() == PassKind::composite)
        {
            moduleAndCompositePasses.push_back(static_cast<ModulePass*>(pass));
        }
    }
    return moduleAndCompositePasses;
}

void PassManager::RunFunctionPasses(Context* context, const std::vector<FunctionPass*>& functionPasses)
{
    FunctionPassDriver driver(context, functionPasses);
    context->GetCode().VisitFunctions(driver);
}

void PassManager::RunModuleAndCompositePasses(Context* context, const std::vector<ModulePass*>& moduleAndCompositePasses)
{
    ModulePassDriver driver(context, moduleAndCompositePasses);
    driver.Run();
}

void PassManager::Run(int logStreamId, Context* context, const std::string& passName, bool verbose)
{
    std::vector<Pass*> passes = CollectPasses(passName);
    std::vector<FunctionPass*> functionPasses = GetFunctionPasses(passes);
    RunFunctionPasses(context, functionPasses);
    std::vector<ModulePass*> moduleAndCompositePasses = GetModuleAndCompositePasses(passes);
    RunModuleAndCompositePasses(context, moduleAndCompositePasses);
    if (verbose)
    {
        util::LogMessage(logStreamId, "==> " + util::Path::ChangeExtension(context->FilePath(), ".s"));
    }
}

void InitPassManager()
{
    PassManager::Init();
    PassManager::Instance().Initialize();
}

void DonePassManager()
{
    PassManager::Done();
}

} // cmajor::systemx::intermediate
