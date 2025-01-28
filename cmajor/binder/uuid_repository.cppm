// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.uuid.repository;

import std.core;
import util.uuid;

export namespace cmajor::binder {

class UuidRepository
{
public:
    int Install(const util::uuid& uuid);
    const util::uuid& GetUuid(int id) const;
private:
    std::unordered_map<util::uuid, int, util::UuidHash> uuidMap;
    std::vector<util::uuid> uuids;
};

} // namespace cmajor::binder
