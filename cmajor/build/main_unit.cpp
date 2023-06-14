// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.main.unit;

import cmajor.binder;
import cmajor.backend;

namespace cmajor::build {

void GenerateMainUnitLLvm(cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    cmajor::ast::CompileUnitNode mainCompileUnit(soul::ast::SourcePos(), util::nil_uuid(), 
        std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cm").generic_string());
    mainCompileUnit.SetSynthesizedUnit();
    mainCompileUnit.SetProgramMainUnit();
    mainCompileUnit.GlobalNs()->AddMember(new cmajor::ast::NamespaceImportNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System")));
    mainCompileUnit.GlobalNs()->AddMember(cmajor::symbols::MakePolymorphicClassArray(rootModule->GetSymbolTable().PolymorphicClasses(), U"@polymorphicClassArray"));
    mainCompileUnit.GlobalNs()->AddMember(cmajor::symbols::MakeStaticClassArray(rootModule->GetSymbolTable().ClassesHavingStaticConstructor(), U"@staticClassArray"));
    cmajor::ast::FunctionNode* mainFunction(new cmajor::ast::FunctionNode(soul::ast::SourcePos(), util::nil_uuid(), cmajor::ast::Specifiers::public_, 
        new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()), U"main", nullptr));
#ifndef _WIN32
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argc")));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::CharNode(soul::ast::SourcePos(), util::nil_uuid()))), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argv")));
#endif
    mainFunction->SetProgramMain();
    cmajor::ast::CompoundStatementNode* mainFunctionBody = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    cmajor::ast::ConstructionStatementNode* constructExitCode = new cmajor::ast::ConstructionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"));
    mainFunctionBody->AddStatement(constructExitCode);
    cmajor::ast::ExpressionStatementNode* rtInitCall = nullptr;
    cmajor::ast::InvokeNode* invokeRtInit = new cmajor::ast::InvokeNode(
        soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RtInit"));
    invokeRtInit->AddArgument(new cmajor::ast::DivNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), 
            new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"@polymorphicClassArray"),
            new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"Length"))),
        new cmajor::ast::LongLiteralNode(soul::ast::SourcePos(), util::nil_uuid(), 4))); // 4 64-bit integers per entry
    invokeRtInit->AddArgument(new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"@polymorphicClassArray"), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"CBegin"))));
    invokeRtInit->AddArgument(new cmajor::ast::DivNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), 
            new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"@staticClassArray"),
            new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"Length"))),
        new cmajor::ast::LongLiteralNode(soul::ast::SourcePos(), util::nil_uuid(), 2))); // 2 64-bit integers per entry
    invokeRtInit->AddArgument(new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"@staticClassArray"), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"CBegin"))));
    invokeRtInit->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"GlobalInitCompileUnits"));
    rtInitCall = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), invokeRtInit);
    mainFunctionBody->AddStatement(rtInitCall);
