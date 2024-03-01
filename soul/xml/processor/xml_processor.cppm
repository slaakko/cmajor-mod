// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xml_processor;

import std.core;
import soul.xml.content.handler;
import soul.xml.processor.attribute;
import soul.ast.source.pos;
import soul.lexer.base;

export namespace soul::xml::processor {

struct Tag
{
    Tag() : sourcePos(), name() {}
    Tag(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : sourcePos(sourcePos_), name(name_) {}
    soul::ast::SourcePos sourcePos;
    std::string name;
};

class XmlProcessor
{
public:
    XmlProcessor(soul::xml::XmlContentHandler* contentHandler_);
    virtual ~XmlProcessor();
    virtual void StartDocument(soul::lexer::LexerBase<char32_t>* lexer_, const soul::ast::SourcePos& sourcePos);
    virtual void EndDocument();
    virtual void XmlVersion(const std::string& xmlVersion);
    virtual void XmlEncoding(const std::string& xmlEncoding);
    virtual void Standalone(bool standalone);
    virtual void Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment);
    virtual void PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data);
    virtual void BeginStartTag(const soul::ast::SourcePos& sourcePos, const std::string& tagName);
    virtual void EndStartTag();
    virtual void EndTag(const soul::ast::SourcePos& sourcePos, const std::string& tagName);
    virtual void AddAttribute(const soul::ast::SourcePos& sourcePos, const std::string& attName, const std::string& attValue);
    virtual void BeginAttributeValue();
    virtual void EndAttributeValue();
    virtual void AddAttValueChar(char32_t chr);
    virtual std::string AttValue() const;
    virtual void CharRef(const soul::ast::SourcePos& sourcePos, char32_t chr);
    virtual void EntityRef(const soul::ast::SourcePos& sourcePos, const std::string& entityName);
    virtual void Text(const soul::ast::SourcePos& sourcePos, const std::string& text);
    virtual void CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata);
private:
    void ParseQualifiedName(const soul::ast::SourcePos& sourcePos, const std::string& qualifiedName, std::string& localName, std::string& prefix) const;
    std::string GetNamespaceUri(const std::string& namespacePrefix, const soul::ast::SourcePos& sourcePos) const;
    soul::lexer::LexerBase<char32_t>* lexer;
    soul::xml::XmlContentHandler* contentHandler;
    std::stack<Tag> tagStack;
    Tag currentTag;
    soul::xml::processor::Attributes attributes;
    std::u32string attValue;
    std::stack<std::u32string> attValueStack;
    std::string currentNamespaceUri;
    std::stack<std::string> namespaceUriStack;
    std::string currentNamespacePrefix;
    std::stack<std::string> namespacePrefixStack;
    std::map<std::string, std::string> namespacePrefixMap;
};

} // namespace soul::xml::processor
