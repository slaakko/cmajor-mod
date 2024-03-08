// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.intermediate.util;

import std.core;

export namespace cmajor::masm::intermediate {

class Function;

template<typename PtrT>
inline void AddPtrToSet(PtrT* ptr, std::vector<PtrT*>& set)
{
    if (std::find(set.cbegin(), set.cend(), ptr) == set.cend())
    {
        set.push_back(ptr);
    }
}

template<typename PtrT>
bool RemovePtrFromSet(PtrT* ptr, std::vector<PtrT*>& set)
{
    auto it = std::remove(set.begin(), set.end(), ptr);
    if (it == set.cend())
    {
        return false;
    }
    else
    {
        set.erase(it, set.end());
        return true;
    }
}

template<typename PtrT>
inline bool PtrInSet(PtrT* ptr, const std::vector<PtrT*>& set)
{
    return std::find(set.cbegin(), set.cend(), ptr) != set.cend();
}

} // cmajor::masm::intermediate
