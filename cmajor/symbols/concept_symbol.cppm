// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.concepts;

import cmajor.ast.specifier;
import cmajor.ast.concepts; 
import cmajor.ast.parameter;
import cmajor.symbols.symbol;
import cmajor.symbols.container.symbol;
import soul.ast.source.pos;
import soul.xml.element;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class ConceptSymbol;
class TemplateParameterSymbol;

class ConceptGroupSymbol : public Symbol
{
public:
    ConceptGroupSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "concept_group"; }
    bool IsExportSymbol() const override { return false; }
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void AddConcept(ConceptSymbol* conceptSymbol);
    void RemoveConcept(ConceptSymbol* conceptSymbol);
    bool IsEmpty() const;
    ConceptSymbol* GetConcept(int arity);
    bool HasProjectMembers() const override;
    void AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "ConceptGroupSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "CO"; }
    std::string GetSymbolCategoryDescription() const override { return "concept"; }
    std::string GetSymbolHelp() const override;
    std::vector<Symbol*> GetParamHelpSymbols() const override;
private:
    std::unordered_map<int, ConceptSymbol*> arityConceptMap;
};

class ConceptSymbol : public ContainerSymbol
{
public:
    ConceptSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    bool IsParentSymbol() const override { return true; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceConcept(ConceptSymbol* conceptSymbol) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    void AddMember(Symbol* member) override;
    std::string TypeString() const override { return "concept"; }
    std::u32string SimpleName() const override { return groupName; }
    void ComputeName();
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void SetTypeId(const util::uuid& typeId_) { typeId = typeId_; }
    const util::uuid& TypeId() const { return typeId; }
    const std::u32string& GroupName() const { return groupName; }
    void SetGroupName(const std::u32string& groupName_) { groupName = groupName_; }
    std::u32string CodeName() const override { return groupName; }
    int Arity() const { return templateParameters.size(); }
    cmajor::ast::ConceptNode* GetConceptNode() { return conceptNode.get(); }
    ConceptSymbol* RefinedConcept() const { return refinedConcept; }
    void SetRefinedConcept(ConceptSymbol* refinedConcept_) { refinedConcept = refinedConcept_; }
    const std::vector<TemplateParameterSymbol*>& TemplateParameters() const { return templateParameters; }
    std::u32string Info() const override { return groupName; }
    const char* ClassName() const override { return "ConceptSymbol"; }
    bool HasSource() const { return hasSource; }
    void SetHasSource() { hasSource = true; }
    void Check() override;
    void SetConceptGroup(ConceptGroupSymbol* conceptGroup_) { conceptGroup = conceptGroup_; }
    std::unique_ptr<Symbol> RemoveFromParent() override;
    std::string GetSymbolCategoryStr() const override { return "CO"; }
    std::string GetSymbolCategoryDescription() const override { return "concept"; }
private:
    util::uuid typeId;
    std::u32string groupName;
    std::vector<TemplateParameterSymbol*> templateParameters;
    std::unique_ptr<cmajor::ast::ConceptNode> conceptNode;
    ConceptSymbol* refinedConcept;
    bool hasSource;
    ConceptGroupSymbol* conceptGroup;
};

class AxiomSymbol : public ContainerSymbol
{
public:
    AxiomSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId, const std::u32string& name_);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    bool IsExportSymbol() const override { return false; }
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "AxiomSymbol"; }
};
} // namespace cmajor::symbols
