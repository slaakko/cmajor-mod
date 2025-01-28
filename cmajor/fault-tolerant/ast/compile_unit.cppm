// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.compile.unit;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.namespaces;
import util;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class Project;

class CompileUnitNode : public SyntaxNode
{
public:
    CompileUnitNode();
    void SetCompileUnitIndex(int compileUnitIndex_) { compileUnitIndex = compileUnitIndex_; }
    int CompileUnitIndex() const { return compileUnitIndex; }
    NamespaceDefinitionNode* GlobalNs() const { return globalNs.get(); }
    void SetFilePath(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
    void SetValid(bool valid_) { valid = valid_; }
    bool IsValid() const { return valid; }
    void SetLineStarts(std::vector<int>&& lineStarts_);
    const std::vector<int>& LineStarts() const { return lineStarts; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
    void SetProject(Project* project_) { project = project_; }
    Project* GetProject() const { return project; }
private:
    int compileUnitIndex;
    std::string filePath;
    std::unique_ptr<NamespaceDefinitionNode> globalNs;
    std::vector<int> lineStarts;
    Project* project;
    bool valid;
};

} // namespace cmajor::fault::tolerant::ast
