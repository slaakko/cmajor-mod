// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/multiprecision/cpp_int.hpp>

export module cmajor.rt.classes;

import std.core;
import util.uuid;

export namespace cmajor::rt {

void InitClasses(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray);
bool DynamicInitVmtsAndCompare(void* vmt1, void* vmt2);
boost::multiprecision::uint128_t GetClassId(const util::uuid& typeId);

} // namespace cmajor::rt
