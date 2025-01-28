// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.resource.file.editor;

import util;

namespace cmajor::view {

ResourceFileEditorCreateParams::ResourceFileEditorCreateParams(const std::string& filePath_) : editorCreateParams(), textViewCreateParams(), filePath(filePath_)
{
    editorCreateParams.WindowClassName("cmajor.view.ResourceFileEditor");
    editorCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    editorCreateParams.BackgroundColor(wing::Color::White);
    editorCreateParams.SetDock(wing::Dock::fill);
}

ResourceFileEditorCreateParams& ResourceFileEditorCreateParams::Defaults()
{
    return *this;
}

ResourceFileEditor::ResourceFileEditor(ResourceFileEditorCreateParams& createParams) : Editor(createParams.editorCreateParams), textView(nullptr)
{
    std::unique_ptr<wing::TextView> textViewPtr(new wing::TextView(createParams.textViewCreateParams.Defaults()));
    textView = textViewPtr.get();
    textView->SetFlag(wing::ControlFlags::scrollSubject);
    wing::ScrollableControl* scrollableTextView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(textViewPtr.release()).SetDock(wing::Dock::fill)));
    AddChild(scrollableTextView);
    textView->SetFilePath(createParams.filePath);
    std::u32string content;
    try
    {
        content = util::ToUtf32(util::ReadFile(createParams.filePath));
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + createParams.filePath);
    }
    textView->SetTextContent(content);
    textView->SetIndentSize(4);
    textView->SetDoubleBuffered();
}

const std::string& ResourceFileEditor::FilePath() const
{
    return textView->FilePath();
}

bool ResourceFileEditor::IsDirty() const
{
    return textView->IsDirty();
}

void ResourceFileEditor::ResetDirty()
{
    textView->ResetDirty();
}

void ResourceFileEditor::Save()
{
    textView->SaveText();
}

void ResourceFileEditor::Select()
{
    textView->Select();
}

void ResourceFileEditor::Undo()
{
    textView->Undo();
}

void ResourceFileEditor::Redo()
{
    textView->Redo();
}

wing::TextView* ResourceFileEditor::GetTextView() const
{
    return textView;
}

} // namespace cmajor::view
