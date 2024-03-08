// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.location;

import cmajor.masm.assembly;
import std.core;

export namespace cmajor::masm::intermediate {

enum class Locations : int
{
    none = 0, reg = 1 << 0, frame = 1 << 1
};

constexpr Locations operator|(Locations left, Locations right)
{
    return Locations(int(left) | int(right));
}

constexpr Locations operator&(Locations left, Locations right)
{
    return Locations(int(left) & int(right));
}

constexpr Locations operator~(Locations locs)
{
    return Locations(~int(locs));
}

struct FrameLocation
{
    FrameLocation() : reg(masm::assembly::RegisterGroupKind::rbp), index(-1), offset(0), size(0) {}
    FrameLocation(masm::assembly::RegisterGroupKind reg_, int index_, int64_t offset_, int64_t size_) : reg(reg_), index(index_), offset(offset_), size(size_) {}
    bool Valid() const { return index != -1; }
    int64_t ItemOffset() const { return offset + size; }
    masm::assembly::RegisterGroupKind reg;
    int index;
    int64_t offset;
    int64_t size;
};

struct ArgLocation
{
    ArgLocation(int index_, int64_t offset_, int64_t size_) : index(index_), offset(offset_), size(size_) {}
    int64_t ItemOffset() const { return offset + size; }
    int index;
    int64_t offset;
    int64_t size;
};

class Frame
{
public:
    Frame();
    FrameLocation GetParamLocation(int64_t size);
    FrameLocation GetFrameLocation(int64_t size);
    int64_t Size() const;
    int64_t LocalSize() const { return localSize; }
private:
    std::vector<FrameLocation> paramLocations;
    std::vector<FrameLocation> frameLocations;
    int64_t localSize;
};

class CallFrame
{
public:
    CallFrame();
    void AllocateArgLocation(int64_t size);
    ArgLocation GetArgLocation(int index);
private:
    std::vector<ArgLocation> argLocations;
};

} // cmajor::masm::intermediate
