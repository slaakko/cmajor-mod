// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.assembler.opcode;

import cmajor.systemx.assembler.visitor;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::assembler {

class OpCodeMap
{
public:
    static OpCodeMap& Instance();
    const std::string& GetOpCodeName(int opCode) const;
    int GetOpCode(const std::string& opCodeName) const;
    int MaxOpCodeNameLength() const { return maxOpCodeNameLength; }
private:
    OpCodeMap();
    int maxOpCodeNameLength;
    std::map<std::string, int> codeValueMap;
    std::map<int, std::string> codeNameMap;
};

OpCodeMap& OpCodeMap::Instance()
{
    static OpCodeMap instance;
    return instance;
}

OpCodeMap::OpCodeMap() : maxOpCodeNameLength(0)
{
    codeValueMap["IS"] = IS;
    codeNameMap[IS] = "IS";
    codeValueMap["EXTERN"] = EXTERN;
    codeNameMap[EXTERN] = "EXTERN";
    codeValueMap["LINKONCE"] = LINKONCE;
    codeNameMap[LINKONCE] = "LINKONCE";
    codeValueMap["FUNC"] = FUNC;
    codeNameMap[FUNC] = "FUNC";
    codeValueMap["ENDF"] = ENDF;
    codeNameMap[ENDF] = "ENDF";
    codeValueMap["STRUCT"] = STRUCT;
    codeNameMap[STRUCT] = "STRUCT";
    codeValueMap["ENDS"] = ENDS;
    codeNameMap[ENDS] = "ENDS";
    codeValueMap["BYTE"] = BYTE;
    codeNameMap[BYTE] = "BYTE";
    codeValueMap["WYDE"] = WYDE;
    codeNameMap[WYDE] = "WYDE";
    codeValueMap["TETRA"] = TETRA;
    codeNameMap[TETRA] = "TETRA";
    codeValueMap["OCTA"] = OCTA;
    codeNameMap[OCTA] = "OCTA";
    codeValueMap[".LINK"] = LINK;
    codeNameMap[LINK] = ".LINK";
    codeValueMap[".CODE"] = CODE;
    codeNameMap[CODE] = ".CODE";
    codeValueMap[".DATA"] = DATA;
    codeNameMap[DATA] = ".DATA";
    codeValueMap[".DEBUG"] = DEBUG;
    codeNameMap[DEBUG] = ".DEBUG";
    codeValueMap["LDA"] = LDA;
    codeNameMap[LDA] = "LDA";
    codeValueMap["SET"] = SET;
    codeNameMap[SET] = "SET";
    codeValueMap["BSPEC"] = BSPEC;
    codeNameMap[BSPEC] = "BSPEC";
    codeValueMap["ESPEC"] = ESPEC;
    codeNameMap[ESPEC] = "ESPEC";
    for (const auto& valueName : codeNameMap)
    {
        int n = valueName.second.length();
        if (n > maxOpCodeNameLength)
        {
            maxOpCodeNameLength = n;
        }
    }
}

const std::string& OpCodeMap::GetOpCodeName(int opCode) const
{
    auto it = codeNameMap.find(opCode);
    if (it != codeNameMap.cend())
    {
        return it->second;
    }
    else
    {
        throw std::runtime_error("invalid opCode " + std::to_string(opCode));
    }
}

int OpCodeMap::GetOpCode(const std::string& opCodeName) const
{
    int opCode = cmajor::systemx::machine::GetOpCode(opCodeName);
    if (opCode == -1)
    {
        auto it = codeValueMap.find(opCodeName);
        if (it != codeValueMap.cend())
        {
            return it->second;
        }
    }
    return opCode;
}

OpCode::OpCode(const soul::ast::SourcePos& sourcePos_, const std::string& name_) : Node(NodeKind::opCodeNode, sourcePos_), name(name_), value(-1)
{
}

OpCode::OpCode(int value_) : Node(NodeKind::opCodeNode, soul::ast::SourcePos()), value(value_)
{
}

void OpCode::MakeImmediate()
{
    ++value;
}

void OpCode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void OpCode::Write(util::CodeFormatter& formatter)
{
    formatter.Write(util::Format(GetOpCodeName(value), MaxOpCodeWidth(), util::FormatWidth::min));
}

std::string GetOpCodeName(int opCode)
{
    if (opCode >= 256)
    {
        return OpCodeMap::Instance().GetOpCodeName(opCode);
    }
    else if (opCode >= 0)
    {
        return cmajor::systemx::machine::GetOpCodeName(static_cast<uint8_t>(opCode));
    }
    else
    {
        throw std::runtime_error("invalid opCode " + std::to_string(opCode));
    }
}

int GetOpCode(const std::string& opCodeName)
{
    return OpCodeMap::Instance().GetOpCode(opCodeName);
}

int MaxOpCodeWidth()
{
    return std::max(OpCodeMap::Instance().MaxOpCodeNameLength(), cmajor::systemx::machine::MaxOpCodeNameLength());
}

} // namespace cmajor::systemx::assembler
