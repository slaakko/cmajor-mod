// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.spg;

import util;

namespace soul::ast::spg {

std::string ParserKindStr(ParserKind kind)
{
    switch (kind)
    {
        case ParserKind::alternativeParser:
        {
            return "alternativeParser";
        }
        case ParserKind::sequenceParser:
        {
            return "sequenceParser";
        }
        case ParserKind::differenceParser:
        {
            return "differenceParser";
        }
        case ParserKind::listParser:
        {
            return "listParser";
        }
        case ParserKind::lookaheadParser:
        {
            return "lookaheadParser";
        }
        case ParserKind::kleeneParser:
        {
            return "kleeneParser";
        }
        case ParserKind::positiveParser:
        {
            return "positiveParser";
        }
        case ParserKind::optionalParser:
        {
            return "optionalParser";
        }
        case ParserKind::expectationParser:
        {
            return "expectationParser";
        }
        case ParserKind::actionParser:
        {
            return "actionParser";
        }
        case ParserKind::nonterminalParser:
        {
            return "nonterminalParser";
        }
        case ParserKind::emptyParser:
        {
            return "emptyParser";
        }
        case ParserKind::anyParser:
        {
            return "anyParser";
        }
        case ParserKind::tokenParser:
        {
            return "tokenParser";
        }
        case ParserKind::charParser:
        {
            return "charParser";
        }
        case ParserKind::stringParser:
        {
            return "stringParser";
        }
        case ParserKind::charSetParser:
        {
            return "charSetParser";
        }
        case ParserKind::groupParser:
        {
            return "groupParser";
        }
        case ParserKind::switchParser:
        {
            return "switchParser";
        }
        case ParserKind::caseParser:
        {
            return "caseParser";
        }
        case ParserKind::ruleParser:
        {
            return "ruleParser";
        }
        case ParserKind::grammarParser:
        {
            return "grammarParser";
        }
    }
    return "<unknown parser kind>";
}

void ThrowCannotBeOptimized()
{
    throw std::runtime_error("grammars that contain character, string or character set parsers cannot be optimized currently");
}

ParamVar::ParamVar(const soul::ast::SourcePos& sourcePos_, ParamVarKind kind_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_) :
    sourcePos(sourcePos_), kind(kind_), type(type_), name(name_)
{
}

ParamVar::~ParamVar()
{
}

Parameter::Parameter(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_) : 
    ParamVar(sourcePos_, ParamVarKind::parameter, type_, name_)
{
}

ParamVar* Parameter::Clone() const
{
    return new Parameter(GetSourcePos(), static_cast<soul::ast::cpp::TypeIdNode*>(Type()->Clone()), Name());
}

Variable::Variable(const soul::ast::SourcePos& sourcePos_, soul::ast::cpp::TypeIdNode* type_, const std::string& name_) : 
    ParamVar(sourcePos_, ParamVarKind::variable, type_, name_)
{
}

ParamVar* Variable::Clone() const
{
    return new Variable(GetSourcePos(),static_cast<soul::ast::cpp::TypeIdNode*>(Type()->Clone()), Name());
}

CharSet::CharSet() : inverse(false)
{
}

void CharSet::AddRange(const Range& range)
{
    ranges.push_back(range);
}

CharSet* CharSet::Clone() const
{
    CharSet* clone = new CharSet();
    if (inverse)
    {
        clone->SetInverse();
    }
    for (const auto& range : ranges)
    {
        clone->AddRange(range);
    }
    return clone;
}

TokenSet::TokenSet()
{
}

bool TokenSet::AddToken(const std::string& token)
{
    auto it = tokens.find(token);
    if (it != tokens.cend())
    {
        return false;
    }
    else
    {
        tokens.insert(token);
        return true;
    }
}

bool TokenSet::Merge(const TokenSet& that)
{
    bool changed = false;
    for (const auto& token : that.tokens)
    {
        if (AddToken(token))
        {
            changed = true;
        }
    }
    return changed;
}

bool TokenSet::Contains(const std::string& token) const
{
    return tokens.find(token) != tokens.cend();
}

bool TokenSet::Intersects(const TokenSet& that) const
{
    for (const auto& token : that.tokens)
    {
        if (Contains(token))
        {
            return true;
        }
    }
    return false;
}

std::string TokenSet::ToString() const
{
    std::string set("{");
    bool first = true;
    for (const auto& token : tokens)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            set.append(", ");
        }
        set.append(token);
    }
    set.append("}");
    return set;
}

Parser::Parser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_) : parent(nullptr), sourcePos(sourcePos_), kind(kind_)
{
}

Parser::~Parser()
{
}

UnaryParser::UnaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* child_) : Parser(sourcePos_, kind_), child(child_)
{
    child->SetParent(this);
}

void UnaryParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        child->ComputeFirst(changed, visited);
        if (First().Merge(child->First()))
        {
            changed = true;
        }
    }
}

BinaryParser::BinaryParser(const soul::ast::SourcePos& sourcePos_, ParserKind kind_, Parser* left_, Parser* right_) : Parser(sourcePos_, kind_), left(left_), right(right_)
{
    left->SetParent(this);
    right->SetParent(this);
}

ChoiceParser::ChoiceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) :
    BinaryParser(sourcePos_, ParserKind::alternativeParser, left_, right_), optimize(false)
{
}

Parser* ChoiceParser::Clone() const
{
    return new ChoiceParser(GetSourcePos(), Left()->Clone(), Right()->Clone());
}

void ChoiceParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ChoiceParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        Left()->ComputeFirst(changed, visited);
        Right()->ComputeFirst(changed, visited);
        if (First().Merge(Left()->First()))
        {
            changed = true;
        }
        if (First().Merge(Right()->First()))
        {
            changed = true;
        }
    }
}

void ChoiceParser::SetOptimizationFlag(int& count)
{
    Parser* parent = Parent();
    while (parent)
    {
        if (parent->IsDifferenceParser())
        {
            return;
        }
        parent = parent->Parent();
    }
    if (!First().Contains("*"))
    {
        if (!Left()->First().Contains("#") && !Right()->First().Contains("#") &&!Left()->First().Contains("*") && !Right()->First().Contains("*"))
        {
            if (!Left()->First().Intersects(Right()->First()))
            {
                optimize = true;
                ++count;
            }
        }
    }
}

CaseParser::CaseParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::caseParser, child_)
{
}

Parser* CaseParser::Clone() const
{
    CaseParser* clone = new CaseParser(GetSourcePos(), Child()->Clone());
    return clone;
}

void CaseParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SwitchParser::SwitchParser(const soul::ast::SourcePos& sourcePos_) : Parser(sourcePos_, ParserKind::switchParser)
{
}

Parser* SwitchParser::Clone() const
{
    SwitchParser* clone = new SwitchParser(GetSourcePos());
    for (const auto& caseParser : caseParsers)
    {
        clone->AddCaseParser(static_cast<CaseParser*>(caseParser->Clone()));
    }
    return clone;
}

void SwitchParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SwitchParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        for (const auto& caseParser : caseParsers)
        {
            caseParser->ComputeFirst(changed, visited);
            if (First().Merge(caseParser->First()))
            {
                changed = true;
            }
        }
    }
}

void SwitchParser::AddCaseParser(CaseParser* caseParser)
{
    caseParsers.push_back(std::unique_ptr<CaseParser>(caseParser));
}

SequenceParser::SequenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) : BinaryParser(sourcePos_, ParserKind::sequenceParser, left_, right_)
{
}

Parser* SequenceParser::Clone() const
{
    return new SequenceParser(GetSourcePos(), Left()->Clone(), Right()->Clone());
}

void SequenceParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SequenceParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        Left()->ComputeFirst(changed, visited);
        Right()->ComputeFirst(changed, visited);
        if (First().Merge(Left()->First()))
        {
            changed = true;
        }
        if (First().Contains("#"))
        {
            if (First().Merge(Right()->First()))
            {
                changed = true;
            }
        }
    }
}

DifferenceParser::DifferenceParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) : BinaryParser(sourcePos_, ParserKind::differenceParser, left_, right_)
{
}

Parser* DifferenceParser::Clone() const
{
    return new DifferenceParser(GetSourcePos(), Left()->Clone(), Right()->Clone());
}

void DifferenceParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DifferenceParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        Left()->ComputeFirst(changed, visited);
        if (First().Merge(Left()->First()))
        {
            changed = true;
        }
    }
}

ListParser::ListParser(const soul::ast::SourcePos& sourcePos_, Parser* left_, Parser* right_) :
    UnaryParser(sourcePos_, ParserKind::listParser, 
        new SequenceParser(sourcePos_, left_, 
            new KleeneParser(sourcePos_, 
                new SequenceParser(sourcePos_, right_, left_->Clone())))), 
    left(left_), right(right_)
{
}

Parser* ListParser::Clone() const
{
    return new ListParser(GetSourcePos(), left->Clone(), right->Clone());
}

void ListParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LookaheadParser::LookaheadParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::lookaheadParser, child_)
{
}

Parser* LookaheadParser::Clone() const
{
    return new LookaheadParser(GetSourcePos(), Child()->Clone());
}

void LookaheadParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

KleeneParser::KleeneParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::kleeneParser, child_)
{
}

Parser* KleeneParser::Clone() const
{
    return new KleeneParser(GetSourcePos(), Child()->Clone());
}

void KleeneParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void KleeneParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        Child()->ComputeFirst(changed, visited);
        if (First().Merge(Child()->First()))
        {
            changed = true;
        }
        bool epsilonAdded = First().AddToken("#");
        if (epsilonAdded)
        {
            changed = true;
        }
    }
}

PositiveParser::PositiveParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::positiveParser, child_)
{
}

Parser* PositiveParser::Clone() const
{
    return new PositiveParser(GetSourcePos(), Child()->Clone());
}

void PositiveParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

OptionalParser::OptionalParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::optionalParser, child_)
{
}

Parser* OptionalParser::Clone() const
{
    return new OptionalParser(GetSourcePos(), Child()->Clone());
}

void OptionalParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void OptionalParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        Child()->ComputeFirst(changed, visited);
        if (First().Merge(Child()->First()))
        {
            changed = true;
        }
        bool epsilonAdded = First().AddToken("#");
        if (epsilonAdded)
        {
            changed = true;
        }
    }
}

ExpectationParser::ExpectationParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::expectationParser, child_)
{
}

Parser* ExpectationParser::Clone() const
{
    return new ExpectationParser(GetSourcePos(), Child()->Clone());
}

void ExpectationParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ActionParser::ActionParser(const soul::ast::SourcePos& sourcePos_, Parser* child_, soul::ast::cpp::CompoundStatementNode* successCode_, soul::ast::cpp::CompoundStatementNode* failureCode_) :
    UnaryParser(sourcePos_, ParserKind::actionParser, child_), successCode(successCode_), failureCode(failureCode_)
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
    return new ActionParser(GetSourcePos(), Child()->Clone(), clonedSuccessCode, clonedFailureCode);
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

NonterminalParser::NonterminalParser(const soul::ast::SourcePos& sourcePos_, const std::string& ruleName_, const std::string& instanceName_, soul::ast::cpp::ExprListNode* args_) :
    Parser(sourcePos_, ParserKind::nonterminalParser), ruleName(ruleName_), instanceName(instanceName_), arguments(args_), rule(nullptr), recursive(false)
{
}

Parser* NonterminalParser::Clone() const
{
    soul::ast::cpp::ExprListNode* clonedArgs = nullptr;
    if (arguments)
    {
        clonedArgs = static_cast<soul::ast::cpp::ExprListNode*>(arguments->Clone());
    }
    return new NonterminalParser(GetSourcePos(), ruleName, instanceName, clonedArgs);
}

void NonterminalParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NonterminalParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        rule->ComputeFirst(changed, visited);
        if (First().Merge(rule->First()))
        {
            changed = true;
        }
    }
}

EmptyParser::EmptyParser(const soul::ast::SourcePos& sourcePos_) : Parser(sourcePos_, ParserKind::emptyParser)
{
}

Parser* EmptyParser::Clone() const
{
    return new EmptyParser(GetSourcePos());
}

void EmptyParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EmptyParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        if (First().AddToken("#"))
        {
            changed = true;
        }
    }
}

AnyParser::AnyParser(const soul::ast::SourcePos& sourcePos_) : Parser(sourcePos_, ParserKind::anyParser)
{
}

Parser* AnyParser::Clone() const
{
    return new AnyParser(GetSourcePos());
}

void AnyParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AnyParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        if (First().AddToken("*"))
        {
            changed = true;
        }
    }
}

TokenParser::TokenParser(const soul::ast::SourcePos& sourcePos_, const std::string& tokenName_) : Parser(sourcePos_, ParserKind::tokenParser), tokenName(tokenName_)
{
}

Parser* TokenParser::Clone() const
{
    return new TokenParser(GetSourcePos(), tokenName);
}

void TokenParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TokenParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        if (First().AddToken(tokenName))
        {
            changed = true;
        }
    }
}

CharParser::CharParser(const soul::ast::SourcePos& sourcePos_, char32_t chr_) : Parser(sourcePos_, ParserKind::charParser), chr(chr_)
{
}

Parser* CharParser::Clone() const
{
    return new CharParser(GetSourcePos(), chr);
}

void CharParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CharParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    ThrowCannotBeOptimized();
}

StringParser::StringParser(const soul::ast::SourcePos& sourcePos_, const std::u32string& str_) : Parser(sourcePos_, ParserKind::stringParser), str(str_)
{
}

void StringParser::SetArrayName(const std::string& arrayName_)
{
    arrayName = arrayName_;
}

Parser* StringParser::Clone() const
{
    return new StringParser(GetSourcePos(), str);
}

void StringParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StringParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    ThrowCannotBeOptimized();
}

CharSetParser::CharSetParser(const soul::ast::SourcePos& sourcePos_, CharSet* charSet_) : Parser(sourcePos_, ParserKind::charSetParser), charSet(charSet_)
{
}

