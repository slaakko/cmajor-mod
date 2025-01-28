// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.nodiscard.attribute.processor;

import std.core;
import cmajor.binder.attribute.binder;
import cmajor.symbols;

export namespace cmajor::binder {

class NoDiscardAttributeProcessor : public AttributeProcessor
{
public:
    NoDiscardAttributeProcessor();
    void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol) override;
};

} // namespace cmajor::binder
