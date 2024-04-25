// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.register_allocator;

namespace cmajor::masm::intermediate {

RegisterAllocator::RegisterAllocator()
{
}

RegisterAllocator::~RegisterAllocator()
{
}

SpillData::SpillData() : registerGroupToSpill(nullptr), instToSpill(nullptr)
{
}

} // cmajor::masm::intermediate
