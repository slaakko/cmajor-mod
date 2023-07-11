// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.evaluation.guard;

import util;
import std.core;

export namespace cmajor::debug {

class Debugger;

struct EvaluationGuard
{
    EvaluationGuard(Debugger& debugger_);
    ~EvaluationGuard();
    Debugger& debugger;
    util::JsonValue* value;
};

} // namespace cmajor::debug
