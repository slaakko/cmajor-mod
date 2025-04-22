
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/intermediate/intermediate_code.parser' using soul parser generator spg version 5.0.0

export module cmajor.systemx.intermediate.code.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.types;
import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.metadata;

using namespace cmajor::systemx::intermediate::context;
using namespace cmajor::systemx::intermediate::types;
using namespace cmajor::systemx::intermediate::code;
using namespace cmajor::systemx::intermediate::metadata;

export namespace cmajor::systemx::intermediate::code::parser {

template<typename LexerT>
struct IntermediateCodeParser
{
    static void Parse(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match IntermediateCodeFile(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match CompileUnitHeader(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match CompileUnitId(LexerT& lexer);
    static soul::parser::Match TypeDeclarations(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match TypeDeclaration(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match TypeId(LexerT& lexer);
    static soul::parser::Match FundamentalTypeId(LexerT& lexer);
    static soul::parser::Match TypeRef(LexerT& lexer);
    static soul::parser::Match StructureType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match ArrayType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match FunctionType(LexerT& lexer, cmajor::systemx::intermediate::Context* context, int32_t typeId);
    static soul::parser::Match DataDefinitions(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match DataDefinition(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match Constant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match BoolConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match SByteConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ByteConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ShortConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match UShortConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match IntConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match UIntConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match LongConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ULongConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match FloatConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match DoubleConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match AddressConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ArrayConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match StructureConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match StringConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match StringArrayConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match StringArrayPrefix(LexerT& lexer);
    static soul::parser::Match ConversionConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ClsIdConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match SymbolConstant(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match Value(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type);
    static soul::parser::Match RegValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type);
    static soul::parser::Match ResultRegValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type);
    static soul::parser::Match SymbolValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type);
    static soul::parser::Match LiteralValue(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Type* type);
    static soul::parser::Match FunctionDefinitions(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match FunctionDefinition(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match FunctionHeader(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match BasicBlock(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::Function* function);
    static soul::parser::Match Label(LexerT& lexer);
    static soul::parser::Match Operand(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match Instructions(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::BasicBlock* basicBlock);
    static soul::parser::Match Instruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match StoreInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ArgInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match JmpInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match BranchInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ProcedureCallInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match RetInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match SwitchInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match ValueInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match Operation(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match UnaryInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match BinaryInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match ParamInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match LocalInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match LoadInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match ElemAddrInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match PtrOffsetInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match PtrDiffInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match FunctionCallInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match TrapInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::RegValue* result);
    static soul::parser::Match NoOperationInstruction(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match Metadata(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataStruct(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataField(LexerT& lexer, cmajor::systemx::intermediate::Context* context, cmajor::systemx::intermediate::MetadataStruct* parent);
    static soul::parser::Match MetadataItem(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataBool(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataLong(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataString(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataArray(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
    static soul::parser::Match MetadataRef(LexerT& lexer, cmajor::systemx::intermediate::Context* context);
};

} // namespace cmajor::systemx::intermediate::code::parser
