// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.document.handler;

import soul.xml.dom;
import util;

namespace soul::xml {

DocumentHandler::DocumentHandler(soul::xml::ParsingFlags flags_) : currentParentNode(nullptr), flags(flags_)
{
}

void DocumentHandler::StartDocument(const soul::ast::SourcePos& sourcePos)
{
    document.reset(new soul::xml::Document(sourcePos));
    currentParentNode = document.get();
}

void DocumentHandler::XmlVersion(const std::string& xmlVersion)
{
    document->SetXmlVersion(xmlVersion);
}

void DocumentHandler::XmlEncoding(const std::string& xmlEncoding)
{
    document->SetXmlEncoding(xmlEncoding);
}

void DocumentHandler::Standalone(bool standalone)
{
    document->SetXmlStandalone(standalone);
}

void DocumentHandler::Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment)
{
    AddTextContent();
    currentParentNode->AppendChild(new soul::xml::Comment(sourcePos, comment));
}

void DocumentHandler::PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data)
{
    AddTextContent();
    currentParentNode->AppendChild(new soul::xml::ProcessingInstruction(sourcePos, target, data));
}

void DocumentHandler::Text(const soul::ast::SourcePos& sourcePos, const std::string& text)
{
    if (!textSourcePos.IsValid())
    {
        textSourcePos = sourcePos;
    }
    textContent.append(text);
}

void DocumentHandler::CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata)
{
    AddTextContent();
    currentParentNode->AppendChild(new soul::xml::CDataSection(sourcePos, cdata));
}

void DocumentHandler::StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName,
    const soul::xml::processor::Attributes& attributes)
{
    AddTextContent(true);
    parentNodeStack.push(currentParentNode);
    elementStack.push(std::move(currentElement));
    currentElement.reset(new soul::xml::Element(sourcePos, qualifiedName));
    currentParentNode = currentElement.get();
    for (const auto& attribute : attributes.GetAttributes())
    {
        currentElement->SetAttribute(sourcePos, attribute->QualifiedName(), attribute->Value());
    }
    currentElement->SetOwnerDocument(document.get());
    if (!namespaceUri.empty())
    {
        currentElement->SetNamespaceUri(namespaceUri);
    }
}

void DocumentHandler::EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName) 
{
    AddTextContent();
    currentParentNode = parentNodeStack.top();
    parentNodeStack.pop();
    currentParentNode->AppendChild(currentElement.release());
    currentElement = std::move(elementStack.top());
    elementStack.pop();
}

void DocumentHandler::SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName)
{
    if ((flags & ParsingFlags::generateWarnings) != ParsingFlags::none)
    {
        auto lexer = GetLexer();
        std::cout << "warning: skipped entity '" << entityName << "' in file " <<
        
            lexer->FileName() << " line " << std::to_string(sourcePos.line) << ":\n" << lexer->ErrorLines(sourcePos.pos) << std::endl;
    }
    AddTextContent();
    currentParentNode->AppendChild(MakeEntityReference(entityName));
}

void DocumentHandler::AddTextContent()
{
    AddTextContent(false);
}

void DocumentHandler::AddTextContent(bool addSpace)
{
    if (currentElement)
    {
        textContent = util::ToUtf8(util::TrimAll(util::ToUtf32(textContent)));
        if (!textContent.empty())
        {
            if (addSpace)
            {
                textContent.append(1, ' ');
            }
            currentElement->AppendChild(new soul::xml::Text(textSourcePos, textContent));
            textSourcePos = soul::ast::SourcePos();
        }
    }
    textContent.clear();
}

} // namespace soul::xml
