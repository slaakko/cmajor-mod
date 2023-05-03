
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/concept.parser' using soul parser generator spg version 4.1.0

export module cmajor.concepts.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::concepts::parser {

template<typename LexerT>
struct ConceptParser
{
    static soul::parser::Match Concept(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Refinement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConceptBody(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode);
    static soul::parser::Match ConceptBodyConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode);
    static soul::parser::Match TypeNameConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match SignatureConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter);
    static soul::parser::Match ConstructorConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter);
    static soul::parser::Match DestructorConstraint(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::IdentifierNode* firstTypeParameter);
    static soul::parser::Match MemberFunctionConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match FunctionConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match EmbeddedConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match WhereConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match DisjunctiveConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConjunctiveConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PrimaryConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match AtomicConstraintExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PredicateConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match IsConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConceptOrTypeName(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match MultiParamConstraint(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Axiom(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ConceptNode* conceptNode);
    static soul::parser::Match AxiomBody(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::AxiomNode* axiomNode);
    static soul::parser::Match AxiomStatement(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::concepts::parser
