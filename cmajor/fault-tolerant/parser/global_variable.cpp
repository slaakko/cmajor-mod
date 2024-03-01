// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.global.variable;

import cmajor.fault.tolerant.parser.specifier;
import cmajor.fault.tolerant.parser.type_expr;
import cmajor.fault.tolerant.parser.identifier;
import cmajor.fault.tolerant.parser.expression;
import cmajor.lexers;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::token;

soul::parser::Match ParseGlobalVariable(Context& context, Lexer& lexer)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::SpecifiersNode> specifiers;
    soul::parser::Match specifierMatch = ParseSpecifiers(context, lexer);
    if (specifierMatch.hit)
    {
        specifiers.reset(static_cast<cmajor::fault::tolerant::ast::SpecifiersNode*>(specifierMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Node> type;
    soul::parser::Match typeMatch = ParseTypeExpr(context, lexer);
    if (typeMatch.hit)
    {
        type.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(typeMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::IdentifierNode> id;
    soul::parser::Match idMatch = ParseIdentifier(context, lexer, cmajor::fault::tolerant::ast::CompletionContext::none);
    if (idMatch.hit)
    {
        id.reset(static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(idMatch.value));
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::GlobalVariableNode> globalVariable;
    globalVariable.reset(new cmajor::fault::tolerant::ast::GlobalVariableNode(soul::ast::Span(), cmajor::fault::tolerant::ast::CompletionContext::none));
    globalVariable->SetSpecifiers(specifiers.release());
    globalVariable->SetType(type.release());
    globalVariable->SetId(id.release());
    int64_t save = lexer.GetPos();
    if (*lexer == ASSIGN)
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::AssignNode> assign;
        assign.reset(new cmajor::fault::tolerant::ast::AssignNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
        std::unique_ptr<cmajor::fault::tolerant::ast::Node> initializer;
        soul::parser::Match initializerMatch = ParseExpression(context, lexer);
        if (initializerMatch.hit)
        {
            initializer.reset(static_cast<cmajor::fault::tolerant::ast::Node*>(initializerMatch.value));
            globalVariable->SetAssign(assign.release());
            globalVariable->SetInitializer(initializer.release());
        }
        else
        {
            return soul::parser::Match(false);
        }

    }
    else
    {
        lexer.SetPos(save);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SemicolonNode> semicolon;
    if (*lexer == SEMICOLON)
    {
        semicolon.reset(new cmajor::fault::tolerant::ast::SemicolonNode(lexer.GetSpan(lexer.GetPos()), cmajor::fault::tolerant::ast::CompletionContext::none));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    globalVariable->SetSemicolon(semicolon.release());
    globalVariable->Make();
    return soul::parser::Match(true, globalVariable.release());
}

} // namespace cmajor::fault::tolerant::parser
