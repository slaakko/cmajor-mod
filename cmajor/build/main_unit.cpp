// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.main.unit;

import cmajor.build.resources;
import cmajor.binder;
import cmajor.backend;
import cmajor.ir.emitting.context;
import cmajor.build.config;
import soul.ast.span;
import util;
import std.filesystem;

namespace cmajor::build {


void GenerateMainUnitLLvmConsole(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    std::string cmajorLibDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "lib"));
    std::string cmajorBinDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "bin"));
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    if (verbose)
    {
        util::LogMessage(rootModule->LogStreamId(), "Generating program...");
    }
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::string clangxxPath = GetClangXXPathFromBuildConfig();
    std::string clangxxVersionCommand = clangxxPath + " --version";
    util::ExecuteResult versionExecuteResult = util::Execute(clangxxVersionCommand);
    if (versionExecuteResult.exitCode != 0)
    {
        throw std::runtime_error("getting clang++ version failed with error code " + std::to_string(versionExecuteResult.exitCode) + ": " + std::move(versionExecuteResult.output));
    }
    std::string command = clangxxPath;
    command.append(" -g");
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    {
        std::ofstream mainFile(mainFilePath);
        util::CodeFormatter formatter(mainFile);
        std::string returnTypeName;
        std::string retval;
        if (returnType->IsVoidType())
        {
            returnTypeName = "void";
        }
        else if (returnType->IsIntType())
        {
            returnTypeName = "int";
            retval = "retval = ";
        }
        else
        {
            throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
        }
        std::string parameters;
        std::string arguments;
        if (userMainFunctionSymbol->Parameters().size() == 0)
        {
            parameters = "()";
            arguments = "()";
        }
        else if (userMainFunctionSymbol->Parameters().size() == 2)
        {
            parameters.append("(");
            if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
            {
                parameters.append("int argc");
            }
            else
            {
                throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
            }
            if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
            {
                parameters.append(", const char** argv");
            }
            else
            {
                throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
            }
            parameters.append(")");
            arguments = "(argc, argv)";
        }
        else
        {
            throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
        }
        formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
        formatter.WriteLine("extern \"C\" void RtmInit();");
        formatter.WriteLine("extern \"C\" void RtmDone();");
        formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
        formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
        formatter.WriteLine();
        formatter.WriteLine("int main(int argc, const char** argv)");
        formatter.WriteLine("{");
        formatter.IncIndent();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
        }
        else
        {
            formatter.WriteLine("RtmInit();");
        }
        formatter.WriteLine("int retval = 0;");
        formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
        }
        else
        {
            formatter.WriteLine("RtmDone();");
        }
        formatter.WriteLine("return retval;");
        formatter.DecIndent();
        formatter.WriteLine("}");
    }
    command.append(" ").append(mainFilePath);
    command.append(" -std=c++20");
    int n = rootModule->LibraryFilePaths().size();
    for (int i = 0; i < n; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->LibraryFilePaths()[i]));
    }
    int nr = rootModule->ResourceFilePaths().size();
    for (int i = 0; i < nr; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->ResourceFilePaths()[i]));
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrt.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom_parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.ast.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.lexer.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.util.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_processor.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xpath.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.xml.xpath.lexer.classmap.res"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "z.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zlibstat.lib"))));
    }
    else
    {
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrt.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom_parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.ast.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.lexer.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.util.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_processor.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xpath.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.xml.xpath.lexer.classmap.res"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zd.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zlibstatd.lib"))));
    }
    command.append(" -o " + util::QuotedPath(project->ExecutableFilePath()));

    util::ExecuteResult executeResult = util::Execute(command);
    if (executeResult.exitCode != 0)
    {
        throw std::runtime_error("compilation failed with error code " + std::to_string(executeResult.exitCode) + ": " + std::move(executeResult.output));
    }
}

