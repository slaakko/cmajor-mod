// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <dom/index.hpp>
#include <dom/visitor.hpp>
#include <dom/element.hpp>
#include <dom/document.hpp>
#include <string>
#include <map>

namespace soul::xml {

class BuildIndexVisitor : public Visitor
{
public:
    BuildIndexVisitor(std::map<std::string, Element*>& index_);
    void BeginVisit(Element& element) override;
private:
    std::map<std::string, Element*>& index;
};

BuildIndexVisitor::BuildIndexVisitor(std::map<std::string, Element*>& index_) : index(index_)
{
}

void BuildIndexVisitor::BeginVisit(Element& element)
{
    std::string id = element.GetAttribute("id");
    if (!id.empty())
    {
        index[id] = &element;
    }
}

void BuildIndex(Document* document)
{
    BuildIndexVisitor visitor(document->Index());
    document->Accept(visitor);
}

} // namespace soul::xml