#ifdef _WIN32
    cmajor::ast::ConstructionStatementNode* argc = new cmajor::ast::ConstructionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argc"));
    argc->AddArgument(new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RtArgc")));
    mainFunctionBody->AddStatement(argc);
    cmajor::ast::ConstructionStatementNode* argv = new cmajor::ast::ConstructionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::ConstNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(), 
            new cmajor::ast::CharNode(soul::ast::SourcePos(), util::nil_uuid())))),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argv"));
    argv->AddArgument(new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RtArgv")));
    mainFunctionBody->AddStatement(argv);
#endif
    cmajor::ast::CompoundStatementNode* tryBlock = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    if (!rootModule->GetSymbolTable().JsonClasses().empty())
    {
        cmajor::ast::ExpressionStatementNode* registerJsonClassesCall = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(),
            new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RegisterJsonClasses")));
        tryBlock->AddStatement(registerJsonClassesCall);
    }
    cmajor::symbols::FunctionSymbol* userMain = rootModule->GetSymbolTable().MainFunctionSymbol();
    cmajor::ast::InvokeNode* invokeMain = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), userMain->GroupName()));
    if (!userMain->Parameters().empty())
    {
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argc"));
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argv"));
    }
    cmajor::ast::StatementNode* callMainStatement = nullptr;
    if (!userMain->ReturnType() || userMain->ReturnType()->IsVoidType())
    {
        callMainStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), invokeMain);
    }
    else
    {
        callMainStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
            new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"), invokeMain);
    }
    cmajor::ast::InvokeNode* invokeInitialize = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"Initialize"));
    cmajor::ast::StatementNode* callInitializeStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), invokeInitialize);
    tryBlock->AddStatement(callInitializeStatement);
    tryBlock->AddStatement(callMainStatement);
    cmajor::ast::TryStatementNode* tryStatement = new cmajor::ast::TryStatementNode(soul::ast::SourcePos(), util::nil_uuid(), tryBlock);
    cmajor::ast::CompoundStatementNode* catchBlock = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    cmajor::ast::InvokeNode* consoleError = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System.Console"), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"Error")));
    cmajor::ast::DotNode* writeLine = new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), consoleError, 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"WriteLine"));
    cmajor::ast::InvokeNode* printEx = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), writeLine);
    cmajor::ast::InvokeNode* exToString = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ex"),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ToString")));
    printEx->AddArgument(exToString);
    cmajor::ast::ExpressionStatementNode* printExStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), printEx);
    catchBlock->AddStatement(printExStatement);
    cmajor::ast::AssignmentStatementNode* assignExitCodeStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"),
        new cmajor::ast::IntLiteralNode(soul::ast::SourcePos(), util::nil_uuid(), 1));
    catchBlock->AddStatement(assignExitCodeStatement);
    cmajor::ast::CatchNode* catchAll = new cmajor::ast::CatchNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::ConstNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::LValueRefNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System.Exception"))), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ex"), catchBlock);
    tryStatement->AddCatch(catchAll);
    mainFunctionBody->AddStatement(tryStatement);
    cmajor::ast::ExpressionStatementNode* rtDoneCall = nullptr;
    rtDoneCall = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RtDone")));
    mainFunctionBody->AddStatement(rtDoneCall);
    cmajor::ast::InvokeNode* exitCall = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"RtExit"));
    exitCall->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"));
    cmajor::ast::ExpressionStatementNode* rtExitCall = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), exitCall);
    mainFunctionBody->AddStatement(rtExitCall);
    cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"));
    mainFunctionBody->AddStatement(returnStatement);
    mainFunction->SetBody(mainFunctionBody);
    mainCompileUnit.GlobalNs()->AddMember(mainFunction);
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
        AnalyzeControlFlow(boundMainCompileUnit);
    }
    cmajor::backend::BackEnd* backend = cmajor::backend::GetCurrentBackEnd();
    std::unique_ptr<cmajor::ir::EmittingContext> emittingContext = backend->CreateEmittingContext(cmajor::symbols::GetOptimizationLevel());
    std::unique_ptr<cmajor::ir::Emitter> emitter = backend->CreateEmitter(emittingContext.get());
    std::unique_ptr<cmajor::codegen::CodeGenerator> codeGenerator = backend->CreateCodeGenerator(emitter.get());
    boundMainCompileUnit.Accept(*codeGenerator);
    objectFilePaths.push_back(boundMainCompileUnit.ObjectFilePath());
}

