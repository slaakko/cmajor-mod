// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.evaluation.guard;

import cmajor.debug.debugger;

namespace cmajor::debug {

EvaluationGuard::EvaluationGuard(Debugger& debugger_) : debugger(debugger_), value(debugger.ReleaseResult())
{
}

EvaluationGuard::~EvaluationGuard()
{
    debugger.ResetResult(value);
}

} // namespace cmajor::debug
