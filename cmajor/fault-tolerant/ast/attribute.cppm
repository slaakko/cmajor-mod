// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.attribute;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;

export namespace cmajor::fault::tolerant::ast {

class AttributeNode : public SyntaxNode
{
public:
    AttributeNode(IdentifierNode* name_, Node* value_);
    IdentifierNode* Name() const { return name.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    Node* Value() const { return value.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> name;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<Node> value;
};

class AttributeListNode : public ListNode
{
public:
    AttributeListNode();
    void AddAttribute(AttributeNode* attribute);
    const std::vector<AttributeNode*>& Attributes() const { return attributes; }
private:
    std::vector<AttributeNode*> attributes;
};

class AttributesNode : public SyntaxNode
{
public:
    AttributesNode();
    void SetLBracket(LBracketNode* lbracket_);
    LBracketNode* LBracket() const { return lbracket.get(); }
    void SetRBracket(RBracketNode* rbracket_);
    RBracketNode* RBracket() const { return rbracket.get(); }
    AttributeListNode* AttributeList() { return &attributeList; }
    void AddAttribute(AttributeNode* attribute);
    void AddComma(CommaNode* comma);
    AttributeNode* GetAttribute(const std::u16string& name) const;
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LBracketNode> lbracket;
    std::unique_ptr<RBracketNode> rbracket;
    AttributeListNode attributeList;
    std::map<std::u16string, AttributeNode*> attributeMap;
};

} // namespace cmajor::fault::tolerant::ast

