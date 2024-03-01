// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.manual.reset.event;

export namespace wing {

class ManualResetEvent
{
public:
    ManualResetEvent();
    ~ManualResetEvent();
    void Reset();
    void Set();
    void WaitFor();
private:
    void* handle;
};

} // wing
