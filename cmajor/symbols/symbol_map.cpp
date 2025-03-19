// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.map;

import soul.xml.xpath;
import soul.xml.dom;
import soul.xml.dom.parser;
import cmajor.ast;
import util;

namespace cmajor::symbols {

std::mutex symbolMapMtx;

SymbolMap* globalSymbolMap = nullptr;

SymbolMap::SymbolMap(const std::string& config, BackEnd backend, int optLevel)
{
    cmajor::ast::BackEnd astBackEnd = cmajor::ast::BackEnd::llvm;
    switch (backend)
    {
        case BackEnd::llvm:
        {
            astBackEnd = cmajor::ast::BackEnd::llvm;
            break;
        }
        case BackEnd::systemx:
        {
            astBackEnd = cmajor::ast::BackEnd::systemx;
            break;
        }
        case BackEnd::cpp:
        {
            astBackEnd = cmajor::ast::BackEnd::cpp;
            break;
        }
        case BackEnd::masm:
        {
            astBackEnd = cmajor::ast::BackEnd::masm;
            break;
        }
        case BackEnd::cm:
        {
            astBackEnd = cmajor::ast::BackEnd::cm;
            break;
        }
        case BackEnd::sbin:
        {
            astBackEnd = cmajor::ast::BackEnd::sbin;
            break;
        }
    }
    filePath = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorSystemLibDir(config, astBackEnd, optLevel), "symbol_map.xml"));
}

void SymbolMap::Read()
{
    if (!std::filesystem::exists(filePath)) return;
    std::unique_ptr<soul::xml::Document> doc = soul::xml::ParseXmlFile(filePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> fullNameNodeSet = soul::xml::xpath::EvaluateToNodeSet("/symbol_map/fullName", doc.get());
    int n = fullNameNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* fullNameNode = fullNameNodeSet->GetNode(i);
        if (fullNameNode->IsElementNode())
        {
            soul::xml::Element* fullNameElement = static_cast<soul::xml::Element*>(fullNameNode);
            std::string fullName = fullNameElement->GetAttribute("value");
            std::unique_ptr<soul::xml::xpath::NodeSet> mangledNameNodeSet = soul::xml::xpath::EvaluateToNodeSet("mangledName", fullNameElement);
            int m = mangledNameNodeSet->Count();
            for (int j = 0; j < m; ++j)
            {
                soul::xml::Node* mangledNameNode = mangledNameNodeSet->GetNode(j);
                if (mangledNameNode->IsElementNode())
                {
                    soul::xml::Element* mangledNameElement = static_cast<soul::xml::Element*>(mangledNameNode);
                    std::string mangledName = mangledNameElement->GetAttribute("value");
                    fullNameMangledNameMap[fullName].insert(mangledName);
                }
            }
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> mangledNameNodeSet = soul::xml::xpath::EvaluateToNodeSet("/symbol_map/mangledName", doc.get());
    int nm = mangledNameNodeSet->Count();
    for (int i = 0; i < nm; ++i)
    {
        soul::xml::Node* mangledNameNode = mangledNameNodeSet->GetNode(i);
        if (mangledNameNode->IsElementNode())
        {
            soul::xml::Element* mangledNameElement = static_cast<soul::xml::Element*>(mangledNameNode);
            std::string mangledName = mangledNameElement->GetAttribute("value");
            std::unique_ptr<soul::xml::xpath::NodeSet> fullNameNodeSet = soul::xml::xpath::EvaluateToNodeSet("fullName", mangledNameElement);
            int m = fullNameNodeSet->Count();
            for (int j = 0; j < m; ++j)
            {
                soul::xml::Node* fullNameNode = fullNameNodeSet->GetNode(j);
                if (fullNameNode->IsElementNode())
                {
                    soul::xml::Element* fullNameElement = static_cast<soul::xml::Element*>(fullNameNode);
                    std::string fullName = fullNameElement->GetAttribute("value");
                    mangledNameFullNameMap[mangledName].insert(fullName);
                }
            }
        }
    }
}

void SymbolMap::Write()
{
    soul::xml::Document doc;
    soul::xml::Element* root = soul::xml::MakeElement("symbol_map");
    doc.AppendChild(root);
    for (const auto& p : fullNameMangledNameMap)
    {
        const std::string& fullName = p.first;
        soul::xml::Element* fullNameElement = soul::xml::MakeElement("fullName");
        fullNameElement->SetAttribute("value", fullName);
        root->AppendChild(fullNameElement);
        for (const auto& mangledName : p.second)
        {
            soul::xml::Element* mangledNameElement = soul::xml::MakeElement("mangledName");
            mangledNameElement->SetAttribute("value", mangledName);
            fullNameElement->AppendChild(mangledNameElement);
        }
    }
    for (const auto& p : mangledNameFullNameMap)
    {
        const std::string& mangledName = p.first;
        soul::xml::Element* mangledNameElement = soul::xml::MakeElement("mangledName");
        mangledNameElement->SetAttribute("value", mangledName);
        root->AppendChild(mangledNameElement);
        for (const auto& fullName : p.second)
        {
            soul::xml::Element* fullNameElement = soul::xml::MakeElement("fullName");
            fullNameElement->SetAttribute("value", fullName);
            mangledNameElement->AppendChild(fullNameElement);
        }
    }
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
}

void SymbolMap::Add(FunctionSymbol* functionSymbol)
{
    std::lock_guard lock(symbolMapMtx);
    std::string fullName = functionSymbol->DigestSource();
    if (fullName.empty())
    {
        fullName = util::ToUtf8(functionSymbol->FullName());
    }
    std::string mangledName = util::ToUtf8(functionSymbol->MangledName());
    fullNameMangledNameMap[fullName].insert(mangledName);
    if (fullNameMangledNameMap[fullName].size() > 1)
    {
        std::cout << fullName << " : " << fullNameMangledNameMap[fullName].size() << "\n";
    }
    mangledNameFullNameMap[mangledName].insert(fullName);
}

void SymbolMap::Check()
{   
    std::cout << "checking symbol map:" << "\n";
    for (const auto& fn : fullNameMangledNameMap)
    {
        if (fn.second.size() != 1)
        {
            std::cout << fn.first << " count=" << fn.second.size() << "\n";
        }
    }
    for (const auto& fn : mangledNameFullNameMap)
    {
        if (fn.second.size() != 1)
        {
            std::cout << fn.first << " count=" << fn.second.size() << "\n";
        }
    }
    std::cout << "symbol map checked" << "\n";
}

SymbolMap* GetSymbolMap() 
{
    return globalSymbolMap;
}

void SetSymbolMap(SymbolMap* symbolMap)
{
    globalSymbolMap = symbolMap;
}

} // namespace cmajor::symbols
