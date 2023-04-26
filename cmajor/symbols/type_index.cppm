// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.type.index;

import std.core;
import cmajor.ir.emitter;
import util.binary.stream.writer;
import util.uuid;

export namespace cmajor::symbols {

class TypeSymbol;

class TypeIndex
{
public:
    void AddType(const util::uuid& typeId, TypeSymbol* typeSymbol, cmajor::ir::Emitter& emittert);
    void Write(util::BinaryStreamWriter& writer);
private:
    std::recursive_mutex mtx;
    //std::unordered_map<util::uuid, cmajor::debug::DIType*, boost::hash<util::uuid>> typeMap;
    //std::map<util::uuid, cmajor::debug::DIType*> typeMap;
    //std::vector<std::unique_ptr<cmajor::debug::DIType>> diTypes;
};
} // namespace cmajor::symbols
