// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.view.source.code.view;

import cmajor.view.debug.strip;
import cmajor.debuggers;
import cmajor.service;
import cmajor.token;
import wing;
import util;

namespace cmajor::view {

CmajorSourceCodeViewCreateParams::CmajorSourceCodeViewCreateParams() : sourceCodeViewCreateParams()
{
    sourceCodeViewCreateParams.WindowClassName("cmajor.view.CmajorSourceCodeView");
    debugLocationBackgroundColor = wing::Color(255, 241, 129);
    debugLocationTextColor = wing::Color::Black;
    debugLocationPadding = wing::Padding(0, 2, 4, 0);
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::Defaults()
{
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::WindowClassName(const std::string& windowClassName_)
{
    sourceCodeViewCreateParams.WindowClassName(windowClassName_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    sourceCodeViewCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::WindowStyle(int windowStyle_)
{
    sourceCodeViewCreateParams.WindowStyle(windowStyle_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    sourceCodeViewCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    sourceCodeViewCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::Text(const std::string& text_)
{
    sourceCodeViewCreateParams.Text(text_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::Location(const wing::Point& location_)
{
    sourceCodeViewCreateParams.Location(location_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::SetSize(const wing::Size& size_)
{
    sourceCodeViewCreateParams.SetSize(size_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::SetAnchors(wing::Anchors anchors_)
{
    sourceCodeViewCreateParams.SetAnchors(anchors_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::SetDock(wing::Dock dock_)
{
    sourceCodeViewCreateParams.SetDock(dock_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::TextColor(const wing::Color& textColor_)
{
    sourceCodeViewCreateParams.TextColor(textColor_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::SelectionBackgroundColor(const wing::Color& selectionBackgroundColor_)
{
    sourceCodeViewCreateParams.SelectionBackgroundColor(selectionBackgroundColor_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::FontFamilyName(const std::string& fontFamilyName_)
{
    sourceCodeViewCreateParams.FontFamilyName(fontFamilyName_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::FontSize(float fontSize_)
{
    sourceCodeViewCreateParams.FontSize(fontSize_);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::SetSourceCodeTokenStyle(wing::SourceCodeTokenKind tokenKind, wing::TokenStyle tokenStyle)
{
    sourceCodeViewCreateParams.SetSourceCodeTokenStyle(tokenKind, tokenStyle);
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::DebugLocationBackgroundColor(const wing::Color& debugLocationBackgroundColor_)
{
    debugLocationBackgroundColor = debugLocationBackgroundColor_;
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::DebugLocationTextColor(const wing::Color& debugLocationTextColor_)
{
    debugLocationTextColor = debugLocationTextColor_;
    return *this;
}

CmajorSourceCodeViewCreateParams& CmajorSourceCodeViewCreateParams::DebugLocationPadding(const wing::Padding& debugLocationPadding_)
{
    debugLocationPadding = debugLocationPadding_;
    return *this;
}

CmajorSourceCodeView::CmajorSourceCodeView(CmajorSourceCodeViewCreateParams& createParams) :
    SourceCodeView(createParams.sourceCodeViewCreateParams), 
    lexer(cmajor::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), "")),
    debugLocationBackgroundColor(createParams.debugLocationBackgroundColor),
    debugLocationTextColor(createParams.debugLocationTextColor),
    debugLocationPadding(createParams.debugLocationPadding),
    debugStrip(nullptr),
    editor(nullptr)
{
    SetBackgroundItemName("cmajor.editor.background");
    lexer.SetCommentTokenId(cmajor::token::COMMENT);
    std::set<int> blockCommentStates;
    blockCommentStates.insert(124);
    blockCommentStates.insert(125);
    blockCommentStates.insert(126);
    blockCommentStates.insert(127);
    lexer.SetBlockCommentStates(blockCommentStates);
    SetFixed();
    SetIndentSize(4);
    SetMouseHoverMs(wing::DefaultMouseHoverMs());
}

void CmajorSourceCodeView::ToggleBreakpoint()
{
    if (debugStrip)
    {
        int line = CaretLine();
        cmajor::debugger::Breakpoint* bp = debugStrip->GetBreakpoint(line);
        if (bp)
        {
            debugStrip->RemoveBreakpoint(bp);
            debugStrip->Invalidate();
        }
        else
        {
            cmajor::debugger::Breakpoint* bp = new cmajor::debugger::Breakpoint(line);
            debugStrip->AddBreakpoint(bp);
            debugStrip->Invalidate();
        }
    }
}

void CmajorSourceCodeView::UpdateColors()
{
    SourceCodeView::UpdateColors();
    debugLocationBackgroundColor = wing::GetColor("debug.location.background");
    debugLocationTextColor = wing::GetColor("debug.location.text");
}

CmajorSourceCodeView::LexerType::TokenLineType CmajorSourceCodeView::TokenizeLine(const std::u32string& line, int lineNumber, int startState)
{
    LexerType::TokenLineType tokenLine = lexer.TokenizeLine(line, lineNumber, startState);
    int tokenIndex = 0;
    for (LexerType::TokenType& token : tokenLine.tokens)
    {
        std::u32string s = token.match.ToString();
        switch (startState)
        {
        case 0:
        {
            if (token.id == soul::lexer::CONTINUE_TOKEN)
            {
                if (s.length() >= 2)
                {
                    if (s[0] == '/' && s[1] == '/')
                    {
                        token.id = cmajor::token::COMMENT;
                        tokenLine.endState = 0;
                    }
                    else if (s[0] == '/' && s[1] == '*')
                    {
                        token.id = cmajor::token::COMMENT;
                    }
                }
                else if (!s.empty())
                {
                    if (util::IsWhiteSpace(s[0]))
                    {
                        token.id = cmajor::token::WS;
                        if (tokenIndex == tokenLine.tokens.size() - 1)
                        {
                            tokenLine.endState = 0;
                        }
                    }
                }
            }
            break;
        }
        default:
        {
            if (lexer.BlockCommentStates().find(startState) != lexer.BlockCommentStates().cend())
            {
                token.id = cmajor::token::COMMENT;
            }
            break;
        }
        }
        startState = 0;
        ++tokenIndex;
    }
    return tokenLine;
}

wing::SourceCodeTokenKind CmajorSourceCodeView::GetTokenKind(const LexerType::TokenType& token) const
{
    switch (token.id)
    {
    case cmajor::token::AUTO:
    case cmajor::token::ASSERT: case cmajor::token::ELIF: case cmajor::token::ENDIF: case cmajor::token::BOOL: case cmajor::token::TRUE: case cmajor::token::FALSE:
    case cmajor::token::SBYTE: case cmajor::token::BYTE: case cmajor::token::SHORT: case cmajor::token::USHORT: case cmajor::token::INT: case cmajor::token::UINT:
    case cmajor::token::LONG: case cmajor::token::ULONG: case cmajor::token::FLOAT: case cmajor::token::DOUBLE: case cmajor::token::CHAR: case cmajor::token::WCHAR:
    case cmajor::token::UCHAR: case cmajor::token::VOID: case cmajor::token::ENUM: case cmajor::token::CAST: case cmajor::token::INTERFACE: case cmajor::token::NAMESPACE:
    case cmajor::token::USING: case cmajor::token::STATIC: case cmajor::token::EXTERN: case cmajor::token::AS: case cmajor::token::IS: case cmajor::token::EXPLICIT:
    case cmajor::token::DELEGATE: case cmajor::token::INLINE: case cmajor::token::CDECL: case cmajor::token::NOTHROW: case cmajor::token::PUBLIC:
    case cmajor::token::PROTECTED: case cmajor::token::PRIVATE: case cmajor::token::INTERNAL: case cmajor::token::VIRTUAL: case cmajor::token::ABSTRACT:
    case cmajor::token::OVERRIDE: case cmajor::token::SUPPRESS: case cmajor::token::WINAPI: case cmajor::token::OPERATOR: case cmajor::token::CLASS:
    case cmajor::token::RETURN: case cmajor::token::IF: case cmajor::token::ELSE: case cmajor::token::SWITCH: case cmajor::token::CASE: case cmajor::token::DEFAULT:
    case cmajor::token::WHILE: case cmajor::token::DO: case cmajor::token::FOR: case cmajor::token::BREAK: case cmajor::token::CONTINUE: case cmajor::token::GOTO:
    case cmajor::token::TYPEDEF: case cmajor::token::TYPENAME: case cmajor::token::TYPEID: case cmajor::token::CONST: case cmajor::token::CONSTEXPR:
    case cmajor::token::NULLLIT: case cmajor::token::THIS: case cmajor::token::BASE: case cmajor::token::CONSTRUCT: case cmajor::token::DESTROY:
    case cmajor::token::NEW: case cmajor::token::DELETE: case cmajor::token::SIZEOF: case cmajor::token::TRY: case cmajor::token::CATCH: case cmajor::token::THROW:
    case cmajor::token::CONCEPT: case cmajor::token::WHERE: case cmajor::token::AXIOM: case cmajor::token::AND: case cmajor::token::OR:
    {
        return wing::SourceCodeTokenKind::keyword;
    }
    case cmajor::token::ID:
    {
        return wing::SourceCodeTokenKind::identifier;
    }
    case cmajor::token::EXCLAMATION: case cmajor::token::UNIT_TEST: case cmajor::token::EQUIVALENCE: case cmajor::token::IMPLICATION:
    case cmajor::token::DISJUNCTION: case cmajor::token::AMPAMP: case cmajor::token::BITOR: case cmajor::token::BITXOR: case cmajor::token::AMP:
    case cmajor::token::EQ: case cmajor::token::NEQ: case cmajor::token::LEQ: case cmajor::token::GEQ: case cmajor::token::SHIFTLEFT: case cmajor::token::SHIFTRIGHT:
    case cmajor::token::PLUS: case cmajor::token::MINUS: case cmajor::token::STAR: case cmajor::token::DIV: case cmajor::token::REM: case cmajor::token::PLUSPLUS:
    case cmajor::token::MINUSMINUS: case cmajor::token::CPL: case cmajor::token::DOT: case cmajor::token::ARROW: case cmajor::token::LBRACKET:
    case cmajor::token::RBRACKET: case cmajor::token::LPAREN: case cmajor::token::RPAREN: case cmajor::token::LANGLE: case cmajor::token::RANGLE:
    case cmajor::token::COMMA: case cmajor::token::ASSIGN: case cmajor::token::COLON:
    case cmajor::token::SEMICOLON: case cmajor::token::HASH:
    {
        return wing::SourceCodeTokenKind::plain;
    }
    case cmajor::token::LBRACE:
    {
        return wing::SourceCodeTokenKind::beginBlock;
    }
    case cmajor::token::RBRACE:
    {
        return wing::SourceCodeTokenKind::endBlock;
    }
    case cmajor::token::WS:
    {
        return wing::SourceCodeTokenKind::space;
    }
    case cmajor::token::FLOATINGLIT: case cmajor::token::INTLIT:
    {
        return wing::SourceCodeTokenKind::number;
    }
    case cmajor::token::CHARLIT:
    {
        return wing::SourceCodeTokenKind::character;
    }
    case cmajor::token::STRINGLIT:
    {
        return wing::SourceCodeTokenKind::string;
    }
    case cmajor::token::COMMENT:
    {
        return wing::SourceCodeTokenKind::comment;
    }
    }
    return wing::SourceCodeTokenKind::plain;
}

void CmajorSourceCodeView::DrawHilites(wing::Graphics& graphics, int lineIndex, const wing::PointF& origin)
{
    if (!debugStrip) return;
    wing::SourceSpan debugLocation = debugStrip->DebugLocation();
    if (!debugLocation.IsEmpty() && debugLocation.line == lineIndex + 1)
    {
        DrawDebugLocationHilite(graphics, debugLocation, origin);
    }
}

void CmajorSourceCodeView::OnMouseHover(wing::MouseEventArgs& args)
{
    SourceCodeView::OnMouseHover(args);
    int lineNumber = 0;
    int columnNumber = 0;
    GetLineColumn(args.location, lineNumber, columnNumber);
    if (lineNumber >= 1 && lineNumber <= Lines().size() && columnNumber >= 1 && columnNumber <= GetLineLength(lineNumber))
    {
        std::u32string tokenText = GetTokenText(lineNumber, columnNumber);
        if (!tokenText.empty())
        {
            ExpressionHoverEventArgs expressionHoverArgs;
            expressionHoverArgs.expression = util::ToUtf8(tokenText);
            wing::Point loc = args.location;
            TranslateContentLocation(loc);
            loc = ClientToScreen(loc);
            expressionHoverArgs.screenLoc = loc;
            OnExpressionHover(expressionHoverArgs);
        }
    }
}

void CmajorSourceCodeView::OnExpressionHover(ExpressionHoverEventArgs& args)
{
    expressionHover.Fire(args);
}

void CmajorSourceCodeView::DrawDebugLocationHilite(wing::Graphics& graphics, const wing::SourceSpan& debugLocation, const wing::PointF& origin)
{
    std::string hiliteText = util::ToUtf8(GetText(debugLocation));
    int n = std::max(0, static_cast<int>(debugLocation.ecol - debugLocation.scol));
    wing::RectF r(wing::PointF(origin.X + CharWidth() * (debugLocation.scol - 1), origin.Y), wing::SizeF(CharWidth() * n, CharHeight()));
    wing::Brush* backgroundBrush = GetOrInsertBrush(debugLocationBackgroundColor);
    wing::RectF f(wing::PointF(r.X - debugLocationPadding.left, r.Y - debugLocationPadding.top), wing::SizeF(r.Width + debugLocationPadding.Horizontal(), r.Height + debugLocationPadding.Vertical()));
    graphics.FillRectangle(backgroundBrush, f);
    if (!hiliteText.empty())
    {
        wing::Brush* textBrush = GetOrInsertBrush(debugLocationTextColor);
        DrawString(graphics, hiliteText, GetFont(), wing::PointF(r.X, r.Y), *textBrush);
    }
}

} // namespace cmajor::view
