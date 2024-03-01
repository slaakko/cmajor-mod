// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.init.done;

import util.unicode;
import util.ansi;
import util.time;
import util.socket;

namespace util {

void Init()
{
    util::UnicodeInit();
    util::SocketInit();
    util::AnsiInit();
    util::TimeInit();
}

void Done()
{
    util::TimeDone();
    util::AnsiDone();
    util::SocketDone();
    util::UnicodeDone();
}

} // namespace util
