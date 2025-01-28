// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.source.code.view;

import cmajor.lexers;
import soul.lexer;
import wing;
import std.core;

export namespace cmajor::view {

struct ExpressionHoverEventArgs
{
    std::string expression;
    wing::Point screenLoc;
};

using ExpressionHoverEvent = wing::EventWithArgs<ExpressionHoverEventArgs>;
using ExitExpressionHoverEvent = wing::Event;

class DebugStrip;
class CmajorEditor;

struct CmajorSourceCodeViewCreateParams
{
    CmajorSourceCodeViewCreateParams();
    CmajorSourceCodeViewCreateParams& Defaults();
    CmajorSourceCodeViewCreateParams& WindowClassName(const std::string& windowClassName_);
    CmajorSourceCodeViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    CmajorSourceCodeViewCreateParams& WindowStyle(int windowStyle_);
    CmajorSourceCodeViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    CmajorSourceCodeViewCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    CmajorSourceCodeViewCreateParams& Text(const std::string& text_);
    CmajorSourceCodeViewCreateParams& Location(const wing::Point& location_);
    CmajorSourceCodeViewCreateParams& SetSize(const wing::Size& size_);
    CmajorSourceCodeViewCreateParams& SetAnchors(wing::Anchors anchors_);
    CmajorSourceCodeViewCreateParams& SetDock(wing::Dock dock_);
    CmajorSourceCodeViewCreateParams& TextColor(const wing::Color& textColor_);
    CmajorSourceCodeViewCreateParams& SelectionBackgroundColor(const wing::Color& selectionBackgroundColor_);
    CmajorSourceCodeViewCreateParams& FontFamilyName(const std::string& fontFamilyName_);
    CmajorSourceCodeViewCreateParams& FontSize(float fontSize_);
    CmajorSourceCodeViewCreateParams& SetSourceCodeTokenStyle(wing::SourceCodeTokenKind tokenKind, wing::TokenStyle tokenStyle);
    CmajorSourceCodeViewCreateParams& DebugLocationBackgroundColor(const wing::Color& debugLocationBackgroundColor_);
    CmajorSourceCodeViewCreateParams& DebugLocationTextColor(const wing::Color& debugLocationTextColor_);
    CmajorSourceCodeViewCreateParams& DebugLocationPadding(const wing::Padding& debugLocationPadding_);
    wing::SourceCodeViewCreateParams sourceCodeViewCreateParams;
    wing::Color debugLocationBackgroundColor;
    wing::Color debugLocationTextColor;
    wing::Padding debugLocationPadding;
};

class CmajorSourceCodeView : public wing::SourceCodeView
{
public:
    using LexerType = soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char32_t>, char32_t>;
    CmajorSourceCodeView(CmajorSourceCodeViewCreateParams& createParams);
    DebugStrip* GetDebugStrip() const { return debugStrip; }
    void SetDebugStrip(DebugStrip* debugStrip_) { debugStrip = debugStrip_; }
    CmajorEditor* Editor() const { return editor; }
    void SetEditor(CmajorEditor* editor_) { editor = editor_; }
    void ToggleBreakpoint();
    ExpressionHoverEvent& ExpressionHover() { return expressionHover; }
    void UpdateColors() override;
protected:
    CmajorSourceCodeView::LexerType::TokenLineType TokenizeLine(const std::u32string& line, int lineNumber, int startState) override;
    wing::SourceCodeTokenKind GetTokenKind(const wing::Token& token) const override;
    void DrawHilites(wing::Graphics& graphics, int lineIndex, const wing::PointF& origin) override;
    void OnMouseHover(wing::MouseEventArgs& args) override;
    virtual void OnExpressionHover(ExpressionHoverEventArgs& args);
private:
    void DrawDebugLocationHilite(wing::Graphics& graphics, const wing::SourceSpan& debugLocation, const wing::PointF& origin);
    std::u32string content;
    LexerType lexer;
    wing::Color debugLocationBackgroundColor;
    wing::Color debugLocationTextColor;
    wing::Padding debugLocationPadding;
    DebugStrip* debugStrip;
    CmajorEditor* editor;
    ExpressionHoverEvent expressionHover;
};

} // namespace cmajor::view
