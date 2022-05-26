// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer;

export import soul.lexer.concepts;
export import soul.lexer.lexeme;
export import soul.lexer.token;
export import soul.lexer.keyword;
export import soul.lexer.base;
export import soul.lexer.classmap;
export import soul.lexer.variables;
export import soul.lexer.error;
export import soul.lexer.source.pos;
export import soul.lexer.parsing.log;
export import soul.lexer.xml.parsing.log;
export import soul.lexer.file.map;

import std.core;
import util.unicode;
import soul.ast.slg;
import soul.token.parser;

using namespace util;

export namespace soul::lexer {

enum class LexerFlags : int32_t
{
    none = 0, synchronize = 1 << 0, synchronized = 1 << 1, synchronizedAtLeastOnce = 1 << 2, cursorSeen = 1 << 3, recordedParse = 1 << 4, farthestError = 1 << 5
};

constexpr LexerFlags operator|(LexerFlags left, LexerFlags right)
{
    return static_cast<LexerFlags>(static_cast<int32_t>(left) | static_cast<int32_t>(right));
}

constexpr LexerFlags operator&(LexerFlags left, LexerFlags right)
{
    return static_cast<LexerFlags>(static_cast<int32_t>(left) & static_cast<int32_t>(right));
}

constexpr LexerFlags operator~(LexerFlags flag)
{
    return static_cast<LexerFlags>(~static_cast<int32_t>(flag));
}

template<typename Stack>
    requires RuleStack<Stack>
struct RuleGuard
{
    RuleGuard(Stack& stack_, int ruleId) : stack(stack_)
    {
        stack.PushRule(ruleId);
    }
    ~RuleGuard()
    {
        stack.PopRule();
    }
    Stack& stack;
};

template<typename Char>
const Char* LineStart(const Char* start, const Char* p)
{
    while (p != start && *p != '\n' && *p != '\r')
    {
        --p;
    }
    if (p != start)
    {
        ++p;
    }
    return p;
}

template<typename Char>
const Char* LineEnd(const Char* end, const Char* p)
{
    while (p != end && *p != '\n' && *p != '\r')
    {
        ++p;
    }
    return p;
}

template<typename Machine, typename Char>
    requires FiniteStateMachine<Machine, Char>
class Lexer : public LexerBase<Char>
{
public:
    using CharType = Char;
    using TokenType = Token<Char, LexerBase<Char>>;
    using VariableClassType = Machine::Variables;

