// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.derivations;

import std.core;

export namespace cmajor::fault::tolerant::symbols {

enum class Derivation : uint8_t
{
    none = 0, constDerivation = 1, lvalueRefDerivation = 2, rvalueRefDerivation = 3, pointerDerivation = 4, max
};

struct Derivations
{
    std::vector<Derivation> vec;
    bool IsEmpty() const { return vec.empty(); }
};

Derivations Merge(const Derivations& left, const Derivations& right);

inline bool operator==(const Derivations& left, const Derivations& right)
{
    return left.vec == right.vec;
}

inline bool operator<(const Derivations& left, const Derivations& right)
{
    return left.vec < right.vec;
}

bool HasDerivation(const Derivations& derivations, Derivation derivation);
int PointerCount(const Derivations& derivations);
Derivations Plain(const Derivations& derivations);
Derivations RemoveConst(const Derivations& derivations);
Derivations RemovePointer(const Derivations& derivations);
Derivations RemoveLValueRef(const Derivations& derivations);
Derivations RemoveRValueRef(const Derivations& derivations);
Derivations UnifyDerivations(const Derivations& left, const Derivations& right);
int CountMatchingDerivations(const Derivations& left, const Derivations& right);

class SymbolReader;
class SymbolWriter;

void Write(SymbolWriter& writer, const Derivations& derivations);
void Read(SymbolReader& reader, Derivations& derivations);

} // namespace cmajor::fault::tolerant::symbols
