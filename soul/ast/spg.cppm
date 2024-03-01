// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.spg;

import std.core;
import soul.ast.common;
import soul.ast.cpp;
import soul.ast.source.pos;

export namespace soul::ast::spg {

struct Range
{
    int32_t first;
    int32_t last;
};

enum class ParamVarKind
{
    parameter, variable
};

class ParamVar
{
public:
    ParamVar(const soul::ast::SourcePos& sourcePos_, ParamVarKind kind_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    virtual ~ParamVar();
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    ParamVarKind Kind() const { return kind; }
    soul::ast::cpp::TypeIdNode* Type() const { return type.get(); }
    const std::string& Name() const { return name; }
    virtual ParamVar* Clone() const = 0;
private:
    soul::ast::SourcePos sourcePos;
    ParamVarKind kind;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> type;
    std::string name;
};

class Parameter : public ParamVar
{
public:
    Parameter(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class Variable : public ParamVar
{
public:
    Variable(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class CharSet
{
public:
    CharSet();
    CharSet* Clone() const;
    bool Inverse() const { return inverse; }
    void SetInverse() { inverse = true; }
    const std::vector<Range>& Ranges() const { return ranges; }
    void AddRange(const Range& range);
private:
    bool inverse;
    std::vector<Range> ranges;
};

class TokenSet
{
public:
    TokenSet();
    bool AddToken(const std::string& token);
    bool Merge(const TokenSet& that);
    bool Contains(const std::string& token) const;
    bool Intersects(const TokenSet& that) const;
    std::string ToString() const;
    const std::set<std::string>& Tokens() const { return tokens; }
private:
    std::set<std::string> tokens;
};

class Visitor;

enum class ParserKind
{
    alternativeParser, sequenceParser, differenceParser, listParser, lookaheadParser, kleeneParser, positiveParser, optionalParser, expectationParser, actionParser,
    nonterminalParser, emptyParser, anyParser, tokenParser, charParser, stringParser, charSetParser, groupParser, switchParser, caseParser, ruleParser, grammarParser
};

std::string ParserKindStr(ParserKind kind);

class Parser
{
public:
    Parser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_);
    Parser* Parent() const { return parent; }
    void SetParent(Parser* parent_) { parent = parent_; }
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    virtual ~Parser();
    virtual Parser* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual bool IsTokenSwitch() const { return false; }
    virtual bool IsActionToken() const { return false; }
    virtual std::string Name() const = 0;
    virtual void ComputeFirst(bool& changed, std::set<Parser*>& visited) = 0;
    ParserKind Kind() const { return kind; }
    bool IsNonterminalParser() const { return kind == ParserKind::nonterminalParser; }
    bool IsTokenParser() const { return kind == ParserKind::tokenParser; }
    bool IsListParser() const { return kind == ParserKind::listParser; }
    bool IsSwitchParser() const { return kind == ParserKind::switchParser; }
    bool IsDifferenceParser() const { return kind == ParserKind::differenceParser; }
    const TokenSet& First() const { return first; }
    TokenSet& First() { return first; }
private:
    Parser* parent;
    soul::ast::SourcePos sourcePos;
    ParserKind kind;
    TokenSet first;
};

class UnaryParser : public Parser
{
public:
    UnaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* child_);
    Parser* Child() const { return child.get(); }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::unique_ptr<Parser> child;
};

class BinaryParser : public Parser
{
public:
    BinaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* left_, Parser* right_);
    Parser* Left() const { return left.get(); }
    Parser* Right() const { return right.get(); }
private:
    std::unique_ptr<Parser> left;
    std::unique_ptr<Parser> right;
};

class ChoiceParser : public BinaryParser
{
public:
    ChoiceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "choice"; }
    bool IsTokenSwitch() const override { return Left()->IsActionToken() && Right()->IsActionToken() || Left()->IsTokenSwitch() && Right()->IsActionToken(); }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
    void SetOptimizationFlag(int& count);
    bool Optimize() const { return optimize; }
private:
    bool optimize;
};

class CaseParser : public UnaryParser
{
public:
    CaseParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "case"; }
};

class SwitchParser : public Parser
{
public:
    SwitchParser(const soul::ast::SourcePos& sourcePos_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "switch"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
    const std::vector<std::unique_ptr<CaseParser>>& CaseParsers() const { return caseParsers; }
    void AddCaseParser(CaseParser* caseParser);
private:
    std::vector<std::unique_ptr<CaseParser>> caseParsers;
};

class SequenceParser : public BinaryParser
{
public:
    SequenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "sequence"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class DifferenceParser : public BinaryParser
{
public:
    DifferenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "difference"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class ListParser : public UnaryParser
{
public:
    ListParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_);
    Parser* Clone() const override;
    Parser* Left() const { return left; }
    Parser* Right() const { return right; }
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "list"; }
private:
    Parser* left;
    Parser* right;
};

class LookaheadParser : public UnaryParser
{
public:
    LookaheadParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "lookahead"; }
};

class KleeneParser : public UnaryParser
{
public:
    KleeneParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "kleene"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class PositiveParser : public UnaryParser
{
public:
    PositiveParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "positive"; }
};

class OptionalParser : public UnaryParser
{
public:
    OptionalParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "optional"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class ExpectationParser : public UnaryParser
{
public:
    ExpectationParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "expectation"; }
};

class ActionParser : public UnaryParser
{
public:
    ActionParser(const soul::ast::SourcePos& sourcePos_, Parser* child_, soul::ast::cpp::CompoundStatementNode* successCode_, soul::ast::cpp::CompoundStatementNode* failureCode_);
    soul::ast::cpp::CompoundStatementNode* SuccessCode() const { return successCode.get(); }
    soul::ast::cpp::CompoundStatementNode* FailureCode() const { return failureCode.get(); }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "action"; }
    bool IsActionToken() const override;
private:
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> successCode;
    std::unique_ptr<soul::ast::cpp::CompoundStatementNode> failureCode;
};

