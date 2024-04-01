
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/masm/intermediate/intermediate.token' using soul lexer generator version 5.0.0

export module cmajor.masm.intermediate.token;

import std.core;

export namespace cmajor::masm::intermediate::token {

constexpr int32_t tokenSetID = 908871979;

constexpr int64_t ID = (static_cast<int64_t>(tokenSetID) << 32) | 1;
constexpr int64_t TYPES = (static_cast<int64_t>(tokenSetID) << 32) | 2;
constexpr int64_t DATA = (static_cast<int64_t>(tokenSetID) << 32) | 3;
constexpr int64_t FUNCTION = (static_cast<int64_t>(tokenSetID) << 32) | 4;
constexpr int64_t EXTERN = (static_cast<int64_t>(tokenSetID) << 32) | 5;
constexpr int64_t INLINE = (static_cast<int64_t>(tokenSetID) << 32) | 6;
constexpr int64_t STORE = (static_cast<int64_t>(tokenSetID) << 32) | 7;
constexpr int64_t ARG = (static_cast<int64_t>(tokenSetID) << 32) | 8;
constexpr int64_t JMP = (static_cast<int64_t>(tokenSetID) << 32) | 9;
constexpr int64_t BRANCH = (static_cast<int64_t>(tokenSetID) << 32) | 10;
constexpr int64_t CALL = (static_cast<int64_t>(tokenSetID) << 32) | 11;
constexpr int64_t RET = (static_cast<int64_t>(tokenSetID) << 32) | 12;
constexpr int64_t SWITCH = (static_cast<int64_t>(tokenSetID) << 32) | 13;
constexpr int64_t NOP = (static_cast<int64_t>(tokenSetID) << 32) | 14;
constexpr int64_t NOT = (static_cast<int64_t>(tokenSetID) << 32) | 15;
constexpr int64_t NEG = (static_cast<int64_t>(tokenSetID) << 32) | 16;
constexpr int64_t SIGNEXTEND = (static_cast<int64_t>(tokenSetID) << 32) | 17;
constexpr int64_t ZEROEXTEND = (static_cast<int64_t>(tokenSetID) << 32) | 18;
constexpr int64_t TRUNCATE = (static_cast<int64_t>(tokenSetID) << 32) | 19;
constexpr int64_t BITCAST = (static_cast<int64_t>(tokenSetID) << 32) | 20;
constexpr int64_t INTTOFLOAT = (static_cast<int64_t>(tokenSetID) << 32) | 21;
constexpr int64_t FLOATTOINT = (static_cast<int64_t>(tokenSetID) << 32) | 22;
constexpr int64_t INTTOPTR = (static_cast<int64_t>(tokenSetID) << 32) | 23;
constexpr int64_t PTRTOINT = (static_cast<int64_t>(tokenSetID) << 32) | 24;
constexpr int64_t ADD = (static_cast<int64_t>(tokenSetID) << 32) | 25;
constexpr int64_t SUB = (static_cast<int64_t>(tokenSetID) << 32) | 26;
constexpr int64_t MUL = (static_cast<int64_t>(tokenSetID) << 32) | 27;
constexpr int64_t DIV = (static_cast<int64_t>(tokenSetID) << 32) | 28;
constexpr int64_t MOD = (static_cast<int64_t>(tokenSetID) << 32) | 29;
constexpr int64_t AND = (static_cast<int64_t>(tokenSetID) << 32) | 30;
constexpr int64_t OR = (static_cast<int64_t>(tokenSetID) << 32) | 31;
constexpr int64_t XOR = (static_cast<int64_t>(tokenSetID) << 32) | 32;
constexpr int64_t SHL = (static_cast<int64_t>(tokenSetID) << 32) | 33;
constexpr int64_t SHR = (static_cast<int64_t>(tokenSetID) << 32) | 34;
constexpr int64_t EQUAL = (static_cast<int64_t>(tokenSetID) << 32) | 35;
constexpr int64_t LESS = (static_cast<int64_t>(tokenSetID) << 32) | 36;
constexpr int64_t PARAM = (static_cast<int64_t>(tokenSetID) << 32) | 37;
constexpr int64_t LOCAL = (static_cast<int64_t>(tokenSetID) << 32) | 38;
constexpr int64_t LOAD = (static_cast<int64_t>(tokenSetID) << 32) | 39;
constexpr int64_t ELEMADDR = (static_cast<int64_t>(tokenSetID) << 32) | 40;
constexpr int64_t PTROFFSET = (static_cast<int64_t>(tokenSetID) << 32) | 41;
constexpr int64_t PTRDIFF = (static_cast<int64_t>(tokenSetID) << 32) | 42;
constexpr int64_t TYPE = (static_cast<int64_t>(tokenSetID) << 32) | 43;
constexpr int64_t OFFSET = (static_cast<int64_t>(tokenSetID) << 32) | 44;
constexpr int64_t SIZE = (static_cast<int64_t>(tokenSetID) << 32) | 45;
constexpr int64_t ALIGNMENT = (static_cast<int64_t>(tokenSetID) << 32) | 46;
constexpr int64_t VOID = (static_cast<int64_t>(tokenSetID) << 32) | 47;
constexpr int64_t BOOL = (static_cast<int64_t>(tokenSetID) << 32) | 48;
constexpr int64_t SBYTE = (static_cast<int64_t>(tokenSetID) << 32) | 49;
constexpr int64_t BYTE = (static_cast<int64_t>(tokenSetID) << 32) | 50;
constexpr int64_t SHORT = (static_cast<int64_t>(tokenSetID) << 32) | 51;
constexpr int64_t USHORT = (static_cast<int64_t>(tokenSetID) << 32) | 52;
constexpr int64_t INT = (static_cast<int64_t>(tokenSetID) << 32) | 53;
constexpr int64_t UINT = (static_cast<int64_t>(tokenSetID) << 32) | 54;
constexpr int64_t LONG = (static_cast<int64_t>(tokenSetID) << 32) | 55;
constexpr int64_t ULONG = (static_cast<int64_t>(tokenSetID) << 32) | 56;
constexpr int64_t FLOAT = (static_cast<int64_t>(tokenSetID) << 32) | 57;
constexpr int64_t DOUBLE = (static_cast<int64_t>(tokenSetID) << 32) | 58;
constexpr int64_t TRUE = (static_cast<int64_t>(tokenSetID) << 32) | 59;
constexpr int64_t FALSE = (static_cast<int64_t>(tokenSetID) << 32) | 60;
constexpr int64_t NULL_TOKEN = (static_cast<int64_t>(tokenSetID) << 32) | 61;
constexpr int64_t CONV = (static_cast<int64_t>(tokenSetID) << 32) | 62;
constexpr int64_t LPAREN = (static_cast<int64_t>(tokenSetID) << 32) | 63;
constexpr int64_t RPAREN = (static_cast<int64_t>(tokenSetID) << 32) | 64;
constexpr int64_t COMMA = (static_cast<int64_t>(tokenSetID) << 32) | 65;
constexpr int64_t LBRACE = (static_cast<int64_t>(tokenSetID) << 32) | 66;
constexpr int64_t RBRACE = (static_cast<int64_t>(tokenSetID) << 32) | 67;
constexpr int64_t AST = (static_cast<int64_t>(tokenSetID) << 32) | 68;
constexpr int64_t COLON = (static_cast<int64_t>(tokenSetID) << 32) | 69;
constexpr int64_t SEMICOLON = (static_cast<int64_t>(tokenSetID) << 32) | 70;
constexpr int64_t LBRACKET = (static_cast<int64_t>(tokenSetID) << 32) | 71;
constexpr int64_t RBRACKET = (static_cast<int64_t>(tokenSetID) << 32) | 72;
constexpr int64_t ASSIGN = (static_cast<int64_t>(tokenSetID) << 32) | 73;
constexpr int64_t AT = (static_cast<int64_t>(tokenSetID) << 32) | 74;
constexpr int64_t DOLLAR = (static_cast<int64_t>(tokenSetID) << 32) | 75;
constexpr int64_t EXCLAMATION = (static_cast<int64_t>(tokenSetID) << 32) | 76;
constexpr int64_t TYPEID = (static_cast<int64_t>(tokenSetID) << 32) | 77;
constexpr int64_t CLSID = (static_cast<int64_t>(tokenSetID) << 32) | 78;
constexpr int64_t STRING = (static_cast<int64_t>(tokenSetID) << 32) | 79;
constexpr int64_t NUMBER = (static_cast<int64_t>(tokenSetID) << 32) | 80;
constexpr int64_t HEXNUM = (static_cast<int64_t>(tokenSetID) << 32) | 81;

} // cmajor::masm::intermediate::token
