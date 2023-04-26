// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.type.map;

import util;

namespace cmajor::symbols {

TypeMap::TypeMap() : nextClassTemplateSpecializationId(1), nextOtherTypeId(1000000)
{
}
/*
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
        std::unique_ptr<sngxml::dom::Element> typeElement(new sngxml::dom::Element(U"type"));
        typeElement->SetAttribute(U"id", U"type_" + ToUtf32(std::to_string(typeId)));
        typeElement->AppendChild(std::move(type->ToDomElement(*this)));
        typeIdTypeElementMap[typeId] = std::move(typeElement);
        return typeId;
    }
}
*/

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
