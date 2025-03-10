// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.sbin.compare_utils.stripped_asm;
import cmajor.sbin.machine_x64;
import cmajor.sbin.assembly;
import cmajor.masm.build;
import std.core;
import util;

namespace cmajor::sbin {

std::string MakeStrippedAsmBat(const std::string& file, const std::string& batFile)
{
    std::ofstream makeBat(batFile);
    std::string vcvars64Path = cmajor::masm::build::GetVCVars64Path();
    if (vcvars64Path.empty())
    {
        throw std::runtime_error("vcvars64.bat not configured");
    }
    util::CodeFormatter formatter(makeBat);
    formatter.WriteLine("@echo off");
    formatter.WriteLine("call \"" + vcvars64Path + "\"");
    std::string strippedAsm000FilePath = util::Path::ChangeExtension(file, ".stripped.asm.000");
    formatter.WriteLine("dumpbin /disasm:nobytes /out:\"" + strippedAsm000FilePath + "\"" + " \"" + file + "\"");
    return strippedAsm000FilePath;
}

std::string MakeStrippedAsm000(const std::string& file, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << file << "\n";
    }
    std::string batFile = util::Path::Combine(util::Path::GetDirectoryName(file), "make_stripped_asm.bat");
    std::string strippedAsm000FilePath = MakeStrippedAsmBat(file, batFile);
    std::string commandLine = "cmd /C \"" + batFile + "\"";
    std::pair<int, std::string> exitCodeStrPair = util::ExecuteWin(commandLine);
    if (exitCodeStrPair.first != 0)
    {
        throw std::runtime_error(exitCodeStrPair.second);
    }
    if (verbose)
    {
        std::cout << exitCodeStrPair.second << "\n";
    }
    return strippedAsm000FilePath;
}

class AsmLineProcessor : public cmajor::sbin::assembly::Visitor
{
public:
    AsmLineProcessor();
    void Visit(cmajor::sbin::assembly::InstructionNode& instructionNode) override;
    void Visit(cmajor::sbin::assembly::ContentExprNode& contentExprNode) override;
    void Visit(cmajor::sbin::assembly::BinaryExprNode& binaryExprNode) override;
    void Visit(cmajor::sbin::assembly::RegisterNode& registerNode) override;
    bool Matched() const { return matched; }
    const std::string& Line() const { return line; }
private:
    void ProcessLea(cmajor::sbin::assembly::InstructionNode& inst);
    bool matched;
    std::string line;
    int operandIndex;
    cmajor::sbin::machine_x64::Register reg;
};

AsmLineProcessor::AsmLineProcessor() : matched(false), line(), operandIndex(0), reg(cmajor::sbin::machine_x64::Register::none)
{
}

void AsmLineProcessor::Visit(cmajor::sbin::assembly::InstructionNode& instructionNode)
{
    switch (instructionNode.OpCode())
    {
        case cmajor::sbin::machine_x64::OpCode::JMP:
        case cmajor::sbin::machine_x64::OpCode::JE:
        case cmajor::sbin::machine_x64::OpCode::JNE:
        case cmajor::sbin::machine_x64::OpCode::JZ:
        case cmajor::sbin::machine_x64::OpCode::JNZ:
        case cmajor::sbin::machine_x64::OpCode::JAE:
        {
            matched = true;
            line = "JUMP";
            break;
        }
        case cmajor::sbin::machine_x64::OpCode::LEA:
        {
            ProcessLea(instructionNode);
            break;
        }
    }
}

void AsmLineProcessor::ProcessLea(cmajor::sbin::assembly::InstructionNode& inst)
{
    operandIndex = 0;
    for (const auto& operand : inst.Operands())
    {
        operand->Accept(*this);
        ++operandIndex;
    }
}

void AsmLineProcessor::Visit(cmajor::sbin::assembly::ContentExprNode& contentExprNode)
{
    contentExprNode.Operand()->Accept(*this);
}

