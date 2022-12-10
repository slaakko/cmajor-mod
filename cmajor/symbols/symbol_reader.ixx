// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.symbol.reader;

import cmajor.ast.reader;
import util.binary.stream.reader;
import std.core;
/*
#include <cmajor/symbols/SymbolsApi.hpp>
#include <sngcm/ast/AstReader.hpp>
#include <vector>
*/

export namespace cmajor::symbols {

class Symbol;
class ArrayTypeSymbol;
class DerivedTypeSymbol;
class ClassTemplateSpecializationSymbol;
struct TypeOrConceptRequest;
struct FunctionRequest;
class SymbolTable;
class Module;
class FunctionSymbol;
class ClassTypeSymbol;
class ParameterSymbol;

class SymbolReader
{
public:
    SymbolReader(const std::string& fileName_);
    cmajor::ast::AstReader& GetAstReader() { return astReader; }
    util::BinaryStreamReader& GetBinaryStreamReader() { return astReader.GetBinaryStreamReader(); }
    Symbol* ReadSymbol(Symbol* parent);
    ArrayTypeSymbol* ReadArrayTypeSymbol(Symbol* parent);
    DerivedTypeSymbol* ReadDerivedTypeSymbol(Symbol* parent);
    ClassTemplateSpecializationSymbol* ReadClassTemplateSpecializationSymbol(Symbol* parent);
    ParameterSymbol* ReadParameterSymbol(Symbol* parent);
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    SymbolTable* GetSymbolTable() const { return symbolTable; }
    void SetModule(Module* module_) { module = module_; }
    Module* GetModule() const { return module; }
    void SetConversionsTarget(std::vector<FunctionSymbol*>* conversionsTarget) { conversions = conversionsTarget; }
    void SetArrayTypesTarget(std::vector<ArrayTypeSymbol*>* arrayTypesTarget) { arrayTypes = arrayTypesTarget; }
    void SetDerivedTypesTarget(std::vector<DerivedTypeSymbol*>* derivedTypesTarget) { derivedTypes = derivedTypesTarget; }
    void SetClassTemplateSpecializationTarget(std::vector<ClassTemplateSpecializationSymbol*>* classTemplateSpecializationTarget) { classTemplateSpecializations = classTemplateSpecializationTarget; }
    void SetTypeAndConceptRequestTarget(std::vector<TypeOrConceptRequest>* typeAndConceptRequestTarget) { typeAndConceptRequests = typeAndConceptRequestTarget; }
    void SetFunctionRequestTarget(std::vector<FunctionRequest>* functionRequestTarget) { functionRequests = functionRequestTarget; }
    void AddConversion(FunctionSymbol* conversion);
    void AddArrayType(ArrayTypeSymbol* arrayType);
    void AddDerivedType(DerivedTypeSymbol* derivedType);
    void AddClassTemplateSpecialization(ClassTemplateSpecializationSymbol* classTemplateSpecialization);
    void AddTypeOrConceptRequest(TypeOrConceptRequest&& typeOrConceptRequest);
    void AddFunctionRequest(FunctionRequest&& functionRequest);
    bool SetProjectBit() const { return setProjectBit; }
    void SetProjectBitForSymbols() { setProjectBit = true; }
    void SetRootModule(Module* rootModule_) { rootModule = rootModule_; }
    Module* RootModuleId() const { return rootModule; }
private:
    cmajor::ast::AstReader astReader;
    SymbolTable* symbolTable;
    Module* module;
    Module* rootModule;
    std::vector<FunctionSymbol*>* conversions;
    std::vector<ArrayTypeSymbol*>* arrayTypes;
    std::vector<DerivedTypeSymbol*>* derivedTypes;
    std::vector<ClassTemplateSpecializationSymbol*>* classTemplateSpecializations;
    std::vector<TypeOrConceptRequest>* typeAndConceptRequests;
    std::vector<FunctionRequest>* functionRequests;
    bool setProjectBit;
    bool symbolsCached;
};

} // namespace cmajor::symbols
