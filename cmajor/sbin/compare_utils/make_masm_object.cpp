// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.compare_utils.masm.object;

import cmajor.masm.build;
import util;
import std.filesystem;

namespace cmajor::sbin {

std::string MakeMasmObjectBat(const std::string& file, const std::string& batFile)
{
    std::string masmAsmFilePath = util::Path::ChangeExtension(file, ".masm.asm");
    std::ofstream makeBat(batFile);
    std::string vcvars64Path = cmajor::masm::build::GetVCVars64Path();
    if (vcvars64Path.empty())
    {
        throw std::runtime_error("vcvars64.bat not configured");
    }
    util::CodeFormatter formatter(makeBat);
    formatter.WriteLine("@echo off");
    formatter.WriteLine("call \"" + vcvars64Path + "\"");
    formatter.WriteLine("ml64 /c " + util::Path::GetFileName(masmAsmFilePath));
    std::string objectFilePath = util::Path::ChangeExtension(file, ".masm.obj");
    formatter.WriteLine("dumpbin /all /disasm /out:" + util::Path::GetFileName(objectFilePath) + ".txt" + " " + util::Path::GetFileName(objectFilePath));
    return objectFilePath;
}

std::string MakeMasmObject(const std::string& file, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << file << "\n";
    }
    std::string batFile = util::Path::Combine(util::Path::GetDirectoryName(file), "make_masm_object.bat");
    std::string objectFilePath = MakeMasmObjectBat(file, batFile);
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
    return objectFilePath;
}

} // namespace cmajor::sbin
