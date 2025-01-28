// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.code.view;

import sxgmdb.debug.view;
import cmajor.systemx.kernel;
import cmajor.systemx.machine;
import wing;
import std.core;

export namespace sxgmdb {

enum class CodeViewItemKind
{
    none, selection_background, pc_background, space, currentAddress, breakpoint, assembledByte, label, opCode, number, reg, punctuation, undefined, address
};

struct CodeViewItem
{
    CodeViewItem(CodeViewItemKind kind_, const std::u32string& text_);
    CodeViewItemKind kind;
    std::u32string text;
};

class CodeViewLine
{
public:
    CodeViewLine();
    void AddItem(CodeViewItem&& item);
    const std::vector<CodeViewItem>& Items() const { return items; }
private:
    std::vector<CodeViewItem> items;
};

std::string DefaultCodeViewFontFamilyName();
float DefaultCodeViewFontSize();
wing::Color DefaultSelectLineBackgroundColor();
wing::Color DefaultCurrentAddressColor();
wing::Color DefaultBreakpointColor();
wing::Color DefaultPCColor();
wing::Color DefaultAssembledByteColor();
wing::Color DefaultLabelColor();
wing::Color DefaultOpCodeColor();
wing::Color DefaultNumberColor();
wing::Color DefaultRegisterColor();
wing::Color DefaultPunctuationColor();
wing::Color DefaultUndefinedColor();
wing::Color DefaultAddressColor();

struct CodeViewCreateParams
{
    CodeViewCreateParams();
    CodeViewCreateParams& Defaults();
    wing::ControlCreateParams controlCreateParams;
    std::string fontFamilyName;
    float fontSize;
    wing::Color selectLineBackgroundColor;
};

class CodeView : public wing::Control, public DebugView
{
public:
    CodeView(CodeViewCreateParams& createParams);
    void Reset();
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override;
    void SetProcess(cmajor::systemx::kernel::Process* process_) override;
    void SetDebugger(Debugger* debugger_) override;
    void UpdateView(bool updateCurrentAddress) override;
    void NextLine() override;
    void PrevLine() override;
    void NextQuarter() override;
    void PrevQuarter() override;
    void NextPage() override;
    void PrevPage() override;
    void ToStart() override;
    void ToEnd() override;
    int64_t CurrentAddress() const { return currentAddress; }
    void GotoPrevAddress();
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnMouseDown(wing::MouseEventArgs& args) override;
private:
    void Measure(wing::Graphics& graphics);
    void FetchAddressRange(int64_t startAddress, int64_t byteCount);
    void PaintLines(wing::Graphics& graphics, int64_t pc);
    void PaintLine(wing::Graphics& graphics, const wing::PointF& origin, const CodeViewLine& line, int64_t address, int64_t curAddr, int64_t pc);
    wing::Brush& GetItemBrush(CodeViewItemKind itemKind);
    int64_t ClickAddress(const wing::Point& loc) const;
    const CodeViewItem* ClickItem(const wing::Point& loc, int64_t clickAddress) const;
    int64_t currentAddress;
    std::stack<int64_t> currentAddressStack;
    wing::StringFormat stringFormat;
    float lineHeight;
    float charWidth;
    float viewHeight;
    int numLines;
    int currentLineNumber;
    std::map<int64_t, CodeViewLine> lineMap;
    std::map<int64_t, int64_t> setAddressMap;
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
    Debugger* debugger;
    std::map<CodeViewItemKind, wing::SolidBrush*> brushMap;
    std::map<CodeViewItemKind, wing::Color> colorMap;
    std::vector<std::unique_ptr<wing::SolidBrush>> brushes;
};

} // namespace sxgmdb
