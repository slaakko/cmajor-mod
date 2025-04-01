// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.nodiscard.attribute.processor;

import util;

namespace cmajor::binder {

NoDiscardAttributeProcessor::NoDiscardAttributeProcessor() : AttributeProcessor(U"nodiscard")
{
}

void NoDiscardAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, cmajor::symbols::Context* context)
{
    if (symbol->IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
        if (attribute->Value() == U"true")
        {
            return;
        }
        else if (attribute->Value() == U"false")
        {
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("unknown attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" + util::ToUtf8(attribute->Name()) + "'",
                attribute->GetFullSpan());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("[nodiscard] attribute may be applied only to functions", attribute->GetFullSpan());
    }
    AttributeProcessor::TypeCheck(attribute, symbol, context);
}

} // namespace cmajor::binder
