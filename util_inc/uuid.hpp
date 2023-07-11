// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_UUID_INCLUDED
#define UTIL_UUID_INCLUDED
#include <cstdint>
#include <string>

namespace util_inc {

struct uuid
{
    using value_type = uint8_t;
    uuid();
    uuid(const uuid& that);
    uuid& operator=(const uuid& that);
    uuid(uuid&& that);
    uuid& operator=(uuid&& that);
    static constexpr int static_size() { return 16; }
    const std::uint8_t* begin() const { return &data[0]; }
    const std::uint8_t* end() const { return &data[static_size()]; }
    bool is_nil() const;
    std::uint8_t* begin() { return &data[0]; }
    std::uint8_t* end() { return &data[static_size()]; }
    std::uint8_t data[16];
};

bool operator==(const uuid& left, const uuid& right);

inline bool operator!=(const uuid& left, const uuid& right) { return !(left == right); }

bool operator<(const uuid& left, const uuid& right);

inline bool operator>(const uuid& left, const uuid& right)
{
    return right < left;
}

inline bool operator<=(const uuid& left, const uuid& right)
{
    return !(left > right);
}

inline bool operator>=(const uuid& left, const uuid& right)
{
    return !(left < right);
}

void UuidToInts(const uuid& id, uint64_t& int1, uint64_t& int2);
void IntsToUuid(uint64_t int1, uint64_t int2, uuid& id);
std::string ToString(const uuid& uuid);
uuid ParseUuid(const std::string& str);
std::size_t HashValue(const uuid& u);

inline uuid nil_uuid()
{
    return uuid();
}

struct UuidHash
{
    std::size_t operator()(const uuid& u) const;
};

} // namespace util_inc

#endif // UTIL_UUID_INCLUDED
