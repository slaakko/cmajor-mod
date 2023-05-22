// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.action;

import std.core;
import cmajor.ast;

export namespace cmajor::build {

class Variable
{
public:
    Variable(const std::string& name_, const std::string& value_);
    const std::string& Name() const { return name; }
    const std::string& Value() const { return value; }
private:
    std::string name;
    std::string value;
};

class Variables
{
public:
    Variables();
    Variables(const Variables&) = delete;
    Variables& operator=(const Variables&) = delete;
    void AddVariable(Variable* variable);
    Variable* GetVariable(const std::string& name) const;
private:
    std::map<std::string, Variable*> variableMap;
    std::vector<std::unique_ptr<Variable>> variables;
};

void RunBuildActions(const cmajor::ast::Project& project, const Variables& variables);

} // namespace cmajor::build
