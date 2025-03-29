// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel;

export import cmajor.systemx.kernel.block.file;
export import cmajor.systemx.kernel.block.manager;
export import cmajor.systemx.kernel.clock;
export import cmajor.systemx.kernel.compression;
export import cmajor.systemx.kernel.debug;
export import cmajor.systemx.kernel.debug.help;
export import cmajor.systemx.kernel.debug.msg;
export import cmajor.systemx.kernel.dev.fs;
export import cmajor.systemx.kernel.dir.file;
export import cmajor.systemx.kernel.error;
export import cmajor.systemx.kernel.event.manager;
export import cmajor.systemx.kernel.exception.handling;
export import cmajor.systemx.kernel.exception.handling.traps;
export import cmajor.systemx.kernel.file;
export import cmajor.systemx.kernel.fs;
export import cmajor.systemx.kernel.heap;
export import cmajor.systemx.kernel.host.file.manager;
export import cmajor.systemx.kernel.host.fs;
export import cmajor.systemx.kernel.init.done;
export import cmajor.systemx.kernel.inode.manager;
export import cmajor.systemx.kernel.io;
export import cmajor.systemx.kernel.io.manager;
export import cmajor.systemx.kernel.io.traps;
export import cmajor.systemx.kernel.kernel;
export import cmajor.systemx.kernel.load;
export import cmajor.systemx.kernel.math.traps;
export import cmajor.systemx.kernel.memory.traps;
export import cmajor.systemx.kernel.mount;
export import cmajor.systemx.kernel.msg.queue;
export import cmajor.systemx.kernel.os.api;
export import cmajor.systemx.kernel.pipe;
export import cmajor.systemx.kernel.process;
export import cmajor.systemx.kernel.process.management.traps;
export import cmajor.systemx.kernel.process.manager;
export import cmajor.systemx.kernel.prog;
export import cmajor.systemx.kernel.region;
export import cmajor.systemx.kernel.resource;
export import cmajor.systemx.kernel.root.fs;
export import cmajor.systemx.kernel.scheduler;
export import cmajor.systemx.kernel.socket;
export import cmajor.systemx.kernel.terminal;
export import cmajor.systemx.kernel.time;
export import cmajor.systemx.kernel.time.traps;
export import cmajor.systemx.kernel.trap;
export import cmajor.systemx.kernel.trap.exit;
export import cmajor.systemx.kernel.trap.random;
export import cmajor.systemx.kernel.config;
