// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.processor.attribute;

import std.core;
import soul.ast.source.pos;

export namespace soul::xml::processor {

class Attribute
{
public:
    Attribute(const soul::ast::SourcePos& sourcePos_, const std::string& namespaceUri_, const std::string& localName_, const std::string& qualifiedName_, const std::string& value_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    const std::string& NamespaceUri() const { return namespaceUri; }
    const std::string& LocalName() const { return localName; }
    const std::string& QualifiedName() const { return qualifiedName; }
    const std::string& Value() const { return value; }
    std::string ToString() const;
private:
    soul::ast::SourcePos sourcePos;
    std::string namespaceUri;
    std::string localName;
    std::string qualifiedName;
    std::string value;
};

class Attributes
{
public:
    Attributes();
    void Clear();
    void AddAttribute(Attribute* attribute);
    const std::vector<std::unique_ptr<Attribute>>& GetAttributes() const { return attributes; }
    const std::string* GetAttributeValue(const std::string& qualifiedName) const;
    std::string ToString() const;
private:
    std::vector<std::unique_ptr<Attribute>> attributes;
};

} // namespace soul::xml::processor
