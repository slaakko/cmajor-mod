// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.linear_scan_register_allocator;

import cmajor.masm.intermediate.code;
import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.types;
import util;

namespace cmajor::masm::intermediate {

LiveRange GetLiveRange(Instruction* inst)
{
    if (inst->RequiresLocalRegister())
    {
        int lastUserIndex = inst->Index();
        const std::vector<Instruction*>& users = inst->Users();
        std::vector<Instruction*> finalUsers;
        for (Instruction* user : users)
        {
            if (user->IsArgInstruction())
            {
                while (user && !(user->IsFunctionCallInstruction() || user->IsProcedureCallInstruction()))
                {
                    user = user->Next();
                }
                if (user)
                {
                    finalUsers.push_back(user);
                }
            }
            else
            {
                finalUsers.push_back(user);
            }
        }
        for (Instruction* user : finalUsers)
        {
            if (user->Index() > lastUserIndex)
            {
                lastUserIndex = user->Index();
            }
        }
        LiveRange range(inst->Index(), lastUserIndex);
        return range;
    }
    else
    {
        return LiveRange(inst->Index(), inst->Index());
    }
}

LinearScanRegisterAllocator::LinearScanRegisterAllocator(Function& function, Context* context_) : 
    frame(), liveRanges(), activeInteger(), activeFP(), frameLocations(), registerGroups(), context(context_)
{
    ComputeLiveRanges(function);
}

void LinearScanRegisterAllocator::AddLiveRange(const LiveRange& liveRange, Instruction* inst)
{
    liveRanges.insert(liveRange);
    instructionRangeMap[inst] = liveRange;
    rangeInstructionMap[liveRange].push_back(inst);
    if (inst->IsParamInstruction() || inst->IsLocalInstruction())
    {
        AllocateFrameLocation(inst, false);
    }
}

void LinearScanRegisterAllocator::AddFreeRegGroupToPool(Instruction* inst)
{
    cmajor::masm::assembly::RegisterGroup* reg = GetRegisterGroup(inst);
    if (reg)
    {
        if (reg->IsFloatingPointReg())
        {
            context->AssemblyContext()->GetRegisterPool()->AddLocalXMMRegisterGroup(reg);
        }
        else
        {
            context->AssemblyContext()->GetRegisterPool()->AddLocalRegisterGroup(reg);
        }
        RemoveRegisterGroup(inst);
    }
}

void LinearScanRegisterAllocator::RemoveFromActive(const LiveRange& range, bool integer)
{
    if (integer)
    {
        activeInteger.erase(range);
    }
    else
    {
        activeFP.erase(range);
    }
    for (Instruction* inst : GetInstructions(range))
    {
        locations[inst] = locations[inst] & ~Locations::reg;
    }
}

bool LinearScanRegisterAllocator::NoFreeRegs(bool floatingPoint) const
{
    if (floatingPoint)
    {
        return context->AssemblyContext()->GetRegisterPool()->NumFreeLocalXMMRegisters() == 0;
    }
    else
    {
        return context->AssemblyContext()->GetRegisterPool()->NumFreeLocalRegisters() == 0;
    }
}

cmajor::masm::assembly::RegisterGroup* LinearScanRegisterAllocator::GetRegisterGroup(Instruction* inst) const
{
    auto it = registerGroups.find(inst);
    if (it != registerGroups.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void LinearScanRegisterAllocator::RemoveRegisterGroup(Instruction* inst)
{
    registerGroups.erase(inst);
}

FrameLocation LinearScanRegisterAllocator::GetFrameLocation(Instruction* inst) const
{
    auto it = frameLocations.find(inst);
    if (it != frameLocations.cend())
    {
        return it->second;
    }
    else
    {
        return FrameLocation();
    }
}

void LinearScanRegisterAllocator::AllocateRegister(Instruction* inst)
{
    if (inst->IsFloatingPointInstruction())
    {
        registerGroups[inst] = context->AssemblyContext()->GetRegisterPool()->GetLocalXMMRegisterGroup();
    }
    else
    {
        registerGroups[inst] = context->AssemblyContext()->GetRegisterPool()->GetLocalRegisterGroup();
    }
    LiveRange range = GetLiveRange(inst);
    if (inst->IsFloatingPointInstruction())
    {
        activeFP.insert(range);
    }
    else
    {
        activeInteger.insert(range);
    }
    locations[inst] = locations[inst] | Locations::reg;
}

void LinearScanRegisterAllocator::AllocateFrameLocation(Instruction* inst, bool spill)
{
    if (inst->IsParamInstruction())
    {
        ParamInstruction* paramInst = static_cast<ParamInstruction*>(inst);
        int alignment = 8;
        if (paramInst->GetType()->IsFloatingPointType())
        {
            alignment = 16;
        }
        int64_t size = util::Align(paramInst->GetType()->Size(), alignment);
        if (spill)
        {
            frameLocations[paramInst] = frame.GetFrameLocation(size);
            locations[paramInst] = locations[paramInst] | Locations::frame;
        }
        else
        {
            frameLocations[paramInst] = frame.GetParamLocation(size, context->AssemblyContext());
            locations[paramInst] = locations[paramInst] | Locations::frame;
        }
    }
    else if (inst->IsLocalInstruction())
    {
        LocalInstruction* localInst = static_cast<LocalInstruction*>(inst);
        int64_t size = util::Align(localInst->LocalType()->Size(), 8);
        frameLocations[localInst] = frame.GetFrameLocation(size);
        locations[localInst] = locations[localInst] | Locations::frame;
    }
    else
    {
        frameLocations[inst] = frame.GetFrameLocation(8);
        locations[inst] = locations[inst] | Locations::frame;
    }
}

void LinearScanRegisterAllocator::Spill(Instruction* inst)
{
    spillDataVec.clear();
    LiveRange spill;
    bool integer = false;
    if (inst->IsFloatingPointInstruction())
    {
        spill = *--activeFP.cend();
    }
    else
    {
        spill = *--activeInteger.cend();
        integer = true;
    }
    LiveRange range = GetLiveRange(inst);
    for (Instruction* instToSpill : GetInstructions(spill))
    {
        registerGroups[inst] = registerGroups[instToSpill];
        AllocateFrameLocation(instToSpill, true);
        locations[instToSpill] = Locations::frame;
        locations[inst] = locations[inst] | Locations::reg;
        if (integer)
        {
            activeInteger.erase(spill);
            activeInteger.insert(range);
        }
        else
        {
            activeFP.erase(spill);
            activeFP.insert(range);
        }
        SpillData spillData;
        spillData.registerGroupToSpill = registerGroups[instToSpill];
        spillData.spillToFrameLocation = frameLocations[instToSpill];
        spillData.instToSpill = instToSpill;
        spillDataVec.push_back(spillData);
    }
}

void LinearScanRegisterAllocator::RemoveFromRegisterGroups(Instruction* inst)
{
    registerGroups.erase(inst);
}

LiveRange LinearScanRegisterAllocator::GetLiveRange(Instruction* inst) const
{
    auto it = instructionRangeMap.find(inst);
    if (it != instructionRangeMap.cend())
    {
        return it->second;
    }
    else
    {
        return LiveRange(inst->Index(), inst->Index());
    }
}

const std::vector<Instruction*>& LinearScanRegisterAllocator::GetInstructions(const LiveRange& range) const
{
    static std::vector<Instruction*> empty;
    auto it = rangeInstructionMap.find(range);
    if (it != rangeInstructionMap.cend())
    {
        return it->second;
    }
    else
    {
        return empty;
    }
}

void LinearScanRegisterAllocator::ComputeLiveRanges(Function& function)
{
    BasicBlock* block = function.FirstBasicBlock();
    while (block)
    {
        Instruction* inst = block->FirstInstruction();
        while (inst)
        {
            LiveRange liveRange = cmajor::masm::intermediate::GetLiveRange(inst);
            if (liveRange.Valid())
            {
                AddLiveRange(liveRange, inst);
            }
            inst = inst->Next();
        }
        block = block->Next();
    }
}

void LinearScanRegisterAllocator::ExpireOldRanges(const LiveRange& range)
{
    std::vector<LiveRange> toRemove;
    for (const auto& activeRange : activeInteger)
    {
        if (activeRange.end >= range.start) break;
        toRemove.push_back(activeRange);
        for (Instruction* inst : GetInstructions(activeRange))
        {
            AddFreeRegGroupToPool(inst);
        }
    }
    for (const auto& r : toRemove)
    {
        RemoveFromActive(r, true);
    }
    toRemove.clear();
    for (const auto& activeRange : activeFP)
    {
        if (activeRange.end >= range.start) break;
        toRemove.push_back(activeRange);
        for (Instruction* inst : GetInstructions(activeRange))
        {
            AddFreeRegGroupToPool(inst);
        }
    }
    for (const auto& r : toRemove)
    {
        RemoveFromActive(r, false);
    }
}

Locations LinearScanRegisterAllocator::GetLocations(Instruction* inst) const
{
    auto it = locations.find(inst);
    if (it != locations.cend())
    {
        return it->second;
    }
    return Locations::none;
}

void LinearScanRegisterAllocator::AddRegisterLocation(Instruction* inst, cmajor::masm::assembly::RegisterGroup* regGroup)
{
    locations[inst] = locations[inst] | Locations::reg;
    registerGroups[inst] = regGroup;
}

int LinearScanRegisterAllocator::LastActiveLocalRegGroup() const
{
    int lastActiveLocalRegGroup = -1;
    for (const auto& instRegGroup : registerGroups)
    {
        cmajor::masm::assembly::RegisterGroup* regGroup = instRegGroup.second;
        if (regGroup->IsLocal())
        {
            if (int(regGroup->Kind()) > lastActiveLocalRegGroup)
            {
                lastActiveLocalRegGroup = int(regGroup->Kind());
            }
        }
    }
    return lastActiveLocalRegGroup;
}

RegisterAllocationAction LinearScanRegisterAllocator::Run(Instruction* inst)
{
    LiveRange liveRange = GetLiveRange(inst);
    ExpireOldRanges(liveRange);
    if (inst->RequiresLocalRegister())
    {
        if (NoFreeRegs(inst->IsFloatingPointInstruction()))
        {
            Spill(inst);
            return RegisterAllocationAction::spill;
        }
        else
        {
            AllocateRegister(inst);
            return RegisterAllocationAction::registerAllocated;
        }
    }
    else
    {
        return RegisterAllocationAction::none;
    }
}

const std::vector<SpillData>& LinearScanRegisterAllocator::GetSpillData() const
{
    return spillDataVec;
}

std::unique_ptr<LinearScanRegisterAllocator> CreateLinearScanRegisterAllocator(Function& function, Context* context)
{
    std::unique_ptr<LinearScanRegisterAllocator> registerAllocator(new LinearScanRegisterAllocator(function, context));
    return registerAllocator;
}

} // cmajor::masm::intermediate
