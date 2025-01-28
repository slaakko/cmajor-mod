// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.error;

import cmajor.masm.intermediate.context;
import cmajor.symbols;

namespace cmajor::masm::intermediate {

void Error(const std::string& message, const soul::ast::Span& span, Context* context)
{
    if (span.IsValid())
    {
        soul::ast::LineColLen lineColLen;
        const std::vector<int>* lineStartIndeces = context->GetFileMap().LineStartIndeces(context->FileId());
        if (lineStartIndeces)
        {
            lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
        }
        if (lineColLen.IsValid())
        {
            throw cmajor::symbols::Error(message + " at\n'" + context->FilePath() + "':" + std::to_string(lineColLen.line) + ":\n" +
                context->ErrorLines(lineColLen));
        }
        else
        {
            throw cmajor::symbols::Error(message);
        }
    }
    else
    {
        throw cmajor::symbols::Error(message);
    }
}

void Error(const std::string& message, const soul::ast::Span& span, Context* context, const soul::ast::Span& refSpan)
{
    if (span.IsValid() && refSpan.IsValid())
    {
        soul::ast::LineColLen lineColLen;
        soul::ast::LineColLen refLineColLen;
        const std::vector<int>* lineStartIndeces = context->GetFileMap().LineStartIndeces(context->FileId());
        if (lineStartIndeces)
        {
            lineColLen = soul::ast::SpanToLineColLen(span, *lineStartIndeces);
            refLineColLen = soul::ast::SpanToLineColLen(refSpan, *lineStartIndeces);
        }
        if (lineColLen.IsValid() && refLineColLen.IsValid())
        {
            throw cmajor::symbols::Error(message + " at\n'" + context->FilePath() + "':" + std::to_string(lineColLen.line) + ":\n" +
                context->ErrorLines(lineColLen) + ": see reference line " + std::to_string(refLineColLen.line) + ":\n" +
                context->ErrorLines(refLineColLen));
        }
        else
        {
            throw cmajor::symbols::Error(message);
        }
    }
    else
    {
        throw cmajor::symbols::Error(message);
    }
}

} // cmajor::systemx::intermediate
