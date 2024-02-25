// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.basic.type.symbol;

namespace cmajor::fault::tolerant::symbols {

BasicTypeSymbol::BasicTypeSymbol(SymbolKind kind_, const std::u16string& name_) : TypeSymbol(kind_, name_, soul::ast::Span())
{
}

AutoTypeSymbol::AutoTypeSymbol() : BasicTypeSymbol(SymbolKind::autoTypeSymbol, u"auto")
{
}

BoolTypeSymbol::BoolTypeSymbol() : BasicTypeSymbol(SymbolKind::boolTypeSymbol, u"bool")
{
}

SByteTypeSymbol::SByteTypeSymbol() : BasicTypeSymbol(SymbolKind::sbyteTypeSymbol, u"sbyte")
{
}

ByteTypeSymbol::ByteTypeSymbol() : BasicTypeSymbol(SymbolKind::byteTypeSymbol, u"byte")
{
}

ShortTypeSymbol::ShortTypeSymbol() : BasicTypeSymbol(SymbolKind::shortTypeSymbol, u"short")
{
}

UShortTypeSymbol::UShortTypeSymbol() : BasicTypeSymbol(SymbolKind::ushortTypeSymbol, u"ushort")
{
}

IntTypeSymbol::IntTypeSymbol() : BasicTypeSymbol(SymbolKind::intTypeSymbol, u"int")
{
}

UIntTypeSymbol::UIntTypeSymbol() : BasicTypeSymbol(SymbolKind::uintTypeSymbol, u"uint")
{
}

LongTypeSymbol::LongTypeSymbol() : BasicTypeSymbol(SymbolKind::longTypeSymbol, u"long")
{
}

ULongTypeSymbol::ULongTypeSymbol() : BasicTypeSymbol(SymbolKind::ulongTypeSymbol, u"ulong")
{
}

FloatTypeSymbol::FloatTypeSymbol() : BasicTypeSymbol(SymbolKind::floatTypeSymbol, u"float")
{
}

DoubleTypeSymbol::DoubleTypeSymbol() : BasicTypeSymbol(SymbolKind::doubleTypeSymbol, u"double")
{
}

CharTypeSymbol::CharTypeSymbol() : BasicTypeSymbol(SymbolKind::charTypeSymbol, u"char")
{
}

WCharTypeSymbol::WCharTypeSymbol() : BasicTypeSymbol(SymbolKind::wcharTypeSymbol, u"wchar")
{
}

UCharTypeSymbol::UCharTypeSymbol() : BasicTypeSymbol(SymbolKind::ucharTypeSymbol, u"uchar")
{
}

VoidTypeSymbol::VoidTypeSymbol() : BasicTypeSymbol(SymbolKind::voidTypeSymbol, u"void")
{
}

} // namespace cmajor::fault::tolerant::symbols
