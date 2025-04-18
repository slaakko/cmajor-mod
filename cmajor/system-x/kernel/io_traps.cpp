// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.io.traps;

import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.trap;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.pipe;
import cmajor.systemx.kernel.terminal;
import cmajor.systemx.kernel.msg.queue;
import cmajor.systemx.kernel.socket;
import cmajor.systemx.kernel.inode.manager;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

class TrapCreateHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_create"; }
};

uint64_t TrapCreateHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int32_t mode = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        return Create(process, pathAddr, mode);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapOpenHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_open"; }
};

uint64_t TrapOpenHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int32_t flags = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int32_t mode = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        return Open(process, pathAddr, flags, mode);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapCloseHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_close"; }
};

uint64_t TrapCloseHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        Close(process, fd);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapReadHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_read"; }
};

uint64_t TrapReadHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t bufferAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int64_t count = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        return Read(process, fd, bufferAddr, count);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapWriteHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_write"; }
};

uint64_t TrapWriteHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t bufferAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int64_t count = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        return Write(process, fd, bufferAddr, count);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapIOCtlHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_ioctl"; }
};

uint64_t TrapIOCtlHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int32_t item = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int64_t argAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        int64_t argSize = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regDX));
        return IOCtl(process, fd, item, argAddr, argSize);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapUnlinkHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_unlink"; }
};

uint64_t TrapUnlinkHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        Unlink(process, pathAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapSeekHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_seek"; }
};

uint64_t TrapSeekHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t offset = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int32_t whence = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        return Seek(process, fd, offset, whence);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapTellHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_tell"; }
};

uint64_t TrapTellHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        return Tell(process, fd);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapStatHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_stat"; }
};

uint64_t TrapStatHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
    int64_t statBufAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
    int32_t statBufSize = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
    SystemError error;
    if (!Stat(process, pathAddr, statBufAddr, statBufSize, error))
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
    return 0;
}

class TrapGetCWDHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_getcwd"; }
};

uint64_t TrapGetCWDHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t bufAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t bufSize = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        GetCWD(process, bufAddr, bufSize);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapChDirHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_chdir"; }
};

uint64_t TrapChDirHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        ChDir(process, pathAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapMkDirHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_mkdir"; }
};

uint64_t TrapMkDirHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int32_t mode = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        MkDir(process, pathAddr, mode);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapOpenDirHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_opendir"; }
};

uint64_t TrapOpenDirHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        return OpenDir(process, pathAddr);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapCloseDirHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_closedir"; }
};

uint64_t TrapCloseDirHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t dd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        CloseDir(process, dd);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapReadDirHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_readdir"; }
};

uint64_t TrapReadDirHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t dd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t dirEntryBufAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int64_t dirEntryBufSize = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        return ReadDir(process, dd, dirEntryBufAddr, dirEntryBufSize);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapUTimeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_utime"; }
};

uint64_t TrapUTimeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t timeBufAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int64_t timeBufSize = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        UTime(process, pathAddr, timeBufAddr, timeBufSize);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapPipeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_pipe"; }
};

uint64_t TrapPipeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t readerFdAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t writerFdAddr = static_cast<int64_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        MakePipe(process, readerFdAddr, writerFdAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapDupHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_dup"; }
};

uint64_t TrapDupHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t fd = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        return Dup(process, fd);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapChModHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_chmod"; }
};

uint64_t TrapChModHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        int32_t mode = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        ChMod(process, pathAddr, mode);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapChOwnHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_chown"; }
};

uint64_t TrapChOwnHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t pathAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        int32_t uid = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regBX));
        int32_t gid = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        ChOwn(process, pathAddr, uid, gid);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapRenameHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_rename"; }
};

uint64_t TrapRenameHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t sourcePathAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        int64_t targetPathAddr = processor.Regs().Get(cmajor::systemx::machine::regBX);
        Rename(process, sourcePathAddr, targetPathAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapAddDirChangeNotificationHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_add_dir_change_notification"; }
};

uint64_t TrapAddDirChangeNotificationHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t directoryPathsAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        AddDirChangeNotification(process, directoryPathsAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapRemoveDirChangeNotificationsHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_remove_dir_change_notifications"; }
};

uint64_t TrapRemoveDirChangeNotificationsHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        RemoveDirChangeNotifications(process->Id());
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapDirectoriesChangedHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_directories_changed"; }
};

uint64_t TrapDirectoriesChangedHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        return process->DirectoriesChanged();
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapClearDirectoriesChangedHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_clear_directories_changed"; }
};

uint64_t TrapClearDirectoriesChangedHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        process->ClearDirectoriesChanged();
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapMsgQHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_msgq"; }
};

uint64_t TrapMsgQHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t nameAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        return MsgQ(process, nameAddr);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapCloseMsgQHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_close_msgq"; }
};

uint64_t TrapCloseMsgQHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        CloseMsgQ(process, md);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapWaitMsgHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_wait_msg"; }
};

uint64_t TrapWaitMsgHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        WaitMsg(process, md);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapPutMsgHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_put_msg"; }
};

uint64_t TrapPutMsgHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t msgDataAddr = processor.Regs().Get(cmajor::systemx::machine::regBX);
        int32_t msgSize = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regCX));
        PutMsg(process, md, msgDataAddr, msgSize);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapGetMsgQLengthHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_get_msgq_length"; }
};

uint64_t TrapGetMsgQLengthHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        return GetMsgQueueLength(process, md);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapGetMsgSizeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_get_msg_size"; }
};

uint64_t TrapGetMsgSizeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        return GetMsgSize(process, md);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapGetMsgHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_get_msg"; }
};

uint64_t TrapGetMsgHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        int64_t bufferAddr = processor.Regs().Get(cmajor::systemx::machine::regBX);
        GetMsg(process, md, bufferAddr);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapBindTerminalHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_bind_terminal"; }
};

uint64_t TrapBindTerminalHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t md = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        BindTerminal(process, md);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapUnbindTerminalHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_unbind_terminal"; }
};

uint64_t TrapUnbindTerminalHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        UnbindTerminal(process);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapConnectHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_connect"; }
};

uint64_t TrapConnectHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t nodeAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        int64_t serviceddr = processor.Regs().Get(cmajor::systemx::machine::regBX);
        return Connect(process, nodeAddr, serviceddr);
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapLockIOHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_lock_io"; }
};

uint64_t TrapLockIOHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        LockIO(process);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapUnlockIOHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_unlock_io"; }
};

uint64_t TrapUnlockIOHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        UnlockIO(process);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

