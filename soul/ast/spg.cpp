// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.spg;

import util;

namespace soul::ast::spg {

ParamVar::ParamVar(Kind kind_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_) : kind(kind_), type(type_), name(name_)
{
}

ParamVar::~ParamVar()
{
}

Parameter::Parameter(soul::ast::cpp::TypeIdNode* type_, const std::string& name_) : ParamVar(ParamVar::Kind::parameter, type_, name_)
{
}

ParamVar* Parameter::Clone() const
{
    return new Parameter(static_cast<soul::ast::cpp::TypeIdNode*>(Type()->Clone()), Name());
}

Variable::Variable(soul::ast::cpp::TypeIdNode* type_, const std::string& name_) : ParamVar(ParamVar::Kind::variable, type_, name_)
{
}

ParamVar* Variable::Clone() const
{
    return new Variable(static_cast<soul::ast::cpp::TypeIdNode*>(Type()->Clone()), Name());
}

CharSet::CharSet() 
{
    // todo
}

CharSet* CharSet::Clone() const
{
    return new CharSet(); // todo
}

Parser::Parser(ParserKind kind_) : kind(kind_)
{
}

Parser::~Parser()
{
}

UnaryParser::UnaryParser(ParserKind kind_, Parser* child_) : Parser(kind_), child(child_)
{
}

BinaryParser::BinaryParser(ParserKind kind_, Parser* left_, Parser* right_) : Parser(kind_), left(left_), right(right_)
{
}

AlternativeParser::AlternativeParser(Parser* left_, Parser* right_) : BinaryParser(ParserKind::alternativeParser, left_, right_)
{
}

Parser* AlternativeParser::Clone() const
{
    return new AlternativeParser(Left()->Clone(), Right()->Clone());
}

void AlternativeParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SequenceParser::SequenceParser(Parser* left_, Parser* right_) : BinaryParser(ParserKind::sequenceParser, left_, right_)
{
}

Parser* SequenceParser::Clone() const
{
    return new SequenceParser(Left()->Clone(), Right()->Clone());
}

void SequenceParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DifferenceParser::DifferenceParser(Parser* left_, Parser* right_) : BinaryParser(ParserKind::differenceParser, left_, right_)
{
}

Parser* DifferenceParser::Clone() const
{
    return new DifferenceParser(Left()->Clone(), Right()->Clone());
}

void DifferenceParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ListParser::ListParser(Parser* left_, Parser* right_) : 
    UnaryParser(ParserKind::listParser, new SequenceParser(left_, new KleeneParser(new SequenceParser(right_, left_->Clone())))), left(left_), right(right_)
{
}

Parser* ListParser::Clone() const
{
    return new ListParser(left->Clone(), right->Clone());
}

void ListParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LookaheadParser::LookaheadParser(Parser* child_) : UnaryParser(ParserKind::lookaheadParser, child_)
{
}

Parser* LookaheadParser::Clone() const
{
    return new LookaheadParser(Child()->Clone());
}

void LookaheadParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

KleeneParser::KleeneParser(Parser* child_) : UnaryParser(ParserKind::kleeneParser, child_)
{
}

Parser* KleeneParser::Clone() const
{
    return new KleeneParser(Child()->Clone());
}

void KleeneParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PositiveParser::PositiveParser(Parser* child_) : UnaryParser(ParserKind::positiveParser, child_)
{
}

Parser* PositiveParser::Clone() const
{
    return new PositiveParser(Child()->Clone());
}

void PositiveParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OptionalParser::OptionalParser(Parser* child_) : UnaryParser(ParserKind::optionalParser, child_)
{
}

Parser* OptionalParser::Clone() const
{
    return new OptionalParser(Child()->Clone());
}

void OptionalParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ExpectationParser::ExpectationParser(Parser* child_) : UnaryParser(ParserKind::expectationParser, child_)
{
}

Parser* ExpectationParser::Clone() const
{
    return new ExpectationParser(Child()->Clone());
}

void ExpectationParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ActionParser::ActionParser(Parser* child_, soul::ast::cpp::CompoundStatementNode* successCode_, soul::ast::cpp::CompoundStatementNode* failureCode_) :
    UnaryParser(ParserKind::actionParser, child_), successCode(successCode_), failureCode(failureCode_)
{
}

Parser* ActionParser::Clone() const
{
    soul::ast::cpp::CompoundStatementNode* clonedSuccessCode = static_cast<soul::ast::cpp::CompoundStatementNode*>(successCode->Clone());
    soul::ast::cpp::CompoundStatementNode* clonedFailureCode = nullptr;
    if (failureCode)
    {
        clonedFailureCode = static_cast<soul::ast::cpp::CompoundStatementNode*>(failureCode->Clone());
    }
    return new ActionParser(Child()->Clone(), clonedSuccessCode, clonedFailureCode);
}

void ActionParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

bool ActionParser::IsActionToken() const
{
    soul::ast::cpp::CodeEvaluationVisitor visitor;
    if (successCode)
    {
        successCode->Accept(visitor);
        if (!visitor.HasReturn()) return false;
        if (visitor.HasPass()) return false;
    }
    return Child()->IsTokenParser();
}

NonterminalParser::NonterminalParser(const std::string& ruleName_, const std::string& instanceName_, soul::ast::cpp::ExprListNode* args_) : 
    Parser(ParserKind::nonterminalParser), ruleName(ruleName_), instanceName(instanceName_), arguments(args_), rule(nullptr)
{
}

Parser* NonterminalParser::Clone() const
{
    soul::ast::cpp::ExprListNode* clonedArgs = nullptr;
    if (arguments)
    {
        clonedArgs = static_cast<soul::ast::cpp::ExprListNode*>(arguments->Clone());
    }
    return new NonterminalParser(ruleName, instanceName, clonedArgs);
}

void NonterminalParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EmptyParser::EmptyParser() : Parser(ParserKind::emptyParser)
{
}

Parser* EmptyParser::Clone() const
{
    return new EmptyParser();
}

void EmptyParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AnyParser::AnyParser() : Parser(ParserKind::anyParser)
{
}

Parser* AnyParser::Clone() const
{
    return new AnyParser();
}

void AnyParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TokenParser::TokenParser(const std::string& tokenName_) : Parser(ParserKind::tokenParser), tokenName(tokenName_)
{
}

Parser* TokenParser::Clone() const
{
    return new TokenParser(tokenName);
}

void TokenParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharParser::CharParser(char32_t chr_) : Parser(ParserKind::charParser), chr(chr_)
{
}

Parser* CharParser::Clone() const
{
    return new CharParser(chr);
}

void CharParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StringParser::StringParser(const std::u32string& str_) : Parser(ParserKind::stringParser), str(str)
{
}

Parser* StringParser::Clone() const
{
    return new StringParser(str);
}

void StringParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharSetParser::CharSetParser(CharSet* charSet_) : Parser(ParserKind::charSetParser), charSet(charSet_)
{
}

Parser* CharSetParser::Clone() const
{
    return new CharSetParser(charSet->Clone());
}

void CharSetParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GroupingParser::GroupingParser(Parser* child_) : UnaryParser(ParserKind::groupingParser, child_)
{
}

Parser* GroupingParser::Clone() const
{
    return new GroupingParser(Child()->Clone());
}

void GroupingParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RuleParser::RuleParser(const std::string& name_) : Parser(ParserKind::ruleParser), name(name_), id(-1), grammar(nullptr), hasReturn(false)
{
}

void RuleParser::SetInfo(const std::string& info_)
{
    info = info_;
}

void RuleParser::AddParamOrVariable(ParamVar* paramVar)
{
    if (paramVar->GetKind() == ParamVar::Kind::parameter)
    {
        params.push_back(std::unique_ptr<Parameter>(static_cast<Parameter*>(paramVar)));
    }
    else if (paramVar->GetKind() == ParamVar::Kind::variable)
    {
        vars.push_back(std::unique_ptr<Variable>(static_cast<Variable*>(paramVar)));
    }
}

void RuleParser::SetDefinition(Parser* definition_)
{
    definition.reset(definition_);
}

void RuleParser::SetReturnType(soul::ast::cpp::TypeIdNode* returnType_)
{
    returnType.reset(returnType_);
}

void RuleParser::AddNonterminal(NonterminalParser* nonterminal)
{
    nonterminals.push_back(nonterminal);
}

void RuleParser::SetGrammar(GrammarParser* grammar_)
{
    grammar = grammar_;
}

Parser* RuleParser::Clone() const
{
    RuleParser* clone = new RuleParser(name);
    clone->SetId(id);
    clone->SetInfo(info);
    for (const auto& param : params)
    {
        ParamVar* clonedParamVar = static_cast<ParamVar*>(param->Clone());
        clone->AddParamOrVariable(clonedParamVar);
    }
    for (const auto& var : vars)
    {
        ParamVar* clonedParamVar = static_cast<ParamVar*>(var->Clone());
        clone->AddParamOrVariable(clonedParamVar);
    }
    clone->SetDefinition(definition->Clone());
    if (returnType)
    {
        clone->SetReturnType(static_cast<soul::ast::cpp::TypeIdNode*>(returnType->Clone()));
    }
    for (const auto& nonterminal : nonterminals)
    {
        clone->AddNonterminal(static_cast<NonterminalParser*>(nonterminal->Clone()));
    }
    if (hasReturn)
    {
        clone->SetHasReturn();
    }
    return clone;
}

void RuleParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GrammarParser::GrammarParser(const std::string& name_) : Parser(ParserKind::grammarParser), name(name_), main(false)
{
}

void GrammarParser::AddLexer(soul::ast::cpp::TypeIdNode* lexerTypeId)
{
    lexers.push_back(std::unique_ptr< soul::ast::cpp::TypeIdNode>(lexerTypeId));
}

void GrammarParser::AddUsing(const std::string& parserRuleId)
{
    usings.push_back(parserRuleId);
}

void GrammarParser::AddRule(RuleParser* rule)
{
    rule->SetGrammar(this);
    rules.push_back(std::unique_ptr<RuleParser>(rule));
    MapRule(rule);
}

void GrammarParser::MapRule(RuleParser* rule)
{
    auto it = ruleMap.find(rule->Name());
    if (it != ruleMap.cend())
    {
        throw std::runtime_error("rule '" + rule->Name() + "' already added to parser '" + name + "'");
    }
    ruleMap[rule->Name()] = rule;
}

RuleParser* GrammarParser::GetRule(const std::string& ruleName) const
{
    auto it = ruleMap.find(ruleName);
    if (it != ruleMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("rule '" + ruleName + "' not found from parser '" + name + "'");
    }
}

Parser* GrammarParser::Clone() const
{
    throw std::runtime_error("grammar cannot be cloned");
}

void GrammarParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

File::File(FileKind kind_, const std::string& filePath_) : kind(kind_), filePath(filePath_)
{
}

SpgFileDeclaration::SpgFileDeclaration(FileKind fileKind_, const std::string& filePath_) : fileKind(fileKind_), filePath(filePath_)
{
}

SpgFileDeclaration::~SpgFileDeclaration()
{
}

ParserFileDeclaration::ParserFileDeclaration(const std::string& filePath_, bool external_) : SpgFileDeclaration(FileKind::parserFile, filePath_), external(external_)
{
}

ParserFile::ParserFile(const std::string& filePath_) : File(FileKind::parserFile, filePath_), external(false)
{
}

void ParserFile::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParserFile::SetExportModule(soul::ast::common::ExportModule* exportModule_)
{
    if (exportModule)
    {
        throw std::runtime_error("parser file '" + FilePath() + "' already has export module declaration");
    }
    exportModule.reset(exportModule_);
}

void ParserFile::AddImport(soul::ast::common::Import* imprt)
{
    imports.push_back(std::unique_ptr<soul::ast::common::Import>(imprt));
}

void ParserFile::AddParser(soul::ast::spg::GrammarParser* parser)
{
    parsers.push_back(std::unique_ptr< soul::ast::spg::GrammarParser>(parser));
}

SpgFile::SpgFile(const std::string& filePath_, const std::string& projectName_) : File(FileKind::spgFile, filePath_), projectName(projectName_)
{
}

void SpgFile::AddDeclaration(SpgFileDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<SpgFileDeclaration>(declaration));
}

