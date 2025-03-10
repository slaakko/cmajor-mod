// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.compare_utils.object.file.dump;

import cmajor.masm.build;
import util;

namespace cmajor::sbin {

void MakeObjectFileDumpBat(const std::string& file, const std::string& batFile)
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
    formatter.WriteLine("dumpbin /all /disasm /out:\"" + file + ".txt" + "\"" + " \"" + file + "\"");
}

void MakeObjectFileDump(const std::string& file, bool verbose)
{
    if (verbose)
    {
        std::cout << "> " << file << "\n";
    }
    std::string batFile = util::Path::Combine(util::Path::GetDirectoryName(file), "make_object_file_dump.bat");
    MakeObjectFileDumpBat(file, batFile);
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
}

} // namespace cmajor::sbin