void GenerateMainUnitSystemX(cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    cmajor::ast::CompileUnitNode mainCompileUnit(soul::ast::Span(), std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cm").generic_string());
    mainCompileUnit.SetSynthesizedUnit();
    mainCompileUnit.SetProgramMainUnit();
    mainCompileUnit.GlobalNs()->AddMember(new cmajor::ast::NamespaceImportNode(soul::ast::Span(), new cmajor::ast::IdentifierNode(soul::ast::Span(), U"System")));
    cmajor::ast::FunctionNode* mainFunction(new cmajor::ast::FunctionNode(soul::ast::Span(), cmajor::ast::Specifiers::public_, 
        new cmajor::ast::IntNode(soul::ast::Span()), U"main", nullptr));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::Span(), new cmajor::ast::IntNode(soul::ast::Span()), 
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"argc")));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::Span(), new cmajor::ast::PointerNode(soul::ast::Span(), 
        new cmajor::ast::PointerNode(soul::ast::Span(),
        new cmajor::ast::CharNode(soul::ast::Span()))), new cmajor::ast::IdentifierNode(soul::ast::Span(), U"argv")));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::Span(), new cmajor::ast::PointerNode(soul::ast::Span(), 
        new cmajor::ast::PointerNode(soul::ast::Span(),
        new cmajor::ast::CharNode(soul::ast::Span()))), new cmajor::ast::IdentifierNode(soul::ast::Span(), U"envp")));
    mainFunction->SetProgramMain();
    cmajor::ast::CompoundStatementNode* mainFunctionBody = new cmajor::ast::CompoundStatementNode(soul::ast::Span());
    cmajor::ast::ConstructionStatementNode* constructExitCode = new cmajor::ast::ConstructionStatementNode(soul::ast::Span(), 
        new cmajor::ast::IntNode(soul::ast::Span()),
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"exitCode"));
    mainFunctionBody->AddStatement(constructExitCode);
    cmajor::ast::CompoundStatementNode* tryBlock = new cmajor::ast::CompoundStatementNode(soul::ast::Span());
    cmajor::ast::InvokeNode* invokeSetupEnvironment = new cmajor::ast::InvokeNode(soul::ast::Span(), 
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"StartupSetupEnvironment"));
    invokeSetupEnvironment->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::Span(), U"envp"));
    cmajor::ast::StatementNode* callSetEnvironmentStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::Span(), invokeSetupEnvironment);
    tryBlock->AddStatement(callSetEnvironmentStatement);
    cmajor::symbols::FunctionSymbol* userMain = rootModule->GetSymbolTable().MainFunctionSymbol();
    cmajor::ast::InvokeNode* invokeMain = new cmajor::ast::InvokeNode(soul::ast::Span(), new cmajor::ast::IdentifierNode(soul::ast::Span(), userMain->GroupName()));
    if (!userMain->Parameters().empty())
    {
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::Span(), U"argc"));
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::Span(), U"argv"));
    }
    cmajor::ast::StatementNode* callMainStatement = nullptr;
    if (!userMain->ReturnType() || userMain->ReturnType()->IsVoidType())
    {
        callMainStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::Span(), invokeMain);
    }
    else
    {
        callMainStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::Span(), new cmajor::ast::IdentifierNode(soul::ast::Span(), U"exitCode"), invokeMain);
    }
    tryBlock->AddStatement(callMainStatement);
    cmajor::ast::TryStatementNode* tryStatement = new cmajor::ast::TryStatementNode(soul::ast::Span(), tryBlock);
    cmajor::ast::CompoundStatementNode* catchBlock = new cmajor::ast::CompoundStatementNode(soul::ast::Span());
    cmajor::ast::InvokeNode* consoleError = new cmajor::ast::InvokeNode(soul::ast::Span(), new cmajor::ast::DotNode(soul::ast::Span(),
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"System.Console"),
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"Error")));
    cmajor::ast::DotNode* writeLine = new cmajor::ast::DotNode(soul::ast::Span(), consoleError, new cmajor::ast::IdentifierNode(soul::ast::Span(), U"WriteLine"));
    cmajor::ast::InvokeNode* printEx = new cmajor::ast::InvokeNode(soul::ast::Span(), writeLine);
    cmajor::ast::InvokeNode* exToString = new cmajor::ast::InvokeNode(soul::ast::Span(), new cmajor::ast::DotNode(soul::ast::Span(), 
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"ex"),
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"ToString")));
    printEx->AddArgument(exToString);
    cmajor::ast::ExpressionStatementNode* printExStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::Span(), printEx);
    catchBlock->AddStatement(printExStatement);
    cmajor::ast::AssignmentStatementNode* assignExitCodeStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::Span(), 
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"exitCode"),
        new cmajor::ast::IntLiteralNode(soul::ast::Span(), 1));
    catchBlock->AddStatement(assignExitCodeStatement);
    cmajor::ast::CatchNode* catchAll = new cmajor::ast::CatchNode(soul::ast::Span(), new cmajor::ast::ConstNode(soul::ast::Span(), 
        new cmajor::ast::LValueRefNode(soul::ast::Span(),
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"System.Exception"))), new cmajor::ast::IdentifierNode(soul::ast::Span(), U"ex"), catchBlock);
    tryStatement->AddCatch(catchAll);
    mainFunctionBody->AddStatement(tryStatement);
    cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(soul::ast::Span(), 
        new cmajor::ast::IdentifierNode(soul::ast::Span(), U"exitCode"));
    mainFunctionBody->AddStatement(returnStatement);
    mainFunction->SetBody(mainFunctionBody);
    mainCompileUnit.GlobalNs()->AddMember(mainFunction);
    std::lock_guard<std::recursive_mutex> lock(rootModule->Lock());
    cmajor::symbols::SymbolCreatorVisitor symbolCreator(rootModule->GetSymbolTable());
    mainCompileUnit.Accept(symbolCreator);
    cmajor::binder::AttributeBinder attributeBinder(rootModule);
    cmajor::binder::BoundCompileUnit boundMainCompileUnit(*rootModule, &mainCompileUnit, &attributeBinder);
    boundMainCompileUnit.PushBindingTypes();
    cmajor::binder::TypeBinder typeBinder(boundMainCompileUnit);
    mainCompileUnit.Accept(typeBinder);
    boundMainCompileUnit.PopBindingTypes();
    cmajor::binder::StatementBinder statementBinder(boundMainCompileUnit);
    mainCompileUnit.Accept(statementBinder);
    if (boundMainCompileUnit.HasGotos())
    {
        cmajor::binder::AnalyzeControlFlow(boundMainCompileUnit);
    }
    cmajor::backend::BackEnd* backend = cmajor::backend::GetCurrentBackEnd();
    std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = backend->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
    std::unique_ptr<cmajor::ir::Emitter> emitter = backend->CreateEmitter(emittingContext.get());
    std::unique_ptr<cmajor::codegen::CodeGenerator> codeGenerator = backend->CreateCodeGenerator(emitter.get());
    boundMainCompileUnit.Accept(*codeGenerator);
    std::string mainObjectFilePath = boundMainCompileUnit.ObjectFilePath();
    objectFilePaths.push_back(mainObjectFilePath);
}

