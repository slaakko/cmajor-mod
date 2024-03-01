// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.type.map;

import std.core;
import soul.xml.dom;

export namespace cmajor::symbols {

class TypeSymbol;

class TypeMap
{
public:
    TypeMap();
    TypeMap(const TypeMap&) = delete;
    TypeMap& operator=(const TypeMap&) = delete;
    int GetOrInsertType(TypeSymbol* type);
    std::vector<std::unique_ptr<soul::xml::Element>> TypeElements();
private:
    int nextClassTemplateSpecializationId;
    int nextOtherTypeId;
    std::unordered_map<TypeSymbol*, int> typeMap;
    std::map<int, std::unique_ptr<soul::xml::Element>> typeIdTypeElementMap;
};

} // namespace cmajor::symbols
