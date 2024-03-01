// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.serialization.class_registry;

import std.core;

export namespace soul::xml::serialization {

using FactoryFunction = void*(*)();

class ClassRegistry
{
public:
    static ClassRegistry& Instance();
    template<typename T>
    void Register()
    {
        factoryMap[T::StaticClassName()] = T::Create;
    }
    void* Create(const std::string& className);
private:
    ClassRegistry();
    std::map<std::string, FactoryFunction> factoryMap;
};

template<typename T>
void RegisterClass()
{
    ClassRegistry::Instance().Register<T>();
}

template<typename T>
T* CreateObject(const std::string& className)
{
    return static_cast<T*>(ClassRegistry::Instance().Create(className));
}

} // namespace soul::xml::serialization
