// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.time;

int64_t RtmNow()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

void RtmSleep(int64_t nanoseconds)
{
    std::chrono::nanoseconds duration{ nanoseconds };
    std::this_thread::sleep_for(duration);
}

void RtmGetCurrentDate(int16_t* year, int8_t* month, int8_t* day)
{
    std::time_t currentTime;
    std::time(&currentTime);
    struct tm* localTime = nullptr;
    localTime = std::localtime(&currentTime);
    *year = 1900 + localTime->tm_year;
    *month = 1 + localTime->tm_mon;
    *day = localTime->tm_mday;
}

void RtmGetCurrentDateTime(int16_t* year, int8_t* month, int8_t* day, int32_t* secs)
{
    std::time_t currentTime;
    std::time(&currentTime);
    struct tm* localTime = nullptr;
    localTime = std::localtime(&currentTime);
    *year = 1900 + localTime->tm_year;
    *month = 1 + localTime->tm_mon;
    *day = localTime->tm_mday;
    *secs = localTime->tm_hour * 3600 + localTime->tm_min * 60 + localTime->tm_sec;
}
