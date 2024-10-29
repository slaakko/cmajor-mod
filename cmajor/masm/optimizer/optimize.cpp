// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.optimize;

import cmajor.masm.optimizer.inline_expander;
import cmajor.masm.optimizer.identity_operation_remover;
import cmajor.masm.optimizer.jump_optimizer;

namespace cmajor::masm::optimizer {

void Optimize(cmajor::masm::intermediate::Context* intermediateContext)
{
    cmajor::masm::intermediate::Code& code = intermediateContext->GetCode();
    cmajor::masm::intermediate::Function* fn = code.FirstFunction();
    while (fn)
    {
        //fn->Check();
        intermediateContext->IncTotalFunctions();
        RemoveIdentityFunctionCalls(fn);
        bool inlineExpanded = InlineExpand(fn);
        if (inlineExpanded)
        {
            intermediateContext->IncFunctionsInlined();
        }
        OptimizeJumps(fn);
        fn = fn->Next();
    }
}

} // cmajor::masm::optimizer
