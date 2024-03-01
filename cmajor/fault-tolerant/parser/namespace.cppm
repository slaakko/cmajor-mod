// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.namespaces;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

void ParseNamespaceContent(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::NamespaceDefinitionNode* ns);

soul::parser::Match ParseNamespaceImport(Context& context, Lexer& lexer);

soul::parser::Match ParseNamespaceDefinition(Context& context, Lexer& lexer);

} // namespace cmajor::fault::tolerant::parser
