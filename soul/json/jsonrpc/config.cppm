// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.config;

import std.core;
import std.filesystem;

export namespace soul::json::rpc {

enum class Flags : int
{
    none = 0,
    log = 1 << 0
};

constexpr Flags operator|(Flags left, Flags right)
{
    return Flags(int(left) | int(right));
}

constexpr Flags operator&(Flags left, Flags right)
{
    return Flags(int(left) & int(right));
}

constexpr Flags operator~(Flags flags)
{
    return Flags(~int(flags));
}

class Config
{
public:
    static Config& Instance();
    void SetFlag(Flags flag) { flags = flags | flag; }
    bool GetFlag(Flags flag) const { return (flags & flag) != Flags::none; }
    const std::string& LogDir() const { return logDir; }
    void SetLogDir(const std::string& logDir_);
private:
    Config();
    Flags flags;
    std::string logDir;
};


} // namespace soul::json::rpc
