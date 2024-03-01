// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.clone;

import cmajor.ast.function;
import cmajor.ast.statement;
import cmajor.ast.concepts;
import cmajor.ast.templates;
import cmajor.ast.identifier;
import cmajor.ast.parameter;

namespace cmajor::ast {

CloneContext::CloneContext() : instantiateFunctionNode(false), instantiateClassNode(false), makeTestUnits(false)
{
}

CloneContext::~CloneContext()
{
    for (FunctionNode* unitTestFunction : unitTestFunctions)
    {
        delete unitTestFunction;
    }
}

void CloneContext::AddUnitTestFunction(FunctionNode* unitTestFunction)
{
    unitTestFunctions.push_back(unitTestFunction);
}

std::vector<FunctionNode*>& CloneContext::UnitTestFunctions()
{
    return unitTestFunctions;
}

} // namespace cmajor::ast
