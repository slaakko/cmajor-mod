// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.type.map;

import cmajor.symbols.type.symbol;
import util;

namespace cmajor::symbols {

TypeMap::TypeMap() : nextClassTemplateSpecializationId(1), nextOtherTypeId(1000000)
{
}

int TypeMap::GetOrInsertType(TypeSymbol* type)
{
    auto it = typeMap.find(type);
    if (it != typeMap.cend())
    {
        return it->second;
    }
    else
    {
        int typeId = 0;
        if (type->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            typeId = nextClassTemplateSpecializationId++;
        }
        else
        {
            typeId = nextOtherTypeId++;
        }
        typeMap[type] = typeId;
        std::unique_ptr<soul::xml::Element> typeElement(soul::xml::MakeElement("type"));
        typeElement->SetAttribute("id", "type_" + std::to_string(typeId));
        std::unique_ptr<soul::xml::Element> domElement = type->ToDomElement(*this);
        typeElement->AppendChild(domElement.release());
        typeIdTypeElementMap[typeId] = std::move(typeElement);
        return typeId;
    }
}

std::vector<std::unique_ptr<soul::xml::Element>> TypeMap::TypeElements()
{
    std::vector<std::unique_ptr<soul::xml::Element>> typeElements;
    for (auto& p : typeIdTypeElementMap)
    {
        typeElements.push_back(std::move(p.second));
    }
    return typeElements;
}

} // namespace cmajor::symbols
