// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.project;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseQualifiedId(ContainerFileLexer& lexer);

soul::parser::Match ParseProject(ContainerFileLexer& lexer, const std::string& config, cmajor::fault::tolerant::ast::Backend backend);

std::unique_ptr<cmajor::fault::tolerant::ast::Project> ParseProject(const std::string& projectFilePath, const std::string& config, cmajor::fault::tolerant::ast::Backend backend);

} // namespace cmajor::fault::tolerant::parser
