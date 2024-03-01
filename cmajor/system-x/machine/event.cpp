// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.machine.event;

namespace cmajor::systemx::machine {

std::string EventKindStr(EventKind eventKind)
{
    switch (eventKind)
    {
    case EventKind::childExitEvent:
    {
        return "CHILD_EXIT";
    }
    case EventKind::alarmEvent:
    {
        return "ALARM";
    }
    case EventKind::blockFreeEvent:
    {
        return "BLOCK_FREE";
    }
    case EventKind::inodeFreeEvent:
    {
        return "INODE_FREE";
    }
    case EventKind::ioEvent:
    {
        return "IO";
    }
    }
    return std::string();
}

std::string Event::ToString() const
{
    return "EVENT:" + EventKindStr(kind) + ":ID=" + std::to_string(id);
}

} // namespace cmajor::systemx::machine