void CharSetParser::SetArrayName(const std::string& arrayName_)
{
    arrayName = arrayName_;
}

Parser* CharSetParser::Clone() const
{
    return new CharSetParser(GetSourcePos(), charSet->Clone());
}

void CharSetParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    ThrowCannotBeOptimized();
}

void CharSetParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GroupParser::GroupParser(const soul::ast::SourcePos& sourcePos_, Parser* child_) : UnaryParser(sourcePos_, ParserKind::groupParser, child_)
{
}

Parser* GroupParser::Clone() const
{
    return new GroupParser(GetSourcePos(), Child()->Clone());
}

void GroupParser::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RuleParser::RuleParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : 
    Parser(sourcePos_, ParserKind::ruleParser), name(name_), id(-1), index(-1), grammar(nullptr), hasReturn(false), computingFirst(false)
{
}

void RuleParser::SetInfo(const std::string& info_)
{
    info = info_;
}

void RuleParser::AddParamOrVariable(ParamVar* paramVar)
{
    if (paramVar->Kind() == ParamVarKind::parameter)
    {
        params.push_back(std::unique_ptr<Parameter>(static_cast<Parameter*>(paramVar)));
    }
    else if (paramVar->Kind() == ParamVarKind::variable)
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
    RuleParser* clone = new RuleParser(GetSourcePos(), name);
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
    for (auto nonterminal : nonterminals)
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

void RuleParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        definition->ComputeFirst(changed, visited);
        if (First().Merge(definition->First()))
        {
            changed = true;
        }
    }
}

Using::Using(const soul::ast::SourcePos& sourcePos_, const std::string& parserRuleId_) : sourcePos(sourcePos_), parserRuleId(parserRuleId_)
{
}

GrammarParser::GrammarParser(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : 
    Parser(sourcePos_, ParserKind::grammarParser), name(name_), main(false), id(-1), parserFile(nullptr)
{
}

void GrammarParser::AddLexer(soul::ast::cpp::TypeIdNode* lexerTypeId)
{
    lexers.push_back(std::unique_ptr< soul::ast::cpp::TypeIdNode>(lexerTypeId));
}

void GrammarParser::AddUsing(const soul::ast::SourcePos& sourcePos, const std::string& parserRuleId)
{
    usings.push_back(Using(sourcePos, parserRuleId));
}

bool GrammarParser::AddRule(RuleParser* rule)
{
    rule->SetGrammar(this);
    rule->SetIndex(rules.size());
    rules.push_back(std::unique_ptr<RuleParser>(rule));
    return MapRule(rule);
}

bool GrammarParser::MapRule(RuleParser* rule)
{
    auto it = ruleMap.find(rule->Name());
    if (it != ruleMap.cend())
    {
        return false;
    }
    ruleMap[rule->Name()] = rule;
    return true;
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
        return nullptr;
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

void GrammarParser::ComputeFirst(bool& changed, std::set<Parser*>& visited)
{
    if (visited.find(this) == visited.cend())
    {
        visited.insert(this);
        for (const auto& rule : rules)
        {
            rule->ComputeFirst(changed, visited);
        }
    }
}

File::File(FileKind kind_, const std::string& filePath_) : kind(kind_), filePath(filePath_)
{
}

SpgFileDeclaration::SpgFileDeclaration(const soul::ast::SourcePos& sourcePos_, FileKind fileKind_, const std::string& filePath_) : 
    sourcePos(sourcePos_), fileKind(fileKind_), filePath(filePath_)
{
}

SpgFileDeclaration::~SpgFileDeclaration()
{
}

ParserFileDeclaration::ParserFileDeclaration(const soul::ast::SourcePos& sourcePos_, const std::string& filePath_, bool external_) : 
    SpgFileDeclaration(sourcePos_, FileKind::parserFile, filePath_), external(external_)
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
    parser->SetParserFile(this);
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
    rules.push_back(rule);
}

void SpgFile::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

bool SpgFile::AddParser(GrammarParser* parser)
{
    auto it = parserMap.find(parser->Name());
    if (it != parserMap.cend())
    {
        return false;
    }
    parserMap[parser->Name()] = parser;
    return true;
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
        return nullptr;
    }
}

void DefaultVisitor::Visit(ChoiceParser& parser)
{
    parser.Left()->Accept(*this);
    parser.Right()->Accept(*this);
}

void DefaultVisitor::Visit(SwitchParser& parser)
{
    for (const auto& caseParser : parser.CaseParsers())
    {
        caseParser->Accept(*this);
    }
}

void DefaultVisitor::Visit(CaseParser& parser)
{
    parser.Child()->Accept(*this);
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

void DefaultVisitor::Visit(GroupParser& parser)
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
