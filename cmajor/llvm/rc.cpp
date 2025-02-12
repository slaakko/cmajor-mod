// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#pragma warning(disable : 4996)
#include <llvm/Support/LLVMDriver.h>
#pragma warning(default : 4996)

module cmajor.llvm.rc;

int llvm_rc_main(int argc, const char** argv, const llvm::ToolContext&);

namespace cmajor::llvm {

void ResourceCompile(const std::string& rcFilePath, const std::string& resourceFilePath)
{
    const char* resFilePath = resourceFilePath.c_str();
    const char* scriptFilePath = rcFilePath.c_str();
    const char* argv[] = { "llvm-rc", "/V", "/FO", resFilePath, scriptFilePath };
    int retCode = llvm_rc_main(5, argv, { argv[0], nullptr, false });
    if (retCode != 0)
    {
        throw std::runtime_error("error compiling resources in '" + rcFilePath + "', return value is " + std::to_string(retCode));
    }
}

} // namespace cmajor::llvm
