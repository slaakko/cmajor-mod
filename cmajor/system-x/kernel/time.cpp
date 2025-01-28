// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.time;

namespace cmajor::systemx::kernel {

Date CurrentDate()
{
    time_t t = std::time(nullptr);
    struct tm localTime;
    errno_t result = localtime_s(&localTime, &t);
    if (result == 0)
    {
        int16_t year = 1900 + localTime.tm_year;
        int8_t month = 1 + localTime.tm_mon;
        int8_t day = localTime.tm_mday;
        return Date(year, month, day);
    }
    else
    {
        return Date();
    }
}

DateTime CurrentDateTime()
{
    time_t t = std::time(nullptr);
    struct tm localTime;
    errno_t result = localtime_s(&localTime, &t);
    if (result == 0)
    {
        int32_t secs = 3600 * localTime.tm_hour + 60 * localTime.tm_min + localTime.tm_sec;
        DateTime dt(CurrentDate(), secs);
        return dt;
    }
    else
    {
        return DateTime();
    }
}

void WriteDate(const Date& date, int64_t yearAddr, int64_t monthAddr, int64_t dayAddr, uint64_t rv, cmajor::systemx::machine::Memory& mem)
{
    mem.WriteWyde(rv, yearAddr, date.year, cmajor::systemx::machine::Protection::write);
    mem.WriteByte(rv, monthAddr, date.month, cmajor::systemx::machine::Protection::write);
    mem.WriteByte(rv, dayAddr, date.day, cmajor::systemx::machine::Protection::write);
}

void WriteDateTime(const DateTime& dateTime, int64_t yearAddr, int64_t monthAddr, int64_t dayAddr, int64_t secsAddr, uint64_t rv, cmajor::systemx::machine::Memory& mem)
{
    WriteDate(dateTime.date, yearAddr, monthAddr, dayAddr, rv, mem);
    mem.WriteTetra(rv, secsAddr, dateTime.secs, cmajor::systemx::machine::Protection::write);
}

} // namespace cmajor::systemx::kernel