void GenerateMainUnitCppConsole(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    std::string cmajorLibDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "lib"));
    std::string cmajorBinDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "bin"));
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    if (verbose)
    {
        util::LogMessage(rootModule->LogStreamId(), "Generating program...");
    }
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::string gxxPath = GetGXXPathFromBuildConfig();

    std::string gxxVersionCommand = gxxPath + " --version";
    util::ExecuteResult versionExecuteResult = util::Execute(gxxVersionCommand);
    if (versionExecuteResult.exitCode != 0)
    {
        throw std::runtime_error("getting g++ version failed with error code " + std::to_string(versionExecuteResult.exitCode) + ": " + std::move(versionExecuteResult.output));
    }
    std::string command = gxxPath;
    command.append(" -g");
    command.append(" -std=c++20");
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    {
        std::ofstream mainFile(mainFilePath);
        util::CodeFormatter formatter(mainFile);
        std::string returnTypeName;
        std::string retval;
        if (returnType->IsVoidType())
        {
            returnTypeName = "void";
        }
        else if (returnType->IsIntType())
        {
            returnTypeName = "int";
            retval = "retval = ";
        }
        else
        {
            throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
        }
        std::string parameters;
        std::string arguments;
        if (userMainFunctionSymbol->Parameters().size() == 0)
        {
            parameters = "()";
            arguments = "()";
        }
        else if (userMainFunctionSymbol->Parameters().size() == 2)
        {
            parameters.append("(");
            if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
            {
                parameters.append("int argc");
            }
            else
            {
                throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
            }
            if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
            {
                parameters.append(", const char** argv");
            }
            else
            {
                throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
            }
            parameters.append(")");
            arguments = "(argc, argv)";
        }
        else
        {
            throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
        }
        formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
        formatter.WriteLine("extern \"C\" void RtmInit();");
        formatter.WriteLine("extern \"C\" void RtmDone();");
        formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
        formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
        formatter.WriteLine();
        formatter.WriteLine("int main(int argc, const char** argv)");
        formatter.WriteLine("{");
        formatter.IncIndent();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
        }
        else
        {
            formatter.WriteLine("RtmInit();");
        }
        formatter.WriteLine("int retval = 0;");
        formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
        }
        else
        {
            formatter.WriteLine("RtmDone();");
        }
        formatter.WriteLine("return retval;");
        formatter.DecIndent();
        formatter.WriteLine("}");
    }
    command.append(" ").append(mainFilePath);
    int n = rootModule->LibraryFilePaths().size();
    for (int i = n - 1; i >= 0; --i)
    {
        const std::string& libraryFilePath = rootModule->LibraryFilePaths()[i];
        command.append(" ").append(util::QuotedPath(libraryFilePath));
    }
    int nr = rootModule->ResourceFilePaths().size();
    for (int i = 0; i < nr; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->ResourceFilePaths()[i]));
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrt.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xpath.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom_parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_processor.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.lexer.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.ast.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.util.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libz.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.cpp.xml.xpath.lexer.classmap.o")));
        command.append(" -l").append("ws2_32");
    }
    else
    {
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrt.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xpath.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom_parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_processor.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.lexer.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.ast.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.util.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libzd.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.cpp.xml.xpath.lexer.classmap.o")));
        command.append(" -l").append("ws2_32");
    }
    command.append(" -o " + util::QuotedPath(project->ExecutableFilePath()));

    util::ExecuteResult executeResult = util::Execute(command);
    if (executeResult.exitCode != 0)
    {
        throw std::runtime_error("compilation failed with error code " + std::to_string(executeResult.exitCode) + ": " + std::move(executeResult.output));
    }
}

