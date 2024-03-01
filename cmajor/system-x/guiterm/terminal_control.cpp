// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef min
#undef max

module cmajor.systemx.guiterm.terminal.control;

import cmajor.systemx.kernel;
import wing;
import util;

namespace cmajor::systemx::guiterm {

const int caretTimerId = 1;
const int caretTimerPeriod = 5000; // 5 seconds

bool IsPrintChar(char32_t ch)
{
    return ch >= 32 && ch < static_cast<char32_t>(cmajor::systemx::kernel::specialKeyStart);
}

TerminalControlCreateParams::TerminalControlCreateParams() :
    controlCreateParams()
{
    controlCreateParams.WindowClassName("guiterm.terminal");
    controlCreateParams.BackgroundColor(wing::Color(12, 12, 12));
    controlCreateParams.WindowClassBackgroundColor(COLOR_BACKGROUND);
    textColor = wing::Color(204, 204, 204);
    fontFamilyName = "Cascadia Mono";
    fontSize = 10.0f;
}

TerminalControlCreateParams& TerminalControlCreateParams::Defaults()
{
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::Location(const wing::Point& location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::SetSize(const wing::Size& size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::TextColor(const wing::Color& textColor_)
{
    textColor = textColor_;
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::FontFamilyName(const std::string& fontFamilyName_)
{
    fontFamilyName = fontFamilyName_;
    return *this;
}

TerminalControlCreateParams& TerminalControlCreateParams::FontSize(float fontSize_)
{
    fontSize = fontSize_;
    return *this;
}

TerminalControl::TerminalControl(TerminalControlCreateParams& createParams) :
    wing::Control(createParams.controlCreateParams), foreBrush(nullptr), backBrush(nullptr), changed(false), active(false), stringFormat(), charWidth(0), charHeight(0),
    rows(0), cols(0), cursorX(0), cursorY(0), outputEndCursorPosX(0), outputEndCursorPosY(0), inputMode(InputMode::cooked), echo(true), eof(false), pos(0), lineIndex(0)
{
    std::u16string fontFamilyName = util::ToUtf16(createParams.fontFamilyName);
    SetFont(
        wing::Font(wing::FontFamily((WCHAR*)fontFamilyName.c_str()),
            createParams.fontSize,
            wing::FontStyle::FontStyleRegular,
            wing::Unit::UnitPoint));
    defaultForeColor = wing::ConsoleColor::gray;
    defaultBackColor = wing::ConsoleColor::black;
    SetColors(defaultForeColor, defaultBackColor);
    SetSize(40, 120);
    changed = true;
    stringFormat.SetLineAlignment(wing::StringAlignment::StringAlignmentNear);
    stringFormat.SetAlignment(wing::StringAlignment::StringAlignmentNear);
    Invalidate();
}

void TerminalControl::SetColors(wing::ConsoleColor foreColor_, wing::ConsoleColor backColor_)
{
    foreColor = foreColor_;
    backColor = backColor_;
    foreBrush = GetOrInsertSolidBrush(foreColor);
    backBrush = GetOrInsertSolidBrush(backColor);
}

void TerminalControl::SetSize(int32_t rows_, int32_t cols_)
{
    rows = rows_;
    cols = cols_;
    screen.resize(rows);
    for (int i = 0; i < rows; ++i)
    {
        screen[i].resize(cols);
    }
    ClearScreen();
}

void TerminalControl::SetCursorPos(int32_t cursorX_, int32_t cursorY_)
{
    cursorX = std::min(cols - 1, cursorX_);
    cursorY = std::min(rows - 1, cursorY_);
    if (charWidth > 0 && charHeight > 0)
    {
        wing::Point caretPos(charWidth * cursorX, charHeight * cursorY);
        SetCaretPos(caretPos);
    }
}

void TerminalControl::SetOutputEndCursorPos(int32_t x, int32_t y)
{
    outputEndCursorPosX = x;
    outputEndCursorPosY = y;
}

wing::SolidBrush* TerminalControl::GetOrInsertSolidBrush(wing::ConsoleColor color)
{
    auto it = brushMap.find(color);
    if (it != brushMap.cend())
    {
        return it->second;
    }
    wing::SolidBrush* solidBrush = new wing::SolidBrush(GetColor(color));
    brushes.push_back(std::unique_ptr<wing::SolidBrush>(solidBrush));
    brushMap[color] = solidBrush;
    return solidBrush;
}

void TerminalControl::Write(const std::u32string& text)
{
    for (char32_t c : text)
    {
        if (c == '\n')
        {
            ++cursorY;
            cursorX = 0;
            while (cursorY >= rows)
            {
                Scroll();
                --cursorY;
            }
        }
        else
        {
            screen[cursorY][cursorX] = ColorChar(c, foreColor, backColor);
            ++cursorX;
        }
    }
    Invalidate();
}

void TerminalControl::Activate()
{
    active = true;
    Component* component = GetContainer()->Parent();
    while (!component->IsTabPage())
    {
        component = component->GetContainer()->Parent();
    }
    wing::TabPage* tabPage = static_cast<wing::TabPage*>(component);
    tabPage->Select();
    SetFocus();
}

void TerminalControl::HandleKeyDown(wing::KeyEventArgs& args)
{
    OnKeyDown(args);
}

void TerminalControl::HandleKeyPress(wing::KeyPressEventArgs& args)
{
    OnKeyPress(args);
}

void TerminalControl::SetCooked()
{
    inputMode = InputMode::cooked;
}

void TerminalControl::SetRaw()
{
    inputMode = InputMode::raw;
}

void TerminalControl::SetEcho(bool echo_)
{
    echo = echo_;
}

std::vector<uint8_t> TerminalControl::ReadInput(int64_t count)
{
    Activate();
    while (!eof && terminalInputBuffer.empty())
    {
        wing::Application::ProcessMessages();
    }
    std::vector<uint8_t> bytes;
    if (terminalInputBuffer.empty())
    {
        if (eof)
        {
            eof = false;
            return std::vector<uint8_t>();
        }
    }
    else
    {
        int64_t n = std::min(int64_t(terminalInputBuffer.size()), count);
        for (int64_t i = 0; i < n; ++i)
        {
            uint8_t x = terminalInputBuffer.front();
            terminalInputBuffer.pop_front();
            bytes.push_back(x);
        }
        return bytes;
    }
    return std::vector<uint8_t>();
}

void TerminalControl::ScrollToCursor()
{
    wing::Size sz = GetSize();
    int windowLines = static_cast<int>(sz.Height / charHeight);
    wing::Point cursorLocation(0, std::max(static_cast<int>(0), static_cast<int>((cursorY - windowLines) * charHeight)));
    SetContentLocation(cursorLocation);
}

void TerminalControl::ScrollToLine(int lineIndex)
{
    wing::Point loc(0, lineIndex * charHeight);
    SetContentLocation(loc);
}

void TerminalControl::PushLines()
{
    linesStack.push(std::move(lines));
}

void TerminalControl::PopLines()
{
    if (!linesStack.empty())
    {
        lines = std::move(linesStack.top());
        linesStack.pop();
    }
}

void TerminalControl::ClearScreen()
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            screen[i][j] = ColorChar(' ', foreColor, backColor);
        }
    }
}

void TerminalControl::Scroll()
{
    for (int i = 0; i < rows - 1; ++i)
    {
        screen[i] = screen[i + 1];
    }
    screen[rows - 1].resize(cols);
    std::vector<ColorChar>& bottomLine = screen[rows - 1];
    for (int i = 0; i < cols; ++i)
    {
        bottomLine[i] = ColorChar(' ', foreColor, backColor);
    }
    Invalidate();
}

void TerminalControl::Measure(wing::Graphics& graphics)
{
    wing::PointF origin;
    std::string measureString = "This is a XXX test string";
    wing::RectF rect = MeasureString(graphics, measureString, GetFont(), origin, stringFormat);
    charWidth = rect.Width / measureString.length();
    charHeight = rect.Height;
    wing::Size contentSize(wing::Size(static_cast<int>((rect.Width / measureString.length()) * cols + 0.5), static_cast<int>(rect.Height * rows + 0.5)));
    SetContentSize(contentSize);
}

void TerminalControl::OutputLine(wing::Graphics& graphics, wing::PointF& origin, std::u32string& line)
{
    wing::RectF rect(origin, wing::SizeF(line.length(), charHeight));
    graphics.FillRectangle(backBrush, rect);
    std::string ln = util::ToUtf8(line);
    DrawString(graphics, ln, GetFont(), origin, *foreBrush);
    origin.X += charWidth * line.length();
    line.clear();
}

void TerminalControl::HandleInputChar(char32_t ch)
{
    if (inputMode == InputMode::raw)
    {
        if (echo)
        {
            if (IsPrintChar(ch))
            {
                PrintChar(ch);
            }
            else if (ch == '\n')
            {
                PrintNewLine();
            }
        }
        if (ch == cmajor::systemx::kernel::keyControlD)
        {
            HandleEof();
        }
        else
        {
            std::string chars = util::ToUtf8(std::u32string(1, ch));
            for (char c : chars)
            {
                terminalInputBuffer.push_back(static_cast<uint8_t>(c));
            }
        }
    }
    else if (inputMode == InputMode::cooked)
    {
        if (ch < 32)
        {
            switch (ch)
            {
            case '\n':
            {
                HandleNewLine();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyBackspace):
            {
                HandleBackspace();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyTab):
            {
                HandleTab();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyControlD):
            {
                HandleEof();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyEscape):
            {
                HandleEscape();
                break;
            }
            }
        }
        else if (ch >= cmajor::systemx::kernel::specialKeyStart && ch <= cmajor::systemx::kernel::specialKeyEnd)
        {
            switch (ch)
            {
            case static_cast<char32_t>(cmajor::systemx::kernel::keyHome):
            {
                HandleHome();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyEnd):
            {
                HandleEnd();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyLeft):
            {
                HandleLeft();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyRight):
            {
                HandleRight();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyUp):
            {
                HandleUp();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyDown):
            {
                HandleDown();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyControlLeft):
            {
                HandleControlLeft();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyControlRight):
            {
                HandleControlRight();
                break;
            }
            case static_cast<char32_t>(cmajor::systemx::kernel::keyDel):
            {
                HandleDel();
                break;
            }
            }
        }
        else
        {
            HandleChar(ch);
        }
    }
}

void TerminalControl::HandleEof()
{
    eof = true;
}

void TerminalControl::PrintChar(char32_t ch)
{
    std::u32string chars(1, ch);
    Write(chars);
}

void TerminalControl::PrintNewLine()
{
    std::u32string chars(1, '\n');
    Write(chars);
}

void TerminalControl::HandleChar(char32_t ch)
{
    if (pos < line.length())
    {
        line = line.substr(0, pos) + std::u32string(1, ch) + line.substr(pos);
    }
    else
    {
        line.append(1, ch);
    }
    ++pos;
    UpdateLine();
    UpdateCursorPos();
}

void TerminalControl::HandleNewLine()
{
    if (!line.empty())
    {
        bool found = false;
        for (const std::u32string& prevLine : lines)
        {
            if (line == prevLine)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            lines.push_back(line);
            lineIndex = lines.size();
        }
    }
    line.append(1, '\n');
    std::string inputLine = util::ToUtf8(line);
    line.clear();
    pos = 0;
    if (cursorY >= rows - 1)
    {
        Write(U"\n");
    }
    else
    {
        ++cursorY;
    }
    SetCursorPos(0, cursorY);
    outputEndCursorPosX = 0;
    outputEndCursorPosY = cursorY;
    for (char c : inputLine)
    {
        terminalInputBuffer.push_back(static_cast<uint8_t>(c));
    }
}

void TerminalControl::HandleBackspace()
{
    if (pos > 0)
    {
        line = line.substr(0, pos - 1) + line.substr(pos);
        --pos;
        UpdateLine();
        UpdateCursorPos();
    }
}

void TerminalControl::HandleTab()
{
    // todo
}

void TerminalControl::HandleEscape()
{
    ClearUpdateLine();
    line.clear();
    pos = 0;
    UpdateLine();
    UpdateCursorPos();
}

void TerminalControl::HandleHome()
{
    pos = 0;
    UpdateCursorPos();
}

void TerminalControl::HandleEnd()
{
    pos = line.length();
    UpdateCursorPos();
}

void TerminalControl::HandleLeft()
{
    if (pos > 0)
    {
        --pos;
        UpdateCursorPos();
    }
}

void TerminalControl::HandleRight()
{
    if (pos < line.length())
    {
        ++pos;
        UpdateCursorPos();
    }
}

void TerminalControl::HandleUp()
{
    if (lineIndex > 0)
    {
        ClearUpdateLine();
        --lineIndex;
        if (lineIndex < lines.size())
        {
            line = lines[lineIndex];
        }
        UpdateLine();
        HandleEnd();
    }
}

void TerminalControl::HandleDown()
{
    if (lineIndex < int32_t(lines.size()) - 1)
    {
        ClearUpdateLine();
        ++lineIndex;
        line = lines[lineIndex];
        UpdateLine();
        HandleEnd();
    }
}

void TerminalControl::HandleControlLeft()
{
    if (pos > 0)
    {
        --pos;
        if (pos > 0)
        {
            char c = line[pos];
            while (c == ' ')
            {
                if (pos > 0)
                {
                    --pos;
                }
                if (pos > 0)
                {
                    c = line[pos];
                }
                else
                {
                    UpdateCursorPos();
                    return;
                }
            }
            while (c != ' ')
            {
                if (pos > 0)
                {
                    --pos;
                }
                if (pos > 0)
                {
                    c = line[pos];
                }
                else
                {
                    UpdateCursorPos();
                    return;
                }
            }
            if (c == ' ')
            {
                ++pos;
            }
        }
    }
    UpdateCursorPos();
}

void TerminalControl::HandleControlRight()
{
    if (pos < line.length())
    {
        ++pos;
        if (pos < line.length())
        {
            char c = line[pos];
            while (c != ' ')
            {
                if (pos < line.length())
                {
                    ++pos;
                }
                if (pos < line.length())
                {
                    c = line[pos];
                }
                else
                {
                    UpdateCursorPos();
                    return;
                }
            }
            while (c == ' ')
            {
                if (pos < line.length())
                {
                    ++pos;
                }
                if (pos < line.length())
                {
                    c = line[pos];
                }
                else
                {
                    UpdateCursorPos();
                    return;
                }
            }
        }
    }
    UpdateCursorPos();
}

void TerminalControl::HandleDel()
{
    if (pos < line.length())
    {
        ++pos;
        HandleBackspace();
    }
}

void TerminalControl::ClearUpdateLine()
{
    if (!echo) return;
    int32_t prevCursorPosX = cursorX;
    int32_t prevCursorPosY = cursorY;
    SetCursorPos(outputEndCursorPosX, outputEndCursorPosY);
    std::u32string updateLine;
    int32_t x = outputEndCursorPosX;
    int32_t y = outputEndCursorPosY;
    for (int i = 0; i < line.length(); ++i)
    {
        if (x + i < cols)
        {
            updateLine.append(1, ' ');
        }
        else
        {
            Write(updateLine);
            updateLine.clear();
            x = 0;
            ++y;
            updateLine.append(1, ' ');
        }
    }
    if (!updateLine.empty())
    {
        Write(updateLine);
        x += updateLine.length();
        updateLine.clear();
    }
    for (int i = x; i < cols; ++i)
    {
        updateLine.append(1, ' ');
    }
    if (!updateLine.empty())
    {
        Write(updateLine);
    }
    SetCursorPos(prevCursorPosX, prevCursorPosY);
}

void TerminalControl::UpdateLine()
{
    if (!echo) return;
    int32_t prevCursorPosX = cursorX;
    int32_t prevCursorPosY = cursorY;
    SetCursorPos(outputEndCursorPosX, outputEndCursorPosY);
    int32_t x = outputEndCursorPosX;
    int32_t y = outputEndCursorPosY;
    std::u32string updateLine;
    for (int i = 0; i < line.length(); ++i)
    {
        if (x < cols)
        {
            updateLine.append(1, line[i]);
            ++x;
        }
        else
        {
            Write(updateLine);
            updateLine.clear();
            x = 0;
            ++y;
            updateLine.append(1, line[i]);
        }
    }
    if (!updateLine.empty())
    {
        Write(updateLine);
        updateLine.clear();
    }
    for (int i = x; i < cols; ++i)
    {
        updateLine.append(1, ' ');
    }
    if (!updateLine.empty())
    {
        Write(updateLine);
    }
    SetCursorPos(prevCursorPosX, prevCursorPosY);
}

void TerminalControl::UpdateCursorPos()
{
    if (!echo) return;
    int32_t x = (outputEndCursorPosX + pos) % cols;
    int32_t y = outputEndCursorPosY + (outputEndCursorPosX + pos) / cols;
    if (y >= rows)
    {
        y = rows - 1;
    }
    SetCursorPos(x, y);
}

void TerminalControl::OnPaint(wing::PaintEventArgs& args)
{
    if (changed)
    {
        changed = false;
        Measure(args.graphics);
    }
    foreBrush = GetOrInsertSolidBrush(foreColor);
    backBrush = GetOrInsertSolidBrush(backColor);
    for (int i = 0; i < rows; ++i)
    {
        wing::PointF origin(0, i * charHeight);
        std::u32string line;
        for (int j = 0; j < cols; ++j)
        {
            const ColorChar& colorChar = screen[i][j];
            if (colorChar.foreColor != foreColor || colorChar.backColor != backColor)
            {
                if (!line.empty())
                {
                    OutputLine(args.graphics, origin, line);
                }
                if (colorChar.foreColor != foreColor)
                {
                    foreColor = colorChar.foreColor;
                    foreBrush = GetOrInsertSolidBrush(foreColor);
                }
                if (colorChar.backColor != backColor)
                {
                    backColor = colorChar.backColor;
                    backBrush = GetOrInsertSolidBrush(backColor);
                }
            }
            line.append(1, colorChar.ch);
        }
        if (!line.empty())
        {
            OutputLine(args.graphics, origin, line);
        }
    }
}

void TerminalControl::OnKeyDown(wing::KeyEventArgs& args)
{
    switch (args.key)
    {
    case wing::Keys::back:
    {
        HandleInputChar(cmajor::systemx::kernel::keyBackspace);
        args.handled = true;
        break;
    }
    case wing::Keys::tab:
    {
        HandleInputChar(cmajor::systemx::kernel::keyTab);
        args.handled = true;
        break;
    }
    case wing::Keys::return_:
    {
        HandleInputChar(cmajor::systemx::kernel::keyNewLine);
        args.handled = true;
        break;
    }
    case wing::Keys::space:
    {
        HandleInputChar(' ');
        args.handled = true;
        break;
    }
    case wing::Keys::a:
    {
        HandleInputChar('a');
        args.handled = true;
        break;
    }
    case wing::Keys::b:
    {
        HandleInputChar('b');
        args.handled = true;
        break;
    }
    case wing::Keys::c:
    {
        HandleInputChar('c');
        args.handled = true;
        break;
    }
    case wing::Keys::d:
    {
        HandleInputChar('d');
        args.handled = true;
        break;
    }
    case wing::Keys::e:
    {
        HandleInputChar('e');
        args.handled = true;
        break;
    }
    case wing::Keys::f:
    {
        HandleInputChar('f');
        args.handled = true;
        break;
    }
    case wing::Keys::g:
    {
        HandleInputChar('g');
        args.handled = true;
        break;
    }
    case wing::Keys::h:
    {
        HandleInputChar('h');
        args.handled = true;
        break;
    }
    case wing::Keys::i:
    {
        HandleInputChar('i');
        args.handled = true;
        break;
    }
    case wing::Keys::j:
    {
        HandleInputChar('j');
        args.handled = true;
        break;
    }
    case wing::Keys::k:
    {
        HandleInputChar('k');
        args.handled = true;
        break;
    }
    case wing::Keys::l:
    {
        HandleInputChar('l');
        args.handled = true;
        break;
    }
    case wing::Keys::m:
    {
        HandleInputChar('m');
        args.handled = true;
        break;
    }
    case wing::Keys::n:
    {
        HandleInputChar('n');
        args.handled = true;
        break;
    }
    case wing::Keys::o:
    {
        HandleInputChar('o');
        args.handled = true;
        break;
    }
    case wing::Keys::p:
    {
        HandleInputChar('p');
        args.handled = true;
        break;
    }
    case wing::Keys::q:
    {
        HandleInputChar('q');
        args.handled = true;
        break;
    }
    case wing::Keys::r:
    {
        HandleInputChar('r');
        args.handled = true;
        break;
    }
    case wing::Keys::s:
    {
        HandleInputChar('s');
        args.handled = true;
        break;
    }
    case wing::Keys::t:
    {
        HandleInputChar('t');
        args.handled = true;
        break;
    }
    case wing::Keys::u:
    {
        HandleInputChar('u');
        args.handled = true;
        break;
    }
    case wing::Keys::v:
    {
        HandleInputChar('v');
        args.handled = true;
        break;
    }
    case wing::Keys::w:
    {
        HandleInputChar('w');
        args.handled = true;
        break;
    }
    case wing::Keys::x:
    {
        HandleInputChar('x');
        args.handled = true;
        break;
    }
    case wing::Keys::y:
    {
        HandleInputChar('y');
        args.handled = true;
        break;
    }
    case wing::Keys::z:
    {
        HandleInputChar('z');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::a:
    {
        HandleInputChar('A');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::b:
    {
        HandleInputChar('B');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::c:
    {
        HandleInputChar('C');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::d:
    {
        HandleInputChar('D');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::e:
    {
        HandleInputChar('E');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f:
    {
        HandleInputChar('F');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::g:
    {
        HandleInputChar('G');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::h:
    {
        HandleInputChar('H');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::i:
    {
        HandleInputChar('I');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::j:
    {
        HandleInputChar('J');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::k:
    {
        HandleInputChar('K');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::l:
    {
        HandleInputChar('L');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::m:
    {
        HandleInputChar('M');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::n:
    {
        HandleInputChar('N');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::o:
    {
        HandleInputChar('O');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::p:
    {
        HandleInputChar('P');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::q:
    {
        HandleInputChar('Q');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::r:
    {
        HandleInputChar('R');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::s:
    {
        HandleInputChar('S');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::t:
    {
        HandleInputChar('T');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::u:
    {
        HandleInputChar('U');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::v:
    {
        HandleInputChar('V');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::w:
    {
        HandleInputChar('W');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::x:
    {
        HandleInputChar('X');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::y:
    {
        HandleInputChar('Y');
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::z:
    {
        HandleInputChar('Z');
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::a:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlA);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::b:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlB);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::c:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlC);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::d:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlD);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::e:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlE);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::g:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlG);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::h:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlH);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::i:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlI);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::j:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlJ);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::k:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlK);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::m:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlM);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::n:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlN);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::o:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlO);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::p:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlP);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::q:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlQ);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::r:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlR);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::s:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlS);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::t:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlT);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::u:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlU);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::v:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlV);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::w:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlW);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::x:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlX);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::y:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlY);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::z:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlZ);
        args.handled = true;
        break;
    }
    case wing::Keys::escape:
    {
        HandleInputChar(cmajor::systemx::kernel::keyEscape);
        args.handled = true;
        break;
    }
    case wing::Keys::down:
    {
        HandleInputChar(cmajor::systemx::kernel::keyDown);
        args.handled = true;
        break;
    }
    case wing::Keys::up:
    {
        HandleInputChar(cmajor::systemx::kernel::keyUp);
        args.handled = true;
        break;
    }
    case wing::Keys::left:
    {
        HandleInputChar(cmajor::systemx::kernel::keyLeft);
        args.handled = true;
        break;
    }
    case wing::Keys::right:
    {
        HandleInputChar(cmajor::systemx::kernel::keyRight);
        args.handled = true;
        break;
    }
    case wing::Keys::home:
    {
        HandleInputChar(cmajor::systemx::kernel::keyHome);
        args.handled = true;
        break;
    }
    case wing::Keys::f1:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF1);
        args.handled = true;
        break;
    }
    case wing::Keys::f2:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF2);
        args.handled = true;
        break;
    }
    case wing::Keys::f3:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF3);
        args.handled = true;
        break;
    }
    case wing::Keys::f4:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF4);
        args.handled = true;
        break;
    }
    case wing::Keys::f5:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF5);
        args.handled = true;
        break;
    }
    case wing::Keys::f6:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF6);
        args.handled = true;
        break;
    }
    case wing::Keys::f7:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF7);
        args.handled = true;
        break;
    }
    case wing::Keys::f8:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF8);
        args.handled = true;
        break;
    }
    case wing::Keys::f9:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF9);
        args.handled = true;
        break;
    }
    case wing::Keys::f10:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF10);
        args.handled = true;
        break;
    }
    case wing::Keys::f11:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF11);
        args.handled = true;
        break;
    }
    case wing::Keys::f12:
    {
        HandleInputChar(cmajor::systemx::kernel::keyF12);
        args.handled = true;
        break;
    }
    case wing::Keys::delete_:
    {
        HandleInputChar(cmajor::systemx::kernel::keyDel);
        args.handled = true;
        break;
    }
    case wing::Keys::insert:
    {
        HandleInputChar(cmajor::systemx::kernel::keyIns);
        args.handled = true;
        break;
    }
    case wing::Keys::pageDown:
    {
        HandleInputChar(cmajor::systemx::kernel::keyPgDown);
        args.handled = true;
        break;
    }
    case wing::Keys::pageUp:
    {
        HandleInputChar(cmajor::systemx::kernel::keyPgUp);
        args.handled = true;
        break;
    }
    case wing::Keys::print:
    {
        HandleInputChar(cmajor::systemx::kernel::keyPrint);
        args.handled = true;
        break;
    }
    case wing::Keys::end:
    {
        HandleInputChar(cmajor::systemx::kernel::keyEnd);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::delete_:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftDel);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::end:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftEnd);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::home:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftHome);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::left:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftLeft);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::right:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftRight);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::up:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftUp);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::down:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftDown);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::up:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlUp);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::down:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlDown);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::left:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlLeft);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::right:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlRight);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::pageUp:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlPgUp);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::pageDown:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlPgDown);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::home:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlHome);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::end:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlEnd);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::delete_:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlDel);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f1:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF1);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f2:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF2);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f3:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF3);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f4:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF4);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f5:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF5);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f6:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF6);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f7:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF7);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f8:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF8);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f9:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF9);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f10:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF10);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f11:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF11);
        args.handled = true;
        break;
    }
    case wing::Keys::controlModifier | wing::Keys::f12:
    {
        HandleInputChar(cmajor::systemx::kernel::keyControlF12);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::pageUp:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftPgUp);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::pageDown:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftPgDown);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f1:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF1);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f2:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF2);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f3:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF3);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f4:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF4);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f5:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF5);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f6:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF6);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f7:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF7);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f8:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF8);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f9:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF9);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f10:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF10);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f11:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF11);
        args.handled = true;
        break;
    }
    case wing::Keys::shiftModifier | wing::Keys::f12:
    {
        HandleInputChar(cmajor::systemx::kernel::keyShiftF12);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::a:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltA);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::b:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltB);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::c:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltC);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::d:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltD);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::e:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltE);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::g:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltG);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::h:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltH);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::i:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltI);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::j:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltJ);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::k:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltK);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::l:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltL);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::m:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltM);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::n:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltN);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::o:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltO);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::p:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltP);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::q:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltQ);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::r:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltR);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::s:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltS);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::t:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltT);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::u:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltU);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::v:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltV);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::w:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltW);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::x:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltX);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::y:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltY);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::z:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltZ);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f1:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF1);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f2:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF2);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f3:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF3);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f4:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF4);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f5:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF5);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f6:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF6);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f7:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF7);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f8:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF8);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f9:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF9);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f10:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF10);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f11:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF11);
        args.handled = true;
        break;
    }
    case wing::Keys::altModifier | wing::Keys::f12:
    {
        HandleInputChar(cmajor::systemx::kernel::keyAltF12);
        args.handled = true;
        break;
    }
    }
}

void TerminalControl::OnKeyPress(wing::KeyPressEventArgs& args)
{
    HandleInputChar(args.keyChar);
    args.handled = true;
}

} // cmajor::systemx::guiterm
