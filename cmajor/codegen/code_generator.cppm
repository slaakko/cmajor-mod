// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.codegen.code.generator;

import cmajor.binder.bound.node.visitor;

export namespace cmajor::codegen {

class CodeGenerator : public cmajor::binder::BoundNodeVisitor
{
public:
    virtual ~CodeGenerator();
};

} // namespace cmajor::codegen
