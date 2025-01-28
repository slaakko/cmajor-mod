// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.debug.info.to.xml;

import soul.xml.dom;
import std.core;

export namespace cmajor::debug {

std::unique_ptr<soul::xml::Document> GetDebugInfoAsXml(const std::string& cmdbFilePath);
void WriteXmlDebugInfo(soul::xml::Document* debugInfoXmlDoc, const std::string& xmlDebugInfoFilePath);

} // namespace cmajor::debug
