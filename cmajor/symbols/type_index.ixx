module;
#include <boost/uuid/uuid.hpp>
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.type.index;

/*
#include <cmajor/symbols/SymbolsApi.hpp>
#include <cmajor/cmdebug/DIType.hpp>
#include <cmajor/cmdebug/DIEnumConstant.hpp>
#include <cmajor/ir/Emitter.hpp>
#include <soulng/util/BinaryWriter.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <mutex>
*/
import std.core;
import cmajor.ir.emitter;
import util.binary.stream.writer;

export namespace cmajor::symbols {

class TypeSymbol;

class TypeIndex
{
public:
    void AddType(const boost::uuids::uuid& typeId, TypeSymbol* typeSymbol, cmajor::ir::Emitter& emittert);
    void Write(util::BinaryStreamWriter& writer);
private:
    std::recursive_mutex mtx;
    //std::unordered_map<boost::uuids::uuid, cmajor::debug::DIType*, boost::hash<boost::uuids::uuid>> typeMap;
    //std::map<boost::uuids::uuid, cmajor::debug::DIType*> typeMap;
    //std::vector<std::unique_ptr<cmajor::debug::DIType>> diTypes;
};
} // namespace cmajor::symbols
