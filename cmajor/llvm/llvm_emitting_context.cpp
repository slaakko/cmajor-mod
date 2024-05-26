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
#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/IR/DebugInfoMetadata.h>

module cmajor.llvm.emitting.context;

namespace cmllvm {

struct LLVMEmittingContextImpl
{
    LLVMEmittingContextImpl(int optimizationLevel);
    llvm::PassRegistry* passRegistry;
    std::string targetTriple;
    llvm::Triple triple;
    std::unique_ptr<llvm::TargetMachine> targetMachine;
    std::unique_ptr<llvm::DataLayout> dataLayout;
};

LLVMEmittingContextImpl::LLVMEmittingContextImpl(int optimizationLevel) : passRegistry(nullptr)
{
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmPrinters();
    llvm::InitializeAllAsmParsers();

    passRegistry = llvm::PassRegistry::getPassRegistry();
    initializeCore(*passRegistry);
    initializeCodeGen(*passRegistry);

    initializeLoopStrengthReducePass(*passRegistry);
    initializeLowerIntrinsicsPass(*passRegistry);
    //initializeEntryExitInstrumenterPass(*passRegistry);
    //initializePostInlineEntryExitInstrumenterPass(*passRegistry);
    initializeUnreachableBlockElimLegacyPassPass(*passRegistry);
    initializeConstantHoistingLegacyPassPass(*passRegistry);
    initializeScalarOpts(*passRegistry);
    initializeVectorization(*passRegistry);
    initializeScalarizeMaskedMemIntrinLegacyPassPass(*passRegistry);
    initializeExpandReductionsPass(*passRegistry);
    //initializeHardwareLoopsPass(*passRegistry);
    initializeTransformUtils(*passRegistry);

    //llvm::cl::AddExtraVersionPrinter(llvm::TargetRegistry::printRegisteredTargetsForVersion);

#ifdef _WIN32
    targetTriple = "x86_64-pc-windows-msvc";
#else
    targetTriple = llvm::sys::getDefaultTargetTriple();
#endif
    triple = llvm::Triple(targetTriple);
    static llvm::codegen::RegisterCodeGenFlags registerCodeGenFlags;
    std::string error;
    const llvm::Target* target = llvm::TargetRegistry::lookupTarget(llvm::codegen::getMArch(), triple, error);
    if (!target)
    {
        throw std::runtime_error("error: LLVM target not found: " + error);
    }

    llvm::TargetOptions options = llvm::codegen::InitTargetOptionsFromCodeGenFlags(triple);
    llvm::CodeGenOpt::Level optLevel = llvm::CodeGenOpt::None;
    switch (optimizationLevel)
    {
    case 0:
    {
        optLevel = llvm::CodeGenOpt::None;
        break;
    }
    case 1:
    {
        optLevel = llvm::CodeGenOpt::Less;
        break;
    }
    case 2:
    {
        optLevel = llvm::CodeGenOpt::Default;
        break;
    }
    case 3:
    {
        optLevel = llvm::CodeGenOpt::Aggressive;
        break;
    }
    }
    targetMachine.reset(target->createTargetMachine(triple.getTriple(), llvm::codegen::getCPUStr(), llvm::codegen::getFeaturesStr(), options,
        llvm::codegen::getExplicitRelocModel(), llvm::codegen::getExplicitCodeModel(), optLevel));
    if (!targetMachine)
    {
        throw std::runtime_error("error: could not allocate LLVM target machine");
    }
    dataLayout.reset(new llvm::DataLayout(targetMachine->createDataLayout()));
}

LLVMEmittingContext::LLVMEmittingContext(int optimizationLevel_) : optimizationLevel(optimizationLevel_), impl(new LLVMEmittingContextImpl(optimizationLevel))
{
}

LLVMEmittingContext::~LLVMEmittingContext()
{
    delete impl;
}

const std::string& LLVMEmittingContext::TargetTripleStr() const
{
    return impl->targetTriple;
}

void* LLVMEmittingContext::Triple() const
{
    return &(impl->triple);
}

void* LLVMEmittingContext::TargetMachine()
{
    return impl->targetMachine.get();
}

void* LLVMEmittingContext::DataLayout() const
{
    return impl->dataLayout.get();
}

} // namespace cmllvm
