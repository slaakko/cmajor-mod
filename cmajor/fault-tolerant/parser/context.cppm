// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.context;

import cmajor.fault.tolerant.ast;
import std.core;

export namespace cmajor::fault::tolerant::parser {

class Context
{
public:
    Context();
    void SetCompileUnit(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit_);
    cmajor::fault::tolerant::ast::CompileUnitNode* CompileUnit() const { return compileUnit; }
    void BeginParsingTemplateId();
    void EndParsingTemplateId();
    bool ParsingTemplateId() const { return parsingTemplateId; }
    void BeginParsingConcept();
    void EndParsingConcept();
    bool ParsingConcept() const { return parsingConcept; }
    void PushParsingLvalue(bool enable);
    void PopParsingLvalue();
    bool ParsingLvalue() const { return parsingLvalue; }
    void PushParsingExpressionStatement(bool enable);
    void PopParsingExpressionStatement();
    bool ParsingExpressionStatement() const { return parsingExpressionStatement; }
    void BeginParsingArguments();
    void EndParsingArguments();
    bool ParsingArguments() const { return parsingArguments; }
    void PushParsingIsOrAs(bool enable);
    void PopParsingIsOrAs();
    bool ParsingIsOrAs() const { return parsingIsOrAs; }
private:
    cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit;
    bool parsingTemplateId;
    std::stack<bool> parsingTemplateIdStack;
    bool parsingConcept;
    std::stack<bool> parsingConceptStack;
    bool parsingLvalue;
    std::stack<bool> parsingLvalueStack;
    bool parsingExpressionStatement;
    std::stack<bool> parsingExpressionStatementStack;
    bool parsingArguments;
    std::stack<bool> parsingArgumentsStack;
    bool parsingIsOrAs;
    std::stack<bool> parsingIsOrAsStack;
};

} // namespace cmajor::fault::tolerant::parser
