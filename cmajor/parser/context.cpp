// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.parser.context;

namespace cmajor::parser::context {

Context::Context() : 
    parsingTemplateId(false), parsingConcept(false), parsingExpressionStatement(false), parsingLvalue(false), parsingArguments(false), parsingIsOrAs(false), parsingTypeExpr(false),
    compileUnitNode(nullptr)
{
}

void Context::BeginParsingTemplateId()
{
    parsingTemplateIdStack.push(parsingTemplateId);
    parsingTemplateId = true;
}

void Context::EndParsingTemplateId()
{
    parsingTemplateId = parsingTemplateIdStack.top();
    parsingTemplateIdStack.pop();
}

void Context::BeginParsingConcept()
{
    parsingConceptStack.push(parsingConcept);
    parsingConcept = true;
}

void Context::EndParsingConcept()
{
    parsingConcept = parsingConceptStack.top();
    parsingConceptStack.pop();
}

void Context::PushParsingExpressionStatement(bool enable)
{
    parsingExpressionStatementStack.push(parsingExpressionStatement);
    parsingExpressionStatement = enable;
}

void Context::PopParsingExpressionStatement()
{
    parsingExpressionStatement = parsingExpressionStatementStack.top();
    parsingExpressionStatementStack.pop();
}

void Context::PushParsingLvalue(bool enable)
{
    parsingLvalueStack.push(parsingLvalue);
    parsingLvalue = enable;
}

void Context::PopParsingLvalue()
{
    parsingLvalue = parsingLvalueStack.top();
    parsingLvalueStack.pop();
}

void Context::BeginParsingArguments()
{
    parsingArgumentsStack.push(parsingArguments);
    parsingArguments = true;
}

void Context::EndParsingArguments()
{
    parsingArguments = parsingArgumentsStack.top();
    parsingArgumentsStack.pop();
}

void Context::PushParsingIsOrAs(bool enable)
{
    parsingIsOrAsStack.push(parsingIsOrAs);
    parsingIsOrAs = enable;
}

void Context::PopParsingIsOrAs()
{
    parsingIsOrAs = parsingIsOrAsStack.top();
    parsingIsOrAsStack.pop();
}

void Context::BeginParsingTypeExpr()
{
    parsingTypeExprStack.push(parsingTypeExpr);
    parsingTypeExpr = true;
}

void Context::EndParsingTypeExpr()
{
    parsingTypeExpr = parsingTypeExprStack.top();
    parsingTypeExprStack.pop();
}

void Context::SetCompileUnitNode(cmajor::ast::CompileUnitNode* compileUnitNode_)
{
    compileUnitNode = compileUnitNode_;
}

} // namespace cmajor::parser::context
