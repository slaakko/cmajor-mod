// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.system_default.attribute.processor;

import std.core;
import cmajor.binder.attribute.binder;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class SystemDefaultAttributeProcessor : public AttributeProcessor
{
public:
    SystemDefaultAttributeProcessor();
    void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol) override;
};

} // namespace cmajor::binder
