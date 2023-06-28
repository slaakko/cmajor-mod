// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.panel;

//#include <wing/ContainerControl.hpp>
import wing.container.control;

export namespace wing {

class Panel : public ContainerControl
{
public:
    Panel(ControlCreateParams& createParams);
protected:
    void OnPaint(PaintEventArgs& args) override;
};
} // wing