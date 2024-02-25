// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol.factory;

import cmajor.fault.tolerant.symbols.namespace_symbol;
import cmajor.fault.tolerant.symbols.alias.type.symbol;
import cmajor.fault.tolerant.symbols.constant.symbol;
import cmajor.fault.tolerant.symbols.delegate.symbol;
import cmajor.fault.tolerant.symbols.enum_symbol;
import cmajor.fault.tolerant.symbols.variable.symbol;
import cmajor.fault.tolerant.symbols.template_symbol;
import cmajor.fault.tolerant.symbols.function.symbol;
import cmajor.fault.tolerant.symbols.class_symbol;
import cmajor.fault.tolerant.symbols.interface.symbol;
import cmajor.fault.tolerant.symbols.concept_symbol;
import cmajor.fault.tolerant.symbols.basic.type.symbol;
import cmajor.fault.tolerant.symbols.compound.type.symbol;
import cmajor.fault.tolerant.symbols.array.type.symbol;
import cmajor.fault.tolerant.symbols.declaration.block.symbol;

namespace cmajor::fault::tolerant::symbols {

Symbol* CreateSymbol(SymbolKind kind, const std::u16string& name, const soul::ast::Span& span)
{
    switch (kind)
    {
        case SymbolKind::namespaceSymbol:
        {
            return new NamespaceSymbol(name);
        }
        case SymbolKind::aliasTypeSymbol:
        {
            return new AliasTypeSymbol(name, span);
        }
        case SymbolKind::constantSymbol:
        {
            return new ConstantSymbol(name, span);
        }
        case SymbolKind::delegateSymbol:
        {
            return new DelegateSymbol(name, span);
        }
        case SymbolKind::classDelegateSymbol:
        {
            return new ClassDelegateSymbol(name, span);
        }
        case SymbolKind::enumTypeSymbol:
        {
            return new EnumTypeSymbol(name, span);
        }
        case SymbolKind::enumConstantSymbol:
        {
            return new EnumConstantSymbol(name, span);
        }
        case SymbolKind::parameterSymbol:
        {
            return new ParameterSymbol(name, span);
        }
        case SymbolKind::localVariableSymbol:
        {
            return new LocalVariableSymbol(name, span);
        }
        case SymbolKind::memberVariableSymbol:
        {
            return new MemberVariableSymbol(name, span);
        }
        case SymbolKind::globalVariableSymbol:
        {
            return new GlobalVariableSymbol(name, span);
        }
        case SymbolKind::templateParameterSymbol:
        {
            return new TemplateParameterSymbol(name, span);
        }
        case SymbolKind::functionSymbol:
        {
            return new FunctionSymbol(name, span);
        }
        case SymbolKind::staticConstructorSymbol:
        {
            return new StaticConstructorSymbol(name, span);
        }
        case SymbolKind::constructorSymbol:
        {
            return new ConstructorSymbol(name, span);
        }
        case SymbolKind::destructorSymbol:
        {
            return new DestructorSymbol(name, span);
        }
        case SymbolKind::memberFunctionSymbol:
        {
            return new MemberFunctionSymbol(name, span);
        }
        case SymbolKind::conversionFunctionSymbol:
        {
            return new ConversionFunctionSymbol(name, span);
        }
        case SymbolKind::classTypeSymbol:
        {
            return new ClassTypeSymbol(name, span);
        }
        case SymbolKind::interfaceTypeSymbol:
        {
            return new InterfaceTypeSymbol(name, span);
        }
        case SymbolKind::conceptSymbol:
        {
            return new ConceptSymbol(name, span);
        }
        case SymbolKind::typeParameterSymbol:
        {
            return new TypeParameterSymbol(name, span);
        }
        case SymbolKind::autoTypeSymbol:
        {
            return new AutoTypeSymbol();
        }
        case SymbolKind::boolTypeSymbol:
        {
            return new BoolTypeSymbol();
        }
        case SymbolKind::sbyteTypeSymbol:
        {
            return new SByteTypeSymbol();
        }
        case SymbolKind::byteTypeSymbol:
        {
            return new ByteTypeSymbol();
        }
        case SymbolKind::shortTypeSymbol:
        {
            return new ShortTypeSymbol();
        }
        case SymbolKind::ushortTypeSymbol:
        {
            return new UShortTypeSymbol();
        }
        case SymbolKind::intTypeSymbol:
        {
            return new IntTypeSymbol();
        }
        case SymbolKind::uintTypeSymbol:
        {
            return new UIntTypeSymbol();
        }
        case SymbolKind::longTypeSymbol:
        {
            return new LongTypeSymbol();
        }
        case SymbolKind::ulongTypeSymbol:
        {
            return new ULongTypeSymbol();
        }
        case SymbolKind::floatTypeSymbol:
        {
            return new FloatTypeSymbol();
        }
        case SymbolKind::doubleTypeSymbol:
        {
            return new DoubleTypeSymbol();
        }
        case SymbolKind::charTypeSymbol:
        {
            return new CharTypeSymbol();
        }
        case SymbolKind::wcharTypeSymbol:
        {
            return new WCharTypeSymbol();
        }
        case SymbolKind::ucharTypeSymbol:
        {
            return new UCharTypeSymbol();
        }
        case SymbolKind::voidTypeSymbol:
        {
            return new VoidTypeSymbol();
        }
        case SymbolKind::compoundTypeSymbol:
        {
            return new CompoundTypeSymbol(name);
        }
        case SymbolKind::arrayTypeSymbol:
        {
            return new ArrayTypeSymbol(name);
        }
        case SymbolKind::declarationBlockSymbol:
        {
            return new DeclarationBlockSymbol(name, span);
        }
    }
    throw std::runtime_error("CreateSymbol: invalid symbol kind");
}

} // namespace cmajor::fault::tolerant::symbols
