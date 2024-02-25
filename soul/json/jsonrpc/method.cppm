// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.method;

import std.core;
import util;

export namespace soul::json::rpc {

class Parameter
{
public:
    Parameter(util::JsonValueType type_, const std::string& name_, bool optional_);
    util::JsonValueType Type() const { return type; }
    const std::string& Name() const { return name; }
    bool IsOptional() const { return optional; }
private:
    util::JsonValueType type; 
    std::string name;
    bool optional;
};

class ExecutionContext
{
public:
    ExecutionContext();
    void SetWait() { wait = true; }
    bool Wait() const { return wait; }
    void SetExit() { exit = true; }
    bool Exit() const { return exit; }
private:
    bool wait;
    bool exit;
};

class Method
{
public:
    Method(const std::string& name_);
    virtual ~Method();
    void Validate(util::JsonValue* params);
    std::unique_ptr<util::JsonValue> Execute(ExecutionContext& context, util::JsonValue* params);
    virtual std::unique_ptr<util::JsonValue> ExecutePositional(ExecutionContext& context, util::JsonArray* params);
    virtual std::unique_ptr<util::JsonValue> ExecuteNamed(ExecutionContext& context, util::JsonObject* params);
    const std::string& Name() const { return name; }
    void AddParameter(const Parameter& parameter);
    int Arity() const { return parameters.size(); }
    int MinArity() const { return minArity; }
    const std::vector<Parameter>& Parameters() const { return parameters; }
private:
    void ValidatePositional(util::JsonArray* params);
    void ValidateNamed(util::JsonObject* params);
    std::string name;
    std::vector<Parameter> parameters;
    int minArity;
};

} // namespace soul::json::rpc
