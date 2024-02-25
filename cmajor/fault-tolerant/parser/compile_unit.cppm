// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.compile.unit;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseDefinition(Context& context, Lexer& lexer);

std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> ParseCompileUnit(Context& context, Lexer& lexer);

std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> ParseCmajorCompileUnit(const std::string& cmFilePath);

} // namespace cmajor::fault::tolerant::parser