void GenerateMainUnitLLvmWindowsGUI(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    std::string cmajorLibDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "lib"));
    std::string cmajorBinDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "bin"));
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    if (verbose)
    {
        util::LogMessage(rootModule->LogStreamId(), "Generating program...");
    }
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::string clangxxPath = GetClangXXPathFromBuildConfig();
    std::string clangxxVersionCommand = clangxxPath + " --version";
    util::ExecuteResult versionExecuteResult = util::Execute(clangxxVersionCommand);
    if (versionExecuteResult.exitCode != 0)
    {
        throw std::runtime_error("getting clang++ version failed with error code " + std::to_string(versionExecuteResult.exitCode) + ": " + std::move(versionExecuteResult.output));
    }

    std::string command = clangxxPath;
    command.append(" -g");
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    {
        std::ofstream mainFile(mainFilePath);
        util::CodeFormatter formatter(mainFile);
        std::string returnTypeName;
        std::string retval;
        if (returnType->IsVoidType())
        {
            returnTypeName = "void";
        }
        else if (returnType->IsIntType())
        {
            returnTypeName = "int";
            retval = "retval = ";
        }
        else
        {
            throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
        }
        std::string parameters;
        std::string arguments;
        if (userMainFunctionSymbol->Parameters().size() == 0)
        {
            parameters = "()";
            arguments = "()";
        }
        else if (userMainFunctionSymbol->Parameters().size() == 2)
        {
            parameters.append("(");
            if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
            {
                parameters.append("int argc");
            }
            else
            {
                throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
            }
            if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
            {
                parameters.append(", const char** argv");
            }
            else
            {
                throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
            }
            parameters.append(")");
            arguments = "(argc, argv)";
        }
        else
        {
            throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
        }
        formatter.WriteLine("#include <Windows.h>");
        formatter.WriteLine();
        formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
        formatter.WriteLine("extern \"C\" void RtmInit();");
        formatter.WriteLine("extern \"C\" void RtmDone();");
        formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
        formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
        formatter.WriteLine("extern \"C\" void WinSetInstance();");
        formatter.WriteLine("extern \"C\" void WinDone();");
        formatter.WriteLine();
        formatter.WriteLine("int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)");
        formatter.WriteLine("{");
        formatter.IncIndent();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
        }
        else
        {
            formatter.WriteLine("RtmInit();");
        }
        formatter.WriteLine("WinSetInstance();");
        formatter.WriteLine("int retval = 0;");
        formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
        formatter.WriteLine("WinDone();");
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
        }
        else
        {
            formatter.WriteLine("RtmDone();");
        }
        formatter.WriteLine("return retval;");
        formatter.DecIndent();
        formatter.WriteLine("}");
    }
    command.append(" ").append(mainFilePath);
    command.append(" -std=c++20");
    int n = rootModule->LibraryFilePaths().size();
    for (int i = 0; i < n; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->LibraryFilePaths()[i]));
    }
    int nr = rootModule->ResourceFilePaths().size();
    for (int i = 0; i < nr; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->ResourceFilePaths()[i]));
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrtwin.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrt.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom_parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.ast.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.lexer.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.util.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_parser.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_processor.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xpath.release.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.xml.xpath.lexer.classmap.res"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "z.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zlibstat.lib"))));
        command.append(" ").append("-luser32");
        command.append(" ").append("-lgdi32");
        command.append(" ").append("-lshell32");
        command.append(" ").append("-ladvapi32");
        command.append(" ").append("-lcomdlg32");
        command.append(" ").append("-lole32");
    }
    else
    {
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrtwin.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.cmrt.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.dom_parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.ast.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.lexer.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.util.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_parser.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xml_processor.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "cmajor.llvm.xpath.debug.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.xml.xpath.lexer.classmap.res"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zd.lib"))));
        command.append(" ").append(util::QuotedPath(util::GetFullPath(util::Path::Combine(cmajorLibDir, "zlibstatd.lib"))));
        command.append(" ").append("-luser32");
        command.append(" ").append("-lgdi32");
        command.append(" ").append("-lshell32");
        command.append(" ").append("-ladvapi32");
        command.append(" ").append("-lcomdlg32");
        command.append(" ").append("-lole32");
    }
    command.append(" -o " + util::QuotedPath(project->ExecutableFilePath()));

    util::ExecuteResult executeResult = util::Execute(command);
    if (executeResult.exitCode != 0)
    {
        throw std::runtime_error("compilation failed with error code " + std::to_string(executeResult.exitCode) + ": " + std::move(executeResult.output));
    }
}

