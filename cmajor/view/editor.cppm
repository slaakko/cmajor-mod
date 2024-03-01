// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.editor;

import wing;
import std.core;

export namespace cmajor::view {

class Editor : public wing::ContainerControl
{
public:
    Editor(wing::ControlCreateParams& createParams);
    virtual bool IsCmajorEditor() const { return false; }
    virtual const std::string& FilePath() const = 0;
    virtual bool IsDirty() const = 0;
    virtual void ResetDirty() = 0;
    virtual bool IsCCDirty() const;
    virtual void ResetCCDirty();
    virtual void Save() = 0;
    virtual void Select() = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
    virtual wing::TextView* GetTextView() const = 0;
};

} // namespace cmajor::view
