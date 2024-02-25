// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.interface;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.attribute;
import cmajor.fault.tolerant.ast.specifier;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.classes;

export namespace cmajor::fault::tolerant::ast {

class InterfaceNode : public SyntaxNode
{
public:
    InterfaceNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetInterfaceKeyword(InterfaceKeywordNode* interfaceKeyword_);
    InterfaceKeywordNode* InterfaceKeyword() const { return interfaceKeyword.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddMemberFunction(MemberFunctionNode* memberFunction);
    NodeList<MemberFunctionNode>* MemberFunctions() { return &memberFunctions; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<InterfaceKeywordNode> interfaceKeyword;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<MemberFunctionNode> memberFunctions;
    std::unique_ptr<RBraceNode> rbrace;
};

} // namespace cmajor::fault::tolerant::ast
