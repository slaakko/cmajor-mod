// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.parser.operators;

export namespace cmajor::parser::operators {

enum class Operator
{
    none = 0,
    or_, and_, eq, neq, less, greater, lessOrEq, greaterOrEq, is, as, plusplus, minusminus, plus, minus, mul, div, rem, not_, bitOr, bitXor, bitAnd, shiftLeft, shiftRight,
    complement, deref, addrOf
};

}
