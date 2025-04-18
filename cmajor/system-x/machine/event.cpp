// =================================
// Copyright (c) 2025 Seppo Laakko
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
        case EventKind::pipeNotEmptyEvent:
        {
            return "PIPE_NOT_EMPTY";
        }
        case EventKind::pipeNotFullEvent:
        {
            return "PIPE_NOT_FULL";
        }
        case EventKind::terminalInputEvent:
        {
            return "TERMINAL_INPUT";
        }
        case EventKind::msgQEvent:
        {
            return "MSG_Q";
        }
        case EventKind::ioLockedEvent:
        {
            return "IO_LOCKED";
        }
    }
    return std::string();
}

std::string Event::ToString() const
{
    return "EVENT:" + EventKindStr(kind) + ":ID=" + std::to_string(id);
}

} // namespace cmajor::systemx::machine
