// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.reader;

import cmajor.symbols.symbol;
import cmajor.symbols.array.type.symbol;
import cmajor.symbols.derived.type.symbol;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.symbol.table;

namespace cmajor::symbols {

SymbolReader::SymbolReader(const std::string& fileName_) :
    astReader(fileName_), symbolTable(nullptr), module(nullptr), rootModule(nullptr), conversions(nullptr), arrayTypes(nullptr), derivedTypes(nullptr),
    classTemplateSpecializations(nullptr), typeAndConceptRequests(nullptr), functionRequests(nullptr), setProjectBit(false), symbolsCached(false)
{
}

Symbol* SymbolReader::ReadSymbol(Symbol* parent)
{
    SymbolType symbolType = static_cast<SymbolType>(GetBinaryStreamReader().ReadByte());
    soul::ast::SourcePos sourcePos = astReader.ReadSourcePos();
    util::uuid sourceModuleId;
    GetBinaryStreamReader().ReadUuid(sourceModuleId);
    std::u32string name = GetBinaryStreamReader().ReadUtf32String();
    Symbol* symbol = SymbolFactory::Instance().CreateSymbol(symbolType, sourcePos, sourceModuleId, name);
    symbol->SetModule(module);
    symbol->SetParent(parent);
    symbol->Read(*this);
    return symbol;
}

ArrayTypeSymbol* SymbolReader::ReadArrayTypeSymbol(Symbol* parent)
{
    Symbol* symbol = ReadSymbol(parent);
    if (symbol->GetSymbolType() == SymbolType::arrayTypeSymbol)
    {
        return static_cast<ArrayTypeSymbol*>(symbol);
    }
    else
    {
        throw std::runtime_error("internal error: array type symbol expected");
    }
}

DerivedTypeSymbol* SymbolReader::ReadDerivedTypeSymbol(Symbol* parent)
{
    Symbol* symbol = ReadSymbol(parent);
    if (symbol->GetSymbolType() == SymbolType::derivedTypeSymbol)
    {
        return static_cast<DerivedTypeSymbol*>(symbol);
    }
    else
    {
        throw std::runtime_error("internal error: derived type symbol expected");
    }
}

ClassTemplateSpecializationSymbol* SymbolReader::ReadClassTemplateSpecializationSymbol(Symbol* parent)
{
    Symbol* symbol = ReadSymbol(parent);
    if (symbol->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
    {
        return static_cast<ClassTemplateSpecializationSymbol*>(symbol);
    }
    else
    {
        throw std::runtime_error("internal error: class template specialization symbol expected");
    }
}

ParameterSymbol* SymbolReader::ReadParameterSymbol(Symbol* parent)
{
    Symbol* symbol = ReadSymbol(parent);
    if (symbol->GetSymbolType() == SymbolType::parameterSymbol)
    {
        return static_cast<ParameterSymbol*>(symbol);
    }
    else
    {
        throw std::runtime_error("internal error: parameter symbol expected");
    }
}

void SymbolReader::AddConversion(FunctionSymbol* conversion)
{
    if (conversions)
    {
        conversions->push_back(conversion);
    }
    else
    {
        throw std::runtime_error("reader conversions target not set");
    }
}

void SymbolReader::AddArrayType(ArrayTypeSymbol* arrayType)
{
    if (arrayTypes)
    {
        arrayTypes->push_back(arrayType);
    }
    else
    {
        throw std::runtime_error("reader array types target not set");
    }
}

void SymbolReader::AddDerivedType(DerivedTypeSymbol* derivedType)
{
    if (derivedTypes)
    {
        derivedTypes->push_back(derivedType);
    }
    else
    {
        throw std::runtime_error("reader derived types target not set");
    }
}

void SymbolReader::AddClassTemplateSpecialization(ClassTemplateSpecializationSymbol* classTemplateSpecialization)
{
    if (classTemplateSpecializations)
    {
        classTemplateSpecializations->push_back(classTemplateSpecialization);
    }
    else
    {
        throw std::runtime_error("reader class template specialization target not set");
    }
}

void SymbolReader::AddTypeOrConceptRequest(TypeOrConceptRequest&& typeOrConceptRequest)
{
    if (typeAndConceptRequests)
    {
        typeAndConceptRequests->push_back(std::move(typeOrConceptRequest));
    }
    else
    {
        throw std::runtime_error("reader type and concept request target not set");
    }
}

void SymbolReader::AddFunctionRequest(FunctionRequest&& functionRequest)
{
    if (functionRequests)
    {
        functionRequests->push_back(std::move(functionRequest));
    }
    else
    {
        throw std::runtime_error("reader function request target not set");
    }
}

} // namespace cmajor::symbols
