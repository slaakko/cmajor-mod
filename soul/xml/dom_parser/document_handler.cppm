// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.document.handler;

import std.core;
import soul.xml.document;
import soul.xml.element;
import soul.xml.content.handler;
import soul.xml.dom.parser;

export namespace soul::xml {

class DocumentHandlerData;

class DocumentHandler : public soul::xml::XmlContentHandler
{
public:
    DocumentHandler(soul::xml::ParsingFlags flags_);
    std::unique_ptr<soul::xml::Document> GetDocument() { return std::move(document); };
    void StartDocument(const soul::ast::SourcePos& sourcePos) override;
    void XmlVersion(const std::string& xmlVersion) override;
    void XmlEncoding(const std::string& xmlEncoding) override;
    void Standalone(bool standalone) override;
    void Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment) override;
    void PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data) override;
    void Text(const soul::ast::SourcePos& sourcePos, const std::string& text) override;
    void CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata) override;
    void StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName,
        const soul::xml::processor::Attributes& attributes) override;
    void EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName) override;
    void SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName) override;
private:
    void AddTextContent();
    void AddTextContent(bool addSpace);
    soul::xml::ParsingFlags flags;
    std::unique_ptr<soul::xml::Document> document;
    soul::xml::ParentNode* currentParentNode;
    std::stack<soul::xml::ParentNode*> parentNodeStack;
    std::unique_ptr<soul::xml::Element> currentElement;
    std::stack<std::unique_ptr<soul::xml::Element>> elementStack;
    soul::ast::SourcePos textSourcePos;
    std::string textContent;
};

} // namespace soul::xml
