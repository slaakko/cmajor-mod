// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module sxgmdb.reg.view;

import util;

namespace sxgmdb {

std::string DefaultRegisterViewFontFamilyName()
{
    return "Cascadia Mono";
}

float DefaultRegisterViewFontSize()
{
    return 10.0f;
}

wing::Color DefaultRegNameColor()
{
    return wing::Color(78, 201, 176);
}

wing::Color DefaultRegValueColor()
{
    return wing::Color(155, 155, 155);
}

RegisterViewCreateParams::RegisterViewCreateParams()
{
    controlCreateParams.WindowClassName("system.x.db.RegisterView");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(wing::Color::White);
    controlCreateParams.SetDock(wing::Dock::fill);
    fontFamilyName = DefaultRegisterViewFontFamilyName();
    fontSize = DefaultRegisterViewFontSize();
    regNameColor = DefaultRegNameColor();
    regValueColor = DefaultRegValueColor();
}

RegisterViewCreateParams& RegisterViewCreateParams::Defaults()
{
    return *this;
}

RegisterView::RegisterView(RegisterViewCreateParams& createParams) :
    wing::Control(createParams.controlCreateParams), process(nullptr), lineHeight(0), charWidth(0), regNameBrush(createParams.regNameColor), regValueBrush(createParams.regValueColor)
{
    std::u16string fontFamilyName = util::ToUtf16(createParams.fontFamilyName);
    SetFont(wing::Font(wing::FontFamily((const WCHAR*)fontFamilyName.c_str()), createParams.fontSize, wing::FontStyle::FontStyleRegular, wing::Unit::UnitPoint));
    SetDoubleBuffered();
    stringFormat.SetLineAlignment(wing::StringAlignment::StringAlignmentNear);
    stringFormat.SetAlignment(wing::StringAlignment::StringAlignmentNear);
}

void RegisterView::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    //machine = machine_;
}

void RegisterView::SetProcess(cmajor::systemx::kernel::Process* process_)
{
    process = process_;
}

void RegisterView::UpdateView(bool updateCurrentAddress)
{
    Invalidate();
}

void RegisterView::OnPaint(wing::PaintEventArgs& args)
{
    try
    {
        args.graphics.Clear(BackgroundColor());
        if (!process) return;
        Measure(args.graphics);
        Paint(args.graphics);
        wing::Control::OnPaint(args);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(nullptr, ex.what());
    }
}

void RegisterView::Measure(wing::Graphics& graphics)
{
    wing::PointF origin;
    std::string testString = "This is a test line";
    wing::RectF r = MeasureString(graphics, testString, GetFont(), origin, stringFormat);
    lineHeight = r.Height;
    charWidth = r.Width / testString.length();
}

void RegisterView::Paint(wing::Graphics& graphics)
{
    wing::PointF origin;
    float col1x = 21 * charWidth;
    float col2x = 2 * 21 * charWidth;
    float col3x = 3 * 21 * charWidth;

    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    PrintReg(origin, graphics, "ax", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regAX)));
    PrintReg(wing::PointF(origin.X + col1x, origin.Y), graphics, "ex", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regEX)));
    PrintReg(wing::PointF(origin.X + col2x, origin.Y), graphics, "$0", "#" + util::ToHexString(processor->Regs().Get(0)));
    PrintReg(wing::PointF(origin.X + col3x, origin.Y), graphics, "$4", "#" + util::ToHexString(processor->Regs().Get(4)));
    origin.Y = origin.Y + lineHeight;
    PrintReg(origin, graphics, "bx", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regBX)));
    PrintReg(wing::PointF(origin.X + col1x, origin.Y), graphics, "ix", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regIX)));
    PrintReg(wing::PointF(origin.X + col2x, origin.Y), graphics, "$1", "#" + util::ToHexString(processor->Regs().Get(1)));
    PrintReg(wing::PointF(origin.X + col3x, origin.Y), graphics, "$5", "#" + util::ToHexString(processor->Regs().Get(5)));
    origin.Y = origin.Y + lineHeight;
    PrintReg(origin, graphics, "cx", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regCX)));
    PrintReg(wing::PointF(origin.X + col1x, origin.Y), graphics, "fp", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regFP)));
    PrintReg(wing::PointF(origin.X + col2x, origin.Y), graphics, "$2", "#" + util::ToHexString(processor->Regs().Get(2)));
    PrintReg(wing::PointF(origin.X + col3x, origin.Y), graphics, "$6", "#" + util::ToHexString(processor->Regs().Get(6)));
    origin.Y = origin.Y + lineHeight;
    PrintReg(origin, graphics, "dx", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regDX)));
    PrintReg(wing::PointF(origin.X + col1x, origin.Y), graphics, "sp", "#" + util::ToHexString(processor->Regs().Get(cmajor::systemx::machine::regSP)));
    PrintReg(wing::PointF(origin.X + col2x, origin.Y), graphics, "$3", "#" + util::ToHexString(processor->Regs().Get(3)));
    PrintReg(wing::PointF(origin.X + col3x, origin.Y), graphics, "$7", "#" + util::ToHexString(processor->Regs().Get(7)));
}

void RegisterView::PrintReg(const wing::PointF& origin, wing::Graphics& graphics, const std::string& regName, const std::string& regValue)
{
    wing::DrawString(graphics, regName, GetFont(), origin, regNameBrush);
    wing::DrawString(graphics, regValue, GetFont(), wing::PointF(origin.X + charWidth * 3, origin.Y), regValueBrush);
}

} // namespace sxgmdb

