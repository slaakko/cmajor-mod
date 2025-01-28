// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.method.registry;

import std.core;

export namespace soul::json::rpc {

class Method;

void AddMethod(Method* method);
Method* GetMethod(const std::string& methodName);

} // namespace soul::json::rpc
