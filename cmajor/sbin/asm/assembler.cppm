// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.assembly.assembler;

import cmajor.sbin.assembly.visitor;
import cmajor.sbin.assembly.asm_file;
import cmajor.sbin.coff;
import cmajor.sbin.machine_x64;
import soul.ast.span;
import soul.lexer;
import util;
import std.core;

export namespace cmajor::sbin::assembly {

enum class EmitOperation
{
    emitCode, emitData
};

class Assembler : public Visitor, public cmajor::sbin::machine_x64::Emitter
{
public:
    Assembler(const std::string& asmFilePath_, int logStreamId_, bool verbose_, bool printLines_);
    void Assemble();
    void MakeSymbols();
    void EmitData();
    void EmitCode();
    void Visit(SymbolNode& symbolNode) override;
    void Visit(DataDefinitionNode& dataDefinitionNode) override;
    void Visit(BinaryExprNode& binaryExprNode) override;
    void Visit(UnaryExprNode& unaryExprNode) override;
    void Visit(RegisterNode& registerNode) override;
    void Visit(ContentExprNode& contentExprNode) override;
    void Visit(SizeExprNode& sizeExprNode) override;
    void Visit(ParenthesizedExprNode& parenthesizedExprNode) override;
    void Visit(HexNumberNode& hexNumberNode) override;
    void Visit(IntegerNode& integerNode) override;
    void ProcessIntegerValue(uint64_t value, const soul::ast::Span& span);
    void Visit(RealNode& realNode) override;
    void Visit(StringNode& stringNode) override;
    void Visit(InstructionNode& instructionNode) override;
    void Visit(FunctionDefinitionNode& functionDefinitionNode) override;
    void Visit(MacroDefinitionNode& macroDefinitionNode) override;
    Symbol* GetSymbol(const std::string& symbolName) const;
    void AddSymbol(Symbol* symbol);
    void EmitByte(uint8_t x) override;
    void EmitWord(uint16_t x) override;
    void EmitDword(uint32_t x) override;
    void EmitQword(uint64_t x) override;
    void EmitFloat(float x) override;
    void EmitDouble(double x) override;
    void ThrowError(const std::string& errorMessage, const soul::ast::Span& span) override;
    void EmitCallFunctionSymbol(Symbol* symbol);
    void EmitLeaSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span);
    void EmitMovReg64Symbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span);
    void EmitMovSdSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span);
    void EmitMovSsSymbol(cmajor::sbin::machine_x64::Register reg, Symbol* symbol, const soul::ast::Span& span);
    void EmitJmp(Symbol* symbol, const soul::ast::Span& span);
    void EmitJe(Symbol* symbol, const soul::ast::Span& span);
    void EmitJne(Symbol* symbol, const soul::ast::Span& span);
    void EmitJae(Symbol* symbol, const soul::ast::Span& span);
    void AddJmpPos(int64_t pos, Symbol* symbol, const soul::ast::Span& span);
    void ResolveJumps();
    void ResolveSymbolDifferences();
    void WriteObjectFile();
private:
    bool verbose;
    bool printLines;
    int logStreamId;
    soul::lexer::FileMap fileMap;
    std::string asmFilePath;
    std::string objFilePath;
    std::unique_ptr<AsmFileNode> asmFile;
    std::unique_ptr<cmajor::sbin::coff::CoffObjectFile> objectFile;
    util::MemoryStream codeStream;
    cmajor::sbin::coff::Section* codeSection;
    util::MemoryStream dataStream;
    cmajor::sbin::coff::Section* dataSection;
    std::map<std::string, Symbol*> symbolMap;
    std::vector<std::unique_ptr<Symbol>> symbols;
    std::unique_ptr<util::LittleEndianBinaryStreamWriter> currentWriter;
    EmitOperation emitOperation;
    DataInstKind dataInstKind;
    NodeKind operandKind;
    cmajor::sbin::machine_x64::Register reg;
    cmajor::sbin::machine_x64::Register contentReg0;
    cmajor::sbin::machine_x64::Register contentReg1;
    cmajor::sbin::machine_x64::Register contentReg2;
    cmajor::sbin::machine_x64::Register indexReg;
    cmajor::sbin::machine_x64::Register baseReg;
    bool negate;
    int64_t immediate;
    uint8_t scale;
    bool content;
    int32_t displacement;
    Symbol* symbolOperand;
    Symbol* symbol0;
    Symbol* symbol1;
    std::vector<JmpPos> jmpPositions;
    std::vector<SymbolDifference> symbolDifferences;
    Symbol* functionSymbol;
};

} // namespace cmajor::sbin::assembly
