// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.reg.view;

import sxgmdb.debug.view;
import cmajor.systemx.kernel;
import cmajor.systemx.machine;
import wing;
import std.core;

export namespace sxgmdb {

std::string DefaultRegisterViewFontFamilyName();
float DefaultRegisterViewFontSize();
wing::Color DefaultRegNameColor();
wing::Color DefaultRegValueColor();

struct RegisterViewCreateParams
{
    RegisterViewCreateParams();
    RegisterViewCreateParams& Defaults();
    wing::ControlCreateParams controlCreateParams;
    std::string fontFamilyName;
    float fontSize;
    wing::Color regNameColor;
    wing::Color regValueColor;
};

class RegisterView : public wing::Control, public DebugView
{
public:
    RegisterView(RegisterViewCreateParams& createParams);
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override;
    void SetProcess(cmajor::systemx::kernel::Process* process_) override;
    void UpdateView(bool updateCurrentAddress) override;
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void Measure(wing::Graphics& graphics);
    void Paint(wing::Graphics& graphics);
    void PrintReg(const wing::PointF& origin, wing::Graphics& graphics, const std::string& regName, const std::string& regValue);
    //cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::kernel::Process* process;
    wing::StringFormat stringFormat;
    float lineHeight;
    float charWidth;
    wing::SolidBrush regNameBrush;
    wing::SolidBrush regValueBrush;
};

} // namespace sxgmdb
