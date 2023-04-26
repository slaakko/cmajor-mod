// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.xml_processor;

import soul.xml.error;
import soul.xml.processor.entity;
import util;

namespace soul::xml::processor {

XmlProcessor::XmlProcessor(soul::xml::XmlContentHandler* contentHandler_) : lexer(nullptr), contentHandler(contentHandler_)
{
    if (!contentHandler)
    {
        throw std::runtime_error("error: XmlProcessor: content handler is null");
    }
}

XmlProcessor::~XmlProcessor()
{
}

void XmlProcessor::StartDocument(soul::lexer::LexerBase<char32_t>* lexer_, const soul::ast::SourcePos& sourcePos)
{
    lexer = lexer_;
    contentHandler->SetLexer(lexer);
    contentHandler->StartDocument(sourcePos);
}

void XmlProcessor::EndDocument()
{
    contentHandler->EndDocument();
}

void XmlProcessor::XmlVersion(const std::string& xmlVersion)
{
    contentHandler->XmlVersion(xmlVersion);
}

void XmlProcessor::XmlEncoding(const std::string& xmlEncoding)
{
    contentHandler->XmlEncoding(xmlEncoding);
}

void XmlProcessor::Standalone(bool standalone)
{
    contentHandler->Standalone(standalone);
}

void XmlProcessor::Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment)
{
    contentHandler->Comment(sourcePos, comment);
}

void XmlProcessor::PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data)
{
    contentHandler->PI(sourcePos, target, data);
}

void XmlProcessor::BeginStartTag(const soul::ast::SourcePos& sourcePos, const std::string& tagName)
{
    tagStack.push(currentTag);
    currentTag = Tag(sourcePos, tagName);
    attributes.Clear();
    namespaceUriStack.push(currentNamespaceUri);
    namespacePrefixStack.push(currentNamespacePrefix);
}

void XmlProcessor::EndStartTag()
{
    std::string localName;
    std::string prefix;
    ParseQualifiedName(currentTag.sourcePos, currentTag.name, localName, prefix);
    if (prefix == "xmlns")
    {
        throw XmlException("error: 'xmlns' prefix cannot be declared for an element in file " + 
            lexer->FileName() + " line " + std::to_string(currentTag.sourcePos.line) + ":\n" + lexer->ErrorLines(currentTag.sourcePos.pos), currentTag.sourcePos);
    }
    std::string namespaceUri = GetNamespaceUri(prefix, currentTag.sourcePos);
    contentHandler->StartElement(currentTag.sourcePos, namespaceUri, localName, currentTag.name, attributes);
}

void XmlProcessor::EndTag(const soul::ast::SourcePos& sourcePos, const std::string& tagName)
{
    if (tagStack.empty())
    {
        throw soul::xml::XmlException("error: end tag '" + tagName + "' has no corresponding start tag in file " + 
            lexer->FileName() + " line " + std::to_string(sourcePos.line) + ":\n" + lexer->ErrorLines(sourcePos.pos), sourcePos);
    }
    if (tagName != currentTag.name)
    {
        throw soul::xml::XmlException("error: end tag '" + tagName + "' does not match start tag '" + currentTag.name + "' in file " +
            lexer->FileName() + " line " + std::to_string(sourcePos.line) + ":\n" + lexer->ErrorLines(sourcePos.pos) + 
            "\nsee reference line " + std::to_string(currentTag.sourcePos.line) + ":\n" + lexer->ErrorLines(currentTag.sourcePos.pos),
            sourcePos);
    }
    std::string localName;
    std::string prefix;
    ParseQualifiedName(sourcePos, currentTag.name, localName, prefix);
    if (prefix == "xmlns")
    {
        throw XmlException("error: 'xmlns' prefix cannot be declared for an element in file " +
            lexer->FileName() + " line " + std::to_string(sourcePos.line) + ":\n" + lexer->ErrorLines(sourcePos.pos), sourcePos);
    }
    std::string namespaceUri = GetNamespaceUri(prefix, sourcePos);
    contentHandler->EndElement(namespaceUri, localName, currentTag.name);
    if (namespaceUriStack.empty())
    {
        throw std::runtime_error("namespace URI stack is empty");
    }
    currentNamespaceUri = namespaceUriStack.top();
    namespaceUriStack.pop();
    namespacePrefixMap.erase(currentNamespacePrefix);
    if (namespacePrefixStack.empty())
    {
        throw std::runtime_error("namespace prefix stack is empty");
    }
    currentNamespacePrefix = namespacePrefixStack.top();
    namespacePrefixStack.pop();
    namespacePrefixMap[currentNamespacePrefix] = currentNamespaceUri;
    currentTag = tagStack.top();
    tagStack.pop();
}

