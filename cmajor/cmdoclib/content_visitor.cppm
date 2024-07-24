// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.content_visitor;

import cmdoclib.input;
import soul.xml.dom;
import std.core;

export namespace cmdoclib {

class ContentVisitor : public soul::xml::Visitor
{
public:
    ContentVisitor(Input* input_, soul::xml::Element* parentElement_, soul::xml::Document* moduleXmlDoc_, const std::vector<soul::xml::Document*>& otherModuleXmlDocs_, 
        const std::string& prefix_);
    void BeginVisit(soul::xml::Element& element) override;
    void EndVisit(soul::xml::Element& element) override;
    void Visit(soul::xml::Text& text) override;
    void Visit(soul::xml::CDataSection& cdataSection)  override;
    void Visit(soul::xml::EntityReference& entityReference) override;
private:
    Input* input;
    soul::xml::Element* parentElement;
    std::stack<soul::xml::Element*> elementStack;
    std::stack<std::unique_ptr<soul::xml::Element>> currentElementStack;
    std::unique_ptr<soul::xml::Element> currentElement;
    soul::xml::Document* moduleXmlDoc;
    std::vector<soul::xml::Document*> otherModuleXmlDocs;
    bool hasContent;
    std::string prefix;
    std::stack<bool> inRefStack;
    bool inRef;
    bool space;
};

} // namespace cmdoclib

