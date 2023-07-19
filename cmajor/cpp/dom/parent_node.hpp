// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_PARENT_NODE_INCLUDED
#define DOM_PARENT_NODE_INCLUDED
#include <dom/node.hpp>
#include <memory>

namespace soul::xml {

class ParentNode : public Node
{
public:
    ParentNode(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    ~ParentNode();
    virtual void AppendChild(Node* child);
    virtual void InsertBefore(Node* newChild, Node* refChild);
    virtual std::unique_ptr<Node> RemoveChild(Node* child);
    virtual std::unique_ptr<Node> ReplaceChild(Node* newChild, Node* oldChild);
    bool HasChildNodes() const final { return firstChild != nullptr; }
    Node* FirstChild() const { return firstChild; }
    Node* LastChild() const { return lastChild; }
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
    void WalkChildren(NodeOperation& operation) override;
    void WalkDescendant(NodeOperation& operation) override;
    void WalkDescendantOrSelf(NodeOperation& operation) override;
    void WalkPreceding(NodeOperation& operation) override;
    void WalkPrecedingOrSelf(NodeOperation& operation) override;
private:
    Node* firstChild;
    Node* lastChild;
};

} // namespace soul::xml

#endif // DOM_PARENT_NODE_INCLUDED
