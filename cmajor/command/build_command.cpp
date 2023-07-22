// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.command;

namespace cmajor::command {

BuildCommand::BuildCommand() :
    verbose(false), quiet(false), clean(false), rebuild(false), emitIR(false), linkWithDebugRuntime(false), disableModuleCache(false),
    singleThreadedCompile(false), buildAllDependencies(false)
{
}

CompileError::CompileError() : line(0), scol(0), ecol(0)
{
}

BuildResult::BuildResult()
{
}

} // cmajor::command
