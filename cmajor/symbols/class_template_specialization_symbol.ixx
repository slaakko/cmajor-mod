export module cmajor.symbols.class_.template_specialization.symbol;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.scope;
import cmajor.symbols.class_.type.symbol;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.writer;
import cmajor.ast.node;
import cmajor.ir.emitter;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::symbols {

class ClassTypeSymbol;
//class TypeSymbol;

enum class ClassTemplateSpecializationFlags : uint8_t
{
    none = 0,
    prototype = 1 << 0,
    constraintChecked = 1 << 1,
    hasFullInstantiation = 1 << 2
};

inline ClassTemplateSpecializationFlags operator|(ClassTemplateSpecializationFlags left, ClassTemplateSpecializationFlags right)
{
    return ClassTemplateSpecializationFlags(uint8_t(left) | uint8_t(right));
}

inline ClassTemplateSpecializationFlags operator&(ClassTemplateSpecializationFlags left, ClassTemplateSpecializationFlags right)
{
    return ClassTemplateSpecializationFlags(uint8_t(left) & uint8_t(right));
}

inline ClassTemplateSpecializationFlags operator~(ClassTemplateSpecializationFlags flags)
{
    return ClassTemplateSpecializationFlags(~uint8_t(flags));
}

std::u32string MakeClassTemplateSpecializationName(ClassTypeSymbol* classTemplate, const std::vector<TypeSymbol*>& templateArgumentTypes);

class ClassTemplateSpecializationSymbol : public ClassTypeSymbol
{
public:
    ClassTemplateSpecializationSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    ClassTemplateSpecializationSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, std::u32string& name_, ClassTypeSymbol* classTemplate_, const std::vector<TypeSymbol*>& templateArgumentTypes_);
    ~ClassTemplateSpecializationSymbol();
    std::u32string SimpleName() const override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    bool IsPrototypeTemplateSpecialization() const override;
    void* IrType(cmajor::ir::Emitter& emitter) override;
    ClassTypeSymbol* GetClassTemplate() { return classTemplate; }
    const ContainerScope* GetArrowScope() const override;
    ContainerScope* GetArrowScope() override;
    const std::vector<TypeSymbol*>& TemplateArgumentTypes() const { return templateArgumentTypes; }
    std::vector<TypeSymbol*>& TemplateArgumentTypes() { return templateArgumentTypes; }
    void SetGlobalNs(std::unique_ptr<cmajor::ast::Node>&& globalNs_);
    cmajor::ast::Node* GlobalNs() { return globalNs.get(); }
    void SetFileScope(FileScope* fileScope_);
    FileScope* ReleaseFileScope();
    void SetPrototype() { SetFlag(ClassTemplateSpecializationFlags::prototype); }
    bool IsPrototype() const { return GetFlag(ClassTemplateSpecializationFlags::prototype); }
    void SetConstraintChecked() { SetFlag(ClassTemplateSpecializationFlags::constraintChecked); }
    bool IsConstraintChecked() { return GetFlag(ClassTemplateSpecializationFlags::constraintChecked); }
    bool HasFullInstantiation() const { return GetFlag(ClassTemplateSpecializationFlags::hasFullInstantiation); }
    void SetHasFullInstantiation() { SetFlag(ClassTemplateSpecializationFlags::hasFullInstantiation); }
    void ResetHasFullInstantiation() { ResetFlag(ClassTemplateSpecializationFlags::hasFullInstantiation); }
    void SetFlag(ClassTemplateSpecializationFlags flag) { flags = flags | flag; }
    bool GetFlag(ClassTemplateSpecializationFlags flag) const { return (flags & flag) != ClassTemplateSpecializationFlags::none; }
    void ResetFlag(ClassTemplateSpecializationFlags flag) { flags = flags & ~flag; }
    // TODO
    //TypeSymbol* UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::unordered_map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    TypeSymbol* UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId) override;
    std::u32string Id() const override;
    const char* ClassName() const override { return "ClassTemplateSpecializationSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "CL"; }
    std::string GetSymbolCategoryDescription() const override { return "class"; }
private:
    ClassTypeSymbol* classTemplate;
    std::vector<TypeSymbol*> templateArgumentTypes;
    std::unique_ptr<cmajor::ast::Node> globalNs;
    std::unique_ptr<FileScope> fileScope;
    ClassTemplateSpecializationFlags flags;
};
} // namespace cmajor::symbols