void GenerateMainUnitCppWindowsGUI(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    std::string cmajorLibDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "lib"));
    std::string cmajorBinDir = util::GetFullPath(util::Path::Combine(cmajor::ast::CmajorRootDir(), "bin"));
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    if (verbose)
    {
        util::LogMessage(rootModule->LogStreamId(), "Generating program...");
    }
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::string gxxPath = GetGXXPathFromBuildConfig();

    std::string gxxVersionCommand = gxxPath + " --version";
    util::ExecuteResult versionExecuteResult = util::Execute(gxxVersionCommand);
    if (versionExecuteResult.exitCode != 0)
    {
        throw std::runtime_error("getting g++ version failed with error code " + std::to_string(versionExecuteResult.exitCode) + ": " + std::move(versionExecuteResult.output));
    }
    std::string command = gxxPath;
    command.append(" -g");
    command.append(" -std=c++20");
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    {
        std::ofstream mainFile(mainFilePath);
        util::CodeFormatter formatter(mainFile);
        std::string returnTypeName;
        std::string retval;
        if (returnType->IsVoidType())
        {
            returnTypeName = "void";
        }
        else if (returnType->IsIntType())
        {
            returnTypeName = "int";
            retval = "retval = ";
        }
        else
        {
            throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
        }
        std::string parameters;
        std::string arguments;
        if (userMainFunctionSymbol->Parameters().size() == 0)
        {
            parameters = "()";
            arguments = "()";
        }
        else if (userMainFunctionSymbol->Parameters().size() == 2)
        {
            parameters.append("(");
            if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
            {
                parameters.append("int argc");
            }
            else
            {
                throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
            }
            if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
            {
                parameters.append(", const char** argv");
            }
            else
            {
                throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
            }
            parameters.append(")");
            arguments = "(argc, argv)";
        }
        else
        {
            throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
        }
        formatter.WriteLine("#include <Windows.h>");
        formatter.WriteLine();
        formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
        formatter.WriteLine("extern \"C\" void RtmInit();");
        formatter.WriteLine("extern \"C\" void RtmDone();");
        formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
        formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
        formatter.WriteLine("extern \"C\" void WinSetInstance();");
        formatter.WriteLine("extern \"C\" void WinDone();");
        formatter.WriteLine();
        formatter.WriteLine("int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)");
        formatter.WriteLine("{");
        formatter.IncIndent();
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
        }
        else
        {
            formatter.WriteLine("RtmInit();");
        }
        formatter.WriteLine("WinSetInstance();");
        formatter.WriteLine("int retval = 0;");
        formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
        formatter.WriteLine("WinDone();");
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
        {
            formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
        }
        else
        {
            formatter.WriteLine("RtmDone();");
        }
        formatter.WriteLine("return retval;");
        formatter.DecIndent();
        formatter.WriteLine("}");
    }
    command.append(" ").append(mainFilePath);
    command.append(" -std=c++20");
    int n = rootModule->LibraryFilePaths().size();
    for (int i = n - 1; i >= 0; --i)
    {
        const std::string& libraryFilePath = rootModule->LibraryFilePaths()[i];
        command.append(" ").append(util::QuotedPath(libraryFilePath));
    }
    int nr = rootModule->ResourceFilePaths().size();
    for (int i = 0; i < nr; ++i)
    {
        command.append(" ").append(util::QuotedPath(rootModule->ResourceFilePaths()[i]));
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrtwin.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrt.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xpath.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom_parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_processor.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.parser.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.lexer.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.ast.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.util.release.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libz.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.cpp.xml.xpath.lexer.classmap.o")));
        command.append(" ").append("-lws2_32");
        command.append(" ").append("-lgdiplus");
        command.append(" ").append("-luser32");
        command.append(" ").append("-lgdi32");
        command.append(" ").append("-lshell32");
        command.append(" ").append("-ladvapi32");
        command.append(" ").append("-lcomdlg32");
        command.append(" ").append("-lole32");
    }
    else
    {
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrtwin.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.cmrt.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xpath.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom_parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.dom.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.xml_processor.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.parser.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.lexer.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.ast.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libcmajor.cpp.util.debug.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "libzd.a")));
        command.append(" ").append(util::GetFullPath(util::Path::Combine(cmajorLibDir, "soul.cpp.xml.xpath.lexer.classmap.o")));
        command.append(" ").append("-lws2_32");
        command.append(" ").append("-lgdiplus");
        command.append(" ").append("-luser32");
        command.append(" ").append("-lgdi32");
        command.append(" ").append("-lshell32");
        command.append(" ").append("-ladvapi32");
        command.append(" ").append("-lcomdlg32");
        command.append(" ").append("-lole32");

    }
    command.append(" -o " + util::QuotedPath(project->ExecutableFilePath()));

    util::ExecuteResult executeResult = util::Execute(command);
    if (executeResult.exitCode != 0)
    {
        throw std::runtime_error("compilation failed with error code " + std::to_string(executeResult.exitCode) + ": " + std::move(executeResult.output));
    }
}

