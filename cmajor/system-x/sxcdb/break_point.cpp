// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.sxcdb.break_point;

namespace cmajor::systemx::sxcdb {

bool operator<(const BreakPoint& left, const BreakPoint& right)
{
    return left.Id() < right.Id();
}

} // namespace cmajor::systemx::sxcdb
