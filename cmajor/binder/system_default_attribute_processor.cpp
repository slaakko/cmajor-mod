// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.system_default.attribute.processor;

import util;

namespace cmajor::binder {

SystemDefaultAttributeProcessor::SystemDefaultAttributeProcessor() : AttributeProcessor(U"system_default")
{
}

void SystemDefaultAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, cmajor::symbols::Context* context)
{
    if (symbol->IsFunctionSymbol())
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
        if (attribute->Value().empty() || attribute->Value() == U"true")
        {
            functionSymbol->SetSystemDefault();
        }
        else if (attribute->Value() != U"false")
        {
            throw cmajor::symbols::Exception("unknown attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" +
                util::ToUtf8(attribute->Name()) + "'", attribute->GetFullSpan());
        }
    }
    else
    {
        AttributeProcessor::TypeCheck(attribute, symbol, context);
    }
}

} // namespace cmajor::binder
