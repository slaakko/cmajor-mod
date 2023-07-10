// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.view.editor;

namespace cmajor::view {

Editor::Editor(wing::ControlCreateParams& createParams) : wing::ContainerControl(createParams)
{
}

bool Editor::IsCCDirty() const
{
    return false;
}

void Editor::ResetCCDirty()
{
}

} // namespace cmajor::view
