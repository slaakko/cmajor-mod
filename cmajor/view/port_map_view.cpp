// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.port.map.view;

import cmajor.service;

namespace cmajor::view {

PortMapView::PortMapView() : wing::Control(wing::ControlCreateParams().WindowClassName("cmajor.view.PortMapView").WindowClassBackgroundColor(COLOR_WINDOW).
    BackgroundColor(wing::Color::White).SetDock(wing::Dock::fill)),
    container(this), treeView(nullptr), child(nullptr)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(wing::TreeViewCreateParams().AddToolTip(false)));
    treeView = treeViewPtr.get();
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scrollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeView.get();
    container.AddChild(scrollableTreeView.release());
    Clear();
}

void PortMapView::Clear()
{
    treeView->SetRoot(new wing::TreeViewNode("Port Map Entries"));
    child->Invalidate();
}

void PortMapView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Root()->ExpandAll();
    treeView->Invalidate();
    child->Invalidate();
    wing::Control::OnPaint(args);
}

void PortMapView::Update(const std::vector<bpm::PortLease>& portLeases)
{
    Clear();
    for (const bpm::PortLease& portLease : portLeases)
    {
        wing::TreeViewNode* node = new wing::TreeViewNode("Port " + std::to_string(portLease.portNumber));
        treeView->Root()->AddChild(node);
        wing::TreeViewNode* processNode = new wing::TreeViewNode("Process: " + portLease.processName);
        node->AddChild(processNode);
        wing::TreeViewNode* pidNode = new wing::TreeViewNode("PID: " + std::to_string(portLease.pid));
        node->AddChild(pidNode);
        wing::TreeViewNode* startTimeNode = new wing::TreeViewNode("Lease start time: " + portLease.leaseStartTime.ToString());
        node->AddChild(startTimeNode);
        if (portLease.leaseRenewalTime != util::DateTime())
        {
            wing::TreeViewNode* renewalTimeNode = new wing::TreeViewNode("Lease renewal time: " + portLease.leaseRenewalTime.ToString());
            node->AddChild(renewalTimeNode);
        }
        wing::TreeViewNode* stateNode = new wing::TreeViewNode("Lease state: " + portLease.leaseState);
        node->AddChild(stateNode);
    }
    Invalidate();
    cmajor::service::PutOutputServiceMessage(std::to_string(portLeases.size()) + " ports in the port map");
}

void PortMapView::OnLocationChanged()
{
    wing::Control::OnLocationChanged();
    SetChildPos();
}

void PortMapView::OnSizeChanged()
{
    wing::Control::OnSizeChanged();
    SetChildPos();
}

void PortMapView::SetChildPos()
{
    child->SetLocation(wing::Point());
    child->SetSize(GetSize());
}

} // namespace cmajor::view
