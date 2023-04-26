// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.test;

import std.core;
import util;
import soul.lexer.base;
import soul.lexer.token;
import soul.ast.slg;

export namespace soul::lexer {

template<typename Lexer>
void TestLexer(Lexer& lexer, util::CodeFormatter& formatter)
{
    soul::ast::slg::TokenCollection* tokens = lexer.GetTokenCollection();
    ++lexer;
    while (*lexer != soul::lexer::END_TOKEN && *lexer != soul::lexer::INVALID_TOKEN)
    {
        auto token = lexer.CurrentToken();
        auto tokenInfo = tokens->GetToken(token.id);
        if (tokenInfo)
        {
            formatter.WriteLine(tokenInfo->Name() + "[" + util::ToUtf8(token.ToString()) + "]");
        }
        else
        {
            formatter.WriteLine("<unknown token>");
        }
        ++lexer;
    }
    if (*lexer == soul::lexer::END_TOKEN)
    {
        formatter.WriteLine("success");
    }
    else
    {
        formatter.WriteLine("failure");
    }
}

} // namespace soul::lexer
