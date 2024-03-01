// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.guiterm.terminal.control;

import wing;
import std.core;

export  namespace cmajor::systemx::guiterm {

struct TerminalControlCreateParams
{
    TerminalControlCreateParams();
    TerminalControlCreateParams& Defaults();
    TerminalControlCreateParams& WindowClassName(const std::string& windowClassName_);
    TerminalControlCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    TerminalControlCreateParams& WindowStyle(int windowStyle_);
    TerminalControlCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    TerminalControlCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    TerminalControlCreateParams& Text(const std::string& text_);
    TerminalControlCreateParams& Location(const wing::Point& location_);
    TerminalControlCreateParams& SetSize(const wing::Size& size_);
    TerminalControlCreateParams& SetAnchors(wing::Anchors anchors_);
    TerminalControlCreateParams& SetDock(wing::Dock dock_);
    TerminalControlCreateParams& TextColor(const wing::Color& textColor_);
    TerminalControlCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    TerminalControlCreateParams& FontSize(float fontSize_);
    wing::ControlCreateParams controlCreateParams;
    wing::Color textColor;
    std::string fontFamilyName;
    float fontSize;
};

struct ColorChar
{
    ColorChar() : ch(' '), foreColor(), backColor() {}
    ColorChar(char32_t ch_, wing::ConsoleColor foreColor_, wing::ConsoleColor backColor_) : ch(ch_), foreColor(foreColor_), backColor(backColor_) {}
    char32_t ch;
    wing::ConsoleColor foreColor;
    wing::ConsoleColor backColor;
};

enum class InputMode
{
    cooked = 0, raw = 1
};

class TerminalControl : public wing::Control
{
public:
    TerminalControl(TerminalControlCreateParams& createParams);
    void SetColors(wing::ConsoleColor foreColor_, wing::ConsoleColor backColor_);
    void SetSize(int32_t rows_, int32_t cols_);
    int Rows() const { return rows; }
    int Cols() const { return cols; }
    void SetCursorPos(int32_t cursorX_, int32_t cursorY_);
    void SetOutputEndCursorPos(int32_t x, int32_t y);
    int32_t CursorX() const { return cursorX; }
    int32_t CursorY() const { return cursorY; }
    wing::ConsoleColor DefaultForeColor() const { return defaultForeColor; }
    wing::ConsoleColor DefaultBackColor() const { return defaultBackColor; }
    void Write(const std::u32string& text);
    void ClearScreen();
    void Activate();
    bool IsActive() const { return active; }
    void HandleKeyDown(wing::KeyEventArgs& args);
    void HandleKeyPress(wing::KeyPressEventArgs& args);
    void SetCooked();
    void SetRaw();
    void SetEcho(bool echo_);
    std::vector<uint8_t> ReadInput(int64_t count);
    void ScrollToCursor();
    void ScrollToLine(int lineIndex);
    void PushLines();
    void PopLines();
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnKeyDown(wing::KeyEventArgs& args) override;
    void OnKeyPress(wing::KeyPressEventArgs& args) override;
private:
    void Measure(wing::Graphics& graphics);
    void Scroll();
    wing::SolidBrush* GetOrInsertSolidBrush(wing::ConsoleColor color);
    void OutputLine(wing::Graphics& graphics, wing::PointF& origin, std::u32string& line);
    void HandleInputChar(char32_t ch);
    void HandleEof();
    void PrintChar(char32_t ch);
    void PrintNewLine();
    void HandleChar(char32_t ch);
    void HandleNewLine();
    void HandleBackspace();
    void HandleTab();
    void HandleEscape();
    void HandleHome();
    void HandleEnd();
    void HandleLeft();
    void HandleRight();
    void HandleUp();
    void HandleDown();
    void HandleControlLeft();
    void HandleControlRight();
    void HandleDel();
    void ClearUpdateLine();
    void UpdateLine();
    void UpdateCursorPos();
    wing::StringFormat stringFormat;
    std::map<wing::ConsoleColor, wing::SolidBrush*> brushMap;
    std::vector<std::unique_ptr<wing::SolidBrush>> brushes;
    wing::ConsoleColor foreColor;
    wing::ConsoleColor backColor;
    wing::ConsoleColor defaultForeColor;
    wing::ConsoleColor defaultBackColor;
    wing::SolidBrush* foreBrush;
    wing::SolidBrush* backBrush;
    int32_t rows;
    int32_t cols;
    int32_t cursorX;
    int32_t cursorY;
    int32_t outputEndCursorPosX;
    int32_t outputEndCursorPosY;
    bool changed;
    bool active;
    std::vector<std::vector<ColorChar>> screen;
    float charWidth;
    float charHeight;
    InputMode inputMode;
    bool echo;
    bool eof;
    std::list<uint8_t> terminalInputBuffer;
    std::vector<std::u32string> lines;
    std::stack<std::vector<std::u32string>> linesStack;
    std::u32string line;
    int32_t lineIndex;
    int pos;
};

} // cmajor::systemx::guiterm
