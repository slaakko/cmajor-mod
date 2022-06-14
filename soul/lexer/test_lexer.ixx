// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.test;

import std.core;
import soul.lexer.base;
import soul.lexer.token;
import soul.ast.slg;

export namespace soul::lexer {

template<typename Lexer>
void TestLexer(Lexer& lexer)
{
    soul::ast::slg::TokenCollection* tokens = lexer.GetTokenCollection();
    ++lexer;
    while (*lexer != soul::lexer::END_TOKEN && *lexer != soul::lexer::INVALID_TOKEN)
    {
        auto token = tokens->GetToken(*lexer);
        if (token)
        {
            std::cout << token->Name() << "(" << token->Id() << ")" << std::endl;
        }
        else
        {
            std::cout << "<unknown token>" << std::endl;
        }
        ++lexer;
    }
    if (*lexer == soul::lexer::END_TOKEN)
    {
        std::cout << "success" << std::endl;
    }
    else
    {
        std::cout << "failure" << std::endl;
    }
}

} // namespace soul::lexer
