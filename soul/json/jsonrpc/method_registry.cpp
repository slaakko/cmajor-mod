// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.method.registry;

import soul.json.rpc.method;
import soul.json.rpc.error;

namespace soul::json::rpc {

class MethodRegistry
{
public:
    static MethodRegistry& Instance();
    void AddMethod(Method* method);
    Method* GetMethod(const std::string& methodName) const;
private:
    std::vector<std::unique_ptr<Method>> methods;
    std::map<std::string, Method*> methodMap;
};

MethodRegistry& MethodRegistry::Instance()
{
    static MethodRegistry instance;
    return instance;
}

void MethodRegistry::AddMethod(Method* method)
{
    auto it = methodMap.find(method->Name());
    if (it != methodMap.end())
    {
        throw std::runtime_error("method '" + method->Name() + "' already registered");
    }
    methods.push_back(std::unique_ptr<Method>(method));
    methodMap[method->Name()] = method;
}

Method* MethodRegistry::GetMethod(const std::string& methodName) const
{
    auto it = methodMap.find(methodName);
    if (it != methodMap.end())
    {
        return it->second;
    }
    else
    {
        throw JsonRpcException(methodNotFound, "method '" + methodName + "' not found");
    }
}

void AddMethod(Method* method)
{
    MethodRegistry::Instance().AddMethod(method);
}

Method* GetMethod(const std::string& methodName)
{
    return MethodRegistry::Instance().GetMethod(methodName);
}

} // namespace soul::json::rpc
