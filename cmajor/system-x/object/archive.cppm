// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.archive;

import std.core;

export namespace cmajor::systemx::object {

void CreateArchive(int logStreamId, const std::string& archiveFilePath, const std::vector<std::string>& objectFilePaths, bool verbose);

} // namespace cmajor::systemx::object
