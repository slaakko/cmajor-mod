// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.dominator.tree.cooper;

import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.util;
import cmajor.systemx.intermediate.visit;
import soul.xml.dom;
import util;

namespace cmajor::systemx::intermediate {

DominatorTreeNode::DominatorTreeNode(DominatorTreeCooper* tree_, int number_, BasicBlock* basicBlock_) : tree(tree_), number(number_), basicBlock(basicBlock_), children(this)
{
}

DominatorTreeNode* DominatorTreeNode::Parent() const
{
    if (tree->Root() == this)
    {
        return nullptr;
    }
    else
    {
        return static_cast<DominatorTreeNode*>(children.Parent());
    }
}

void DominatorTreeNode::AddChild(DominatorTreeNode* child)
{
    children.AddChild(child);
}

void DominatorTreeNode::AddToDominatorFrontierSet(BasicBlock* block)
{
    AddPtrToSet(block, dominanceFrontierSet);
}

DominatorTreeCooper::DominatorTreeCooper(Function& function_) : function(function_), numRounds(0), nodes(this), startNodeNumber(undefinedBlockNumber)
{
}

int DominatorTreeCooper::GetBlockNumber(BasicBlock* block) const
{
    auto it = basicBlockNumberMap.find(block);
    if (it != basicBlockNumberMap.cend())
    {
        return it->second;
    }
    else
    {
        return undefinedBlockNumber;
    }
}

DominatorTreeNode* DominatorTreeCooper::Root() const
{
    return static_cast<DominatorTreeNode*>(nodes.FirstChild());
}

soul::xml::Element* ToXmlElement(DominatorTreeNode* node)
{
    soul::xml::Element* element = soul::xml::MakeElement("node");
    element->SetAttribute("id", node->GetBasicBlock()->Name());
    element->SetAttribute("number", std::to_string(node->Number()));
    for (BasicBlock* dominanceFrontierNode : node->DominanceFrontierSet())
    {
        soul::xml::Element* frontier_element = soul::xml::MakeElement("dominance_frontier_node");
        frontier_element->SetAttribute("id", dominanceFrontierNode->Name());
        element->AppendChild(frontier_element);
    }
    DominatorTreeNode* child = node->FirstChild();
    while (child)
    {
        soul::xml::Element* childElement = ToXmlElement(child);
        element->AppendChild(childElement);
        child = child->Next();
    }
    return element;
}

soul::xml::Element* DominatorTreeCooper::ToXml()
{
    soul::xml::Element* treeElement = soul::xml::MakeElement("dominator_tree");
    treeElement->SetAttribute("numRounds", std::to_string(numRounds));
    DominatorTreeNode* root = Root();
    soul::xml::Element* rootElement = ToXmlElement(root);
    treeElement->AppendChild(rootElement);
    return treeElement;
}

void DominatorTreeCooper::WriteXmlDocument(const std::string& filePath)
{
    soul::xml::Element* element = ToXml();
    soul::xml::Document document;
    document.AppendChild(element);
    std::ofstream file(filePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    document.Write(formatter);
}

int DominatorTreeCooper::Intersect(int b1, int b2) const
{
    int finger1 = b1;
    int finger2 = b2;
    while (finger1 != finger2)
    {
        while (finger1 < finger2)
        {
            finger1 = doms[finger1];
        }
        while (finger2 < finger1)
        {
            finger2 = doms[finger2];
        }
    }
    return finger1;
}

BasicBlock* DominatorTreeCooper::ImmediateDominator(BasicBlock* block) const
{
    if (block == function.FirstBasicBlock())
    {
        return nullptr;
    }
    else
    {
        DominatorTreeNode* node = GetNode(block);
        if (node)
        {
            return node->Parent()->GetBasicBlock();
        }
        else
        {
            throw std::runtime_error("node for basic block '" + std::to_string(block->Id()) + "' not found");
        }
    }
}

const std::vector<BasicBlock*>& DominatorTreeCooper::DominanceFrontier(BasicBlock* block) const
{
    DominatorTreeNode* node = GetNode(block);
    if (node)
    {
        return node->DominanceFrontierSet();
    }
    else
    {
        throw std::runtime_error("node for basic block '" + std::to_string(block->Id()) + "' not found");
    }
}

void DominatorTreeCooper::Init(const std::vector<BasicBlock*>& reversePostorder)
{
    int n = reversePostorder.size();
    basicBlocks.resize(n + 1);
    for (int i = 0; i < n; ++i)
    {
        BasicBlock* block = reversePostorder[i];
        int blockNumber = n - i;
        basicBlocks[blockNumber] = block;
        basicBlockNumberMap[block] = blockNumber;
    }
    doms.resize(n + 1);
    for (int i = 0; i <= n; ++i)
    {
        doms[i] = undefinedBlockNumber;
    }
    startNodeNumber = GetBlockNumber(function.FirstBasicBlock());
    doms[startNodeNumber] = startNodeNumber;
}

void DominatorTreeCooper::Run(const std::vector<BasicBlock*>& reversePostorder)
{
    bool changed = true;
    while (changed)
    {
        ++numRounds;
        changed = false;
        for (BasicBlock* block : reversePostorder)
        {
            if (!block->Predecessors().empty())
            {
                int b = GetBlockNumber(block);
                BasicBlock* firstPredecessor = block->Predecessors()[0];
                int newIdom = GetBlockNumber(firstPredecessor);
                int pn = block->Predecessors().size();
                for (int i = 1; i < pn; ++i)
                {
                    BasicBlock* predecessor = block->Predecessors()[i];
                    int p = GetBlockNumber(predecessor);
                    if (doms[p] != undefinedBlockNumber)
                    {
                        newIdom = Intersect(p, newIdom);
                    }
                }
                if (doms[b] != newIdom)
                {
                    doms[b] = newIdom;
                    changed = true;
                }
            }
        }
    }
}

void DominatorTreeCooper::Compute()
{
    ComputeDominatorTree();
    ComputeDominanceFrontier();
}

void DominatorTreeCooper::ComputeDominatorTree()
{
    std::vector<BasicBlock*> reversePostorder = ReversePostorder(function);
    Init(reversePostorder);
    Run(reversePostorder);
    MakeTree();
}

void DominatorTreeCooper::MakeTree()
{
    nodeVec.resize(doms.size());
    BasicBlock* block = function.FirstBasicBlock();
    int blockNumber = GetBlockNumber(block);
    DominatorTreeNode* root = new DominatorTreeNode(this, blockNumber, block);
    nodeVec[blockNumber] = root;
    nodes.AddChild(root);
    nodeMap[block] = root;
    block = block->Next();
    while (block)
    {
        int blockNumber = GetBlockNumber(block);
        DominatorTreeNode* node = new DominatorTreeNode(this, blockNumber, block);
        nodeMap[block] = node;
        nodeVec[blockNumber] = node;
        block = block->Next();
    }
    for (int blockNumber = 1; blockNumber < doms.size(); ++blockNumber)
    {
        if (blockNumber != startNodeNumber)
        {
            DominatorTreeNode* node = nodeVec[blockNumber];
            DominatorTreeNode* parent = nodeVec[doms[blockNumber]];
            parent->AddChild(node);
        }
    }
}

void DominatorTreeCooper::ComputeDominanceFrontier()
{
    BasicBlock* block = function.FirstBasicBlock();
    while (block)
    {
        if (block->Predecessors().size() > 1)
        {
            int b = GetBlockNumber(block);
            for (BasicBlock* predecessor : block->Predecessors())
            {
                int runner = GetBlockNumber(predecessor);
                while (runner != doms[b])
                {
                    DominatorTreeNode* runnerNode = nodeVec[runner];
                    runnerNode->AddToDominatorFrontierSet(block);
                    runner = doms[runner];
                }
            }
        }
        block = block->Next();
    }
}

DominatorTreeNode* DominatorTreeCooper::GetNode(BasicBlock* block) const
{
    auto it = nodeMap.find(block);
    if (it != nodeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

bool DominatorTreeCooper::Dominates(BasicBlock* x, BasicBlock* y) const
{
    if (x == y)
    {
        return true;
    }
    DominatorTreeNode* y_node = GetNode(y);
    if (y_node)
    {
        DominatorTreeNode* x_node = GetNode(x);
        if (x_node)
        {
            return IsSameParentOrAncestor(x_node, y_node);
        }
        else
        {
            throw std::runtime_error("node for basic block '" + std::to_string(x->Id()) + "' not found");
        }
    }
    else
    {
        throw std::runtime_error("node for basic block '" + std::to_string(y->Id()) + "' not found");
    }
}

bool DominatorTreeCooper::IsSameParentOrAncestor(DominatorTreeNode* x, DominatorTreeNode* y) const
{
    if (x)
    {
        if (x == y)
        {
            return true;
        }
        else
        {
            return IsSameParentOrAncestor(x->Parent(), y);
        }
    }
    else
    {
        return false;
    }
}

} // cmajor::systemx::intermediate
