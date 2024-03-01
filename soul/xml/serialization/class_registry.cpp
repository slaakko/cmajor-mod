// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.serialization.class_registry;

namespace soul::xml::serialization {

ClassRegistry& ClassRegistry::Instance()
{
    static ClassRegistry instance;
    return instance;
}

ClassRegistry::ClassRegistry()
{
}

void* ClassRegistry::Create(const std::string& className)
{
    auto it = factoryMap.find(className);
    if (it != factoryMap.cend())
    {
        FactoryFunction factoryFunction = it->second;
        return factoryFunction();
    }
    else
    {
        throw std::runtime_error("soul.xml.serialization.class_registry: error class '" + className + "' not registered");
    }
}

} // namespace soul::xml::serialization
