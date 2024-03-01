module cmajor.symbols.warning;

// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================
import soul.ast.span;
import util;
import std.core;

namespace cmajor::symbols {

Warning::Warning(const std::u32string& project_, const std::string& message_) : project(project_), message(message_)
{
}

void Warning::SetReferences(const std::vector<soul::ast::FullSpan>& references_)
{
    references = references_;
}

CompileWarningCollection::CompileWarningCollection()
{
}

void CompileWarningCollection::AddWarning(const Warning& warning)
{
    warnings.push_back(warning);
}

} // namespace cmajor::symbols
