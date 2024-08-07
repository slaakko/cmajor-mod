// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.optimize;

import cmajor.masm.optimizer.inline_expander;
import cmajor.masm.optimizer.identity_operation_remover;

namespace cmajor::masm::optimizer {

void Optimize(cmajor::masm::intermediate::Context* intermediateContext)
{
    cmajor::masm::intermediate::Code& code = intermediateContext->GetCode();
    cmajor::masm::intermediate::Function* fn = code.FirstFunction();
    while (fn)
    {
        RemoveIdentityFunctionCalls(fn);
        InlineExpand(fn);
        fn = fn->Next();
    }
}

} // cmajor::masm::optimizer
