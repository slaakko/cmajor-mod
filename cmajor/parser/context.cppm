// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.parser.context;

import std.core;
import util.uuid;
import cmajor.ast;

export namespace cmajor::parser::context {

class Context
{
public:
    Context();
    const util::uuid& ModuleId() const { return moduleId; }
    void SetModuleId(const util::uuid& moduleId_) { moduleId = moduleId_; }
    void BeginParsingTemplateId();
    void EndParsingTemplateId();
    bool ParsingTemplateId() const { return parsingTemplateId; }
    void BeginParsingConcept();
    void EndParsingConcept();
    bool ParsingConcept() const { return parsingConcept; }
    void PushParsingExpressionStatement(bool enable);
    void PopParsingExpressionStatement();
    bool ParsingExpressionStatement() const { return parsingExpressionStatement; }
    void PushParsingLvalue(bool enable);
    void PopParsingLvalue();
    bool ParsingLvalue() const { return parsingLvalue; }
    void BeginParsingArguments();
    void EndParsingArguments();
    bool ParsingArguments() const { return parsingArguments; }
    void PushParsingIsOrAs(bool enable);
    void PopParsingIsOrAs();
    bool ParsingIsOrAs() const { return parsingIsOrAs; }
    void BeginParsingTypeExpr();
    void EndParsingTypeExpr();
    bool ParsingTypeExpr() const { return parsingTypeExpr; }
    cmajor::ast::CompileUnitNode* CompileUnitNode() const { return compileUnitNode; }
    void SetCompileUnitNode(cmajor::ast::CompileUnitNode* compileUnitNode_);
private:
    util::uuid moduleId;
    bool parsingTemplateId;
    std::stack<bool> parsingTemplateIdStack;
    bool parsingConcept;
    std::stack<bool> parsingConceptStack;
    bool parsingExpressionStatement;
    std::stack<bool> parsingExpressionStatementStack;
    bool parsingLvalue;
    std::stack<bool> parsingLvalueStack;
    bool parsingArguments;
    std::stack<bool> parsingArgumentsStack;
    bool parsingIsOrAs;
    std::stack<bool> parsingIsOrAsStack;
    bool parsingTypeExpr;
    std::stack<bool> parsingTypeExprStack;
    cmajor::ast::CompileUnitNode* compileUnitNode;
};

} // namespace cmajor::parser::context
