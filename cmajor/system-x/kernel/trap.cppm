// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.trap;

import cmajor.systemx.machine;
import cmajor.systemx.object;
import std.core;

export namespace cmajor::systemx::kernel {

const uint8_t trap_exit = 0;
const uint8_t trap_memory_page_size = 1;
const uint8_t trap_heap_start = 2;
const uint8_t trap_heap_length = 3;
const uint8_t trap_allocate_memory_pages = 4;
const uint8_t trap_dump_heap = 5;
const uint8_t trap_random_seed = 6;
const uint8_t trap_current_time_point = 7;
const uint8_t trap_sleep = 8;
const uint8_t trap_current_date = 9;
const uint8_t trap_current_date_time = 10;
const uint8_t trap_times = 11;
const uint8_t trap_pow = 12;
const uint8_t trap_throw = 13;
const uint8_t trap_catch = 14;
const uint8_t trap_resume = 15;
const uint8_t trap_stack_trace = 16;
const uint8_t trap_get_system_error = 17;
const uint8_t trap_fork = 18;
const uint8_t trap_exec = 19;
const uint8_t trap_wait = 20;
const uint8_t trap_getpid = 21;
const uint8_t trap_create = 22;
const uint8_t trap_open = 23;
const uint8_t trap_close = 24;
const uint8_t trap_read = 25;
const uint8_t trap_write = 26;
const uint8_t trap_ioctl = 27;
const uint8_t trap_unlink = 28;
const uint8_t trap_seek = 29;
const uint8_t trap_tell = 30;
const uint8_t trap_stat = 31;
const uint8_t trap_getcwd = 32;
const uint8_t trap_chdir = 33;
const uint8_t trap_mkdir = 34;
const uint8_t trap_opendir = 35;
const uint8_t trap_closedir = 36;
const uint8_t trap_readdir = 37;
const uint8_t trap_utime = 38;
const uint8_t trap_mcpy = 39;
const uint8_t trap_open_resource = 40;
const uint8_t trap_close_resource = 41;
const uint8_t trap_get_resource_size = 42;
const uint8_t trap_read_resource = 43;
const uint8_t trap_decompress = 44;
const uint8_t trap_get_decompressed_data_size = 45;
const uint8_t trap_get_decompressed_data = 46;
const uint8_t trap_close_decompression = 47;
const uint8_t trap_get_host_name = 48;
const uint8_t trap_get_user_name = 49;
const uint8_t trap_pipe = 50;
const uint8_t trap_dup = 51;
const uint8_t trap_setuid = 52;
const uint8_t trap_setgid = 53;
const uint8_t trap_getuid = 54;
const uint8_t trap_getgid = 55;
const uint8_t trap_seteuid = 56;
const uint8_t trap_setegid = 57;
const uint8_t trap_geteuid = 58;
const uint8_t trap_getegid = 59;
const uint8_t trap_umask = 60;
const uint8_t trap_chmod = 61;
const uint8_t trap_chown = 62;
const uint8_t trap_rename = 63;
const uint8_t trap_add_dir_change_notification = 64;
const uint8_t trap_remove_dir_change_notifications = 65;
const uint8_t trap_directories_changed = 66;
const uint8_t trap_clear_directories_changed = 67;
const uint8_t trap_child_times = 68;
const uint8_t trap_msgq = 69;
const uint8_t trap_close_msgq = 70;
const uint8_t trap_wait_msg = 71;
const uint8_t trap_put_msg = 72;
const uint8_t trap_get_msgq_length = 73;
const uint8_t trap_get_msg_size = 74;
const uint8_t trap_get_msg = 75;
const uint8_t trap_bind_terminal = 76;
const uint8_t trap_unbind_terminal = 77;
const uint8_t trap_timer_msg = 78;
const uint8_t trap_connect = 79;
const uint8_t trap_get_debug_mode = 80;
const uint8_t trap_set_debug_mode = 81;
const uint8_t trap_write_debug_message = 82;
const uint8_t trap_kill = 83;
const uint8_t trap_waitpid = 84;
const uint8_t trap_start = 85;
const uint8_t trap_stop = 86;
const uint8_t trap_get_port = 87;
const uint8_t trap_lock_io = 88;
const uint8_t trap_unlock_io = 89;

class TrapHandler
{
public:
    virtual ~TrapHandler();
    virtual uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) = 0;
    virtual std::string TrapName() const = 0;
};

void SetTrapHandler(uint8_t trap, TrapHandler* handler);
void AddTrapsToSymbolTable(cmajor::systemx::object::SymbolTable& symbolTable);
void InitTrap();
void DoneTrap();

} // namespace cmajor::systemx::kernel
