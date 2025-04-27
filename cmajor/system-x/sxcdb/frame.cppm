// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.sxcdb.frame;

import cmajor.systemx.object;
import cmajor.systemx.kernel;
import soul.ast.span;
import std.core;

export namespace cmajor::systemx::sxcdb {

class Frame
{
public:
    Frame();
    Frame(int index_, uint64_t pc_, cmajor::systemx::object::FunctionTableEntry* entry_, soul::ast::LineColLen lineColLen_, int32_t idx_);
    int Index() const { return index; }
    uint64_t PC() const { return pc; }
    cmajor::systemx::object::FunctionTableEntry* Entry() const { return entry; }
    const soul::ast::LineColLen& LineColLen() const { return lineColLen; }
    int32_t Idx() const { return idx; }
private:
    int index;
    uint64_t pc;
    cmajor::systemx::object::FunctionTableEntry* entry;
    soul::ast::LineColLen lineColLen;
    int32_t idx;
};

class Frames
{
public:
    Frames();
    int FrameCount() const { return static_cast<int>(frames.size()); }
    const Frame& GetFrame(int frameIndex) const { return frames[frameIndex]; }
    void AddFrame(const Frame& frame);
private:
    std::vector<Frame> frames;
};

Frames GetFrames(cmajor::systemx::kernel::Process* process);

} // namespace cmajor::systemx::sxcdb
