// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.linear.scan.reg.allocator;

import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.reg.allocator;

export namespace cmajor::systemx::intermediate {

struct LiveRange
{
    LiveRange() : start(-1), end(-1) {}
    LiveRange(int start_, int end_) : start(start_), end(end_) {}
    bool Valid() const { return start != -1 && end != -1; }
    int start;
    int end;
};

inline bool operator<(const LiveRange& left, const LiveRange& right)
{
    if (left.start < right.start) return true;
    if (left.start > right.start) return false;
    return left.end < right.end;
}

struct LiveRangeByStart
{
    bool operator()(const LiveRange& left, const LiveRange& right) const
    {
        if (left.start < right.start) return true;
        if (left.start > right.start) return false;
        return left.end < right.end;
    }
};

struct LiveRangeByEnd
{
    bool operator()(const LiveRange& left, const LiveRange& right) const
    {
        if (left.end < right.end) return true;
        if (left.end > right.end) return false;
        return left.start < right.start;
    }
};

LiveRange GetLiveRange(Instruction* inst);

class LinearScanRegisterAllocator : public RegisterAllocator
{
public:
    LinearScanRegisterAllocator(Function& function, Context* context_);
    void AddLiveRange(const LiveRange& liveRange, Instruction* inst);
    void AddFreeRegToPool(Instruction* inst);
    void RemoveFromActive(const LiveRange& range);
    bool NoFreeRegs() const;
    const std::set<LiveRange, LiveRangeByStart>& LiveRanges() const { return liveRanges; }
    const std::set<LiveRange, LiveRangeByEnd> Active() const { return active; }
    FrameLocation GetFrameLocation(Instruction* inst) const;
    Register GetRegister(Instruction* inst) const override;
    void RemoveRegister(Instruction* inst);
    void AllocateRegister(Instruction* inst);
    void AllocateFrameLocation(Instruction* inst);
    void Spill(Instruction* inst);
    LiveRange GetLiveRange(Instruction* inst) const;
    Frame& GetFrame() override { return frame; }
    const std::vector<Instruction*>& GetInstructions(const LiveRange& range) const;
    RegisterAllocationAction Run(Instruction* inst) override;
    Locations GetLocations(Instruction* inst) const override;
    virtual void AddRegisterLocation(Instruction* inst, const Register& reg) override;
    int LastActiveLocalReg() const override;
    const std::vector<SpillData>& GetSpillData() const override;
private:
    void ComputeLiveRanges(Function& function);
    void ExpireOldRanges(const LiveRange& range);
    Frame frame;
    std::set<LiveRange, LiveRangeByStart> liveRanges;
    std::set<LiveRange, LiveRangeByEnd> active;
    std::map<Instruction*, FrameLocation> frameLocations;
    std::map<Instruction*, Register> registers;
    std::map<Instruction*, LiveRange> instructionRangeMap;
    std::map<LiveRange, std::vector<Instruction*>> rangeInstructionMap;
    std::map<Instruction*, Locations> locations;
    std::vector<SpillData> spillDataVec;
    Context* context;
};

std::unique_ptr<LinearScanRegisterAllocator> LinearScanRegisterAllocation(Function& function, Context* context);

} // cmajor::systemx::::intermediate
