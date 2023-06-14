// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.assembler.visitor;

import std.core;

export namespace cmajor::systemx::assembler {

class DecimalConstant;
class HexadecimalConstant;
class ByteConstant;
class WydeConstant;
class TetraConstant;
class CharacterConstant;
class StringConstant;
class ClsIdConstant;
class UnaryExpression;
class BinaryExpression;
class ParenthesizedExpression;
class OpCode;
class LocalSymbol;
class GlobalSymbol;
class At;
class OperandList;
class Instruction;

class Visitor
{
public:
    virtual ~Visitor();
    virtual void Visit(DecimalConstant& node) {}
    virtual void Visit(HexadecimalConstant& node) {}
    virtual void Visit(ByteConstant& node) {}
    virtual void Visit(WydeConstant& node) {}
    virtual void Visit(TetraConstant& node) {}
    virtual void Visit(CharacterConstant& node) {}
    virtual void Visit(StringConstant& node) {}
    virtual void Visit(ClsIdConstant& node) {}
    virtual void Visit(UnaryExpression& node) {}
    virtual void Visit(BinaryExpression& node) {}
    virtual void Visit(ParenthesizedExpression& node) {}
    virtual void Visit(OpCode& node) {}
    virtual void Visit(LocalSymbol& node) {}
    virtual void Visit(GlobalSymbol& node) {}
    virtual void Visit(At& node) {}
    virtual void Visit(OperandList& node) {}
    virtual void Visit(Instruction& node) {}
};

} // namespace cmsx::assembler
