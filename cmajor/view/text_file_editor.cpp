// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.text.file.editor;

import util;

namespace cmajor::view {

TextFileEditorCreateParams::TextFileEditorCreateParams(const std::string& filePath_) : editorCreateParams(), textViewCreateParams(), filePath(filePath_)
{
    editorCreateParams.WindowClassName("cmajor.view.TextFileEditor");
    editorCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    editorCreateParams.BackgroundColor(wing::Color::White);
    editorCreateParams.SetDock(wing::Dock::fill);
}

TextFileEditorCreateParams& TextFileEditorCreateParams::Defaults()
{
    return *this;
}

TextFileEditor::TextFileEditor(TextFileEditorCreateParams& createParams) : Editor(createParams.editorCreateParams), textView(nullptr)
{
    std::unique_ptr<wing::TextView> textViewPtr(new wing::TextView(createParams.textViewCreateParams.Defaults()));
    textView = textViewPtr.get();
    textView->SetFlag(wing::ControlFlags::scrollSubject);
    wing::ScrollableControl* scrollableTextView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(textViewPtr.release()).SetDock(wing::Dock::fill)));
    AddChild(scrollableTextView);
    textView->SetFilePath(createParams.filePath);
    std::u32string content = util::ToUtf32(util::ReadFile(createParams.filePath));
    textView->SetTextContent(content);
    textView->SetIndentSize(4);
    textView->SetDoubleBuffered();
}

const std::string& TextFileEditor::FilePath() const
{
    return textView->FilePath();
}

bool TextFileEditor::IsDirty() const
{
    return textView->IsDirty();
}

void TextFileEditor::ResetDirty()
{
    textView->ResetDirty();
}

void TextFileEditor::Save()
{
    textView->SaveText();
}

void TextFileEditor::Select()
{
    textView->Select();
}

void TextFileEditor::Undo()
{
    textView->Undo();
}

void TextFileEditor::Redo()
{
    textView->Redo();
}

wing::TextView* TextFileEditor::GetTextView() const
{
    return textView;
}

} // namespace cmajor::view