class RuleParser;

class NonterminalParser : public Parser
{
public:
    NonterminalParser(const soul::ast::SourcePos& sourcePos_, const std::string& ruleName_, const std::string& instanceName_, soul::ast::cpp::ExprListNode* args_);
    const std::string& RuleName() const { return ruleName; }
    const std::string& InstanceName() const { return instanceName; }
    const std::unique_ptr<soul::ast::cpp::ExprListNode>& Arguments() const { return arguments; }
    void SetRule(RuleParser* rule_) { rule = rule_; }
    RuleParser* Rule() const { return rule; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "nonterminal"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string ruleName;
    std::string instanceName;
    std::unique_ptr<soul::ast::cpp::ExprListNode> arguments;
    RuleParser* rule;
    bool recursive;
};

class EmptyParser : public Parser
{
public:
    EmptyParser(const soul::ast::SourcePos& sourcePos_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "empty"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class AnyParser : public Parser
{
public:
    AnyParser(const soul::ast::SourcePos& sourcePos_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "any"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
};

class TokenParser : public Parser
{
public:
    TokenParser(const soul::ast::SourcePos& sourcePos_, const std::string& tokenName_);
    const std::string& TokenName() const { return tokenName; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "token"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string tokenName;
};

class CharParser : public Parser
{
public:
    CharParser(const soul::ast::SourcePos& sourcePos_, char32_t chr_);
    char32_t Chr() const { return chr; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "char"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    char32_t chr;
};

class StringParser : public Parser
{
public:
    StringParser(const soul::ast::SourcePos& sourcePos_, const std::u32string& str_);
    const std::u32string& Str() const { return str; }
    const std::string& ArrayName() const { return arrayName; }
    void SetArrayName(const std::string& arrayName_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "string"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::u32string str;
    std::string arrayName;
};

class CharSetParser : public Parser
{
public:
    CharSetParser(const soul::ast::SourcePos& sourcePos_, CharSet* charSet_);
    CharSet* GetCharSet() const { return charSet.get(); }
    const std::string& ArrayName() const { return arrayName; }
    void SetArrayName(const std::string& arrayName_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "charSet"; }
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::unique_ptr<CharSet> charSet;
    std::string arrayName;
};

class GroupParser : public UnaryParser
{
public:
    GroupParser(const soul::ast::SourcePos& sourcePos_, Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "group"; }
};

class GrammarParser;

class RuleParser : public Parser
{
public:
    RuleParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    std::string Name() const override { return name; }
    void SetIndex(int32_t index_) { index = index_; }
    int32_t Index() const { return index; }
    void SetId(int64_t id_) { id = id_; }
    int64_t Id() const { return id; }
    void SetInfo(const std::string& info_);
    const std::string& Info() const { return info; }
    void AddParamOrVariable(ParamVar* paramVar);
    const std::vector<std::unique_ptr<Parameter>>& Params() const { return params; }
    const std::vector<std::unique_ptr<Variable>>& Vars() const { return vars; }
    void SetDefinition(Parser* definition_);
    Parser* Definition() const { return definition.get(); }
    void SetReturnType(soul::ast::cpp::TypeIdNode* returnType_);
    soul::ast::cpp::TypeIdNode* ReturnType() const { return returnType.get(); }
    void AddNonterminal(NonterminalParser* nonterminal);
    const std::vector<NonterminalParser*>& Nonterminals() const { return nonterminals; }
    void SetGrammar(GrammarParser* grammar_);
    GrammarParser* Grammar() const { return grammar; }
    bool HasReturn() const { return hasReturn; }
    void SetHasReturn() { hasReturn = true; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string name;
    int32_t index;
    int64_t id;
    std::string info;
    std::vector<std::unique_ptr<Parameter>> params;
    std::vector<std::unique_ptr<Variable>> vars;
    std::unique_ptr<Parser> definition;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> returnType;
    std::vector<NonterminalParser*> nonterminals;
    GrammarParser* grammar;
    bool hasReturn;
    bool computingFirst;
};

struct Using
{
    Using(const soul::ast::SourcePos& sourcePos_, const std::string& parserRuleId_);
    soul::ast::SourcePos sourcePos;
    std::string parserRuleId;
};

class ParserFile;

class GrammarParser : public Parser
{
public:
    GrammarParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_);
    std::string Name() const override { return name; }
    bool Main() const { return main; }
    void SetMain() { main = true; }
    int32_t Id() const { return id; }
    void SetId(int32_t id_) { id = id_; }
    ParserFile* GetParserFile() const { return parserFile; }
    void SetParserFile(ParserFile* parserFile_) { parserFile = parserFile_; }
    void AddLexer(soul::ast::cpp::TypeIdNode* lexerTypeId);
    const std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>>& Lexers() const { return lexers; }
    void AddUsing(const soul::ast::SourcePos& sourcePos, const std::string& parserRuleId);
    const std::vector<Using>& Usings() const { return usings; }
    bool AddRule(RuleParser* rule);
    bool MapRule(RuleParser* rule);
    RuleParser* GetRule(const std::string& ruleName) const;
    const std::vector<std::unique_ptr<RuleParser>>& Rules() const { return rules; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    void ComputeFirst(bool& changed, std::set<Parser*>& visited) override;
private:
    std::string name;
    bool main;
    int32_t id;
    ParserFile* parserFile;
    std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>> lexers;
    std::vector<Using> usings;
    std::vector<std::unique_ptr<RuleParser>> rules;
    std::map<std::string, RuleParser*> ruleMap;
};

enum class FileKind
{
    spgFile, parserFile
};

class File
{
public:
    File(FileKind kind_, const std::string& filePath_);
    FileKind Kind() const { return kind; }
    const std::string& FilePath() const { return filePath; }
    virtual void Accept(Visitor& visitor) = 0;
private:
    FileKind kind;
    std::string filePath;
};

class SpgFileDeclaration
{
public:
    SpgFileDeclaration(const soul::ast::SourcePos& sourcePos_, FileKind fileKind_, const std::string& filePath_);
    virtual ~SpgFileDeclaration();
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    FileKind GetFileKind() const { return fileKind; }
    const std::string& FilePath() const { return filePath; }
private:
    soul::ast::SourcePos sourcePos;
    FileKind fileKind;
    std::string filePath;
};

class ParserFileDeclaration : public SpgFileDeclaration
{
public:
    ParserFileDeclaration(const soul::ast::SourcePos& sourcePos_, const std::string& filePath_, bool external_);
    bool External() const { return external; }
private:
    bool external;
};

class ParserFile : public File
{
public:
    ParserFile(const std::string& filePath_);
    void SetExportModule(soul::ast::common::ExportModule* exportModule_);
    soul::ast::common::ExportModule* ExportModule() const { return exportModule.get(); }
    void AddImport(soul::ast::common::Import* imprt);
    const std::vector<std::unique_ptr<soul::ast::common::Import>>& Imports() const { return imports; }
    void AddParser(soul::ast::spg::GrammarParser* parser);
    const std::vector<std::unique_ptr<soul::ast::spg::GrammarParser>>& Parsers() const { return parsers; }
    bool IsExternal() const { return external; }
    void SetExternal() { external = true; }
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<soul::ast::common::ExportModule> exportModule;
    std::vector<std::unique_ptr<soul::ast::common::Import>> imports;
    std::vector<std::unique_ptr<soul::ast::spg::GrammarParser>> parsers;
    bool external;
};

class SpgFile : public File
{
public:
    SpgFile(const std::string& filePath_, const std::string& projectName_);
    const std::string& ProjectName() const { return projectName; }
    void AddDeclaration(SpgFileDeclaration* declaration);
    const std::vector<std::unique_ptr<SpgFileDeclaration>>& Declarations() const { return declarations; }
    void AddParserFile(ParserFile* parserFile);
    const std::vector<std::unique_ptr<ParserFile>>& ParserFiles() const { return parserFiles; }
    bool AddParser(GrammarParser* parser);
    GrammarParser* GetParser(const std::string& name) const;
    void AddRule(RuleParser* rule);
    const std::vector<RuleParser*>& Rules() const { return rules; }
    void Accept(Visitor& visitor) override;
private:
    std::string projectName;
    std::vector<std::unique_ptr<SpgFileDeclaration>> declarations;
    std::vector<std::unique_ptr<ParserFile>> parserFiles;
    std::map<std::string, GrammarParser*> parserMap;
    std::vector<RuleParser*> rules;
};

class Visitor
{
public:
    virtual void Visit(ChoiceParser& parser) {}
    virtual void Visit(SwitchParser& parser) {}
    virtual void Visit(CaseParser& parser) {}
    virtual void Visit(SequenceParser& parser) {}
    virtual void Visit(DifferenceParser& parser) {}
    virtual void Visit(ListParser& parser) {}
    virtual void Visit(LookaheadParser& parser) {}
    virtual void Visit(KleeneParser& parser) {}
    virtual void Visit(PositiveParser& parser) {}
    virtual void Visit(OptionalParser& parser) {}
    virtual void Visit(ExpectationParser& parser) {}
    virtual void Visit(ActionParser& parser) {}
    virtual void Visit(NonterminalParser& parser) {}
    virtual void Visit(EmptyParser& parser) {}
    virtual void Visit(AnyParser& parser) {}
    virtual void Visit(TokenParser& parser) {}
    virtual void Visit(CharParser& parser) {}
    virtual void Visit(StringParser& parser) {}
    virtual void Visit(CharSetParser& parser) {}
    virtual void Visit(GroupParser& parser) {}
    virtual void Visit(RuleParser& parser) {}
    virtual void Visit(GrammarParser& parser) {}
    virtual void Visit(ParserFile& parserFile) {}
    virtual void Visit(SpgFile& spgFile) {}
};

class DefaultVisitor : public Visitor
{
public:
    void Visit(ChoiceParser& parser) override;
    void Visit(SwitchParser& parser) override;
    void Visit(CaseParser& parser) override;
    void Visit(SequenceParser& parser) override;
    void Visit(DifferenceParser& parser) override;
    void Visit(ListParser& parser) override;
    void Visit(LookaheadParser& parser) override;
    void Visit(KleeneParser& parser) override;
    void Visit(PositiveParser& parser) override;
    void Visit(OptionalParser& parser) override;
    void Visit(ExpectationParser& parser) override;
    void Visit(ActionParser& parser) override;
    void Visit(GroupParser& parser) override;
    void Visit(RuleParser& parser) override;
    void Visit(GrammarParser& parser) override;
    void Visit(ParserFile& parserFile) override;
    void Visit(SpgFile& spgFile) override;
};

} // namespace soul::ast::spg
