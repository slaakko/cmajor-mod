// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.resource.file.editor;

import cmajor.view.editor;
import wing;
import std.core;

export namespace cmajor::view{

struct ResourceFileEditorCreateParams
{
    ResourceFileEditorCreateParams(const std::string& filePath_);
    ResourceFileEditorCreateParams& Defaults();
    wing::ControlCreateParams editorCreateParams;
    wing::TextViewCreateParams textViewCreateParams;
    std::string filePath;
};

class ResourceFileEditor : public Editor
{
public:
    ResourceFileEditor(ResourceFileEditorCreateParams& createParams);
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
