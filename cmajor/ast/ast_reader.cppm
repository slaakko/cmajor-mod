
// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.reader;

import std.core;
import util;
import cmajor.ast.specifier;
import soul.ast.source.pos;

export namespace cmajor::ast {

class Node;
class IdentifierNode;
class LabelNode;
class StatementNode;
class DefaultStatementNode;
class CompoundStatementNode;
class ConstraintNode;
class WhereConstraintNode;
class ConceptIdNode;
class ConceptNode;
class TemplateIdNode;
class ConditionalCompilationExpressionNode;
class ConditionalCompilationPartNode;
class AttributesNode;
class AttributeNode;
class NamespaceNode;

class AstReader
{
public:
    AstReader(const std::string& fileName_);
    const std::string& FileName() const { return fileName; }
    util::BinaryStreamReader& GetBinaryStreamReader() { return binaryStreamReader; }
    Node* ReadNode();
    AttributeNode* ReadAttributeNode();
    AttributesNode* ReadAttributesNode();
    IdentifierNode* ReadIdentifierNode();
    ConstraintNode* ReadConstraintNode();
    ConceptIdNode* ReadConceptIdNode();
    LabelNode* ReadLabelNode();
    StatementNode* ReadStatementNode();
    DefaultStatementNode* ReadDefaultStatementNode();
    CompoundStatementNode* ReadCompoundStatementNode();
    ConditionalCompilationExpressionNode* ReadConditionalCompilationExpressionNode();
    cmajor::ast::ConditionalCompilationPartNode* ReadConditionalCompilationPartNode();
    TemplateIdNode* ReadTemplateIdNode();
    WhereConstraintNode* ReadWhereConstraintNode();
    ConceptNode* ReadConceptNode();
    NamespaceNode* ReadNamespaceNode();
    Specifiers ReadSpecifiers();
    soul::ast::SourcePos ReadSourcePos();
private:
    std::string fileName;
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamReader binaryStreamReader;
};

} // namespace cmajor::ast