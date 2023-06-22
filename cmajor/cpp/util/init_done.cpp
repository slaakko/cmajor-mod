// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <util/init_done.hpp>
#include <util/unicode.hpp>
//#include <util/socket.hpp>

namespace util {

void Init()
{
    util::UnicodeInit();
    //util::SocketInit();
    //util::AnsiInit();
    //util::TimeInit();
}

void Done()
{
    //util::TimeDone();
    //util::AnsiDone();
    //util::SocketDone();
    util::UnicodeDone();
}

} // namespace util
