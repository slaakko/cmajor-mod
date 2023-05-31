// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/IR/LLVMContext.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/PassRegistry.h>
#include <llvm/InitializePasses.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/IR/Module.h>
#include <llvm/ADT/Triple.h>
#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/CodeGen.h>

module cmajor.llvm.emitting.context;

namespace cmajor::llvm {

struct LLvmEmittingContextImpl
{
    LLvmEmittingContextImpl(int optimizationLevel);
    std::unique_ptr<::llvm::LLVMContext> context;
    ::llvm::PassRegistry* passRegistry;
    std::string targetTriple;
    ::llvm::Triple triple;
    std::unique_ptr<::llvm::TargetMachine> targetMachine;
    std::unique_ptr<::llvm::DataLayout> dataLayout;
};

LLvmEmittingContextImpl::LLvmEmittingContextImpl(int optimizationLevel) : context(new ::llvm::LLVMContext()), passRegistry(nullptr)
{
    ::llvm::InitializeAllTargets();
    ::llvm::InitializeAllTargetMCs();
    ::llvm::InitializeAllAsmPrinters();
    ::llvm::InitializeAllAsmParsers();

    passRegistry = ::llvm::PassRegistry::getPassRegistry();
    initializeCore(*passRegistry);
    initializeCodeGen(*passRegistry);
    initializeLoopStrengthReducePass(*passRegistry);
    initializeLowerIntrinsicsPass(*passRegistry);
    initializeEntryExitInstrumenterPass(*passRegistry);
    initializePostInlineEntryExitInstrumenterPass(*passRegistry);
    initializeUnreachableBlockElimLegacyPassPass(*passRegistry);
    initializeConstantHoistingLegacyPassPass(*passRegistry);
    initializeScalarOpts(*passRegistry);
    initializeVectorization(*passRegistry);
    initializeScalarizeMaskedMemIntrinLegacyPassPass(*passRegistry);
    initializeExpandReductionsPass(*passRegistry);
    initializeHardwareLoopsPass(*passRegistry);
    initializeTransformUtils(*passRegistry);

    ::llvm::cl::AddExtraVersionPrinter(::llvm::TargetRegistry::printRegisteredTargetsForVersion);

#ifdef _WIN32
    targetTriple = "x86_64-pc-windows-msvc";
#else
    targetTriple = ::llvm::sys::getDefaultTargetTriple();
#endif
    triple = ::llvm::Triple(targetTriple);
    static ::llvm::codegen::RegisterCodeGenFlags registerCodeGenFlags;
    std::string error;
    const ::llvm::Target* target = ::llvm::TargetRegistry::lookupTarget(::llvm::codegen::getMArch(), triple, error);
    if (!target)
    {
        throw std::runtime_error("error: LLVM target not found: " + error);
    }

    ::llvm::TargetOptions options = ::llvm::codegen::InitTargetOptionsFromCodeGenFlags(triple);
    ::llvm::CodeGenOpt::Level optLevel = ::llvm::CodeGenOpt::None;
    switch (optimizationLevel)
    {
        case 0:
        {
            optLevel = ::llvm::CodeGenOpt::None; 
            break;
        }
        case 1:
        {
            optLevel = ::llvm::CodeGenOpt::Less;
            break;
        }
        case 2:
        {
            optLevel = ::llvm::CodeGenOpt::Default;
            break;
        }
        case 3:
        {
            optLevel = ::llvm::CodeGenOpt::Aggressive;
            break;
        }
    }
    targetMachine.reset(target->createTargetMachine(triple.getTriple(), ::llvm::codegen::getCPUStr(), ::llvm::codegen::getFeaturesStr(), options, 
        ::llvm::codegen::getExplicitRelocModel(), ::llvm::codegen::getExplicitCodeModel(), optLevel));
    if (!targetMachine)
    {
        throw std::runtime_error("error: could not allocate LLVM target machine");
    }
    dataLayout.reset(new ::llvm::DataLayout(targetMachine->createDataLayout()));
}

LLvmEmittingContext::LLvmEmittingContext(int optimizationLevel_) : optimizationLevel(optimizationLevel_), impl(new LLvmEmittingContextImpl(optimizationLevel))
{
}

LLvmEmittingContext::~LLvmEmittingContext()
{
    delete impl;
}

void* LLvmEmittingContext::NativeContext()
{
    return impl->context.get();
}

const std::string& LLvmEmittingContext::TargetTripleStr() const
{
    return impl->targetTriple;
}

void* LLvmEmittingContext::Triple() const
{
    return &(impl->triple);
}

void* LLvmEmittingContext::TargetMachine()
{
    return impl->targetMachine.get();
}

void* LLvmEmittingContext::DataLayout() const
{
    return impl->dataLayout.get();
}

} // cmajor::llvm
