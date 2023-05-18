// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.clone;

import std.core;

export namespace cmajor::ast {

class FunctionNode;

class CloneContext
{
public:
    CloneContext();
    ~CloneContext();
    void SetInstantiateFunctionNode() { instantiateFunctionNode = true; }
    bool InstantiateFunctionNode() const { return instantiateFunctionNode; }
    void SetInstantiateClassNode() { instantiateClassNode = true; }
    bool InstantiateClassNode() const { return instantiateClassNode; }
    void SetMakeTestUnits() { makeTestUnits = true; }
    bool MakeTestUnits() const { return makeTestUnits; }
    void AddUnitTestFunction(FunctionNode* unitTestFunction);
    std::vector<FunctionNode*>& UnitTestFunctions();
private:
    bool instantiateFunctionNode;
    bool instantiateClassNode;
    bool makeTestUnits;
    std::vector<FunctionNode*> unitTestFunctions;
};

} // namespace cmajor::ast
