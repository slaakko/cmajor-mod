
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/masm/intermediate/intermediate.parser' using soul parser generator spg version 5.0.0

export module cmajor.masm.intermediate.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.types;
import cmajor.masm.intermediate.code;
import cmajor.masm.intermediate.metadata;

using namespace cmajor::masm::intermediate::context;
using namespace cmajor::masm::intermediate::types;
using namespace cmajor::masm::intermediate::code;
using namespace cmajor::masm::intermediate::metadata;

export namespace cmajor::masm::intermediate::parser {

template<typename LexerT>
struct IntermediateParser
{
    static void Parse(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match IntermediateFile(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match TypeDeclarations(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match TypeDeclaration(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match TypeId(LexerT& lexer);
    static soul::parser::Match FundamentalTypeId(LexerT& lexer);
    static soul::parser::Match TypeRef(LexerT& lexer);
    static soul::parser::Match StructureType(LexerT& lexer, cmajor::masm::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match ArrayType(LexerT& lexer, cmajor::masm::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match FunctionType(LexerT& lexer, cmajor::masm::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match DataDefinitions(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match DataDefinition(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match Constant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match BoolConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match SByteConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ByteConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ShortConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match UShortConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match IntConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match UIntConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match LongConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ULongConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match FloatConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match DoubleConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match AddressConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ArrayConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match StructureConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match StringConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match StringArrayConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match StringArrayPrefix(LexerT& lexer);
    static soul::parser::Match ConversionConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ClsIdConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match SymbolConstant(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match Value(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Type* type);
    static soul::parser::Match RegValue(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Type* type);
    static soul::parser::Match ResultRegValue(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Type* type);
    static soul::parser::Match SymbolValue(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Type* type);
    static soul::parser::Match LiteralValue(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Type* type);
    static soul::parser::Match FunctionDefinitions(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match FunctionDeclaration(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match FunctionDefinition(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match FunctionHeader(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match BasicBlock(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::Function* function);
    static soul::parser::Match Label(LexerT& lexer);
    static soul::parser::Match Operand(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match Instructions(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::BasicBlock* basicBlock);
    static soul::parser::Match Instruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match StoreInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ArgInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match JmpInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match BranchInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ProcedureCallInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match RetInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match SwitchInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match ValueInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match Operation(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match UnaryInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match BinaryInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match ParamInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match LocalInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match LoadInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match ElemAddrInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match PtrOffsetInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match PtrDiffInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match FunctionCallInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::RegValue* result);
    static soul::parser::Match BlockValue(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match NoOperationInstruction(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match Metadata(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataStruct(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataField(LexerT& lexer, cmajor::masm::intermediate::Context* context, cmajor::masm::intermediate::MetadataStruct* parent);
    static soul::parser::Match MetadataItem(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataBool(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataLong(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataString(LexerT& lexer, cmajor::masm::intermediate::Context* context);
    static soul::parser::Match MetadataRef(LexerT& lexer, cmajor::masm::intermediate::Context* context);
};

} // namespace cmajor::masm::intermediate::parser
