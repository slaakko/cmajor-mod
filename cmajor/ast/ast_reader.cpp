// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================
module cmajor.ast.reader;

import cmajor.ast.node;
import cmajor.ast.attribute;
import cmajor.ast.concept_;
import cmajor.ast.template_;
import cmajor.ast.statement;
import cmajor.ast.identifier;

namespace cmajor::ast {
AstReader::AstReader(const std::string& fileName_) :
    fileStream(fileName_, util::OpenMode::binary | util::OpenMode::read), bufferedStream(fileStream), binaryStreamReader(bufferedStream) 
{
}

Node* AstReader::ReadNode()
{
    NodeType nodeType = static_cast<NodeType>(binaryStreamReader.ReadByte());
    soul::ast::SourcePos sourcePos = ReadSourcePos();

    util::uuid moduleId;
    binaryStreamReader.ReadUuid(moduleId);
    Node* node = NodeFactory::Instance().CreateNode(nodeType, sourcePos, moduleId);
    node->Read(*this);
    return node;
}

AttributeNode* AstReader::ReadAttributeNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::attributeNode)
    {
        return static_cast<AttributeNode*>(node);
    }
    else
    {
        throw std::runtime_error("attribute node expected");
    }
}

AttributesNode* AstReader::ReadAttributesNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::attributesNode)
    {
        return static_cast<AttributesNode*>(node);
    }
    else
    {
        throw std::runtime_error("attributes node expected");
    }
}

IdentifierNode* AstReader::ReadIdentifierNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::identifierNode)
    {
        return static_cast<IdentifierNode*>(node);
    }
    else
    {
        throw std::runtime_error("identifier node expected");
    }
}

ConstraintNode* AstReader::ReadConstraintNode()
{
    Node* node = ReadNode();
    if (node->NodeIsConstraintNode())
    {
        return static_cast<ConstraintNode*>(node);
    }
    else
    {
        throw std::runtime_error("constraint node expected");
    }
}

ConceptIdNode* AstReader::ReadConceptIdNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::conceptIdNode)
    {
        return static_cast<ConceptIdNode*>(node);
    }
    else
    {
        throw std::runtime_error("concept id node expected");
    }
}
LabelNode* AstReader::ReadLabelNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::labelNode)
    {
        return static_cast<LabelNode*>(node);
    }
    else
    {
        throw std::runtime_error("label node expected");
    }
}

StatementNode* AstReader::ReadStatementNode()
{
    Node* node = ReadNode();
    if (node->IsStatementNode())
    {
        return static_cast<StatementNode*>(node);
    }
    else
    {
        throw std::runtime_error("statement node expected");
    }
}

DefaultStatementNode* AstReader::ReadDefaultStatementNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::defaultStatementNode)
    {
        return static_cast<DefaultStatementNode*>(node);
    }
    else
    {
        throw std::runtime_error("default statement node expected");
    }
}

CompoundStatementNode* AstReader::ReadCompoundStatementNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::compoundStatementNode)
    {
        return static_cast<CompoundStatementNode*>(node);
    }
    else
    {
        throw std::runtime_error("compound statement node expected");
    }
}

ConditionalCompilationExpressionNode* AstReader::ReadConditionalCompilationExpressionNode()
{
    Node* node = ReadNode();
    if (node->IsConditionalCompilationExpressionNode())
    {
        return static_cast<ConditionalCompilationExpressionNode*>(node);
    }
    else
    {
        throw std::runtime_error("conditional compilation expression node expected");
    }
}

ConditionalCompilationPartNode* AstReader::ReadConditionalCompilationPartNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::conditionalCompilationPartNode)
    {
        return static_cast<ConditionalCompilationPartNode*>(node);
    }
    else
    {
        throw std::runtime_error("conditional compilation part node expected");
    }
}

TemplateIdNode* AstReader::ReadTemplateIdNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::templateIdNode)
    {
        return static_cast<TemplateIdNode*>(node);
    }
    else
    {
        throw std::runtime_error("template id node expected");
    }
}


WhereConstraintNode* AstReader::ReadWhereConstraintNode()
{
    Node* node = ReadNode();
    if (node->GetNodeType() == NodeType::whereConstraintNode)
    {
        return static_cast<WhereConstraintNode*>(node);
    }
    else
    {
        throw std::runtime_error("where constraint node expected");
    }
}


ConceptNode* AstReader::ReadConceptNode()
{
    Node* node = ReadNode();
    if (node->IsConceptNode())
    {
        return static_cast<ConceptNode*>(node);
    }
    else
    {
        throw std::runtime_error("concept node expected");
    }
}

Specifiers AstReader::ReadSpecifiers()
{
    return static_cast<Specifiers>(binaryStreamReader.ReadUInt());
}

soul::ast::SourcePos AstReader::ReadSourcePos()
{
        uint32_t file = binaryStreamReader.ReadULEB128UInt();
        uint32_t line = binaryStreamReader.ReadULEB128UInt();
        uint32_t col = binaryStreamReader.ReadULEB128UInt();
        return soul::ast::SourcePos(static_cast<int32_t>(file), static_cast<int32_t>(line), static_cast<int32_t>(col));
}
/*
void AstReader::SetModuleMaps(const util::uuid& rootModuleId_, std::unordered_map<int16_t, std::string>* moduleNameTable_, std::unordered_map<std::string, int16_t>* moduleIdMap_)
{
    rootModuleId = rootModuleId_;
    moduleNameTable = moduleNameTable_;
    moduleIdMap = moduleIdMap_;
}
*/
} // namespace cmajor::ast
