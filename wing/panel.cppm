// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.panel;

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
