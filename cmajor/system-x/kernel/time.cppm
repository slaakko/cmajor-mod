// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.time;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

struct Date
{
    Date() : year(0), month(0), day() {}
    Date(int16_t year_, int8_t month_, int8_t day_) : year(year_), month(month_), day(day_) {}
    int16_t year;
    int8_t month;
    int8_t day;
};

struct DateTime
{
    DateTime() : date(), secs() {}
    DateTime(Date date_, int32_t secs_) : date(date_), secs(secs_) {}
    Date date;
    int32_t secs;
};

Date CurrentDate();
DateTime CurrentDateTime();
void WriteDate(const Date& date, int64_t yearAddr, int64_t monthAddr, int64_t dayAddr, uint64_t rv, cmajor::systemx::machine::Memory& mem);
void WriteDateTime(const DateTime& dateTime, int64_t yearAddr, int64_t monthAddr, int64_t dayAddr, int64_t secsAddr, uint64_t rv, cmajor::systemx::machine::Memory& mem);


} // namespace cmajor::systemx::kernel
