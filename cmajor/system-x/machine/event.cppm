// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.event;

import std.core;

export namespace cmajor::systemx::machine {

enum class EventKind
{
    none, childExitEvent, alarmEvent, blockFreeEvent, inodeFreeEvent, ioEvent, pipeNotEmptyEvent, pipeNotFullEvent, terminalInputEvent, msgQEvent
};

std::string EventKindStr(EventKind eventKind);

struct Event
{
    Event() : kind(EventKind::none), id(-1) {}
    Event(EventKind kind_, int32_t id_) : kind(kind_), id(id_) {}
    std::string ToString() const;
    EventKind kind;
    int32_t id;
};

inline bool operator==(const Event& left, const Event& right)
{
    return left.kind == right.kind && left.id == right.id;
}

inline bool operator<(const Event& left, const Event& right)
{
    if (left.kind < right.kind) return true;
    if (left.kind > right.kind) return false;
    return left.id < right.id;
}

} // namespace cmajor::systemx::machine
