// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.text.file.editor;

import cmajor.view.editor;
import wing;
import std.core;

export namespace cmajor::view {

struct TextFileEditorCreateParams
{
    TextFileEditorCreateParams(const std::string& filePath_);
    TextFileEditorCreateParams& Defaults();
    wing::ControlCreateParams editorCreateParams;
    wing::TextViewCreateParams textViewCreateParams;
    std::string filePath;
};

class TextFileEditor : public Editor
{
public:
    TextFileEditor(TextFileEditorCreateParams& createParams);
    const std::string& FilePath() const override;
    bool IsDirty() const override;
    void ResetDirty() override;
    void Save() override;
    void Select() override;
    void Undo() override;
    void Redo() override;
    wing::TextView* GetTextView() const override;
private:
    wing::TextView* textView;
};

} // namespace cmajor::view
