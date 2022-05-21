// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.spg;

import std.core;

export namespace soul::ast::spg {

std::string ToNamespaceName(const std::string& qualifiedId);

class ExportModule
{
public:
    ExportModule(const std::string& moduleName_);
    const std::string& ModuleName() const { return moduleName; }
private:
    std::string moduleName;
};

enum class ImportPrefix
{
    interfacePrefix, implementationPrefix
};

class Import
{
public:
    Import(const std::string& moduleName_, ImportPrefix prefix_);
    const std::string& ModuleName() const { return moduleName; }
    ImportPrefix Prefix() const { return prefix; }
private:
    std::string moduleName;
    ImportPrefix prefix;
};

} // namespace soul::ast::spg
