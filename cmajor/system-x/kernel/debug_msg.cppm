// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.debug.msg;

import cmajor.systemx.kernel.msg.queue;
import std.core;

export namespace cmajor::systemx::kernel {

std::string GetMsgStr(const Msg& msg);

} // namespace cmajor::systemx::kernel
