module;
#include <boost/uuid/uuid.hpp>

export module util.uuid;

import std.core;

export namespace util {

void UuidToInts(const boost::uuids::uuid& id, uint64_t& int1, uint64_t& int2);
void IntsToUuid(uint64_t int1, uint64_t int2, boost::uuids::uuid& id);
void RandomUuid(boost::uuids::uuid& id);

} // namespace util
