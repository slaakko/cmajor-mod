// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.spg;

import std.core;
import soul.ast.common;
import soul.ast.cpp;

export namespace soul::ast::spg {

class ParamVar
{
public:
    enum class Kind { parameter, variable };
    ParamVar(Kind kind_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    virtual ~ParamVar();
    Kind GetKind() const { return kind; }
    soul::ast::cpp::TypeIdNode* Type() const { return type.get(); }
    const std::string& Name() const { return name; }
    virtual ParamVar* Clone() const = 0;
private:
    Kind kind;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> type;
    std::string name;
};

class Parameter : public ParamVar
{
public:
    Parameter(soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class Variable : public ParamVar
{
public:
    Variable(soul::ast::cpp::TypeIdNode* type_, const std::string& name_);
    ParamVar* Clone() const override;
};

class CharSet
{
    // todo
public:
    CharSet();
    CharSet* Clone() const;
};

class Visitor;

enum class ParserKind
{
    alternativeParser, sequenceParser, differenceParser, listParser, lookaheadParser, kleeneParser, positiveParser, optionalParser, expectationParser, actionParser,
    nonterminalParser, emptyParser, anyParser, tokenParser, charParser, stringParser, charSetParser, groupingParser, ruleParser, grammarParser
};

class Parser
{
public:
    Parser(ParserKind kind_);
    virtual ~Parser();
    virtual Parser* Clone() const = 0;
    virtual void Accept(Visitor& visitor) = 0;
    virtual bool IsTokenSwitch() const { return false; }
    virtual bool IsActionToken() const { return false; }
    virtual std::string Name() const = 0;
    ParserKind Kind() const { return kind; }
    bool IsNonterminalParser() const { return kind == ParserKind::nonterminalParser; }
    bool IsTokenParser() const { return kind == ParserKind::tokenParser; }
private:
    ParserKind kind;
};

class UnaryParser : public Parser
{
public:
    UnaryParser(ParserKind kind_, Parser* child_);
    Parser* Child() const { return child.get(); }
private:
    std::unique_ptr<Parser> child;
};

class BinaryParser : public Parser
{
public:
    BinaryParser(ParserKind kind_, Parser* left_, Parser* right_);
    Parser* Left() const { return left.get(); }
    Parser* Right() const { return right.get(); }
private:
    std::unique_ptr<Parser> left;
    std::unique_ptr<Parser> right;
};

class AlternativeParser : public BinaryParser
{
public:
    AlternativeParser(Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "alternative"; }
    bool IsTokenSwitch() const override { return Left()->IsActionToken() && Right()->IsActionToken() || Left()->IsTokenSwitch() && Right()->IsActionToken(); }
};

class SequenceParser : public BinaryParser
{
public:
    SequenceParser(Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "sequence"; }
};

class DifferenceParser : public BinaryParser
{
public:
    DifferenceParser(Parser* left_, Parser* right_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "difference"; }
};

class ListParser : public UnaryParser
{
public:
    ListParser(Parser* left_, Parser* right_);
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
    LookaheadParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "lookahead"; }
};

class KleeneParser : public UnaryParser
{
public:
    KleeneParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "kleene"; }
};

class PositiveParser : public UnaryParser
{
public:
    PositiveParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "positive"; }
};

class OptionalParser : public UnaryParser
{
public:
    OptionalParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "optional"; }
};

class ExpectationParser : public UnaryParser
{
public:
    ExpectationParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "expectation"; }
};

class ActionParser : public UnaryParser
{
public:
    ActionParser(Parser* child_, soul::ast::cpp::CompoundStatementNode* successCode_, soul::ast::cpp::CompoundStatementNode* failureCode_);
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
    NonterminalParser(const std::string& ruleName_, const std::string& instanceName_, soul::ast::cpp::ExprListNode* args_);
    const std::string& RuleName() const { return ruleName; }
    const std::string& InstanceName() const { return instanceName; }
    const std::unique_ptr<soul::ast::cpp::ExprListNode>& Arguments() const { return arguments; }
    void SetRule(RuleParser* rule_) { rule = rule_; }
    RuleParser* Rule() const { return rule; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "nonterminal"; }
private:
    std::string ruleName;
    std::string instanceName;
    std::unique_ptr<soul::ast::cpp::ExprListNode> arguments;
    RuleParser* rule;
};