void GenerateMainUnitSystemX(cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    cmajor::ast::CompileUnitNode mainCompileUnit(soul::ast::SourcePos(), util::nil_uuid(), std::filesystem::path(rootModule->OriginalFilePath()).parent_path().append("__main__.cm").generic_string());
    mainCompileUnit.SetSynthesizedUnit();
    mainCompileUnit.SetProgramMainUnit();
    mainCompileUnit.GlobalNs()->AddMember(new cmajor::ast::NamespaceImportNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System")));
    cmajor::ast::FunctionNode* mainFunction(new cmajor::ast::FunctionNode(soul::ast::SourcePos(), util::nil_uuid(), cmajor::ast::Specifiers::public_, 
        new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()), U"main", nullptr));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argc")));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::CharNode(soul::ast::SourcePos(), util::nil_uuid()))), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argv")));
    mainFunction->AddParameter(new cmajor::ast::ParameterNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::PointerNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::CharNode(soul::ast::SourcePos(), util::nil_uuid()))), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"envp")));
    mainFunction->SetProgramMain();
    cmajor::ast::CompoundStatementNode* mainFunctionBody = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    cmajor::ast::ConstructionStatementNode* constructExitCode = new cmajor::ast::ConstructionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IntNode(soul::ast::SourcePos(), util::nil_uuid()),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"));
    mainFunctionBody->AddStatement(constructExitCode);
    cmajor::ast::CompoundStatementNode* tryBlock = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    cmajor::ast::InvokeNode* invokeSetupEnvironment = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"StartupSetupEnvironment"));
    invokeSetupEnvironment->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"envp"));
    cmajor::ast::StatementNode* callSetEnvironmentStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), invokeSetupEnvironment);
    tryBlock->AddStatement(callSetEnvironmentStatement);
    cmajor::symbols::FunctionSymbol* userMain = rootModule->GetSymbolTable().MainFunctionSymbol();
    cmajor::ast::InvokeNode* invokeMain = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), userMain->GroupName()));
    if (!userMain->Parameters().empty())
    {
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argc"));
        invokeMain->AddArgument(new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"argv"));
    }
    cmajor::ast::StatementNode* callMainStatement = nullptr;
    if (!userMain->ReturnType() || userMain->ReturnType()->IsVoidType())
    {
        callMainStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), invokeMain);
    }
    else
    {
        callMainStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"), invokeMain);
    }
    tryBlock->AddStatement(callMainStatement);
    cmajor::ast::TryStatementNode* tryStatement = new cmajor::ast::TryStatementNode(soul::ast::SourcePos(), util::nil_uuid(), tryBlock);
    cmajor::ast::CompoundStatementNode* catchBlock = new cmajor::ast::CompoundStatementNode(soul::ast::SourcePos(), util::nil_uuid());
    cmajor::ast::InvokeNode* consoleError = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System.Console"),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"Error")));
    cmajor::ast::DotNode* writeLine = new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), consoleError, new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"WriteLine"));
    cmajor::ast::InvokeNode* printEx = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), writeLine);
    cmajor::ast::InvokeNode* exToString = new cmajor::ast::InvokeNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::DotNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ex"),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ToString")));
    printEx->AddArgument(exToString);
    cmajor::ast::ExpressionStatementNode* printExStatement = new cmajor::ast::ExpressionStatementNode(soul::ast::SourcePos(), util::nil_uuid(), printEx);
    catchBlock->AddStatement(printExStatement);
    cmajor::ast::AssignmentStatementNode* assignExitCodeStatement = new cmajor::ast::AssignmentStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"),
        new cmajor::ast::IntLiteralNode(soul::ast::SourcePos(), util::nil_uuid(), 1));
    catchBlock->AddStatement(assignExitCodeStatement);
    cmajor::ast::CatchNode* catchAll = new cmajor::ast::CatchNode(soul::ast::SourcePos(), util::nil_uuid(), new cmajor::ast::ConstNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::LValueRefNode(soul::ast::SourcePos(), util::nil_uuid(),
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"System.Exception"))), new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"ex"), catchBlock);
    tryStatement->AddCatch(catchAll);
    mainFunctionBody->AddStatement(tryStatement);
    cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(soul::ast::SourcePos(), util::nil_uuid(), 
        new cmajor::ast::IdentifierNode(soul::ast::SourcePos(), util::nil_uuid(), U"exitCode"));
    mainFunctionBody->AddStatement(returnStatement);
    mainFunction->SetBody(mainFunctionBody);
    mainCompileUnit.GlobalNs()->AddMember(mainFunction);
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

void GenerateMainUnit(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        {
            switch (cmajor::symbols::GetBackEnd())
            {
                case cmajor::symbols::BackEnd::llvm:
                {
                    GenerateMainUnitLLvm(rootModule, objectFilePaths);
                    break;
                }
                case cmajor::symbols::BackEnd::systemx:
                {
                    GenerateMainUnitSystemX(rootModule, objectFilePaths);
                    break;
                }
            }
            break;
        }
    }
}

} // namespace cmajor::build
