// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.selector;

import cmajor.backend.llvm;
import cmajor.backend.systemx;
import cmajor.backend.cpp;
import cmajor.backend.masm;
import cmajor.backend.sbin;

namespace cmajor::backend {

LLvmBackEnd llvmBackEnd;
SystemXBackEnd systemxBackEnd;
CppBackEnd cppBackEnd;
MasmBackEnd masmBackEnd;
SBinBackEnd sbinBackEnd;
BackEnd* backend = &llvmBackEnd;

BackEnd::~BackEnd()
{
}

void SetCurrentBackEnd(BackEndKind backEndKind)
{
    switch (backEndKind)
    {
        case BackEndKind::llvmBackEnd: backend = &llvmBackEnd; break;
        case BackEndKind::systemxBackEnd: backend = &systemxBackEnd; break;
        case BackEndKind::cppBackEnd: backend = &cppBackEnd; break;
        case BackEndKind::masmBackEnd: backend = &masmBackEnd; break;
        case BackEndKind::sbinBackEnd: backend = &sbinBackEnd; break;
    }
}

BackEnd* GetCurrentBackEnd()
{
    return backend;
}

} // namespace cmajor::backend
