// =================================
// Copyright (c) 2025 Seppo Laakko
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
    FrameLocation() : reg(masm::assembly::RegisterGroupKind::rbp), index(-1), offset(0), size(0), macro(nullptr) {}
    FrameLocation(masm::assembly::RegisterGroupKind reg_, int index_, int64_t offset_, int64_t size_) : reg(reg_), index(index_), offset(offset_), size(size_), macro(nullptr) {}
    void Dump();
    bool Valid() const { return index != -1; }
    masm::assembly::RegisterGroupKind reg;
    int index;
    int64_t offset;
    int64_t size;
    masm::assembly::Macro* macro;
};

struct ArgLocation
{
    ArgLocation(int index_, int64_t offset_, int64_t size_) : reg(masm::assembly::RegisterGroupKind::rsp), index(index_), offset(offset_), size(size_) {}
    masm::assembly::RegisterGroupKind reg;
    int index;
    int64_t offset;
    int64_t size;
};

class Frame
{
public:
    Frame();
    FrameLocation GetParamLocation(int64_t size, cmajor::masm::assembly::Context* assemblyContext);
    FrameLocation GetFrameLocation(int64_t size);
    void SetRbxPushed() { rbxPushed = true; }
    bool RbxPushed() const { return rbxPushed; }
    int64_t Size() const;
    int64_t CalleeParamAreaSize() const { return calleeParamAreaSize; }
    int64_t XMMSaveRegSize() const { return 16 * numUsedXMMRegs; }
    void SetCalleeParamAreaSize(int64_t calleeParamAreaSize_) { calleeParamAreaSize = calleeParamAreaSize_; }
    void SetMacroValues(cmajor::masm::assembly::Context* assemblyContext);
    void SetNumUsedXMMRegs(int numUsedXMMRegs_) { numUsedXMMRegs = numUsedXMMRegs_; }
    int GetNumUsedXMMRegs() const { return numUsedXMMRegs; }
    void Dump();
private:
    std::vector<FrameLocation> paramLocations;
    std::vector<FrameLocation> frameLocations;
    int64_t calleeParamAreaSize;
    int numUsedXMMRegs;
    bool rbxPushed;
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
