// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/object/expr.lexer' using soul lexer generator slg version 5.0.0

export module cmajor.systemx.object.expr.lexer;

import std.core;
import soul.lexer;
import soul.ast.slg;
import util;
import cmajor.systemx.object.expr.token;

using namespace soul;
using namespace soul::lexer;
using namespace cmajor::systemx::object::expr::token;

export namespace cmajor::systemx::object::expr::lexer {

std::mutex& MakeLexerMtx();

template<typename Char>
struct ExprLexer;

template<typename Char>
soul::lexer::Lexer<ExprLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName);

template<typename Char>
soul::lexer::Lexer<ExprLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName);

soul::ast::slg::TokenCollection* GetTokens();

struct ExprLexer_Variables : public soul::lexer::Variables
{
    ExprLexer_Variables();
};

template<typename Char>
struct ExprLexer
{
    using Variables = ExprLexer_Variables;

    static int32_t NextState(int32_t state, Char chr, soul::lexer::LexerBase<Char>& lexer)
    {
        soul::lexer::ClassMap<Char>* classmap = lexer.GetClassMap();
        int32_t cls = classmap->GetClass(chr);
        switch (state)
        {
            case 0:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    {
                        return 1;
                    }
                    case 3:
                    {
                        return 2;
                    }
                    case 4:
                    case 5:
                    {
                        return 3;
                    }
                    case 7:
                    {
                        return 4;
                    }
                    case 8:
                    {
                        return 5;
                    }
                    case 9:
                    {
                        return 6;
                    }
                    case 10:
                    {
                        return 7;
                    }
                    case 11:
                    {
                        return 8;
                    }
                    case 12:
                    {
                        return 9;
                    }
                    case 13:
                    {
                        return 10;
                    }
                    case 14:
                    {
                        return 11;
                    }
                    case 15:
                    {
                        return 12;
                    }
                    case 16:
                    {
                        return 13;
                    }
                    case 17:
                    {
                        return 14;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 14:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(10, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 13:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(9, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 12:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(8, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 11:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(7, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 10:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(6, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 9:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(5, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 8:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(4, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 7:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(3, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 6:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(2, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 9:
                    {
                        return 6;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 5:
            {
                switch (cls)
                {
                    case 9:
                    {
                        return 6;
                    }
                    case 18:
                    {
                        return 15;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 15:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(11, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                return -1;
            }
            case 4:
            {
                switch (cls)
                {
                    case 9:
                    {
                        return 6;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 3:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(0, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 4:
                    case 5:
                    {
                        return 3;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 2:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 9:
                    {
                        return 16;
                    }
                    case 3:
                    {
                        return 17;
                    }
                    case 6:
                    {
                        return 18;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 18:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 9:
                    {
                        return 16;
                    }
                    case 3:
                    {
                        return 17;
                    }
                    case 6:
                    {
                        return 18;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 17:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 9:
                    {
                        return 16;
                    }
                    case 3:
                    {
                        return 17;
                    }
                    case 6:
                    {
                        return 18;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 16:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 9:
                    {
                        return 16;
                    }
                    case 3:
                    {
                        return 17;
                    }
                    case 6:
                    {
                        return 18;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 1:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(1, lexer);
                if (tokenId == soul::lexer::CONTINUE_TOKEN)
                {
                    token.id = soul::lexer::CONTINUE_TOKEN;
                    return -1;
                }
                else if (tokenId != soul::lexer::INVALID_TOKEN)
                {
                    token.id = tokenId;
                }
                else
                {
                    token.match = prevMatch;
                }
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 9:
                    {
                        return 16;
                    }
                    case 3:
                    {
                        return 17;
                    }
                    case 6:
                    {
                        return 18;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
        }
        return -1;
    }

    static int64_t GetTokenId(int32_t ruleIndex, soul::lexer::LexerBase<Char>& lexer)
    {
        switch (ruleIndex)
        {
            case 0:
            {
                lexer.Retract();
                break;
            }
            case 1:
            {
                lexer.Retract();
                auto &token = lexer.CurrentToken();
                int64_t kw = lexer.GetKeywordToken(token.match);
                if (kw == INVALID_TOKEN) return ID;
                else return kw;
                break;
            }
            case 2:
            {
                lexer.Retract();
                return INTEGER;
                break;
            }
            case 3:
            {
                lexer.Retract();
                return DOLLAR;
                break;
            }
            case 4:
            {
                lexer.Retract();
                return DEREF;
                break;
            }
            case 5:
            {
                lexer.Retract();
                return DOT;
                break;
            }
            case 6:
            {
                lexer.Retract();
                return LBRACKET;
                break;
            }
            case 7:
            {
                lexer.Retract();
                return RBRACKET;
                break;
            }
            case 8:
            {
                lexer.Retract();
                return LPAREN;
                break;
            }
            case 9:
            {
                lexer.Retract();
                return RPAREN;
                break;
            }
            case 10:
            {
                lexer.Retract();
                return COMMA;
                break;
            }
            case 11:
            {
                lexer.Retract();
                return ARROW;
                break;
            }
        }
        return soul::lexer::CONTINUE_TOKEN;
    }
};

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap()
{
    static soul::lexer::ClassMap<Char>* classmap = soul::lexer::MakeClassMap<Char>("cmajor.systemx.object.expr.lexer.classmap");
    return classmap;
}

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap(const std::string& moduleFileName, util::ResourceFlags resourceFlags)
{
    static soul::lexer::ClassMap<Char>* classmap = soul::lexer::MakeClassMap<Char>(moduleFileName, "cmajor.systemx.object.expr.lexer.classmap", resourceFlags);
    return classmap;
}

template<typename Char>
soul::lexer::KeywordMap<Char>* GetKeywords();

template<>
soul::lexer::KeywordMap<char>* GetKeywords<char>();

template<>
soul::lexer::KeywordMap<char8_t>* GetKeywords<char8_t>();

template<>
soul::lexer::KeywordMap<char16_t>* GetKeywords<char16_t>();

template<>
soul::lexer::KeywordMap<char32_t>* GetKeywords<char32_t>();

template<typename Char>
soul::lexer::Lexer<ExprLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(MakeLexerMtx());
    auto lexer = soul::lexer::Lexer<ExprLexer<Char>, Char>(start, end, fileName);
    lexer.SetClassMap(GetClassMap<Char>());
    lexer.SetTokenCollection(GetTokens());
    lexer.SetKeywordMap(GetKeywords<Char>());
    return lexer;
}

template<typename Char>
soul::lexer::Lexer<ExprLexer<Char>, Char> MakeLexer(const std::string& moduleFileName, util::ResourceFlags resourceFlags, const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(MakeLexerMtx());
    auto lexer = soul::lexer::Lexer<ExprLexer<Char>, Char>(start, end, fileName);
    lexer.SetClassMap(GetClassMap<Char>(moduleFileName, resourceFlags));
    lexer.SetTokenCollection(GetTokens());
    lexer.SetKeywordMap(GetKeywords<Char>());
    return lexer;
}

} // namespace cmajor::systemx::object::expr::lexer
