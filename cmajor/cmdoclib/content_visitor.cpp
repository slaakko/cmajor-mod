// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.content_visitor;

import util;

namespace cmdoclib {

ContentVisitor::ContentVisitor(Input* input_, soul::xml::Element* parentElement_, soul::xml::Document* moduleXmlDoc_, 
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs_, const std::string& prefix_) :
    input(input_), parentElement(parentElement_), moduleXmlDoc(moduleXmlDoc_), otherModuleXmlDocs(otherModuleXmlDocs_), hasContent(false), prefix(prefix_),
    inRef(false), space(false)
{
}

void ContentVisitor::BeginVisit(soul::xml::Element& element)
{
    currentElementStack.push(std::move(currentElement));
    inRefStack.push(inRef);
    inRef = false;
    if (element.Name() == "ref")
    {
        if (element.ChildNodes().size() != 1)
        {
            throw std::runtime_error("ref: one text child node with nonempty id expected");
        }
        else
        {
            soul::xml::Node* child = element.ChildNodes()[0];
            if (child->IsTextNode())
            {
                soul::xml::Text* text = static_cast<soul::xml::Text*>(child);
                std::string id = text->Data();
                if (!id.empty())
                {
                    std::string libraryPrefix;
                    std::string link = prefix;
                    soul::xml::Element* element = nullptr;
                    if (moduleXmlDoc)
                    {
                        element = moduleXmlDoc->GetElementById(id);
                    }
                    if (!element)
                    {
                        int n = static_cast<int>(otherModuleXmlDocs.size());
                        for (int i = 0; i < n; ++i)
                        {
                            soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                            element = otherModuleXmlDoc->GetElementById(id);
                            if (element)
                            {
                                std::string moduleName = otherModuleXmlDoc->DocumentElement()->GetAttribute("module");
                                auto it = input->libraryPrefixMap.find(moduleName);
                                if (it != input->libraryPrefixMap.cend())
                                {
                                    libraryPrefix = it->second;
                                    if (!libraryPrefix.empty())
                                    {
                                        libraryPrefix = util::Path::Combine("../../..", libraryPrefix);
                                    }
                                }
                                break;
                            }
                        }
                    }
                    std::string text;
                    if (!element)
                    {
                        text = "ref: id '" + id + "' not found";
                    }
                    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                    if (element && element->Name() == "symbolTable")
                    {
                        link.append("../../").append(id).append("/index.html");
                        text = id;
                    }
                    else if (element)
                    {
                        if (libraryPrefix.empty())
                        {
                            link.append(element->GetAttribute("docPath"));
                        }
                        else
                        {
                            link.append(util::Path::Combine(libraryPrefix, element->GetAttribute("extPath")));
                        }
                        text = element->GetAttribute("name");
                    }
                    linkElement->SetAttribute("href", link);
                    linkElement->AppendChild(soul::xml::MakeText(text));
                    if (hasContent)
                    {
                        parentElement->AppendChild(soul::xml::MakeText(" "));
                    }
                    parentElement->AppendChild(linkElement.release());
                    hasContent = true;
                    inRef = true;
                }
                else
                {
                    throw std::runtime_error("ref: one text child node with nonempty id expected");
                }
            }
            else
            {
                throw std::runtime_error("ref: one text child node with nonempty id expected");
            }
        }
    }
    else if (element.Name() != "desc" && element.Name() != "details")
    {
        if (space)
        {
            parentElement->AppendChild(soul::xml::MakeText(" "));
        }
        currentElement.reset(static_cast<soul::xml::Element*>(element.Clone(false)));
        elementStack.push(parentElement);
        parentElement = currentElement.get();
        hasContent = true;
        space = false;
    }
}

void ContentVisitor::EndVisit(soul::xml::Element& element)
{
    inRef = inRefStack.top();
    inRefStack.pop();
    if (currentElement)
    {
        parentElement = elementStack.top();
        elementStack.pop();
        parentElement->AppendChild(currentElement.release());
    }
    currentElement = std::move(currentElementStack.top());
    currentElementStack.pop();
}

void ContentVisitor::Visit(soul::xml::Text& text)
{
    space = false;
    if (inRef) return;
    if (!text.Data().empty())
    {
        if (text.Data()[text.Data().size() - 1] == ' ')
        {
            space = true;
        }
    }
    std::string textContent = util::Trim(text.Data());
    if (!textContent.empty())
    {
        std::string s;
        if (hasContent)
        {
            s.append(" ");
        }
        s.append(textContent);
        parentElement->AppendChild(soul::xml::MakeText(s));
        hasContent = true;
    }
}

void ContentVisitor::Visit(soul::xml::CDataSection& cdataSection)
{
    if (space)
    {
        parentElement->AppendChild(soul::xml::MakeText(" "));
    }
    parentElement->AppendChild(cdataSection.Clone(true));
    hasContent = true;
    space = false;
}

void ContentVisitor::Visit(soul::xml::EntityReference& entityReference)
{
    if (space)
    {
        parentElement->AppendChild(soul::xml::MakeText(" "));
    }
    parentElement->AppendChild(entityReference.Clone(true));
    hasContent = true;
    space = entityReference.Data() == "nbsp";
}

} // namespace cmdoclib
