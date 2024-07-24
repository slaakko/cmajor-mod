// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.global;

import cmajor.symbols;
import std.core;

export namespace cmdoclib {

void ReadGlobals(const std::vector<std::string>& moduleNames);
void WriteGlobals(const std::vector<std::string>& moduleNames);
void AddDerivedClass(cmajor::symbols::ClassTypeSymbol* baseClass, cmajor::symbols::ClassTypeSymbol* derivedClass);
std::vector<std::string> GetDerivedClassIds(const std::string& moduleName, const std::string& baseClassId);
void AddConceptRefinement(cmajor::symbols::ConceptSymbol* refinedConcept, cmajor::symbols::ConceptSymbol* conceptSymbol);
std::vector<std::string> GetDerivedConceptIds(const std::string& moduleName, const std::string& refinedConceptId);
void WriteModuleXml(const std::string& targetDir, const std::vector<std::string>& moduleNames);

} // namespace cmdoclib