void SpgFile::AddParserFile(ParserFile* parserFile)
{
    parserFiles.push_back(std::unique_ptr<ParserFile>(parserFile));
}

void SpgFile::AddRule(RuleParser* rule)
{
    rule->SetId(rules.size());
    rules.push_back(rule);
}

void SpgFile::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SpgFile::AddParser(GrammarParser* parser)
{
    auto it = parserMap.find(parser->Name());
    if (it != parserMap.cend())
    {
        throw std::runtime_error("parser '" + parser->Name() + "' already exists");
    }
    parserMap[parser->Name()] = parser;
}

GrammarParser* SpgFile::GetParser(const std::string& name) const
{
    auto it = parserMap.find(name);
    if (it != parserMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("parser '" + name + "' not found");
    }
}

void DefaultVisitor::Visit(AlternativeParser& parser)
{
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
}

void DefaultVisitor::Visit(SequenceParser& parser)
{
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
}

void DefaultVisitor::Visit(DifferenceParser& parser)
{
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
}

void DefaultVisitor::Visit(ListParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(LookaheadParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(KleeneParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(PositiveParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(OptionalParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(ExpectationParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(ActionParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(GroupingParser& parser)
{
    parser.Child()->Accept(*this);
}

void DefaultVisitor::Visit(RuleParser& parser)
{
    parser.Definition()->Accept(*this);
}

void DefaultVisitor::Visit(GrammarParser& parser)
{
    for (const auto& rule : parser.Rules())
    {
        rule->Accept(*this);
    }
}

void DefaultVisitor::Visit(ParserFile& parserFile)
{
    for (const auto& parser : parserFile.Parsers())
    {
        parser->Accept(*this);
    }
}

void DefaultVisitor::Visit(SpgFile& spgFile)
{
    for (const auto& parserFile : spgFile.ParserFiles())
    {
        parserFile->Accept(*this);
    }
}

} // namespace soul::ast::spg
