// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.cmajor.editor;

import cmajor.view.editor;
import cmajor.view.source.code.view;
import cmajor.view.debug.strip;
import std.core;

export namespace cmajor::view {

struct CmajorEditorCreateParams
{
    CmajorEditorCreateParams(const std::string& filePath_, CmajorSourceCodeViewCreateParams& sourceCodeViewCreateParams_, DebugStripCreateParams& debugStripCreateParams_);
    CmajorEditorCreateParams& Defaults();
    wing::ControlCreateParams editorCreateParams;
    CmajorSourceCodeViewCreateParams sourceCodeViewCreateParams;
    DebugStripCreateParams debugStripCreateParams;
    std::string filePath;
};

class CmajorEditor : public Editor
{
public:
    CmajorEditor(CmajorEditorCreateParams& createParams);
    bool IsCmajorEditor() const override { return true; }
    const std::string& FilePath() const override;
    bool IsDirty() const override;
    void ResetDirty() override;
    bool IsCCDirty() const override;
    void ResetCCDirty() override;
    void Save() override;
    void Select() override;
    void Undo() override;
    void Redo() override;
    wing::TextView* GetTextView() const override;
    CmajorSourceCodeView* SourceCodeView() { return sourceCodeView; }
    DebugStrip* GetDebugStrip() const { return debugStrip; }
private:
    CmajorSourceCodeView* sourceCodeView;
    DebugStrip* debugStrip;
};

} // namespace cmajor::view
