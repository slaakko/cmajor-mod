// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.info.bs;

namespace cmajor::info::bs {

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

DefinitionSourceLocation::DefinitionSourceLocation() : file(), line(0), scol(0)
{
}

GetDefinitionRequest::GetDefinitionRequest() : projectName(), projectFilePath(), backend(), config(), identifier(), identifierLocation()
{
}

GetDefinitionReply::GetDefinitionReply() : definitionLocation()
{
}

} // cmajor::info::bs