void XmlProcessor::AddAttribute(const soul::ast::SourcePos& sourcePos, const std::string& attName, const std::string& attValue)
{
    std::string localName;
    std::string prefix;
    ParseQualifiedName(sourcePos, attName, localName, prefix);
    if (prefix == "xmlns")
    {
        currentNamespacePrefix = localName;
        currentNamespaceUri = attValue;
        namespacePrefixMap[currentNamespacePrefix] = currentNamespaceUri;
    }
    else if (localName == "xmlns")
    {
        currentNamespacePrefix.clear();
        currentNamespaceUri = attValue;
    }
    std::string namespaceUri = GetNamespaceUri(prefix, sourcePos);
    attributes.AddAttribute(new Attribute(sourcePos, namespaceUri, localName, attName, attValue));
}

void XmlProcessor::BeginAttributeValue()
{
    attValueStack.push(attValue);
    attValue.clear();
}

void XmlProcessor::EndAttributeValue()
{
    if (attValueStack.empty())
    {
        throw std::runtime_error("attribute value stack is empty");
    }
    attValue = attValueStack.top();
    attValueStack.pop();
}

void XmlProcessor::AddAttValueChar(char32_t chr)
{
    attValue.append(1, chr);
}

std::string XmlProcessor::AttValue() const
{
    return util::ToUtf8(attValue);
}

void XmlProcessor::CharRef(const soul::ast::SourcePos& sourcePos, char32_t chr)
{
    if (attValueStack.empty())
    {
        Text(sourcePos, util::ToUtf8(std::u32string(1, chr)));
    }
    else
    {
        AddAttValueChar(chr);
    }
}

void XmlProcessor::EntityRef(const soul::ast::SourcePos& sourcePos, const std::string& entityName)
{
    Entities& entities = Entities::Instance();
    Entity* entity = entities.GetEntity(entityName);
    if (entity)
    {
        if (entity->IsInternal())
        {
            if (attValueStack.empty())
            {
                Text(sourcePos, util::ToUtf8(entity->Value()));
            }
            else
            {
                attValue.append(entity->Value());
            }
        }
        else
        {
            contentHandler->SkippedEntity(sourcePos, entityName);
        }
    }
    else
    {
        contentHandler->SkippedEntity(sourcePos, entityName);
    }
}

void XmlProcessor::Text(const soul::ast::SourcePos& sourcePos, const std::string& text)
{
    contentHandler->Text(sourcePos, text);
}

void XmlProcessor::CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata)
{
    contentHandler->CDataSection(sourcePos, cdata);
}

void XmlProcessor::ParseQualifiedName(const soul::ast::SourcePos& sourcePos, const std::string& qualifiedName, std::string& localName, std::string& prefix) const
{
    std::vector<std::string> components = util::Split(qualifiedName, ':');
    if (components.size() > 2)
    {
        throw XmlException("error: qualified name '" + qualifiedName + "' has more than one ':' character in file " +
            lexer->FileName() + " line " + std::to_string(sourcePos.line) + ":\n" + lexer->ErrorLines(sourcePos.pos), sourcePos);
    }
    else if (components.size() == 2)
    {
        prefix = components[0];
        localName = components[1];
    }
    else
    {
        prefix.clear();
        localName = qualifiedName;
    }
}

std::string XmlProcessor::GetNamespaceUri(const std::string& namespacePrefix, const soul::ast::SourcePos& sourcePos) const
{
    if (namespacePrefix.empty())
    {
        return currentNamespaceUri;
    }
    auto it = namespacePrefixMap.find(namespacePrefix);
    if (it != namespacePrefixMap.cend())
    {
        return it->second;
    }
    else
    {
        if (namespacePrefix != "xml")
        {
            throw XmlException("error: namespace prefix '" + namespacePrefix + "' not bound to any namespace URI in file " +
                lexer->FileName() + " line " + std::to_string(sourcePos.line) + ":\n" + lexer->ErrorLines(sourcePos.pos), sourcePos);
        }
    }
    return std::string();
}

} // namespace soul::xml::processor
