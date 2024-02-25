// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.selector;

//import cmajor.backend.llvm;
import cmajor.backend.systemx;
import cmajor.backend.cpp;

namespace cmajor::backend {

//LLvmBackEnd llvmBackEnd;
SystemXBackEnd systemxBackEnd;
CppBackEnd cppBackEnd;

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
    }
}

BackEnd* GetCurrentBackEnd()
{
    return backend;
}

} // namespace cmajor::backend
