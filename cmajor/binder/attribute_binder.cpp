// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.attribute.binder;

import cmajor.binder.json.attribute.processor;
import cmajor.binder.xml.attribute.processor;
import cmajor.binder.system_default.attribute.processor;

import util;

namespace cmajor::binder {

AttributeProcessor::AttributeProcessor(const std::u32string& attributeName_) : attributeName(attributeName_)
{
}

AttributeProcessor::~AttributeProcessor()
{
}

void AttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol)
{
    throw cmajor::symbols::Exception("attribute '" + util::ToUtf8(attribute->Name()) + "' for symbol type '" + symbol->TypeString() + "' not supported", 
        attribute->GetFullSpan(), symbol->GetFullSpan());
}

void AttributeProcessor::GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope)
{
}

void AttributeProcessor::GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder)
{
}

AttributeBinder::AttributeBinder(cmajor::symbols::Module* module)
{
    AttributeProcessor* jsonAttributeProcessor = new JsonAttributeProcessor(module);
    attributeProcessors.push_back(std::unique_ptr<AttributeProcessor>(jsonAttributeProcessor));
    JsonFieldNameAttributeProcessor* jsonFieldNameAttributeProcessor = new JsonFieldNameAttributeProcessor();
    attributeProcessors.push_back(std::unique_ptr<AttributeProcessor>(jsonFieldNameAttributeProcessor));
    AttributeProcessor* systemDefaultAttributeProcessor = new SystemDefaultAttributeProcessor();
    attributeProcessors.push_back(std::unique_ptr<AttributeProcessor>(systemDefaultAttributeProcessor));
    XmlAttributeProcessor* xmlAttributeProcessor = new XmlAttributeProcessor();
    attributeProcessors.push_back(std::unique_ptr<AttributeProcessor>(xmlAttributeProcessor));
    for (const std::unique_ptr<AttributeProcessor>& attributeProcessor : attributeProcessors)
    {
        attributeProcessorMap[attributeProcessor->AttributeName()] = attributeProcessor.get();
    }
}

void AttributeBinder::BindAttributes(cmajor::ast::AttributesNode* attrs, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope)
{
    if (!attrs) return;
    const std::vector<std::unique_ptr<cmajor::ast::AttributeNode>>& attributes = attrs->GetAttributes();
    for (const std::unique_ptr<cmajor::ast::AttributeNode>& attribute : attributes)
    {
        const std::u32string& attrName = attribute->Name();
        auto it = attributeProcessorMap.find(attrName);
        if (it != attributeProcessorMap.cend())
        {
            AttributeProcessor* processor = it->second;
            processor->TypeCheck(attribute.get(), symbol);
            processor->GenerateSymbols(attribute.get(), symbol, boundCompileUnit, containerScope);
        }
        else
        {
            throw cmajor::symbols::Exception("unknown attribute '" + util::ToUtf8(attrName) + "'", attribute->GetFullSpan());
        }
    }
    cmajor::ast::CloneContext cloneContext;
    symbol->SetAttributes(std::unique_ptr<cmajor::ast::AttributesNode>(static_cast<cmajor::ast::AttributesNode*>(attrs->Clone(cloneContext))));
}

void AttributeBinder::GenerateImplementation(cmajor::ast::AttributesNode* attrs, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder)
{
    if (!attrs) return;
    const std::vector<std::unique_ptr<cmajor::ast::AttributeNode>>& attributes = attrs->GetAttributes();
    for (const std::unique_ptr<cmajor::ast::AttributeNode>& attribute : attributes)
    {
        const std::u32string& attrName = attribute->Name();
        auto it = attributeProcessorMap.find(attrName);
        if (it != attributeProcessorMap.cend())
        {
            AttributeProcessor* processor = it->second;
            processor->GenerateImplementation(attribute.get(), symbol, statementBinder);
        }
        else
        {
            throw cmajor::symbols::Exception("unknown attribute '" + util::ToUtf8(attrName) + "'", attribute->GetFullSpan());
        }
    }
}

} // namespace cmajor::binder
