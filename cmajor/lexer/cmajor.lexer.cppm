// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/lexer/cmajor.lexer' using soul lexer generator slg version 4.1.0

export module cmajor.lexer;

import std.core;
import soul.lexer;
import soul.ast.slg;
import cmajor.token;

using namespace soul;
using namespace soul::lexer;
using namespace cmajor::token;

export namespace cmajor::lexer {

std::mutex& MakeLexerMtx();

template<typename Char>
struct CmajorLexer;

template<typename Char>
soul::lexer::Lexer<CmajorLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName);

soul::ast::slg::TokenCollection* GetTokens();

struct CmajorLexer_Variables : public soul::lexer::Variables
{
    CmajorLexer_Variables();
    double floatingLit;
    bool floatingLitFloat;
    uint64_t intLit;
    bool intLitUnsigned;
    char32_t chrLit;
    int chrLitPrefix;
    std::u32string strLit;
    int strLitPrefix;
    int leftAngleCount;
    bool parsingOperatorFunctionGroupId;
};

template<typename Char>
struct CmajorLexer
{
    using Variables = CmajorLexer_Variables;

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
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 2:
                    {
                        return 2;
                    }
                    case 3:
                    {
                        return 3;
                    }
                    case 4:
                    case 5:
                    case 7:
                    case 11:
                    case 12:
                    case 14:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 4;
                    }
                    case 13:
                    {
                        return 5;
                    }
                    case 15:
                    {
                        return 6;
                    }
                    case 16:
                    {
                        return 7;
                    }
                    case 17:
                    {
                        return 8;
                    }
                    case 18:
                    {
                        return 9;
                    }
                    case 19:
                    {
                        return 10;
                    }
                    case 26:
                    case 28:
                    {
                        return 11;
                    }
                    case 27:
                    {
                        return 12;
                    }
                    case 29:
                    {
                        return 13;
                    }
                    case 31:
                    {
                        return 14;
                    }
                    case 39:
                    {
                        return 15;
                    }
                    case 40:
                    {
                        return 16;
                    }
                    case 41:
                    {
                        return 17;
                    }
                    case 42:
                    {
                        return 18;
                    }
                    case 43:
                    {
                        return 19;
                    }
                    case 44:
                    {
                        return 20;
                    }
                    case 45:
                    {
                        return 21;
                    }
                    case 46:
                    {
                        return 22;
                    }
                    case 47:
                    {
                        return 23;
                    }
                    case 48:
                    {
                        return 24;
                    }
                    case 49:
                    {
                        return 25;
                    }
                    case 50:
                    {
                        return 26;
                    }
                    case 51:
                    {
                        return 27;
                    }
                    case 52:
                    {
                        return 28;
                    }
                    case 53:
                    {
                        return 29;
                    }
                    case 54:
                    {
                        return 30;
                    }
                    case 55:
                    {
                        return 31;
                    }
                    case 56:
                    {
                        return 32;
                    }
                    case 57:
                    {
                        return 33;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 33:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(42, lexer);
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
            case 32:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(41, lexer);
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
            case 31:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(40, lexer);
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
            case 30:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(38, lexer);
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
            case 29:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(37, lexer);
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
            case 28:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(36, lexer);
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
            case 27:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(35, lexer);
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
            case 26:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(34, lexer);
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
            case 25:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(33, lexer);
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
            case 24:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(32, lexer);
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
            case 23:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(29, lexer);
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
            case 22:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(25, lexer);
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
            case 21:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(28, lexer);
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
                    case 40:
                    {
                        return 34;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 34:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(14, lexer);
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
            case 20:
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
            case 19:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(12, lexer);
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
                    case 43:
                    {
                        return 35;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 35:
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
            case 18:
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
                switch (cls)
                {
                    case 42:
                    {
                        return 36;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 36:
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
            case 17:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(18, lexer);
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
                    case 40:
                    {
                        return 37;
                    }
                    case 41:
                    {
                        return 38;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 38:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(20, lexer);
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
            case 37:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(16, lexer);
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
            case 16:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(39, lexer);
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
                    case 40:
                    {
                        return 39;
                    }
                    case 41:
                    {
                        return 40;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 40:
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
            case 39:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(13, lexer);
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
            case 15:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(17, lexer);
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
                    case 39:
                    {
                        return 41;
                    }
                    case 40:
                    {
                        return 42;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 42:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(15, lexer);
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
                    case 41:
                    {
                        return 43;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 43:
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
            case 41:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(19, lexer);
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
            case 14:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 44;
                    }
                    case 30:
                    {
                        return 45;
                    }
                    case 31:
                    {
                        return 46;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 46:
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
            case 45:
            {
                switch (cls)
                {
                    case 11:
                    {
                        return 47;
                    }
                    case 12:
                    {
                        return 48;
                    }
                    case 13:
                    {
                        return 49;
                    }
                    case 14:
                    {
                        return 50;
                    }
                    case 24:
                    {
                        return 51;
                    }
                    case 25:
                    {
                        return 52;
                    }
                    case 27:
                    case 28:
                    {
                        return 53;
                    }
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 54;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 54:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 53:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    case 27:
                    case 28:
                    {
                        return 53;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 52:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 55;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 55:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 55;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 51:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 55;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 50:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 56;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 56:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 57;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 57:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 58;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 58:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 59;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 59:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 60;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 60:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 61;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 61:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 62;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 62:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 63;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 63:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 49:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 64;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 64:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 65;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 65:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 66;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 66:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 67;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 67:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 48:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 68;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 68:
            {
                switch (cls)
                {
                    case 31:
                    {
                        return 46;
                    }
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 68;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 47:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 68;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 44:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 44;
                    }
                    case 31:
                    {
                        return 46;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 13:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 71:
            {
                switch (cls)
                {
                    case 11:
                    {
                        return 72;
                    }
                    case 12:
                    {
                        return 73;
                    }
                    case 13:
                    {
                        return 74;
                    }
                    case 14:
                    {
                        return 75;
                    }
                    case 24:
                    {
                        return 76;
                    }
                    case 25:
                    {
                        return 77;
                    }
                    case 27:
                    case 28:
                    {
                        return 78;
                    }
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 79;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 79:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 78:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    case 27:
                    case 28:
                    {
                        return 78;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 77:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 80;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 80:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 80;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 76:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 80;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 75:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 81;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 81:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 82;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 82:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 83;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 83:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 84;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 84:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 85;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 85:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 86;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 86:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 87;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 87:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 88;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 88:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 74:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 89;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 89:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 90;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 90:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 91;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 91:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 92;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 92:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 73:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 93;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 93:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 93;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 72:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 93;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 70:
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
            case 69:
            {
                switch (cls)
                {
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 69;
                    }
                    case 29:
                    {
                        return 70;
                    }
                    case 30:
                    {
                        return 71;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 12:
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
                    case 11:
                    {
                        return 94;
                    }
                    case 12:
                    {
                        return 95;
                    }
                    case 13:
                    {
                        return 96;
                    }
                    case 14:
                    {
                        return 97;
                    }
                    case 15:
                    {
                        return 98;
                    }
                    case 21:
                    {
                        return 99;
                    }
                    case 22:
                    {
                        return 100;
                    }
                    case 26:
                    {
                        return 101;
                    }
                    case 27:
                    case 28:
                    {
                        return 102;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 102:
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
                    case 13:
                    {
                        return 96;
                    }
                    case 14:
                    {
                        return 97;
                    }
                    case 15:
                    {
                        return 98;
                    }
                    case 21:
                    {
                        return 99;
                    }
                    case 22:
                    {
                        return 100;
                    }
                    case 26:
                    {
                        return 101;
                    }
                    case 27:
                    case 28:
                    {
                        return 102;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 101:
            {
                switch (cls)
                {
                    case 15:
                    {
                        return 98;
                    }
                    case 21:
                    {
                        return 99;
                    }
                    case 22:
                    {
                        return 100;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 101;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 100:
            {
                switch (cls)
                {
                    case 16:
                    {
                        return 103;
                    }
                    case 17:
                    {
                        return 104;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 105;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 105:
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
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 105;
                    }
                    case 23:
                    {
                        return 106;
                    }
                    case 36:
                    {
                        return 107;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 107:
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
            case 106:
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
            case 104:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 105;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 103:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 105;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 99:
            {
                switch (cls)
                {
                    case 16:
                    {
                        return 103;
                    }
                    case 17:
                    {
                        return 104;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 105;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 98:
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
                switch (cls)
                {
                    case 23:
                    {
                        return 106;
                    }
                    case 36:
                    {
                        return 107;
                    }
                    case 21:
                    {
                        return 108;
                    }
                    case 22:
                    {
                        return 109;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 110;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 110:
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
                switch (cls)
                {
                    case 23:
                    {
                        return 106;
                    }
                    case 36:
                    {
                        return 107;
                    }
                    case 21:
                    {
                        return 108;
                    }
                    case 22:
                    {
                        return 109;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 110;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 109:
            {
                switch (cls)
                {
                    case 16:
                    {
                        return 111;
                    }
                    case 17:
                    {
                        return 112;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 113:
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
                switch (cls)
                {
                    case 23:
                    {
                        return 106;
                    }
                    case 36:
                    {
                        return 107;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 112:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 111:
            {
                switch (cls)
                {
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 108:
            {
                switch (cls)
                {
                    case 16:
                    {
                        return 111;
                    }
                    case 17:
                    {
                        return 112;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 113;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 97:
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
                return -1;
            }
            case 96:
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
                return -1;
            }
            case 95:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 114;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 114:
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
                    case 13:
                    {
                        return 96;
                    }
                    case 14:
                    {
                        return 97;
                    }
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 114;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 94:
            {
                switch (cls)
                {
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 114;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 11:
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
                    case 13:
                    {
                        return 96;
                    }
                    case 14:
                    {
                        return 97;
                    }
                    case 15:
                    {
                        return 98;
                    }
                    case 21:
                    {
                        return 99;
                    }
                    case 22:
                    {
                        return 100;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 115;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 115:
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
                    case 13:
                    {
                        return 96;
                    }
                    case 14:
                    {
                        return 97;
                    }
                    case 15:
                    {
                        return 98;
                    }
                    case 21:
                    {
                        return 99;
                    }
                    case 22:
                    {
                        return 100;
                    }
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 115;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 10:
            {
                switch (cls)
                {
                    case 29:
                    {
                        return 116;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 116:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 117;
                    }
                    case 29:
                    {
                        return 118;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 118:
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
            case 117:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 117;
                    }
                    case 29:
                    {
                        return 118;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 9:
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
                    case 19:
                    {
                        return 10;
                    }
                    case 29:
                    {
                        return 13;
                    }
                    case 31:
                    {
                        return 14;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 119;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 119:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 119;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 8:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(22, lexer);
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
                    case 17:
                    {
                        return 120;
                    }
                    case 41:
                    {
                        return 121;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 121:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(31, lexer);
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
            case 120:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(27, lexer);
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
                int64_t tokenId = GetTokenId(21, lexer);
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
                    case 16:
                    {
                        return 122;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 122:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(26, lexer);
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
                int64_t tokenId = GetTokenId(30, lexer);
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
                    case 26:
                    case 27:
                    case 28:
                    {
                        return 110;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 5:
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
                    case 19:
                    {
                        return 10;
                    }
                    case 29:
                    {
                        return 13;
                    }
                    case 31:
                    {
                        return 14;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 119;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 4:
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
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 18:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    {
                        return 119;
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
                int64_t tokenId = GetTokenId(23, lexer);
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
            case 2:
            {
                auto& token = lexer.CurrentToken();
                auto prevMatch = token.match;
                token.match = lexer.CurrentLexeme();
                int64_t tokenId = GetTokenId(24, lexer);
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
                    case 2:
                    {
                        return 123;
                    }
                    case 3:
                    {
                        return 124;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 124:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 126:
            {
                switch (cls)
                {
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 127;
                    }
                    case 2:
                    {
                        return 128;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 128:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 130:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 2:
                    {
                        return 131;
                    }
                    case 3:
                    {
                        return 132;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 132:
            {
                switch (cls)
                {
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 127;
                    }
                    case 2:
                    {
                        return 128;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 131:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 133;
                    }
                    case 1:
                    {
                        return 134;
                    }
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 136;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 136:
            {
                switch (cls)
                {
                    case 3:
                    {
                        return 136;
                    }
                    case 0:
                    {
                        return 137;
                    }
                    case 1:
                    {
                        return 138;
                    }
                    case 2:
                    {
                        return 139;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 140;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 140:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 133;
                    }
                    case 1:
                    {
                        return 134;
                    }
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 136;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 139:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 136;
                    }
                    case 0:
                    {
                        return 141;
                    }
                    case 1:
                    {
                        return 142;
                    }
                    case 2:
                    {
                        return 143;
                    }
                    case 8:
                    case 9:
                    {
                        return 144;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 144:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 136;
                    }
                    case 0:
                    {
                        return 141;
                    }
                    case 1:
                    {
                        return 142;
                    }
                    case 2:
                    {
                        return 143;
                    }
                    case 8:
                    case 9:
                    {
                        return 144;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 143:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 131;
                    }
                    case 0:
                    {
                        return 133;
                    }
                    case 1:
                    {
                        return 134;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 145;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 145:
            {
                switch (cls)
                {
                    case 3:
                    {
                        return 136;
                    }
                    case 0:
                    {
                        return 137;
                    }
                    case 1:
                    {
                        return 138;
                    }
                    case 2:
                    {
                        return 139;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 140;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 142:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    case 0:
                    {
                        return 146;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 146:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 141:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 138:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    case 0:
                    {
                        return 146;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 137:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 135:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 133;
                    }
                    case 1:
                    {
                        return 134;
                    }
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 135;
                    }
                    case 3:
                    {
                        return 136;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 134:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    case 0:
                    {
                        return 146;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 133:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 129:
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
                    case 6:
                    case 7:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 129;
                    }
                    case 2:
                    {
                        return 130;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 127:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 125:
            {
                switch (cls)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 125;
                    }
                    case 3:
                    {
                        return 126;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 123:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 147;
                    }
                    case 1:
                    {
                        return 148;
                    }
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 149;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 149:
            {
                switch (cls)
                {
                    case 0:
                    {
                        return 147;
                    }
                    case 1:
                    {
                        return 148;
                    }
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    case 33:
                    case 34:
                    case 35:
                    case 36:
                    case 37:
                    case 38:
                    case 39:
                    case 40:
                    case 41:
                    case 42:
                    case 43:
                    case 44:
                    case 45:
                    case 46:
                    case 47:
                    case 48:
                    case 49:
                    case 50:
                    case 51:
                    case 52:
                    case 53:
                    case 54:
                    case 55:
                    case 56:
                    case 57:
                    {
                        return 149;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 148:
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
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 0:
                    {
                        return 150;
                    }
                    case 2:
                    {
                        return 151;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 151:
            {
                switch (cls)
                {
                    case 2:
                    {
                        return 123;
                    }
                    case 3:
                    {
                        return 124;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 150:
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
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 2:
                    {
                        return 151;
                    }
                    default:
                    {
                        return -1;
                    }
                }
            }
            case 147:
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
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 2:
                    {
                        return 151;
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
                    case 0:
                    case 1:
                    case 8:
                    case 9:
                    {
                        return 1;
                    }
                    case 2:
                    {
                        return 151;
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
                if (kw == soul::lexer::INVALID_TOKEN) return ID;
                else return kw;
                break;
            }
            case 2:
            {
                lexer.Retract();
                return INTLIT;
                break;
            }
            case 3:
            {
                lexer.Retract();
                return FLOATINGLIT;
                break;
            }
            case 4:
            {
                lexer.Retract();
                return CHARLIT;
                break;
            }
            case 5:
            {
                lexer.Retract();
                return STRINGLIT;
                break;
            }
            case 6:
            {
                lexer.Retract();
                return EQUIVALENCE;
                break;
            }
            case 7:
            {
                lexer.Retract();
                return IMPLICATION;
                break;
            }
            case 8:
            {
                lexer.Retract();
                return DISJUNCTION;
                break;
            }
            case 9:
            {
                lexer.Retract();
                return AMPAMP;
                break;
            }
            case 10:
            {
                lexer.Retract();
                return BITOR;
                break;
            }
            case 11:
            {
                lexer.Retract();
                return BITXOR;
                break;
            }
            case 12:
            {
                lexer.Retract();
                return AMP;
                break;
            }
            case 13:
            {
                lexer.Retract();
                return EQ;
                break;
            }
            case 14:
            {
                lexer.Retract();
                return NEQ;
                break;
            }
            case 15:
            {
                lexer.Retract();
                return LEQ;
                break;
            }
            case 16:
            {
                lexer.Retract();
                return GEQ;
                break;
            }
            case 17:
            {
                lexer.Retract();
                return LANGLE;
                break;
            }
            case 18:
            {
                lexer.Retract();
                return RANGLE;
                break;
            }
            case 19:
            {
                auto vars = static_cast<Variables*>(lexer.GetVariables());
                if (vars->parsingOperatorFunctionGroupId) return soul::lexer::INVALID_TOKEN;
                lexer.Retract();
                return SHIFTLEFT;
                break;
            }
            case 20:
            {
                auto vars = static_cast<Variables*>(lexer.GetVariables());
                if (vars->leftAngleCount > 0) return soul::lexer::INVALID_TOKEN;
                lexer.Retract();
                return SHIFTRIGHT;
                break;
            }
            case 21:
            {
                lexer.Retract();
                return PLUS;
                break;
            }
            case 22:
            {
                lexer.Retract();
                return MINUS;
                break;
            }
            case 23:
            {
                lexer.Retract();
                return STAR;
                break;
            }
            case 24:
            {
                lexer.Retract();
                return DIV;
                break;
            }
            case 25:
            {
                lexer.Retract();
                return REM;
                break;
            }
            case 26:
            {
                lexer.Retract();
                return PLUSPLUS;
                break;
            }
            case 27:
            {
                lexer.Retract();
                return MINUSMINUS;
                break;
            }
            case 28:
            {
                lexer.Retract();
                return EXCLAMATION;
                break;
            }
            case 29:
            {
                lexer.Retract();
                return CPL;
                break;
            }
            case 30:
            {
                lexer.Retract();
                return DOT;
                break;
            }
            case 31:
            {
                lexer.Retract();
                return ARROW;
                break;
            }
            case 32:
            {
                lexer.Retract();
                return LBRACKET;
                break;
            }
            case 33:
            {
                lexer.Retract();
                return RBRACKET;
                break;
            }
            case 34:
            {
                lexer.Retract();
                return LPAREN;
                break;
            }
            case 35:
            {
                lexer.Retract();
                return RPAREN;
                break;
            }
            case 36:
            {
                lexer.Retract();
                return LBRACE;
                break;
            }
            case 37:
            {
                lexer.Retract();
                return RBRACE;
                break;
            }
            case 38:
            {
                lexer.Retract();
                return COMMA;
                break;
            }
            case 39:
            {
                lexer.Retract();
                return ASSIGN;
                break;
            }
            case 40:
            {
                lexer.Retract();
                return COLON;
                break;
            }
            case 41:
            {
                lexer.Retract();
                return SEMICOLON;
                break;
            }
            case 42:
            {
                lexer.Retract();
                return HASH;
                break;
            }
        }
        return soul::lexer::CONTINUE_TOKEN;
    }
};

template<typename Char>
soul::lexer::ClassMap<Char>* GetClassMap()
{
    static soul::lexer::ClassMap<Char>* classmap = soul::lexer::MakeClassMap<Char>("cmajor.lexer.classmap");
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
soul::lexer::Lexer<CmajorLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    std::lock_guard<std::mutex> lock(MakeLexerMtx());
    auto lexer = soul::lexer::Lexer<CmajorLexer<Char>, Char>(start, end, fileName);
    lexer.SetClassMap(GetClassMap<Char>());
    lexer.SetTokenCollection(GetTokens());
    lexer.SetKeywordMap(GetKeywords<Char>());
    return lexer;
}

} // namespace cmajor::lexer
