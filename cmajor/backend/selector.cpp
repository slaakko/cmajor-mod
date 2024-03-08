// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.selector;

//import cmajor.backend.llvm;
import cmajor.backend.systemx;
import cmajor.backend.cpp;
import cmajor.backend.masm;

namespace cmajor::backend {

//LLvmBackEnd llvmBackEnd;
SystemXBackEnd systemxBackEnd;
CppBackEnd cppBackEnd;
MasmBackEnd masmBackEnd;

//BackEnd* backend = &llvmBackEnd;
BackEnd* backend = &cppBackEnd;

BackEnd::~BackEnd()
{
}

void SetCurrentBackEnd(BackEndKind backEndKind)
{
    switch (backEndKind)
    {
        //case BackEndKind::llvmBackEnd: backend = &llvmBackEnd; break;
        case BackEndKind::systemxBackEnd: backend = &systemxBackEnd; break;
        case BackEndKind::cppBackEnd: backend = &cppBackEnd; break;
        case BackEndKind::masmBackEnd: backend = &masmBackEnd; break;
    }
}

BackEnd* GetCurrentBackEnd()
{
    return backend;
}

} // namespace cmajor::backend
