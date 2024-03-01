// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.panel;

namespace wing {

ControlCreateParams& MakePanelCreateParams(ControlCreateParams& createParams)
{
    if (createParams.windowClassName.empty())
    {
        createParams.WindowClassName("wing.Panel");
    }
    createParams.WindowStyle(DefaultChildWindowStyle());
    return createParams;
}

Panel::Panel(ControlCreateParams& createParams) : ContainerControl(MakePanelCreateParams(createParams))
{
}

void Panel::OnPaint(PaintEventArgs& args)
{
    args.graphics.Clear(BackgroundColor());
    ContainerControl::OnPaint(args);
}

} // wing
