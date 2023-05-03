export module cmajor.ast.compile.unit;


// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.ast.namespaces;
import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class CompileUnitNode : public Node
{
public:
    CompileUnitNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    CompileUnitNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::string& filePath_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    const std::string& FilePath() const { return filePath; }
    const NamespaceNode* GlobalNs() const { return globalNs.get(); }
    NamespaceNode* GlobalNs() { return globalNs.get(); }
    void ResetGlobalNs(NamespaceNode* ns);
    void ComputeLineStarts(const std::u32string& sourceFileContent);
    const std::vector<int32_t>& LineStarts() { return lineStarts; }
    void SetSynthesizedUnit() { isSynthesizedUnit = true; }
    bool IsSynthesizedUnit() const { return isSynthesizedUnit; }
    int GetColumn(const soul::ast::SourcePos& sourcePos) const;
    const std::string& Id();
    const std::string& Hash() const { return hash; }
    void SetHash(const std::string& hash_) { hash = hash_; }
    bool IsProgramMainUnit() const { return isProgramMainUnit; }
    void SetProgramMainUnit() { isProgramMainUnit = true; }
private:
    std::string filePath;
    std::unique_ptr<NamespaceNode> globalNs;
    std::vector<int32_t> lineStarts;
    bool isSynthesizedUnit;
    std::string id;
    std::string hash;
    bool isProgramMainUnit;
};

void CombineNamespaces(CompileUnitNode& cu);
void ArrangeClassMembers(CompileUnitNode& cu);
void AddNamespaceImportsForUnnamedNamespaces(CompileUnitNode& cu);

} // namespace cmajor::ast

