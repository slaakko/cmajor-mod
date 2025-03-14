// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.info.bs;

namespace cmajor::info::bs {

BuildCommand::BuildCommand() :
    verbose(false), quiet(false), clean(false), rebuild(false), emitIR(false), disableModuleCache(false),
    singleThreadedCompile(false), buildAllDependencies(false)
{
}

CompileError::CompileError() : line(0), scol(0), ecol(0)
{
}

Warning::Warning() : number(-1), line(0), scol(0), ecol(0)
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

GetDefinitionReply::GetDefinitionReply() : definitionLocation(), succeeded(true), error()
{
}

} // cmajor::info::bs