void InitIOTraps()
{
    SetTrapHandler(trap_create, new TrapCreateHandler());
    SetTrapHandler(trap_open, new TrapOpenHandler());
    SetTrapHandler(trap_close, new TrapCloseHandler());
    SetTrapHandler(trap_read, new TrapReadHandler());
    SetTrapHandler(trap_write, new TrapWriteHandler());
    SetTrapHandler(trap_ioctl, new TrapIOCtlHandler());
    SetTrapHandler(trap_unlink, new TrapUnlinkHandler());
    SetTrapHandler(trap_seek, new TrapSeekHandler());
    SetTrapHandler(trap_tell, new TrapTellHandler());
    SetTrapHandler(trap_stat, new TrapStatHandler());
    SetTrapHandler(trap_getcwd, new TrapGetCWDHandler());
    SetTrapHandler(trap_chdir, new TrapChDirHandler());
    SetTrapHandler(trap_mkdir, new TrapMkDirHandler());
    SetTrapHandler(trap_opendir, new TrapOpenDirHandler());
    SetTrapHandler(trap_closedir, new TrapCloseDirHandler());
    SetTrapHandler(trap_readdir, new TrapReadDirHandler());
    SetTrapHandler(trap_utime, new TrapUTimeHandler());
    SetTrapHandler(trap_pipe, new TrapPipeHandler());
    SetTrapHandler(trap_dup, new TrapDupHandler());
    SetTrapHandler(trap_chmod, new TrapChModHandler());
    SetTrapHandler(trap_chown, new TrapChOwnHandler());
    SetTrapHandler(trap_rename, new TrapRenameHandler());
    SetTrapHandler(trap_add_dir_change_notification, new TrapAddDirChangeNotificationHandler());
    SetTrapHandler(trap_remove_dir_change_notifications, new TrapRemoveDirChangeNotificationsHandler());
    SetTrapHandler(trap_directories_changed, new TrapDirectoriesChangedHandler());
    SetTrapHandler(trap_clear_directories_changed, new TrapClearDirectoriesChangedHandler());
    SetTrapHandler(trap_msgq, new TrapMsgQHandler());
    SetTrapHandler(trap_close_msgq, new TrapCloseMsgQHandler());
    SetTrapHandler(trap_wait_msg, new TrapWaitMsgHandler());
    SetTrapHandler(trap_put_msg, new TrapPutMsgHandler());
    SetTrapHandler(trap_get_msgq_length, new TrapGetMsgQLengthHandler());
    SetTrapHandler(trap_get_msg_size, new TrapGetMsgSizeHandler());
    SetTrapHandler(trap_get_msg, new TrapGetMsgHandler());
    SetTrapHandler(trap_bind_terminal, new TrapBindTerminalHandler());
    SetTrapHandler(trap_unbind_terminal, new TrapUnbindTerminalHandler());
    SetTrapHandler(trap_connect, new TrapConnectHandler());
    SetTrapHandler(trap_lock_io, new TrapLockIOHandler());
    SetTrapHandler(trap_unlock_io, new TrapUnlockIOHandler());
}

void DoneIOTraps()
{
    SetTrapHandler(trap_connect, nullptr);
    SetTrapHandler(trap_get_msg, nullptr);
    SetTrapHandler(trap_get_msg_size, nullptr);
    SetTrapHandler(trap_get_msgq_length, nullptr);
    SetTrapHandler(trap_put_msg, nullptr);
    SetTrapHandler(trap_wait_msg, nullptr);
    SetTrapHandler(trap_close_msgq, nullptr);
    SetTrapHandler(trap_msgq, nullptr);
    SetTrapHandler(trap_clear_directories_changed, nullptr);
    SetTrapHandler(trap_directories_changed, nullptr);
    SetTrapHandler(trap_remove_dir_change_notifications, nullptr);
    SetTrapHandler(trap_add_dir_change_notification, nullptr);
    SetTrapHandler(trap_rename, nullptr);
    SetTrapHandler(trap_chown, nullptr);
    SetTrapHandler(trap_chmod, nullptr);
    SetTrapHandler(trap_dup, nullptr);
    SetTrapHandler(trap_pipe, nullptr);
    SetTrapHandler(trap_utime, nullptr);
    SetTrapHandler(trap_readdir, nullptr);
    SetTrapHandler(trap_closedir, nullptr);
    SetTrapHandler(trap_opendir, nullptr);
    SetTrapHandler(trap_mkdir, nullptr);
    SetTrapHandler(trap_chdir, nullptr);
    SetTrapHandler(trap_getcwd, nullptr);
    SetTrapHandler(trap_stat, nullptr);
    SetTrapHandler(trap_tell, nullptr);
    SetTrapHandler(trap_seek, nullptr);
    SetTrapHandler(trap_unlink, nullptr);
    SetTrapHandler(trap_ioctl, nullptr);
    SetTrapHandler(trap_write, nullptr);
    SetTrapHandler(trap_read, nullptr);
    SetTrapHandler(trap_close, nullptr);
    SetTrapHandler(trap_open, nullptr);
    SetTrapHandler(trap_create, nullptr);
    SetTrapHandler(trap_lock_io, nullptr);
    SetTrapHandler(trap_unlock_io, nullptr);
}

} // namespace cmajor::systemx::kernel
