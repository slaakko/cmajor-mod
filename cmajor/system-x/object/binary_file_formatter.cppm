// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.binary.file.formatter;

import cmajor.systemx.object.instruction.formatter;
import std.core;

export namespace cmajor::systemx::object {

class BinaryFile;
class Section;
class HeaderSection;
class CodeSection;
class DataSection;
class SymbolSection;
class LinkSection;
class DebugSection;
class ResourceSection;
class SymbolTable;
class Symbol;

class BinaryFileFormatter : public InstructionFormatter
{
public:
    BinaryFileFormatter(cmajor::systemx::machine::Machine& machine_, BinaryFile* file_);
    BinaryFile* File() const { return file; }
    Section* CurrentSection() const { return currentSection; }
    virtual void Format();
    virtual void FormatOcta(uint64_t octa);
    virtual void FormatSymbol(Symbol* symbol);
    virtual void FormatSection(HeaderSection* section);
    virtual void FormatSection(CodeSection* section);
    virtual void FormatSection(DataSection* section);
    virtual void FormatSection(SymbolSection* section);
    virtual void FormatSection(LinkSection* section);
    virtual void FormatSection(DebugSection* section);
    virtual void FormatSection(ResourceSection* section);
    virtual void FormatSymbolTable(SymbolTable& symbolTable);
private:
    BinaryFile* file;
    Section* currentSection;
    std::map<int64_t, int64_t> binaryFileFormatterSetAddressMap;
};

} // namespace cmajor::systemx::object
