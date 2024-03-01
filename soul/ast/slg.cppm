// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.slg;

import std.core;
import soul.ast.common;
import soul.ast.cpp;

export namespace soul::ast::slg {

enum class FileKind
{
    tokenFile, keywordFile, expressionFile, lexerFile, slgFile
};

class File
{
public:
    File(FileKind kind_, const std::string& filePath_);
    virtual ~File();
    FileKind Kind() const { return kind; }
    const std::string& FilePath() const { return filePath; }
    bool IsExternal() const { return external; }
    void SetExternal() { external = true; }
private:
    FileKind kind;
    std::string filePath;
    bool external;
};

enum class CollectionKind
{
    tokenCollection, keywordCollection, expressionCollection, lexer
};

class Collection
{
public:
    Collection(CollectionKind kind_, const std::string& name_);
    virtual ~Collection();
    CollectionKind Kind() const { return kind; }
    const std::string& Name() const { return name; }
    void SetFile(File* file_) { file = file_; }
    File* GetFile() const { return file; }
private:
    CollectionKind kind;
    std::string name;
    File* file;
};

class Token
{
public:
    Token(int64_t id_, const std::string& name_, const std::string& info_);
    Token(const std::string& name_, const std::string& info_);
    void SetCollection(Collection* collection_) { collection = collection_; }
    Collection* GetCollection() const { return collection; }
    int64_t Id() const { return id; }
    void SetId(int64_t id_) { id = id_; }
    const std::string& Name() const { return name; }
    const std::string& Info() const { return info; }
private:
    int64_t id;
    std::string name;
    std::string info;
    Collection* collection;
};

class TokenCollection : public Collection
{
public:    
    TokenCollection(const std::string& name_);
    bool Initialized() const { return initialized; }
    void SetInitialized() { initialized = true; }
    void AddToken(Token* token);
    const std::vector<std::unique_ptr<Token>>& Tokens() const { return tokens; }
    int32_t Id() const { return id; }
    Token* GetToken(int64_t id) const;
private:
    bool initialized;
    int32_t id;
    std::vector<std::unique_ptr<Token>> tokens;
    std::map<int64_t, Token*> tokenMap;
};

class TokenFile : public File
{
public:
    TokenFile(const std::string& filePath_);
    void SetTokenCollection(TokenCollection* tokenCollection_);
    TokenCollection* GetTokenCollection() const { return tokenCollection.get(); }
private:
    std::unique_ptr<TokenCollection> tokenCollection;
};

class Keyword
{
public:
    Keyword(const std::string& str_, const std::string& tokenName_, int64_t tokenId_);
    Keyword(const std::string& str_, const std::string& tokenName_);
    void SetCollection(Collection* collection_) { collection = collection_; }
    Collection* GetCollection() const { return collection; }
    const std::string& Str() const { return str; }
    const std::string& TokenName() const { return tokenName; }
    int64_t TokenId() const { return tokenId; }
    void SetTokenId(int64_t tokenId_) { tokenId = tokenId_; }
private:
    std::string str;
    std::string tokenName;
    int64_t tokenId;
    Collection* collection;
};

class KeywordCollection : public Collection
{
public:
    KeywordCollection(const std::string& name_);
    void AddKeyword(Keyword* keyword);
    const std::vector<std::unique_ptr<Keyword>>& Keywords() const { return keywords; }
private:
    std::vector<std::unique_ptr<Keyword>> keywords;
};

class KeywordFile : public File
{
public:
    KeywordFile(const std::string& filePath_);
    void SetKeywordCollection(KeywordCollection* keywordCollection_);
    KeywordCollection* GetKeywordCollection() const { return keywordCollection.get(); }
    void AddImport(soul::ast::common::Import* imp);
    const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const { return imports; }
private:
    std::unique_ptr<KeywordCollection> keywordCollection;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
};

class Expression
{
public:
    Expression(const std::string& id_, const std::string& value_, int line_);
    void SetCollection(Collection* collection_) { collection = collection_; }
    Collection* GetCollection() const { return collection; }
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    const std::string& Id() const { return id; }
    const std::string& Value() const { return value; }
    const std::string& FileName() const;
    int Line() const { return line; }
private:
    int index;
    std::string id;
    std::string value;
    int line;
    Collection* collection;
};

class ExpressionCollection : public Collection
{
public:
    ExpressionCollection(const std::string& name_);
    void AddExpression(Expression* expression);
    const std::vector<std::unique_ptr<Expression>>& Expressions() const { return expressions; }
private:
    std::vector<std::unique_ptr<Expression>> expressions;
};

class ExpressionFile : public File
{
public:
    ExpressionFile(const std::string& filePath_);
    void SetExpressionCollection(ExpressionCollection* expressionCollection_);
    ExpressionCollection* GetExpressionCollection() const { return expressionCollection.get(); }
private:
    std::unique_ptr<ExpressionCollection> expressionCollection;
};

class Rule
{
public:
    Rule(const std::string& expr_, soul::ast::cpp::CompoundStatementNode* code_, int action_, int line_);
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    void SetCollection(Collection* collection_) { collection = collection_; }
    Collection* GetCollection() const { return collection; }
    const std::string& Expr() const { return expr; }
    soul::ast::cpp::CompoundStatementNode* Code() const { return code.get(); }
    int Action() const { return action; }
    int Line() const { return line; }
    int NfaIndex() const { return nfaIndex; }
    void SetNfaIndex(int nfaIndex_) { nfaIndex = nfaIndex_; }
private:
    int index;
    std::string expr;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> code;
    int action;
    int line;
    Collection* collection;
    int nfaIndex;
};

class Variable
{
public:
    Variable(soul::ast::cpp::TypeIdNode* type_,  const std::string& name_);
    soul::ast::cpp::TypeIdNode* Type() const { return type.get(); }
    const std::string& Name() const { return name; }
private:
    std::unique_ptr<soul::ast::cpp::TypeIdNode> type;
    std::string name;
};

class Action
{
public:
    Action(int id_, soul::ast::cpp::CompoundStatementNode* code_);
    int Id() const { return id; }
    soul::ast::cpp::CompoundStatementNode* Code() const { return code.get(); }
private:
    int id;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> code;
};

class Actions
{
public:
    void AddAction(Action* action);
    Action* GetAction(int id) const;
private:
    std::vector<std::unique_ptr<Action>> actions;
    std::map<int, Action*> actionMap;
};

class Lexer : public Collection
{
public:
    Lexer(const std::string& name_);
    void AddRule(Rule* rule);
    const std::vector<std::unique_ptr<Rule>>& Rules() const { return rules; }
    void AddVariable(Variable* variable);
    const std::vector<std::unique_ptr<Variable>>& Variables() const { return variables; }
    void AddAction(Action* action);
    const Actions& GetActions() const { return actions; }
    const std::string& VariableClassName() const { return variableClassName; }
    void SetVariableClassName(const std::string& variableClassName_);
private:
    std::vector<std::unique_ptr<Rule>> rules;
    std::vector<std::unique_ptr<Variable>> variables;
    Actions actions;
    std::string variableClassName;
};

class LexerFile : public File
{
public:
    LexerFile(const std::string& filePath_);
    void SetExportModule(soul::ast::common::ExportModule* exportModule_);
    soul::ast::common::ExportModule* GetExportModule() const { return exportModule.get(); }
    void AddImport(soul::ast::common::Import* imp);
    const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const { return imports; }
    void SetLexer(Lexer* lexer_);
    Lexer* GetLexer() const { return lexer.get(); }
private:
    std::unique_ptr<soul::ast::common::ExportModule> exportModule;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
    std::unique_ptr<Lexer> lexer;
};

enum class SlgFileDeclarationKind
{
    tokenFileDeclaration, keywordFileDeclaration, expressionFileDeclaration, lexerFileDeclararation
};

class SlgFileDeclaration
{
public:
    SlgFileDeclaration(SlgFileDeclarationKind kind_, const std::string& filePath_);
    virtual ~SlgFileDeclaration();
    SlgFileDeclarationKind Kind() const { return kind; }
    const std::string& FilePath() const { return filePath; }
private:
    SlgFileDeclarationKind kind;
    std::string filePath;
};

class TokenFileDeclaration : public SlgFileDeclaration
{
public:
    TokenFileDeclaration(const std::string& filePath_, bool external_);
    bool External() const { return external; }
private:
    bool external;
};

class KeywordFileDeclaration : public SlgFileDeclaration
{
public:
    KeywordFileDeclaration(const std::string& filePath_);
};

class ExpressionFileDeclaration : public SlgFileDeclaration
{
public:
    ExpressionFileDeclaration(const std::string& filePath_);
};

class LexerFileDeclaration : public SlgFileDeclaration
{
public:
    LexerFileDeclaration(const std::string& filePath_);
};

class SlgFile : public File
{
public:
    SlgFile(const std::string& filePath_, const std::string& projectName_);
    const std::string& ProjectName() const { return projectName; }
    void AddDeclaration(SlgFileDeclaration* declaration);
    const std::vector<std::unique_ptr<SlgFileDeclaration>>& Declarations() const { return declarations; }
    void AddTokenFile(TokenFile* tokenFile);
    const std::vector<std::unique_ptr<TokenFile>>& TokenFiles() const { return tokenFiles; }
    void AddKeywordFile(KeywordFile* keywordFile);
    const std::vector<std::unique_ptr<KeywordFile>>& KeywordFiles() const { return keywordFiles; }
    void AddExpressionFile(ExpressionFile* expressionFile);
    const std::vector<std::unique_ptr<ExpressionFile>>& ExpressionFiles() const { return expressionFiles; }
    void AddLexerFile(LexerFile* lexerFile);
    const std::vector<std::unique_ptr<LexerFile>>& LexerFiles() const { return lexerFiles; }
    const std::vector<Collection*>& Collections() const { return collections; }
    Collection* GetCollection(const std::string& name) const;
private:
    std::string projectName;
    std::vector<std::unique_ptr<SlgFileDeclaration>> declarations;
    std::vector<std::unique_ptr<TokenFile>> tokenFiles;
    std::vector<std::unique_ptr<KeywordFile>> keywordFiles;
    std::vector<std::unique_ptr<ExpressionFile>> expressionFiles;
    std::vector<std::unique_ptr<LexerFile>> lexerFiles;
    std::vector<Collection*> collections;
    std::map<std::string, Collection*> collectionMap;
};

class Tokens
{
public:
    Tokens();
    void AddToken(Token* token);
    const std::vector<Token*>& GetTokens() const { return tokens; }
private:
    std::vector<Token*> tokens;
};

class Keywords
{
public:
    Keywords();
    void AddKeyword(Keyword* keyword);
    const std::vector<Keyword*>& GetKeywords() const { return keywords; }
private:
    std::vector<Keyword*> keywords;
};

class Expressions
{
public:
    Expressions();
    void AddExpression(Expression* expression);
    Expression* GetExpression(const std::string& id) const;
private:
    std::vector<Expression*> expressions;
    std::map<std::string, Expression*> expressionMap;
};

} // namespace soul::ast::slg
