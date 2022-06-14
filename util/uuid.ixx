module;
#include <boost/uuid/uuid.hpp>

export module util.uuid;

import std.core;

export namespace util {

void UuidToInts(const boost::uuids::uuid& id, uint64_t& int1, uint64_t& int2);
void IntsToUuid(uint64_t int1, uint64_t int2, boost::uuids::uuid& id);
void RandomUuid(boost::uuids::uuid& id);

// generate a nil uuid
struct nil_generator {
    typedef boost::uuids::uuid result_type;

    boost::uuids::uuid operator()() const {
        // initialize to all zeros
        boost::uuids::uuid u = { {0} };
        return u;
    }
};

inline boost::uuids::uuid nil_uuid() {
    return nil_generator()();
}

} // namespace util
