// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.context;

namespace cmajor::fault::tolerant::parser {

Context::Context() : 
    compileUnit(nullptr), 
    parsingTemplateId(false), 
    parsingConcept(false), 
    parsingLvalue(false), 
    parsingExpressionStatement(false), 
    parsingArguments(false), 
    parsingIsOrAs(false)
{
}

void Context::SetCompileUnit(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit_)
{
    compileUnit = compileUnit_;
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

} // namespace cmajor::fault::tolerant::parser
