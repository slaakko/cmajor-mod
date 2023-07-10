// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module sxgmdb.debug.view;

namespace sxgmdb {

DebugView::DebugView() : visible(false)
{
}

DebugView::~DebugView()
{
}

void DebugView::UpdateView(bool updateCurrentAddress)
{
}

void DebugView::SetMachine(cmajor::systemx::machine::Machine* machine)
{
}

void DebugView::SetProcess(cmajor::systemx::kernel::Process* process)
{
}

void DebugView::SetDebugger(Debugger* debugger)
{
}

void DebugView::WriteLine(const std::string& logMessage)
{
}

void DebugView::Clear()
{
}

void DebugView::ToStart()
{
}

void DebugView::ToEnd()
{
}

void DebugView::NextPage()
{
}

void DebugView::PrevPage()
{
}

void DebugView::NextLine()
{
}

void DebugView::PrevLine()
{
}

void DebugView::NextQuarter()
{
}

void DebugView::PrevQuarter()
{
}

} // namespace sxgmdb

