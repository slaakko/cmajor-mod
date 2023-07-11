
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/debug/debug_command.token' using soul lexer generator version 4.0.0

export module cmajor.debug.command.token;

import std.core;

export namespace cmajor::debug::command::token {

constexpr int32_t tokenSetID = 1294196606;

constexpr int64_t NL = (static_cast<int64_t>(tokenSetID) << 32) | 1;
constexpr int64_t EXIT = (static_cast<int64_t>(tokenSetID) << 32) | 2;
constexpr int64_t E = (static_cast<int64_t>(tokenSetID) << 32) | 3;
constexpr int64_t QUIT = (static_cast<int64_t>(tokenSetID) << 32) | 4;
constexpr int64_t Q = (static_cast<int64_t>(tokenSetID) << 32) | 5;
constexpr int64_t HELP = (static_cast<int64_t>(tokenSetID) << 32) | 6;
constexpr int64_t H = (static_cast<int64_t>(tokenSetID) << 32) | 7;
constexpr int64_t NEXT = (static_cast<int64_t>(tokenSetID) << 32) | 8;
constexpr int64_t N = (static_cast<int64_t>(tokenSetID) << 32) | 9;
constexpr int64_t STEP = (static_cast<int64_t>(tokenSetID) << 32) | 10;
constexpr int64_t S = (static_cast<int64_t>(tokenSetID) << 32) | 11;
constexpr int64_t CONTINUE = (static_cast<int64_t>(tokenSetID) << 32) | 12;
constexpr int64_t C = (static_cast<int64_t>(tokenSetID) << 32) | 13;
constexpr int64_t FINISH = (static_cast<int64_t>(tokenSetID) << 32) | 14;
constexpr int64_t F = (static_cast<int64_t>(tokenSetID) << 32) | 15;
constexpr int64_t UNTIL = (static_cast<int64_t>(tokenSetID) << 32) | 16;
constexpr int64_t U = (static_cast<int64_t>(tokenSetID) << 32) | 17;
constexpr int64_t BREAK = (static_cast<int64_t>(tokenSetID) << 32) | 18;
constexpr int64_t B = (static_cast<int64_t>(tokenSetID) << 32) | 19;
constexpr int64_t DELETE = (static_cast<int64_t>(tokenSetID) << 32) | 20;
constexpr int64_t D = (static_cast<int64_t>(tokenSetID) << 32) | 21;
constexpr int64_t PRINT = (static_cast<int64_t>(tokenSetID) << 32) | 22;
constexpr int64_t P = (static_cast<int64_t>(tokenSetID) << 32) | 23;
constexpr int64_t DEPTH = (static_cast<int64_t>(tokenSetID) << 32) | 24;
constexpr int64_t FRAMES = (static_cast<int64_t>(tokenSetID) << 32) | 25;
constexpr int64_t LIST = (static_cast<int64_t>(tokenSetID) << 32) | 26;
constexpr int64_t L = (static_cast<int64_t>(tokenSetID) << 32) | 27;
constexpr int64_t SHOW = (static_cast<int64_t>(tokenSetID) << 32) | 28;
constexpr int64_t BREAKPOINT = (static_cast<int64_t>(tokenSetID) << 32) | 29;
constexpr int64_t BREAKPOINTS = (static_cast<int64_t>(tokenSetID) << 32) | 30;
constexpr int64_t CAST = (static_cast<int64_t>(tokenSetID) << 32) | 31;
constexpr int64_t TYPEID = (static_cast<int64_t>(tokenSetID) << 32) | 32;
constexpr int64_t BASE = (static_cast<int64_t>(tokenSetID) << 32) | 33;
constexpr int64_t SET = (static_cast<int64_t>(tokenSetID) << 32) | 34;
constexpr int64_t BREAKONTHROW = (static_cast<int64_t>(tokenSetID) << 32) | 35;
constexpr int64_t CONDITION = (static_cast<int64_t>(tokenSetID) << 32) | 36;
constexpr int64_t TRUE = (static_cast<int64_t>(tokenSetID) << 32) | 37;
constexpr int64_t FALSE = (static_cast<int64_t>(tokenSetID) << 32) | 38;
constexpr int64_t ID = (static_cast<int64_t>(tokenSetID) << 32) | 39;
constexpr int64_t PATH = (static_cast<int64_t>(tokenSetID) << 32) | 40;
constexpr int64_t INTEGER = (static_cast<int64_t>(tokenSetID) << 32) | 41;
constexpr int64_t STRING = (static_cast<int64_t>(tokenSetID) << 32) | 42;
constexpr int64_t COLON = (static_cast<int64_t>(tokenSetID) << 32) | 43;
constexpr int64_t AST = (static_cast<int64_t>(tokenSetID) << 32) | 44;
constexpr int64_t AMP = (static_cast<int64_t>(tokenSetID) << 32) | 45;
constexpr int64_t LPAREN = (static_cast<int64_t>(tokenSetID) << 32) | 46;
constexpr int64_t RPAREN = (static_cast<int64_t>(tokenSetID) << 32) | 47;
constexpr int64_t LANGLE = (static_cast<int64_t>(tokenSetID) << 32) | 48;
constexpr int64_t RANGLE = (static_cast<int64_t>(tokenSetID) << 32) | 49;
constexpr int64_t LBRACKET = (static_cast<int64_t>(tokenSetID) << 32) | 50;
constexpr int64_t RBRACKET = (static_cast<int64_t>(tokenSetID) << 32) | 51;
constexpr int64_t DOT = (static_cast<int64_t>(tokenSetID) << 32) | 52;
constexpr int64_t COMMA = (static_cast<int64_t>(tokenSetID) << 32) | 53;
constexpr int64_t PLUS = (static_cast<int64_t>(tokenSetID) << 32) | 54;
constexpr int64_t MINUS = (static_cast<int64_t>(tokenSetID) << 32) | 55;
constexpr int64_t EQ = (static_cast<int64_t>(tokenSetID) << 32) | 56;
constexpr int64_t DISJUNCTION = (static_cast<int64_t>(tokenSetID) << 32) | 57;
constexpr int64_t CONJUNCTION = (static_cast<int64_t>(tokenSetID) << 32) | 58;
constexpr int64_t BITOR = (static_cast<int64_t>(tokenSetID) << 32) | 59;
constexpr int64_t BITXOR = (static_cast<int64_t>(tokenSetID) << 32) | 60;
constexpr int64_t EQUAL = (static_cast<int64_t>(tokenSetID) << 32) | 61;
constexpr int64_t NOTEQUAL = (static_cast<int64_t>(tokenSetID) << 32) | 62;
constexpr int64_t LESSEQUAL = (static_cast<int64_t>(tokenSetID) << 32) | 63;
constexpr int64_t GREATEREQUAL = (static_cast<int64_t>(tokenSetID) << 32) | 64;
constexpr int64_t SHIFTLEFT = (static_cast<int64_t>(tokenSetID) << 32) | 65;
constexpr int64_t SHIFTRIGHT = (static_cast<int64_t>(tokenSetID) << 32) | 66;
constexpr int64_t SLASH = (static_cast<int64_t>(tokenSetID) << 32) | 67;
constexpr int64_t PERCENT = (static_cast<int64_t>(tokenSetID) << 32) | 68;

} // cmajor::debug::command::token
