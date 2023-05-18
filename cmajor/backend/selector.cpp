// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.selector;

import cmajor.backend.llvm;
// import cmajor.backend.systemx;
// import cmajor.backend.cpp;

namespace cmajor::backend {

LLvmBackEnd llvmBackEnd;

BackEnd* backend = &llvmBackEnd;

BackEnd::~BackEnd()
{
}

void SetCurrentBackEnd(BackEndKind backEndKind)
{
    switch (backEndKind)
    {
        case BackEndKind::llvmBackEnd: backend = &llvmBackEnd; break;
        // todo other backends
    }
}

BackEnd* GetCurrentBackEnd()
{
    return backend;
}

} // namespace cmajor::backend
