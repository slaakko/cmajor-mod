// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.region;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Process;

enum class RegionId
{
    none, text, data, stack, pool
};

class Region
{
public:
    Region() : id(RegionId::none), start(0), length(0) {}
    Region(RegionId id_, int64_t start_, int64_t length_) : id(id_), start(start_), length(length_) {}
    bool Valid() const { return id != RegionId::none; }
    RegionId Id() const { return id; }
    int64_t Start() const { return start; }
    int64_t Length() const { return length; }
    void SetLength(int64_t length_) { length = length_; }
private:
    RegionId id;
    int64_t start;
    int64_t length;
};

class RegionTable
{
public:
    RegionTable();
    Region GetRegion(RegionId id) const;
    void SetRegion(const Region& region);
    void AddRegion(const Region& region);
    void FreeRegions();
private:
    std::vector<Region> regions;
};

void FreeRegion(Region& region, Process* fromProcess);
void CopyRegion(const Region& region, Process* fromProcess, Process* toProcess);
void ShareRegion(const Region& region, Process* fromProcess, Process* toProcess);

} // namespace cmajor::systemx::kernel
