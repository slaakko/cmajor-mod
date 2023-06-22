// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CLASSES_HPP
#define CLASSES_HPP
#include <util/uuid.hpp>
#include <boost/multiprecision/cpp_int.hpp>

namespace cmajor::rt {

void InitClasses(int64_t numberOfPolymorphicClassIds, const uint64_t* polymorphicClassIdArray, int64_t numberOfStaticClassIds, const uint64_t* staticClassIdArray);
bool DynamicInitVmtsAndCompare(void* vmt1, void* vmt2);
boost::multiprecision::uint128_t GetClassId(const util::uuid& typeId);

} // namespace cmajor::rt

#endif // CLASSES_HPP
