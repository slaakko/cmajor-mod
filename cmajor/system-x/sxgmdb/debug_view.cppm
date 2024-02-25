// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.debug.view;

import cmajor.systemx.kernel;
import cmajor.systemx.machine;
import std.core;

export namespace sxgmdb {

class Debugger;

class DebugView
{
public:
    DebugView();
    virtual ~DebugView();
    virtual void UpdateView(bool updateCurrentAddress);
    virtual void SetMachine(cmajor::systemx::machine::Machine* machine);
    virtual void SetProcess(cmajor::systemx::kernel::Process* process);
    virtual void SetDebugger(Debugger* debugger);
    virtual void WriteLine(const std::string& logMessage);
    virtual void Clear();
    virtual void ToStart();
    virtual void ToEnd();
    virtual void NextPage();
    virtual void PrevPage();
    virtual void NextLine();
    virtual void PrevLine();
    virtual void NextQuarter();
    virtual void PrevQuarter();
    void SetVisible(bool visible_) { visible = visible_; }
    bool IsVisible() const { return visible; }
private:
    bool visible;
};

} // namespace sxgmdb
