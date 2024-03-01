// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.uuid.repository;

import util;

namespace cmajor::binder {

int UuidRepository::Install(const util::uuid& uuid)
{
    auto it = uuidMap.find(uuid);
    if (it != uuidMap.cend())
    {
        return it->second;
    }
    else
    {
        int id = uuids.size();
        uuidMap[uuid] = id;
        uuids.push_back(uuid);
        return id;
    }
}

const util::uuid& UuidRepository::GetUuid(int id) const
{
    //Assert(id >= 0 && id < uuids.size(), "invalid uuid id");
    return uuids[id];
}
} // namespace cmajor::binder
