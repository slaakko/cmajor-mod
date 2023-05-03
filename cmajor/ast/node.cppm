// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.node;

import std.core;
import soul.ast.source.pos;
import soul.lexer;
import cmajor.ast.specifier;
import cmajor.ast.clone;
import util.uuid;

export namespace cmajor::ast {

enum class NodeType : uint8_t
{
    autoNode, boolNode, sbyteNode, byteNode, shortNode, ushortNode, intNode, uintNode, longNode, ulongNode, floatNode, doubleNode, charNode, wcharNode, ucharNode, voidNode,
    booleanLiteralNode, sbyteLiteralNode, byteLiteralNode, shortLiteralNode, ushortLiteralNode, intLiteralNode, uintLiteralNode, longLiteralNode, ulongLiteralNode,
    floatLiteralNode, doubleLiteralNode, charLiteralNode, wcharLiteralNode, ucharLiteralNode, stringLiteralNode, wstringLiteralNode, ustringLiteralNode, nullLiteralNode, 
    arrayLiteralNode, structuredLiteralNode,
    compileUnitNode, namespaceNode, aliasNode, namespaceImportNode, identifierNode, templateIdNode, functionNode,
    classNode, thisInitializerNode, baseInitializerNode, memberInitializerNode, staticConstructorNode, constructorNode, destructorNode, memberFunctionNode, conversionFunctionNode,
    memberVariableNode,
    interfaceNode, delegateNode, classDelegateNode,
    parenthesizedConstraintNode, disjunctiveConstraintNode, conjunctiveConstraintNode, whereConstraintNode, predicateConstraintNode, isConstraintNode, multiParamConstraintNode, 
    typeNameConstraintNode,
    constructorConstraintNode, destructorConstraintNode, memberFunctionConstraintNode, functionConstraintNode,
    sameConstraintNode, derivedConstraintNode, convertibleConstraintNode, explicitlyConvertibleConstraintNode, commonConstraintNode, nonreferenceTypeConstraintNode,
    axiomStatementNode, axiomNode, conceptIdNode, conceptNode,
    sameConceptNode, derivedConceptNode, convertibleConceptNode, explicitlyConvertibleConceptNode, commonConceptNode, nonreferenceTypeConceptNode,
    labelNode, compoundStatementNode, returnStatementNode, ifStatementNode, whileStatementNode, doStatementNode, forStatementNode, breakStatementNode, continueStatementNode,
    gotoStatementNode, constructionStatementNode, deleteStatementNode, destroyStatementNode, assignmentStatementNode, expressionStatementNode, emptyStatementNode,
    rangeForStatementNode, switchStatementNode, caseStatementNode, defaultStatementNode, gotoCaseStatementNode, gotoDefaultStatementNode,
    throwStatementNode, catchNode, tryStatementNode, assertStatementNode,
    typedefNode, constantNode, enumTypeNode, enumConstantNode, parameterNode, templateParameterNode,
    constNode, lvalueRefNode, rvalueRefNode, pointerNode, arrayNode,
    dotNode, arrowNode, equivalenceNode, implicationNode, disjunctionNode, conjunctionNode, bitOrNode, bitXorNode, bitAndNode,
    equalNode, notEqualNode, lessNode, greaterNode, lessOrEqualNode, greaterOrEqualNode, shiftLeftNode, shiftRightNode,
    addNode, subNode, mulNode, divNode, remNode, notNode, unaryPlusNode, unaryMinusNode, prefixIncrementNode, prefixDecrementNode, complementNode, derefNode, addrOfNode,
    isNode, asNode, indexingNode, invokeNode, postfixIncrementNode, postfixDecrementNode, sizeOfNode, typeNameNode, typeIdNode, castNode, constructNode, newNode, thisNode, baseNode,
    conditionalCompilationDisjunctionNode, conditionalCompilationConjunctionNode, conditionalCompilationNotNode, conditionalCompilationPrimaryNode, 
    conditionalCompilationPartNode, conditionalCompilationStatementNode,
    uuidLiteralNode, cursorIdNode, parenthesizedExpressionNode, globalVariableNode, parenthesizedCondCompExpressionNode, labeledStatementNode, commentNode, functionPtrNode,
    syncNode, syncStatementNode, attributeNode, attributesNode, fullInstantiationRequestNode,
    maxNode
};

std::string NodeTypeStr(NodeType nodeType);

class Visitor;
class AstWriter;
class AstReader;
class ParameterNode;
class TemplateParameterNode;

class Node
{
public:
    Node(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    virtual ~Node();
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    NodeType GetNodeType() const { return nodeType; }
    virtual Node* Clone(CloneContext& cloneContext) const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual void Write(AstWriter& writer);
    virtual void Read(AstReader& reader);
    virtual std::string ToString() const { return std::string(); }
    virtual void AddArgument(Node* argument);
    virtual void AddParameter(ParameterNode* parameter);
    virtual void AddTemplateParameter(TemplateParameterNode* templateParameter);
    virtual bool IsUnsignedTypeNode() const { return false; }
    virtual bool IsStatementNode() const { return false; }
    virtual bool NodeIsConstraintNode() const { return false; }
    virtual bool IsConceptNode() const { return false; }
    virtual bool IsFunctionNode() const { return false; }
    virtual bool IsIntrinsicConceptNode() const { return false; }
    virtual bool IsConditionalCompilationExpressionNode() const { return false; }
    bool IsNamespaceNode() const { return nodeType == NodeType::namespaceNode; }
    virtual Specifiers GetSpecifiers() const { return Specifiers::none; }
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    void SetSourcePos(const soul::ast::SourcePos& sourcePos_) { sourcePos = sourcePos_; }
    const Node* Parent() const { return parent; }
    Node* Parent() { return parent; }
    void SetParent(Node* parent_);
    const util::uuid& ModuleId() const { return moduleId; }
    void SetLexerFlags(soul::lexer::LexerFlags lexerFlags_) { lexerFlags = lexerFlags_; }
    soul::lexer::LexerFlags GetLexerFlags() const { return lexerFlags; }
private:
    NodeType nodeType;
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId;
    Node* parent;
    soul::lexer::LexerFlags lexerFlags;
};

class UnaryNode : public Node
{
public:
    UnaryNode(NodeType nodeType, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UnaryNode(NodeType nodeType, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
private:
    std::unique_ptr<Node> subject;
};

class BinaryNode : public Node
{
public:
    BinaryNode(NodeType nodeType, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BinaryNode(NodeType nodeType, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Left() const { return left.get(); }
    Node* Left() { return left.get(); }
    const Node* Right() const { return right.get(); }
    Node* Right() { return right.get(); }
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class NodeCreator
{
public:
    NodeCreator();
    NodeCreator(const NodeCreator&) = delete;
    NodeCreator& operator=(const NodeCreator&) = delete;
    virtual ~NodeCreator();
    virtual Node* CreateNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId) = 0;
};

class NodeFactory
{
public:
    NodeFactory(const NodeFactory&) = delete;
    NodeFactory& operator=(const NodeFactory&) = delete;
    static NodeFactory& Instance();
    void Register(NodeType nodeType, NodeCreator* creator);
    Node* CreateNode(NodeType nodeType, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
private:
    static std::unique_ptr<NodeFactory> instance;
    std::vector<std::unique_ptr<NodeCreator>> creators;
    NodeFactory();
};

} // namespace cmajor::ast
