// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.archive;

import cmajor.systemx.object.binary.file;
import std.filesystem;
import util;

namespace cmajor::systemx::object {

void CreateArchive(int logStreamId, const std::string& archiveFilePath, const std::vector<std::string>& objectFilePaths, bool verbose)
{
    std::unique_ptr<ArchiveFile> archiveFile(new ArchiveFile(archiveFilePath));
    archiveFile->CreateSections();
    int n = objectFilePaths.size();
    for (int i = 0; i < n; ++i)
    {
        if (verbose)
        {
            util::LogMessage(logStreamId, "> " + objectFilePaths[i]);
        }
        if (std::filesystem::exists(objectFilePaths[i]))
        {
            std::unique_ptr<BinaryFile> binaryFile(ReadBinaryFile(objectFilePaths[i]));
            if (binaryFile->Kind() == BinaryFileKind::objectFile)
            {
                binaryFile->SetParent(archiveFile.get());
                archiveFile->AddObjectFile(static_cast<ObjectFile*>(binaryFile.release()));
            }
            else
            {
                throw std::runtime_error("object file expected: " + objectFilePaths[i]);
            }
        }
    }
    archiveFile->WriteFile();
    if (verbose)
    {
        util::LogMessage(logStreamId, "==> " + archiveFilePath);
    }
}

} // namespace cmajor::systemx::object
