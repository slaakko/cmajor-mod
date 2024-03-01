// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.port.map.view;

import cmajor.binary.portmap.message;
import wing;
import std.core;

export namespace cmajor::view {

class PortMapView : public wing::Control
{
public:
    PortMapView();
    void Clear();
    void Update(const std::vector<bpm::PortLease>& portLeases);
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnLocationChanged() override;
    void OnSizeChanged() override;
private:
    void SetChildPos();
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
};

} // namespace cmajor::view
