// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.assembler;

import cmajor.systemx.assembler.instruction;
import cmajor.systemx.assembler.visitor;
import cmajor.systemx.object;
import soul.lexer;
import soul.ast.source.pos;
import util;
import std.core;

export namespace cmajor::systemx::assembler::assembler {

enum class Stage
{
    none, resolve, generateCode
};

class Assembler : public Visitor
{
public:
    Assembler(const std::string& assemblyFilePath_, const std::string& objectFilePath_);
    void Assemble();
    void Visit(DecimalConstant& node) override;
    void Visit(HexadecimalConstant& node) override;
    void Visit(ByteConstant& node) override;
    void Visit(WydeConstant& node) override;
    void Visit(TetraConstant& node) override;
    void Visit(CharacterConstant& node) override;
    void Visit(StringConstant& node) override;
    void Visit(ClsIdConstant& node) override;
    void Visit(UnaryExpression& node) override;
    void Visit(BinaryExpression& node) override;
    void Visit(ParenthesizedExpression& node) override;
    void Visit(OpCode& node) override;
    void Visit(LocalSymbol& node) override;
    void Visit(GlobalSymbol& node) override;
    void Visit(At& node) override;
    void Visit(OperandList& node) override;
    void Visit(Instruction& node) override;
    void AddInstruction(Instruction* instruction);
    void AddModeInstruction(const std::string& mode, const soul::ast::SourcePos& sourcePos);
    Instruction* CurrentInstruction() const { return currentInstruction; }
    cmajor::systemx::object::ObjectFile* GetObjectFile() const { return objectFile.get(); }
    cmajor::systemx::object::Segment CurrentSegment() const { return currentSegment; }
    cmajor::systemx::object::Section* CurrentSection() const { return currentSection; }
    cmajor::systemx::object::Symbol* CurrentSymbol() const { return currentSymbol; }
    cmajor::systemx::object::Symbol* CurrentFunctionSymbol() const { return currentFunctionSymbol; }
    void SetCurrentFunctionSymbol(cmajor::systemx::object::Symbol* currentFunctionSymbol_) { currentFunctionSymbol = currentFunctionSymbol_; }
    cmajor::systemx::object::Symbol* CurrentStructureSymbol() const { return currentStructureSymbol; }
    void SetCurrentStructureSymbol(cmajor::systemx::object::Symbol* currentStructureSymbol_) { currentStructureSymbol = currentStructureSymbol_; }
    int ParentIndex() const { return parentIndex; }
    void SetParentIndex(int parentIndex_) { parentIndex = parentIndex_; }
    void Error(const std::string& message, const soul::ast::SourcePos& sourcePos);
    Node* MakeDecimalConstant(const soul::ast::SourcePos& sourcePos, const std::string& s);
    Node* MakeHexConstant(const soul::ast::SourcePos& sourcePos, const std::string& s);
    Node* MakeCharConstant(const soul::ast::SourcePos& sourcePos, const std::u32string& s);
    Node* MakeStringConstant(const soul::ast::SourcePos& sourcePos, const std::u32string& s);
    Node* MakeClsIdConstant(const soul::ast::SourcePos& sourcePos, const std::u32string& s);
    void IncLine() { ++line; }
    void EmptyLine();
    void InstructionLine();
    void EmitOpCode(uint8_t opCode);
    void EmitRegisterValue(const std::string& paramName, const cmajor::systemx::object::Value& registerValue);
    void EmitPureByteValue(const std::string& paramName, const cmajor::systemx::object::Value& byteValue);
    void EmitPureWydeValue(const std::string& paramName, const cmajor::systemx::object::Value& wydeValue);
    void EmitPureTetraValue(const std::string& paramName, const cmajor::systemx::object::Value& tetraValue);
    void EmitPureOctaValue(const std::string& paramName, const cmajor::systemx::object::Value& octaValue);
    void EmitSymbolOcta(const cmajor::systemx::object::Value& symbolValue);
    void EmitSetPureRegValue(const cmajor::systemx::object::Value& registerValue, const cmajor::systemx::object::Value& pureValue);
    void EmitAbsoluteRegisterValue(const std::string& paramName, const cmajor::systemx::object::Value& registerValue, const cmajor::systemx::object::Value& symbolValue);
    void EmitForwardLongJump(cmajor::systemx::object::Symbol* symbol);
    void EmitForwardShortJump(const std::string& paramName, uint8_t opcode, const cmajor::systemx::object::Value& registerValue, cmajor::systemx::object::Symbol* symbol);
    void EmitLongOffset(uint32_t offset);
    void EmitShortOffset(uint16_t offset);
    void EmitClsIdCommmand(uint64_t typeIdIndex, const soul::ast::SourcePos& sourcePos);
    void BeginSpec();
    void EndSpec();
    bool InSpec() const { return inSpec; }
    const std::string& GetString(uint64_t val, const soul::ast::SourcePos& sourcePos);
    soul::lexer::FileMap& GetFileMap() { return fileMap; }
private:
    void VisitInstructions();
    void Resolve();
    void GenerateCode();
    void AlignAt();
    Stage stage;
    std::string assemblyFilePath;
    std::unique_ptr<cmajor::systemx::object::ObjectFile> objectFile;
    std::vector<std::unique_ptr<Instruction>> instructions;
    Instruction* currentInstruction;
    cmajor::systemx::object::Value value;
    std::vector< cmajor::systemx::object::Value> operands;
    cmajor::systemx::object::Segment currentSegment;
    cmajor::systemx::object::Section* currentSection;
    cmajor::systemx::object::Symbol* currentSymbol;
    cmajor::systemx::object::Symbol* currentFunctionSymbol;
    cmajor::systemx::object::Symbol* currentStructureSymbol;
    std::vector<util::uuid> typeIds;
    int64_t currentAlignment;
    int parentIndex;
    int line;
    bool prevWasEmptyLine;
    std::stack<bool> specStack;
    bool inSpec;
    std::vector<std::string> strings;
    soul::lexer::FileMap fileMap;
};

} // namespace cmajor::systemx::assembler