void GenerateMainUnitMasmConsole(cmajor::symbols::Module* rootModule, std::vector<std::string>& cppFilePaths)
{
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cppFilePaths.push_back(mainFilePath);
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    std::ofstream mainFile(mainFilePath);
    util::CodeFormatter formatter(mainFile);
    std::string returnTypeName;
    std::string retval;
    if (returnType->IsVoidType())
    {
        returnTypeName = "void";
    }
    else if (returnType->IsIntType())
    {
        returnTypeName = "int";
        retval = "retval = ";
    }
    else
    {
        throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
    }
    std::string parameters;
    std::string arguments;
    if (userMainFunctionSymbol->Parameters().size() == 0)
    {
        parameters = "()";
        arguments = "()";
    }
    else if (userMainFunctionSymbol->Parameters().size() == 2)
    {
        parameters.append("(");
        if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
        {
            parameters.append("int argc");
        }
        else
        {
            throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
        }
        if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
        {
            parameters.append(", const char** argv");
        }
        else
        {
            throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
        }
        parameters.append(")");
        arguments = "(argc, argv)";
    }
    else
    {
        throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
    }
    formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
    formatter.WriteLine("extern \"C\" void RtmInit();");
    formatter.WriteLine("extern \"C\" void RtmDone();");
    formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
    formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
    formatter.WriteLine();
    formatter.WriteLine("int main(int argc, const char** argv)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
    {
        formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
    }
    else
    {
        formatter.WriteLine("RtmInit();");
    }
    formatter.WriteLine("int retval = 0;");
    formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
    {
        formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
    }
    else
    {
        formatter.WriteLine("RtmDone();");
    }
    formatter.WriteLine("return retval;");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void GenerateMainUnitMasmWindowsGUI(cmajor::symbols::Module* rootModule, std::vector<std::string>& cppFilePaths)
{
    std::string mainFilePath = std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cpp").generic_string();
    cppFilePaths.push_back(mainFilePath);
    cmajor::symbols::FunctionSymbol* userMainFunctionSymbol = rootModule->GetSymbolTable().MainFunctionSymbol();
    if (!userMainFunctionSymbol)
    {
        throw std::runtime_error("program has no main function");
    }
    cmajor::symbols::TypeSymbol* returnType = userMainFunctionSymbol->ReturnType();
    std::ofstream mainFile(mainFilePath);
    util::CodeFormatter formatter(mainFile);
    std::string returnTypeName;
    std::string retval;
    if (returnType->IsVoidType())
    {
        returnTypeName = "void";
    }
    else if (returnType->IsIntType())
    {
        returnTypeName = "int";
        retval = "retval = ";
    }
    else
    {
        throw cmajor::symbols::Exception("'void' or 'int' return type expected", userMainFunctionSymbol->GetFullSpan());
    }
    std::string parameters;
    std::string arguments;
    if (userMainFunctionSymbol->Parameters().size() == 0)
    {
        parameters = "()";
        arguments = "()";
    }
    else if (userMainFunctionSymbol->Parameters().size() == 2)
    {
        parameters.append("(");
        if (userMainFunctionSymbol->Parameters()[0]->GetType()->IsIntType())
        {
            parameters.append("int argc");
        }
        else
        {
            throw cmajor::symbols::Exception("'int' parameter type expected", userMainFunctionSymbol->Parameters()[0]->GetFullSpan());
        }
        if (userMainFunctionSymbol->Parameters()[1]->GetType()->IsConstCharPtrPtrType())
        {
            parameters.append(", const char** argv");
        }
        else
        {
            throw cmajor::symbols::Exception("'const char**' parameter type expected", userMainFunctionSymbol->Parameters()[1]->GetFullSpan());
        }
        parameters.append(")");
        arguments = "(argc, argv)";
    }
    else
    {
        throw cmajor::symbols::Exception("either 0 or 2 parameters expected", userMainFunctionSymbol->GetFullSpan());
    }
    formatter.WriteLine("#include <Windows.h>");
    formatter.WriteLine();
    formatter.WriteLine("extern \"C\" " + returnTypeName + " " + util::ToUtf8(userMainFunctionSymbol->MangledName()) + parameters + ";");
    formatter.WriteLine("extern \"C\" void RtmInit();");
    formatter.WriteLine("extern \"C\" void RtmDone();");
    formatter.WriteLine("extern \"C\" void RtmBeginUnitTest(int numAssertions, const char* unitTestFilePath);");
    formatter.WriteLine("extern \"C\" void RtmEndUnitTest(const char* testName, int exitCode);");
    formatter.WriteLine("extern \"C\" void WinSetInstance();");
    formatter.WriteLine("extern \"C\" void WinDone();");
    formatter.WriteLine();
    formatter.WriteLine("int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)");
    formatter.WriteLine("{");
    formatter.IncIndent();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
    {
        formatter.WriteLine("RtmBeginUnitTest(" + std::to_string(cmajor::symbols::GetNumUnitTestAssertions()) + ", \"" + util::StringStr(cmajor::symbols::UnitTestFilePath()) + "\");");
    }
    else
    {
        formatter.WriteLine("RtmInit();");
    }
    formatter.WriteLine("WinSetInstance();");
    formatter.WriteLine("int retval = 0;");
    formatter.WriteLine(retval + util::ToUtf8(userMainFunctionSymbol->MangledName()) + arguments + ";");
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest))
    {
        formatter.WriteLine("RtmEndUnitTest(\"" + cmajor::symbols::UnitTestName() + "\", retval);");
    }
    else
    {
        formatter.WriteLine("RtmDone();");
    }
    formatter.WriteLine("return retval;");
    formatter.DecIndent();
    formatter.WriteLine("}");
}

