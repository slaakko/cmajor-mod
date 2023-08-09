// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.build;

import cmajor.info;
import std.core;

export namespace cmcode {

enum class BuildRequestKind : int
{
    none = 0, build = 1 << 0, clean = 1 << 1, rebuild = 1 << 2, buildDependencies = 1 << 3
};

inline BuildRequestKind operator|(BuildRequestKind left, BuildRequestKind right)
{
    return BuildRequestKind(int(left) | int(right));
}

inline BuildRequestKind operator&(BuildRequestKind left, BuildRequestKind right)
{
    return BuildRequestKind(int(left) & int(right));
}

inline BuildRequestKind operator~(BuildRequestKind flags)
{
    return BuildRequestKind(~int(flags));
}

void StartBuild(const std::string& backend, const std::string& config, const std::string& filePath, BuildRequestKind requestKind);
void StopBuild();

} // namespace cmcode