    Lexer(const Char* start_, const Char* end_, const std::string& fileName_) :
        flags(LexerFlags::none), 
        file(-1),
        line(1),
        current(tokens.end()), 
        token(this),
        separatorChar('\0'), 
        start(start_), 
        end(end_), 
        pos(start), 
        fileName(fileName_), 
        countLines(true), 
        classMap(nullptr),
        vars(nullptr),
        tokenCollection(nullptr),
        keywordMap(nullptr),
        ruleNameVecPtr(nullptr),
        farthestPos(GetPos()),
        log(nullptr)
    {
        ComputeLineStarts();
    }
    int64_t operator*() const
    {
        return current->id;
    }
    void operator++()
    {
        if (current != tokens.end())
        {
            ++current;
        }
        if (current == tokens.end())
        {
            NextToken();
        }
        else
        {
            line = current->line;
        }
        int64_t p = GetPos();
        if (p > farthestPos)
        {
            farthestPos = p;
            farthestRuleContext = ruleContext;
        }
    }
    int64_t GetPos() const
    {
        int32_t p = static_cast<int32_t>(current - tokens.begin());
        return (static_cast<int64_t>(line) << 32) | static_cast<int64_t>(p);
    }
    void SetPos(int64_t pos)
    {
        current = tokens.begin() + static_cast<int32_t>(pos);
        line = static_cast<int32_t>(pos >> 32);
    }
    const Token<Char, LexerBase<Char>>& GetToken(int64_t pos) const override
    {
        int32_t tokenIndex = static_cast<int32_t>(pos);
        if (tokenIndex >= 0 && tokenIndex < tokens.size())
        {
            return tokens[tokenIndex];
        }
        else
        {
            throw std::runtime_error("invalid token index");
        }
    }
    const std::string& FileName() const override
    {
        return fileName;
    }
    int File() const override
    {
        return file;
    }
    void SetFile(int file_)
    {
        file = file_;
    }
    int Line() const override
    {
        return line;
    }
    ClassMap<Char>* GetClassMap() const override
    {
        return classMap;
    }
    void SetClassMap(ClassMap<Char>* classMap_) override
    {
        classMap = classMap_;
    }
    Variables* GetVariables() const override
    {
        return vars;
    }
    void SetVariables(Variables* vars_) override
    {
        vars = vars_;
    }
    soul::ast::slg::TokenCollection* GetTokenCollection() const override
    {
        return tokenCollection;
    }
    void SetTokenCollection(soul::ast::slg::TokenCollection* tokenCollection_) override
    {
        tokenCollection = tokenCollection_;
    }
    KeywordMap<Char>* GetKeywordMap() const override
    {
        return keywordMap;
    }
    void SetKeywordMap(KeywordMap<Char>* keywordMap_) override
    {
        keywordMap = keywordMap_;
    }
    std::vector<std::string>* GetRuleNameVecPtr() const override
    {
        return ruleNameVecPtr;
    }
    void SetRuleNameVecPtr(std::vector<std::string>* ruleNameVecPtr_) override
    { 
        ruleNameVecPtr = ruleNameVecPtr_; 
    }
    LexerFlags Flags() const { return flags; }
    bool GetFlag(LexerFlags flag) const { return (flags & flag) != LexerFlags::none; }
    void SetFlag(LexerFlags flag) { flags = flags | flag; }
    void ResetFlag(LexerFlags flag) { flags = flags & ~flag; }
    void PushRule(int ruleId)
    {
        ruleContext.push_back(ruleId);
    }
    void PopRule()
    {
        ruleContext.pop_back();
    }
    int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const override
    {
        if (keywordMap)
        {
            return keywordMap->GetKeywordToken(lexeme);
        }
        else
        {
            return INVALID_TOKEN;
        }
    }
    void Retract() override
    { 
        token.match.end = pos; 
    }
    Token<Char, LexerBase<Char>>& CurrentToken() override
    {
        return token;
    }
    const Lexeme<Char>& CurrentLexeme() const override
    {
        return lexeme;
    }
    std::string GetTokenName(int64_t tokenID) const
    {
        soul::ast::slg::TokenCollection* tokenCollection = GetTokenCollection();
        auto token = tokenCollection->GetToken(tokenID);
        if (token)
        {
            return token->Name();
        }
        else
        {
            return "<unknown token>";
        }
    }
    std::string GetTokenInfo(int64_t tokenID) const
    {
        soul::ast::slg::TokenCollection* tokenCollection = GetTokenCollection();
        auto token = tokenCollection->GetToken(tokenID);
        if (token)
        {
            return token->Info();
        }
        else
        {
            return "<unknown token>";
        }
    }
    inline int GetLine(int64_t pos) const
    {
        return pos >> 32;
    }
    SourcePos GetSourcePos(int64_t pos) const
    {
        const Char* s = start;
        int line = GetLine(pos);
        if (line < lineStarts.size())
        {
            s = lineStarts[line];
        }
        Token token = GetToken(pos);
        int col = static_cast<int>(token.match.begin - s + 1);
        return SourcePos(pos, file, line, col);
    }
    std::string ErrorLines(int64_t pos) const
    {
        auto token = GetToken(pos);
        std::basic_string<Char> lines;
        const Char* lineStart = LineStart(start, token.match.begin);
        const Char* lineEnd = LineEnd(end, token.match.end);
        lines.append(std::basic_string<Char>(lineStart, token.match.begin));
        lines.append(token.match.ToString());
        lines.append(std::basic_string<Char>(token.match.end, lineEnd));
        lines.append(1, '\n');
        lines.append(token.match.begin - lineStart, ' ');
        lines.append(std::max(static_cast<int64_t>(1), token.match.end - token.match.begin), '^');
        lines.append(lineEnd - token.match.end, ' ');
        lines.append(1, '\n');
        return ToUtf8(lines);
    }
    void ThrowExpectationFailure(int64_t pos, const std::string& name)
    {
        SourcePos sourcePos = GetSourcePos(pos);
        throw ParsingException("parsing error at '" + fileName + ":" + std::to_string(sourcePos.line) + "': " + name + " expected:\n" + ErrorLines(pos), fileName, sourcePos);
    }
    std::string GetParserStateStr() const
    {
        std::string parserStateStr;
        int n = farthestRuleContext.size();
        if (ruleNameVecPtr && n > 0)
        {
            parserStateStr.append("\nParser state:\n");
            for (int i = 0; i < n; ++i)
            {
                int ruleId = farthestRuleContext[i];
                if (ruleId >= 0 && ruleId < ruleNameVecPtr->size())
                {
                    std::string ruleName = (*ruleNameVecPtr)[ruleId];
                    parserStateStr.append(ruleName.append("\n"));
                }
            }
        }
        return parserStateStr;
    }
    std::string GetError(int64_t pos) const
    {
        std::string parserStateStr = GetParserStateStr();
        return "parsing error at '" + fileName + ":" + std::to_string(GetLine(pos)) + "':\n" + ErrorLines(pos) + parserStateStr;
    }
    void ThrowFarthestError()
    {
        SourcePos sourcePos = GetSourcePos(farthestPos);
        throw ParsingException(GetError(farthestPos), fileName, sourcePos);
    }
    void SetLog(ParsingLog* log_) 
    { 
        log = log_; 
    }
    ParsingLog* Log() const 
    { 
        return log; 
    }
    std::string RestOfLine(int maxLineLength) const
    {
        std::string restOfLine(util::ToUtf8(current->match.ToString()) + ToString(current->match.end, pos) + ToString(pos, LineEnd(end, pos)));
        if (maxLineLength != 0)
        {
            restOfLine = restOfLine.substr(0, maxLineLength);
        }
        return restOfLine;
    }
    std::vector<int> GetLineStartIndeces() const
    {
        std::vector<int> lineStartIndeces;
        for (int i = 0; i < lineStarts.size(); ++i)
        {
            lineStartIndeces.push_back(static_cast<int>(lineStarts[i] - start));
        }
        return lineStartIndeces;
    }
private:
    void NextToken()
    {
        int state = 0;
        while (true)
        {
            Char c = separatorChar;
            if (pos != end)
            {
                c = *pos;
            }
            else if (c == '\0')
            {
                break;
            }
            if (state == 0)
            {
                lexeme.begin = pos;
                token.id = INVALID_TOKEN;
                token.line = line;
            }
            if (pos == end)
            {
                lexeme.end = end;
            }
            else
            {
                lexeme.end = pos + 1;
            }
            state = Machine::NextState(state, c, *this);
            if (state == -1)
            {
                if (token.id == CONTINUE_TOKEN)
                {
                    if (pos == end)
                    {
                        break;
                    }
                    else
                    {
                        pos = token.match.end;
                    }
                    state = 0;
                    continue;
                }
                else if (token.id == INVALID_TOKEN)
                {
                    if (pos == end)
                    {
                        break;
                    }
                    else
                    {
                        throw std::runtime_error("soul::lexer::Lexer::NextToken(): error: invalid character '" + ToUtf8(std::u32string(1, c)) + "' in file '" + fileName + "' at line " + std::to_string(line));
                    }
                }
                else
                {
                    tokens.push_back(token);
                    current = tokens.end() - 1;
                    pos = token.match.end;
                    return;
                }
            }
            if (c == '\n' && countLines)
            {
                ++line;
            }
            ++pos;
        }
        token.id = INVALID_TOKEN;
        state = Machine::NextState(state, '\0', *this);
        int64_t p = -1;
        if (token.id != INVALID_TOKEN && token.id != CONTINUE_TOKEN)
        {
            tokens.push_back(token);
            current = tokens.end() - 1;
            p = GetPos();
        }
        Token<Char, LexerBase<Char>> endToken(END_TOKEN, this);
        endToken.match.begin = end;
        endToken.match.end = end;
        tokens.push_back(endToken);
        if (p == -1)
        {
            current = tokens.end() - 1;
            p = GetPos();
        }
        SetPos(p);
    }
    void ComputeLineStarts()
    {
        lineStarts.push_back(pos);
        const Char* p = pos;
        bool startOfLine = true;
        while (p != end)
        {
            if (startOfLine)
            {
                lineStarts.push_back(p);
            }
            startOfLine = *p == '\n';
            ++p;
        }
        lineStarts.push_back(end);
    }
    LexerFlags flags;
    Char separatorChar;
    std::vector<Token<Char, LexerBase<Char>>> tokens;
    std::vector<Token<Char, LexerBase<Char>>>::iterator current;
    Token<Char, LexerBase<Char>> token;
    Lexeme<Char> lexeme;
    int32_t file;
    int32_t line;
    const Char* start;
    const Char* end;
    const Char* pos;
    std::string fileName;
    bool countLines;
    ClassMap<Char>* classMap;
    Variables* vars;
    soul::ast::slg::TokenCollection* tokenCollection;
    KeywordMap<Char>* keywordMap;
    int64_t farthestPos;
    std::vector<int> ruleContext;
    std::vector<int> farthestRuleContext;
    std::vector<const Char*> lineStarts;
    std::vector<std::string>* ruleNameVecPtr;
    ParsingLog* log;
};

inline std::string GetEndTokenInfo()
{
    return "end-of-file";
}

template<typename Lexer>
void WriteBeginRuleToLog(Lexer& lexer, const std::string& ruleName)
{
    lexer.Log()->WriteBeginRule(ruleName);
    lexer.Log()->IncIndent();
    lexer.Log()->WriteTry(lexer.RestOfLine(lexer.Log()->MaxLineLength()));
    lexer.Log()->IncIndent();
}

template<typename Lexer>
void WriteSuccessToLog(Lexer& lexer, int64_t matchPos, const std::string& ruleName)
{
    lexer.Log()->DecIndent();
    lexer.Log()->WriteSuccess(util::ToUtf8(lexer.GetToken(matchPos).ToString()));
    lexer.Log()->DecIndent();
    lexer.Log()->WriteEndRule(ruleName);
}

template<typename Lexer>
void WriteFailureToLog(Lexer& lexer, const std::string& ruleName)
{
    lexer.Log()->DecIndent();
    lexer.Log()->WriteFail();
    lexer.Log()->DecIndent();
    lexer.Log()->WriteEndRule(ruleName);
}

} // namespace soul::lexer

