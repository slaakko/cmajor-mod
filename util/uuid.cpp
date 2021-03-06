// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <rpc.h>
#pragma comment(lib, "rpcrt4.lib")
#endif

module util.uuid;

namespace util {

void UuidToInts(const boost::uuids::uuid& id, uint64_t& int1, uint64_t& int2)
{
#ifndef NDEBUG
    static_assert(boost::uuids::uuid::static_size() == 2 * sizeof(uint64_t), "16 bytes expected");
#endif
    const uint8_t* i = id.begin();
    const uint64_t* i64 = reinterpret_cast<const uint64_t*>(i);
    int1 = *i64++;
    int2 = *i64;
}

void IntsToUuid(uint64_t int1, uint64_t int2, boost::uuids::uuid& id)
{
#ifndef NDEBUG
    static_assert(boost::uuids::uuid::static_size() == 2 * sizeof(uint64_t), "16 bytes expected");
#endif
    uint8_t* i = id.begin();
    uint64_t* i64 = reinterpret_cast<uint64_t*>(i);
    *i64++ = int1;
    *i64 = int2;
}

void RandomUuid(boost::uuids::uuid& id)
{
    UUID u;
    UuidCreate(&u);
    id = *static_cast<boost::uuids::uuid*>(static_cast<void*>(&u));
}

} // namespace util