class EmptyParser : public Parser
{
public:
    EmptyParser();
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "empty"; }
};

class AnyParser : public Parser
{
public:
    AnyParser();
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "any"; }
};

class TokenParser : public Parser
{
public:
    TokenParser(const std::string& tokenName_);
    const std::string& TokenName() const { return tokenName; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "token"; }
private:
    std::string tokenName;
};

class CharParser : public Parser
{
public:
    CharParser(char32_t chr_);
    char32_t Chr() const { return chr; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "char"; }
private:
    char32_t chr;
};

class StringParser : public Parser
{
public:
    StringParser(const std::u32string& str_);
    const std::u32string& Str() const { return str; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "string"; }
private:
    std::u32string str;
};

class CharSetParser : public Parser
{
public:
    CharSetParser(CharSet* charSet_);
    CharSet* GetCharSet() const { return charSet.get(); }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "charSet"; }
private:
    std::unique_ptr<CharSet> charSet;
};

class GroupingParser : public UnaryParser
{
public:
    GroupingParser(Parser* child_);
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
    std::string Name() const override { return "grouping"; }
};

class GrammarParser;

class RuleParser : public Parser
{
public:
    RuleParser(const std::string& name_);
    std::string Name() const override { return name; }
    void SetId(int id_) { id = id_; }
    int Id() const { return id; }
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
private:
    std::string name;
    int id;
    std::string info;
    std::vector<std::unique_ptr<Parameter>> params;
    std::vector<std::unique_ptr<Variable>> vars;
    std::unique_ptr<Parser> definition;
    std::unique_ptr<soul::ast::cpp::TypeIdNode> returnType;
    std::vector<NonterminalParser*> nonterminals;
    GrammarParser* grammar;
    bool hasReturn;
};

class GrammarParser : public Parser
{
public:
    GrammarParser(const std::string& name_);
    std::string Name() const override { return name; }
    bool Main() const { return main; }
    void SetMain() { main = true; }
    void AddLexer(soul::ast::cpp::TypeIdNode* lexerTypeId);
    const std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>>& Lexers() const { return lexers; }
    void AddUsing(const std::string& parserRuleId);
    const std::vector<std::string>& Usings() const { return usings; }
    void AddRule(RuleParser* rule);
    void MapRule(RuleParser* rule);
    RuleParser* GetRule(const std::string& ruleName) const;
    const std::vector<std::unique_ptr<RuleParser>>& Rules() const { return rules; }
    Parser* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::string name;
    bool main;
    std::vector<std::unique_ptr<soul::ast::cpp::TypeIdNode>> lexers;
    std::vector<std::string> usings;
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
    SpgFileDeclaration(FileKind fileKind_, const std::string& filePath_);
    virtual ~SpgFileDeclaration();
    FileKind GetFileKind() const { return fileKind; }
    const std::string& FilePath() const { return filePath; }
private:
    FileKind fileKind;
    std::string filePath;
};

class ParserFileDeclaration : public SpgFileDeclaration
{
public:
    ParserFileDeclaration(const std::string& filePath_, bool external_);
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
    void AddParser(GrammarParser* parser);
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
    virtual void Visit(AlternativeParser& parser) {}
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
    virtual void Visit(GroupingParser& parser) {}
    virtual void Visit(RuleParser& parser) {}
    virtual void Visit(GrammarParser& parser) {}
    virtual void Visit(ParserFile& parserFile) {}
    virtual void Visit(SpgFile& spgFile) {}
};

class DefaultVisitor : public Visitor
{
public:
    void Visit(AlternativeParser& parser) override;
    void Visit(SequenceParser& parser) override;
    void Visit(DifferenceParser& parser) override;
    void Visit(ListParser& parser) override;
    void Visit(LookaheadParser& parser) override;
    void Visit(KleeneParser& parser) override;
    void Visit(PositiveParser& parser) override;
    void Visit(OptionalParser& parser) override;
    void Visit(ExpectationParser& parser) override;
    void Visit(ActionParser& parser) override;
    void Visit(GroupingParser& parser) override;
    void Visit(RuleParser& parser) override;
    void Visit(GrammarParser& parser) override;
    void Visit(ParserFile& parserFile) override;
    void Visit(SpgFile& spgFile) override;
};

} // namespace soul::ast::spg
