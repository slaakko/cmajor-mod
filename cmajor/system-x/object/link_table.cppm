// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.link.table;

import cmajor.systemx.object.copy.range;
import util;
import std.core;

export namespace cmajor::systemx::object {

class Symbol;
class LinkSection;
class SymbolTable;
class LinkCommand;
class ObjectFile;

class UnprocessedLinkCommand
{
public:
    UnprocessedLinkCommand(ObjectFile* objectFile_, LinkCommand* linkCommand_);
    ObjectFile* GetObjectFile() const { return objectFile; }
    LinkCommand* GetLinkCommand() const { return linkCommand; }
private:
    ObjectFile* objectFile;
    LinkCommand* linkCommand;
};

class UnprocessedSymbol
{
public:
    UnprocessedSymbol(Symbol* symbol_, LinkSection* linkSection_, SymbolTable* symbolTable_);
    Symbol* GetSymbol() const { return symbol; }
    LinkSection* GetLinkSection() const { return linkSection; }
    SymbolTable* GetSymbolTable() const { return symbolTable; }
private:
    Symbol* symbol;
    LinkSection* linkSection;
    SymbolTable* symbolTable;
};

class LinkTable
{
public:
    LinkTable(const std::string& classIdFilePath_);
    LinkTable(const LinkTable&) = delete;
    LinkTable& operator=(const LinkTable&) = delete;
    void AddUsedSymbolName(const std::string& symbolName);
    bool IsUsedSymbolName(const std::string& symbolName) const;
    void AddUnprocessedSymbol(Symbol* symbol, LinkSection* linkSection, SymbolTable* symbolTable);
    const std::vector<UnprocessedSymbol>* GetUnprocessedSymbols(const std::string& symbolName) const;
    void ClearUnprocessedSymbols(const std::string& symbolName);
    void ClearUnprocessedSymbols();
    void ExecuteLinkCommands(const std::string& symbolName, int64_t start);
    void AddCopyRange(const CopyRange& copyRange);
    uint64_t GetClassId(const util::uuid& typeId) const;
    void AddLinkCommand(const std::string& symbolName, ObjectFile* objectFile, LinkCommand* linkCommand);
    void CheckUnresolvedSymbols(int logStreamId);
    const std::vector<CopyRange>& CopyRanges() const { return copyRanges; }
private:
    std::string classIdFilePath;
    std::set<std::string> usedSymbolNames;
    std::map<std::string, std::vector<UnprocessedSymbol>> unprocessedSymbolMap;
    std::vector<CopyRange> copyRanges;
    std::map<std::string, std::vector<UnprocessedLinkCommand>> unprocessedLinkCommandMap;
    std::map<util::uuid, uint64_t> classIdMap;
};

} // namespace cmajor::systemx::object
