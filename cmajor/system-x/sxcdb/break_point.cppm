// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.break_point;

import cmajor.systemx.object;
import std.core;

export namespace cmajor::systemx::sxcdb {

class BreakPoint;

bool operator<(const BreakPoint& left, const BreakPoint& right);

class BreakPoint
{
public:
    BreakPoint() : pc(0), id(-1), entry(nullptr), line(-1) {}
    BreakPoint(uint64_t pc_) : pc(pc_), id(-1), entry(nullptr), line(-1) {}
    BreakPoint(uint64_t pc_, int id_, cmajor::systemx::object::FunctionTableEntry* entry_, int line_) : pc(pc_), id(id_), entry(entry_), line(line_) {}
    uint64_t PC() const { return pc; }
    int Id() const { return id; }
    cmajor::systemx::object::FunctionTableEntry* Entry() const { return entry; }
    int Line() const { return line; }
private:
    uint64_t pc;
    int id;
    cmajor::systemx::object::FunctionTableEntry* entry;
    int line;
};

} // namespace cmajor::systemx::sxcdb
