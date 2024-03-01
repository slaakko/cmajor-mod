// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.init.done;

import cmajor.systemx.kernel.trap;
import cmajor.systemx.kernel.trap.exit;
import cmajor.systemx.kernel.memory.traps;
import cmajor.systemx.kernel.io.traps;
import cmajor.systemx.kernel.time.traps;
import cmajor.systemx.kernel.math.traps;
import cmajor.systemx.kernel.trap.random;
import cmajor.systemx.kernel.exception.handling.traps;
import cmajor.systemx.kernel.process.management.traps;
import cmajor.systemx.kernel.kernel;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.process.manager;
import cmajor.systemx.kernel.scheduler;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.clock;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.host.file.manager;
import cmajor.systemx.kernel.io.manager;
import cmajor.systemx.kernel.pipe;
import cmajor.systemx.kernel.terminal;
import cmajor.systemx.kernel.msg.queue;
import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.prog;
import cmajor.systemx.kernel.inode.manager;

namespace cmajor::systemx::kernel {

void Init(bool initTerminal)
{
    InitTrap();
    InitTrapExit();
    InitMemoryTraps();
    InitIOTraps();
    InitTimeTraps();
    InitMathTraps();
    InitTrapRandom();
    InitExceptionHandlingTraps();
    InitProcessManager();
    InitProcessManagementTraps();
    InitScheduler();
    InitEventManager();
    InitBlockManager();
    InitINodeManager();
    InitHostFileManager();
    InitClock();
    InitPipe();
    if (initTerminal)
    {
        InitTerminal();
    }
    InitMsgQueue();
    InitKernel();
    InitDebug();
    InitProg();
}

void Done()
{
    DoneProg();
    DoneDebug();
    DoneKernel();
    DoneMsgQueue();
    DoneTerminal();
    DonePipe();
    DoneClock();
    DoneHostFileManager();
    DoneINodeManager();
    DoneBlockManager();
    DoneEventManager();
    DoneScheduler();
    DoneProcessManagementTraps();
    DoneProcessManager();
    DoneExceptionHandlingTraps();
    DoneTrapRandom();
    DoneMathTraps();
    DoneTimeTraps();
    DoneIOTraps();
    DoneMemoryTraps();
    DoneTrapExit();
    DoneTrap();
}

} // namespace cmajor::systemx::kernel
