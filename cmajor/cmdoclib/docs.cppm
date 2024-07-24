// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.docs;

import std.core;

export namespace cmdoclib {

void BuildDocs(const std::string& solutionName, const std::vector<std::string>& moduleNames);

} // namespace cmdoclib
