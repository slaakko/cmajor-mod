// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.register_allocator;

import cmajor.masm.intermediate.location;
import cmajor.masm.assembly;
import std.core;

export namespace cmajor::masm::intermediate {

class Instruction;

struct SpillData
{
    SpillData();
    cmajor::masm::assembly::RegisterGroup* registerGroupToSpill;
    FrameLocation spillToFrameLocation;
};

enum class RegisterAllocationAction
{
    none, spill, registerAllocated
};

class RegisterAllocator
{
public:
    RegisterAllocator();
    virtual ~RegisterAllocator();
    virtual RegisterAllocationAction Run(Instruction* inst, int argIndex) = 0;
    virtual const std::vector<SpillData>& GetSpillData() const = 0;
    virtual Locations GetLocations(Instruction* inst) const = 0;
    virtual cmajor::masm::assembly::RegisterGroup* GetRegisterGroup(Instruction* inst) const = 0;
    virtual void AddRegisterLocation(Instruction* inst, cmajor::masm::assembly::RegisterGroup* regGroup) = 0;
    virtual Frame& GetFrame() = 0;
    virtual FrameLocation GetFrameLocation(Instruction* inst) const = 0;
    virtual int LastActiveLocalRegGroup() const = 0;
};

} // cmajor::masm::intermediate
