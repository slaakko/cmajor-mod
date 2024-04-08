// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.optimize;

import cmajor.masm.optimizer.inline_expander;

namespace cmajor::masm::optimizer {

void Optimize(cmajor::masm::intermediate::Context* intermediateContext)
{
    cmajor::masm::intermediate::Code& code = intermediateContext->GetCode();
    cmajor::masm::intermediate::Function* fn = code.FirstFunction();
    while (fn)
    {
        InlineExpand(fn);
        fn = fn->Next();
    }
}

} // cmajor::masm::optimizer
