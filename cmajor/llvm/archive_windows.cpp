// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/BinaryFormat/COFF.h>
#include <llvm/BinaryFormat/Magic.h>
#include <llvm/Object/ArchiveWriter.h>
#include <llvm/Object/COFF.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/ErrorOr.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/MemoryBufferRef.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/StringSaver.h>

module cmajor.llvm.archive.windows;

namespace cmajor::llvm {

static void fatalOpenError(::llvm::Error e, ::llvm::Twine file)
{
    if (!e) return;
    ::llvm::handleAllErrors(std::move(e), [&](const ::llvm::ErrorInfoBase& eib)
        {
            throw std::runtime_error("error opening '" + file.str() + "': " + eib.message());
        });
}

static ::llvm::Expected<::llvm::COFF::MachineTypes> getCOFFFileMachine(::llvm::MemoryBufferRef mb)
{
    std::error_code ec;
    auto obj = ::llvm::object::COFFObjectFile::create(mb);
    if (!obj)
    {
        return obj.takeError();
    }
    uint16_t machine = (*obj)->getMachine();
    if (machine != ::llvm::COFF::IMAGE_FILE_MACHINE_AMD64)
    {
        return ::llvm::createStringError(::llvm::inconvertibleErrorCode(),
            "machine not AMD64: " + std::to_string(machine));
    }
    return static_cast<::llvm::COFF::MachineTypes>(machine);
}

static void appendFile(std::vector<::llvm::NewArchiveMember>& members,
    ::llvm::COFF::MachineTypes& machine, ::llvm::MemoryBufferRef mb)
{
    ::llvm::file_magic magic = ::llvm::identify_magic(mb.getBuffer());

    if (magic != ::llvm::file_magic::coff_object)
    {
        throw std::runtime_error(mb.getBufferIdentifier().str() + ": not a COFF object");
    }
    ::llvm::Expected<::llvm::COFF::MachineTypes> maybeFileMachine = getCOFFFileMachine(mb);
    if (!maybeFileMachine)
    {
        ::llvm::handleAllErrors(maybeFileMachine.takeError(), [&](const ::llvm::ErrorInfoBase& eib) {
            throw std::runtime_error(mb.getBufferIdentifier().str() + ": " + eib.message());
            });
    }
    ::llvm::COFF::MachineTypes fileMachine = *maybeFileMachine;
    members.emplace_back(mb);
}

void ArchiveWindows(const std::string& libraryFilePath, const std::vector<std::string>& objectFilePaths)
{
    ::llvm::BumpPtrAllocator alloc;
    ::llvm::StringSaver saver(alloc);
    std::vector<std::unique_ptr<::llvm::MemoryBuffer>> mbs;
    std::vector<::llvm::NewArchiveMember> members;
    ::llvm::COFF::MachineTypes machine = ::llvm::COFF::IMAGE_FILE_MACHINE_AMD64;
    for (const auto& path : objectFilePaths)
    {
        ::llvm::ErrorOr<std::unique_ptr<::llvm::MemoryBuffer>> mOrErr = ::llvm::MemoryBuffer::getFile(path, -1, false);
        fatalOpenError(::llvm::errorCodeToError(mOrErr.getError()), path);
        ::llvm::MemoryBufferRef mbRef = (*mOrErr)->getMemBufferRef();
        appendFile(members, machine, mbRef);
        mbs.push_back(std::move(*mOrErr));
    }
    std::string outputPath = libraryFilePath;
    for (::llvm::NewArchiveMember& member : members)
    {
        if (::llvm::sys::path::is_relative(member.MemberName))
        {
            ::llvm::Expected<std::string> pathOrErr = ::llvm::computeArchiveRelativePath(outputPath, member.MemberName);
            if (pathOrErr)
            {
                member.MemberName = saver.save(*pathOrErr);
            }
        }
    }
    if (::llvm::Error e = ::llvm::writeArchive(outputPath, members,
        /*WriteSymtab=*/true, ::llvm::object::Archive::K_GNU,
        /*Deterministic*/ true, false)) {
        handleAllErrors(std::move(e), [&](const ::llvm::ErrorInfoBase& ei) {
            throw std::runtime_error(outputPath + ": " + ei.message());
            });
    }
}

} // cmajor::llvm
