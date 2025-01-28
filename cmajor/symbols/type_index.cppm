// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.type.index;

import std.core;
import cmajor.ir.emitter;
import cmajor.debug;
import util.binary.stream.writer;
import util.uuid;

export namespace cmajor::symbols {

class TypeSymbol;

class TypeIndex
{
public:
    void AddType(const util::uuid& typeId, TypeSymbol* typeSymbol, cmajor::ir::Emitter& emitter);
    void Write(util::BinaryStreamWriter& writer);
private:
    std::recursive_mutex mtx;
    std::unordered_map<util::uuid, cmajor::debug::DIType*, util::UuidHash> typeMap;
    std::vector<std::unique_ptr<cmajor::debug::DIType>> diTypes;
};
} // namespace cmajor::symbols
