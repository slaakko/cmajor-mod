// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef PROCESSOR_CONTENT_HANDLER_INCLUDED
#define PROCESSOR_CONTENT_HANDLER_INCLUDED
#include <processor/attribute.hpp>
#include <lexer/base.hpp>

namespace soul::xml {

class XmlContentHandler
{
public:
    XmlContentHandler();
    soul::lexer::LexerBase<char32_t>* GetLexer() { return lexer; }
    void SetLexer(soul::lexer::LexerBase<char32_t>* lexer_) { lexer = lexer_; }
    virtual ~XmlContentHandler();
    virtual void StartDocument(const soul::ast::SourcePos& sourcePos) {}
    virtual void EndDocument() {}
    virtual void XmlVersion(const std::string& xmlVersion) {}
    virtual void XmlEncoding(const std::string& xmlEncoding) {}
    virtual void Standalone(bool standalone) {}
    virtual void Comment(const soul::ast::SourcePos& sourcePos, const std::string& comment) {}
    virtual void PI(const soul::ast::SourcePos& sourcePos, const std::string& target, const std::string& data) {}
    virtual void Text(const soul::ast::SourcePos& sourcePos, const std::string& text) {}
    virtual void CDataSection(const soul::ast::SourcePos& sourcePos, const std::string& cdata) {}
    virtual void StartElement(const soul::ast::SourcePos& sourcePos, const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName,
        const soul::xml::processor::Attributes& attributes) {}
    virtual void EndElement(const std::string& namespaceUri, const std::string& localName, const std::string& qualifiedName) {}
    virtual void SkippedEntity(const soul::ast::SourcePos& sourcePos, const std::string& entityName) {}
private:
    soul::lexer::LexerBase<char32_t>* lexer;
};

} // namespace soul::xml

#endif // PROCESSOR_CONTENT_HANDLER_INCLUDED