void GenerateMainUnit(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths, std::vector<std::string>& cppFilePaths)
{
    std::lock_guard<std::recursive_mutex> lock(GetResourceLock());
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        case cmajor::ast::Target::winapp:
        {
            switch (cmajor::symbols::GetBackEnd())
            {
                case cmajor::symbols::BackEnd::llvm:
                {
                    GenerateMainUnitLLvmConsole(project, rootModule, objectFilePaths);
                    break;
                }
                case cmajor::symbols::BackEnd::systemx:
                {
                    GenerateMainUnitSystemX(rootModule, objectFilePaths);
                    break;
                }
                case cmajor::symbols::BackEnd::cpp:
                {
                    GenerateMainUnitCppConsole(project, rootModule, objectFilePaths);
                    break;
                }
                case cmajor::symbols::BackEnd::masm:
                {
                    GenerateMainUnitMasmConsole(rootModule, cppFilePaths);
                    break;
                }
            }
            break;
        }
        case cmajor::ast::Target::winguiapp:
        {
            switch (cmajor::symbols::GetBackEnd())
            {
                case cmajor::symbols::BackEnd::llvm:
                {
                    GenerateMainUnitLLvmWindowsGUI(project, rootModule, objectFilePaths);
                    break; 
                }
                case cmajor::symbols::BackEnd::cpp:
                {
                    GenerateMainUnitCppWindowsGUI(project, rootModule, objectFilePaths);
                    break;
                }
                case cmajor::symbols::BackEnd::masm:
                {
                    GenerateMainUnitMasmWindowsGUI(rootModule, cppFilePaths);
                    break;
                }
            }
            break;
        }
    }
}

} // namespace cmajor::build
