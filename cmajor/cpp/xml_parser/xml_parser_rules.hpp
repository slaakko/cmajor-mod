// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef XML_PARSER_XML_PARSER_RULES_INCLUDED
#define XML_PARSER_XML_PARSER_RULES_INCLUDED
#include <stdint.h>
#include <map>
#include <string>

namespace soul::xml::parser::rules {

std::map<int64_t, std::string>* GetRuleNameMapPtr();

} // soul::xml::parser::rules

#endif // XML_PARSER_XML_PARSER_RULES_INCLUDED
