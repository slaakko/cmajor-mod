
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

class AstReader
{
public:
    AstReader(const std::string& fileName_);
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
    ConditionalCompilationPartNode* ReadConditionalCompilationPartNode();
    TemplateIdNode* ReadTemplateIdNode();
    WhereConstraintNode* ReadWhereConstraintNode();
    ConceptNode* ReadConceptNode();
    Specifiers ReadSpecifiers();
    soul::ast::SourcePos ReadSourcePos();
    //void SetModuleMaps(const util::uuid& rootModuleId, std::unordered_map<int16_t, std::string>* moduleNameTable_, std::unordered_map<std::string, int16_t>* moduleIdMap_); TODO
private:
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamReader binaryStreamReader;
    //util::uuid rootModuleId; TODO
    //std::unordered_map<int16_t, std::string>* moduleNameTable; TODO
    //std::unordered_map<std::string, int16_t>* moduleIdMap; TODO
};

} // namespace cmajor::ast