void AsmLineProcessor::Visit(cmajor::sbin::assembly::BinaryExprNode& binaryExprNode)
{
    bool regRegLeft = false;
    bool regRegRight = false;
    if (operandIndex == 1 && binaryExprNode.Op() == cmajor::sbin::assembly::Operator::plus)
    {
        reg = cmajor::sbin::machine_x64::Register::none;
        binaryExprNode.Left()->Accept(*this);
        if (reg != cmajor::sbin::machine_x64::Register::none)
        {
            regRegLeft = true;
        }
        reg = cmajor::sbin::machine_x64::Register::none;
        binaryExprNode.Right()->Accept(*this);
        if (reg != cmajor::sbin::machine_x64::Register::none)
        {
            regRegRight = true;
        }
    }
    if (regRegLeft && regRegRight)
    {
        matched = true;
        line = "LEA REGS";
    }
}

void AsmLineProcessor::Visit(cmajor::sbin::assembly::RegisterNode& registerNode)
{
    reg = registerNode.Reg();
}

void ProcessLine(const std::string& asmLine, util::CodeFormatter& formatter, bool process)
{
    if (!process)
    {
        formatter.WriteLine(asmLine);
    }
    else
    {
        try
        {
            if (asmLine.starts_with("jne"))
            {
                formatter.WriteLine("JUMP");
            }
            else
            {
                std::unique_ptr<cmajor::sbin::assembly::Node> node = cmajor::sbin::assembly::ParseAsmLine(asmLine);
                AsmLineProcessor lineProcessor;
                node->Accept(lineProcessor);
                if (lineProcessor.Matched())
                {
                    formatter.WriteLine(lineProcessor.Line());
                }
                else
                {
                    formatter.WriteLine(asmLine);
                }
            }
        }
        catch (const std::exception&)
        {
            formatter.WriteLine(asmLine);
        }
    }
}

void MakeStrippedAsm(const std::string& file, bool verbose)
{
    std::string asm000 = MakeStrippedAsm000(file, verbose);
    std::string content = util::ReadFile(asm000);
    std::string asmFilePath = util::Path::ChangeExtension(file, ".stripped.asm");
    std::ofstream asmFile(asmFilePath);
    util::CodeFormatter formatter(asmFile);
    int state = 0;
    std::string line;
    bool processLine = false;
    for (char c : content)
    {
        switch (state)
        {
            case 0:
            {
                if (c == ' ')
                {
                    state = 2;
                }
                else if (c == '\r')
                {
                    processLine = false;
                    state = 1;
                }
                else if (c != '\n')
                {
                    line.append(1, c);
                    processLine = false;
                    state = 5;
                }
                else
                {
                    ProcessLine(line, formatter, false);
                    line.clear();
                }
                break;
            }
            case 1:
            {
                if (c == '\n')
                {
                    ProcessLine(line, formatter, processLine);
                    line.clear();
                    state = 0;
                }
                else if (c != '\r')
                {
                    line.append(1, c);
                    processLine = false;
                    state = 5;
                }
                break;
            }
            case 2:
            {
                if (c != ' ')
                {
                    if (c >= '0' && c <= '9')
                    {
                        state = 3;
                    }
                    else if (c == '\r')
                    {
                        state = 1;
                    }
                    else if (c != '\n')
                    {
                        line.append(1, c);
                    }
                    else
                    {
                        ProcessLine(line, formatter, true);
                        line.clear();
                        state = 0;
                    }
                }
                break;
            }
            case 3:
            {
                if (c == ':')
                {
                    state = 4;
                }
                break;
            }
            case 4:
            {
                if (c != ' ')
                {
                    line.append(1, c);
                    processLine = true;
                    state = 5;
                }
                break;
            }
            case 5:
            {
                if (c == '\r')
                {
                    state = 1;
                }
                else if (c != '\n')
                {
                    line.append(1, c);
                }
                else
                {
                    ProcessLine(line, formatter, processLine);
                    line.clear();
                    state = 0;
                }
                break;
            }
        }
    }
    if (!line.empty())
    {
        ProcessLine(line, formatter, true);
    }
    if (verbose)
    {
        std::cout << "==> " << asmFilePath << "\n";
    }
}

} // namespace cmajor::sbin
