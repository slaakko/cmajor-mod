// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.optimize;

import cmajor.masm.optimizer.dead_code_elimination;
import cmajor.masm.optimizer.locals;
import cmajor.masm.optimizer.arithmetics;
import cmajor.masm.optimizer.inline_expander;
import cmajor.masm.optimizer.identity_operation_remover;
import cmajor.masm.optimizer.jump_optimizer;
import util;

namespace cmajor::masm::optimizer {

Optimizations currentOptimizations = Optimizations::o2;

void Optimize(cmajor::masm::intermediate::Context* intermediateContext)
{
    cmajor::masm::intermediate::Code& code = intermediateContext->GetCode();
    cmajor::masm::intermediate::Function* fn = code.FirstFunction();
    while (fn)
    {
        cmajor::masm::intermediate::Function* next = fn->Next();
        if (fn->IsDefined())
        {
            try
            {
                std::unique_ptr<cmajor::masm::intermediate::Function> clone(fn->Clone());
                try
                {
                    intermediateContext->IncTotalFunctions();
                    if (HasOptimization(Optimizations::deadCodeElimination))
                    {
                        DeadCodeElimination(fn);
                    }
                    if (HasOptimization(Optimizations::identity))
                    {
                        RemoveIdentityFunctionCalls(fn);
                    }
                    if (HasOptimization(Optimizations::inln))
                    {
                        bool inlineExpanded = InlineExpand(fn);
                        if (inlineExpanded)
                        {
                            intermediateContext->IncFunctionsInlined();
                        }
                    }
                    if (HasOptimization(Optimizations::moveLocalsToEntryBlock))
                    {
                        MoveLocalsToEntryBlock(fn);
                    }
                    if (HasOptimization(Optimizations::arithmetic))
                    {
                        OptimizeArithmetics(fn, intermediateContext);
                    }
                    if (HasOptimization(Optimizations::jump))
                    {
                        OptimizeJumps(fn);
                    }
                    if (HasOptimization(Optimizations::deadCodeElimination))
                    {
                        DeadCodeElimination(fn);
                    }
                }
                catch (std::exception& ex)
                {
                    util::LogMessage(-1, "error optimizing function '" + fn->Name() + "': " + ex.what() + "; using unoptimized function");
                    code.Functions()->InsertBefore(clone.release(), fn);
                    code.Functions()->RemoveChild(fn);
                }
            }
            catch (std::exception& ex)
            {
                util::LogMessage(-1, "error cloning function '" + fn->Name() + "': " + ex.what());
            }
        }
        fn = next;
    }
}

} // cmajor::masm::optimizer
