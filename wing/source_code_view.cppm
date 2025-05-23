// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.source.code.view;

import soul.lexer;
import wing.text.view;
import wing.graphics;
import wing.control;
import util.text.util;
import std.core;

export namespace wing {

using Token = soul::lexer::Token<char32_t, soul::lexer::LexerBase<char32_t>>;
using TokenLine = soul::lexer::TokenLine<char32_t, soul::lexer::LexerBase<char32_t>>;

struct SourceSpan
{
    SourceSpan() : line(0), scol(0), ecol(0) {}
    SourceSpan(int line_, short scol_, short ecol_) : line(line_), scol(scol_), ecol(ecol_) {}
    bool IsEmpty() const { return line == 0 && scol == 0 && ecol == 0; }
    int line;
    short scol;
    short ecol;
};

inline bool operator==(const SourceSpan& left, const SourceSpan& right)
{
    return left.line == right.line && left.scol == right.scol && left.ecol == right.ecol;
}

inline bool operator!=(const SourceSpan& left, const SourceSpan& right)
{
    return !(left == right);
}

struct TokenStyle
{
    TokenStyle() : color(Color::Black), fontStyle(FontStyle::FontStyleRegular) {}
    TokenStyle(const Color& color_, FontStyle fontStyle_) : color(color_), fontStyle(fontStyle_) {}
    Color color;
    FontStyle fontStyle;
};

enum class SourceCodeTokenKind : int
{
    plain, space, keyword, identifier, string, character, number, comment, lineNumber, beginBlock, endBlock
};

struct SourceCodeFont
{
    SourceCodeFont(const std::string& fontFamilyName_, float fontSize_, FontStyle fontStyle_);
    std::string fontFamilyName;
    float fontSize;
    FontStyle fontStyle;
};

bool operator<(const SourceCodeFont& left, const SourceCodeFont& right);

struct SourceCodeTextStyle
{
    SourceCodeTextStyle() : font(nullptr), brush(nullptr) {}
    SourceCodeTextStyle(Font* font_, Brush* brush_) : font(font_), brush(brush_) {}
    Font* font;
    Brush* brush;
};

struct SourceCodeViewCreateParams
{
    SourceCodeViewCreateParams();
    SourceCodeViewCreateParams& Defaults();
    SourceCodeViewCreateParams& WindowClassName(const std::string& windowClassName_);
    SourceCodeViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    SourceCodeViewCreateParams& WindowStyle(int windowStyle_);
    SourceCodeViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    SourceCodeViewCreateParams& BackgroundColor(const Color& backgroundColor_);
    SourceCodeViewCreateParams& Text(const std::string& text_);
    SourceCodeViewCreateParams& Location(const Point& location_);
    SourceCodeViewCreateParams& SetSize(const Size& size_);
    SourceCodeViewCreateParams& SetAnchors(Anchors anchors_);
    SourceCodeViewCreateParams& SetDock(Dock dock_);
    SourceCodeViewCreateParams& TextColor(const Color& textColor_);
    SourceCodeViewCreateParams& SelectionBackgroundColor(const Color& selectionBackgroundColor_);
    SourceCodeViewCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    SourceCodeViewCreateParams& FontSize(float fontSize_);
    SourceCodeViewCreateParams& SetSourceCodeTokenStyle(SourceCodeTokenKind tokenKind, TokenStyle tokenStyle);
    TextViewCreateParams textViewCreateParams;
    std::map<SourceCodeTokenKind, TokenStyle> tokenStyleMap;
};

class SourceCodeView : public TextView
{
public:
    SourceCodeView(SourceCodeViewCreateParams& createParams);
    void UpdateColors() override;
protected:
    virtual TokenLine TokenizeLine(const std::u32string& line, int lineNumber, int startState);
    virtual SourceCodeTokenKind GetTokenKind(const Token& token) const;
    virtual void DrawHilites(Graphics& graphics, int lineIndex, const PointF& origin);
    void DrawLine(Graphics& graphics, int lineIndex, const PointF& origin) override;
    int RemoveIndent(int lineIndex) const override;
    int GetIndent(const std::u32string& line, int lineIndex) override;
    void OnLinesChanged() override;
    void OnLineChanged(LineEventArgs& args) override;
    void OnLineDeleted(LineEventArgs& args) override;
    void OnLineInserted(LineEventArgs& args) override;
    int LineNumberFieldLength() const override { return numLineNumberDigits + 1; }
    void SetLineNumberFieldLength(int lineCount) override { numLineNumberDigits = util::Log10(lineCount + 1); }
    TokenStyle GetTokenStyle(SourceCodeTokenKind tokenKind) const;
    Font* GetOrInsertFont(const std::string& fontFamilyName, float fontSize, FontStyle fontStyle);
    SourceCodeTextStyle& GetOrInsertTextStyle(SourceCodeTokenKind tokenKind);
    std::u32string GetText(const SourceSpan& sourceSpan) const;
    std::u32string GetTokenText(int lineNumber, int columnNumber) const;
private:
    bool IsBeginBlockLine(int lineIndex) const;
    bool IsEndBlockLine(int lineIndex) const;
    std::vector<TokenLine> tokenLines;
    int numLineNumberDigits;
    std::map<SourceCodeTokenKind, TokenStyle> tokenStyleMap;
    std::map<SourceCodeTokenKind, SourceCodeTextStyle> textStyleMap;
    std::map<SourceCodeFont, Font*> fontMap;
    std::vector<std::unique_ptr<Font>> fonts;
};

} // wing
