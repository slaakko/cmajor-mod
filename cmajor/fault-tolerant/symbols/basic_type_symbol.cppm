// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.basic.type.symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class BasicTypeSymbol : public TypeSymbol
{
public:
    BasicTypeSymbol(SymbolKind kind_, const std::u16string& name_);
};

class AutoTypeSymbol : public BasicTypeSymbol
{
public:
    AutoTypeSymbol();
};

class BoolTypeSymbol : public BasicTypeSymbol
{
public:
    BoolTypeSymbol();
};

class SByteTypeSymbol : public BasicTypeSymbol
{
public:
    SByteTypeSymbol();
};

class ByteTypeSymbol : public BasicTypeSymbol
{
public:
    ByteTypeSymbol();
};

class ShortTypeSymbol : public BasicTypeSymbol
{
public:
    ShortTypeSymbol();
};

class UShortTypeSymbol : public BasicTypeSymbol
{
public:
    UShortTypeSymbol();
};

class IntTypeSymbol : public BasicTypeSymbol
{
public:
    IntTypeSymbol();
};

class UIntTypeSymbol : public BasicTypeSymbol
{
public:
    UIntTypeSymbol();
};

class LongTypeSymbol : public BasicTypeSymbol
{
public:
    LongTypeSymbol();
};

class ULongTypeSymbol : public BasicTypeSymbol
{
public:
    ULongTypeSymbol();
};

class FloatTypeSymbol : public BasicTypeSymbol
{
public:
    FloatTypeSymbol();
};

class DoubleTypeSymbol : public BasicTypeSymbol
{
public:
    DoubleTypeSymbol();
};

class CharTypeSymbol : public BasicTypeSymbol
{
public:
    CharTypeSymbol();
};

class WCharTypeSymbol : public BasicTypeSymbol
{
public:
    WCharTypeSymbol();
};

class UCharTypeSymbol : public BasicTypeSymbol
{
public:
    UCharTypeSymbol();
};

class VoidTypeSymbol : public BasicTypeSymbol
{
public:
    VoidTypeSymbol();
};

} // namespace cmajor::fault::tolerant::symbols
