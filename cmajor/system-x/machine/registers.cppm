// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <assert.h>

export module cmajor.systemx.machine.registers;

import std.core;

export namespace cmajor::systemx::machine {

const uint8_t regAX = 255;
const uint8_t regBX = 254;
const uint8_t regCX = 253;
const uint8_t regDX = 252;
const uint8_t regEX = 251;
const uint8_t regSP = 250;
const uint8_t regFP = 249;
const uint8_t regIX = 248;
const uint8_t regP0 = 247;
const uint8_t regP1 = 246;
const uint8_t regP2 = 245;
const uint8_t regP3 = 244;
const uint8_t regP4 = 243;
const uint8_t regP5 = 242;
const uint8_t regP6 = 241;
const uint8_t regP7 = 240;
const uint8_t firstGlobalReg = 240;

const int numParamRegs = 8;

uint8_t GetParamRegNumber(int param);

//      register                           description                          S = saved, P = put
// ===============================================================================================
const uint8_t rA = 0;      // arithmetic status register           S P
const uint8_t rB = 1;      // bootstrap register (trip)            S P
const uint8_t rC = 2;      // cycle counter
const uint8_t rD = 3;      // dividend register                    S P
const uint8_t rE = 4;      // epsilon register                     S P
const uint8_t rF = 5;      // failure location register              P
const uint8_t rG = 6;      // global threshold register            S P
const uint8_t rH = 7;      // himult register                      S P
const uint8_t rI = 8;      // interval counter                     S P
const uint8_t rJ = 9;      // return-jump register                 S P
const uint8_t rK = 10;     // interrupt mask register
const uint8_t rL = 11;     // local threshold register             S P
const uint8_t rM = 12;     // multiplex mask register              S P
const uint8_t rN = 13;     // serial number
const uint8_t rO = 14;     // register stack offset
const uint8_t rP = 15;     // prediction register                  S P
const uint8_t rQ = 16;     // interrupt request register
const uint8_t rR = 17;     // remainder register                   S P
const uint8_t rS = 18;     // register stack pointer
const uint8_t rT = 19;     // trap address register
const uint8_t rU = 20;     // usage counter
const uint8_t rV = 21;     // virtual translation register
const uint8_t rW = 22;     // where-interrupted register (trip)    S P
const uint8_t rX = 23;     // execution register (trip)            S P
const uint8_t rY = 24;     // Y operand (trip)                     S P
const uint8_t rZ = 25;     // Z operand (trip)                     S P
const uint8_t rBB = 26;    // bootstrap register (trap)            P
const uint8_t rTT = 27;    // dynamic trap address register
const uint8_t rWW = 28;    // where-interrupted register (trap)    P
const uint8_t rXX = 29;    // execution register (trap)            P
const uint8_t rYY = 30;    // Y operand (trap)                     P
const uint8_t rZZ = 31;    // Z operand (trap)                     P

class Registers
{
public:
    Registers();
    uint64_t Get(uint8_t regNum)
    {
        uint64_t rg = GetSpecial(rG);
        if (regNum >= rg)
        {
            return globalRegs[regNum];
        }
        else
        {
            uint64_t rl = GetSpecial(rL);
            if (regNum >= rl)
            {
                SetSpecial(rL, static_cast<uint64_t>(regNum + 1));
            }
            return localRegs[regNum];
        }
    }
    void Set(uint8_t regNum, uint64_t value)
    {
        uint64_t rg = GetSpecial(rG);
        if (regNum >= rg)
        {
            globalRegs[regNum] = value;
        }
        else
        {
            uint64_t rl = GetSpecial(rL);
            if (regNum >= rl)
            {
                SetSpecial(rL, static_cast<uint64_t>(regNum + 1));
            }
            localRegs[regNum] = value;
        }
    }
    uint64_t GetSpecial(uint8_t regNum) const
    {
        assert(regNum < 32);
        return globalRegs[regNum];
    }
    void SetSpecial(uint8_t regNum, uint64_t value)
    {
        assert(regNum < 32);
        globalRegs[regNum] = value;
    }
    uint64_t GetPC() const 
    { 
        if (pc == 2)
        {
            int x = 0;
        }
        return pc; 
    }
    void SetPC(uint64_t pc_)  
    {  
        if (pc_ == 2)
        {
            int x = 0;
        }
        pc = pc_;
    }
    void SetInterrupt(uint64_t interruptBit);
    void ResetInterrupt(uint64_t interruptBit);
    uint64_t GetInterruptBits();
    const std::string& GetGlobalRegName(uint8_t reg) const;
private:
    uint64_t localRegs[256];
    uint64_t globalRegs[256];
    uint64_t pc;
};

const std::string& GetGlobalRegName(uint8_t globalReg);
uint8_t GetGlobalRegNumber(const std::string& globalRegName);

} // namespace cmajor::systemx::machine
