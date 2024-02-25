// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.solution;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseSolution(ContainerFileLexer& lexer);

std::unique_ptr<cmajor::fault::tolerant::ast::Solution> ParseSolution(const std::string& solutionFilePath);

} // namespace cmajor::fault::tolerant::parser
