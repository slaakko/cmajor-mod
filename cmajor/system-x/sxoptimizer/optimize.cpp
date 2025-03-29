// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.optimizer.optimize;

namespace cmajor::systemx::optimizer {

Optimizations currentOptimizations = Optimizations::none;

void Optimize(cmajor::systemx::intermediate::Context* intermediateContext)
{
    if (currentOptimizations == Optimizations::o0) return;

}

} // namespace cmajor::systemx::optimizer
