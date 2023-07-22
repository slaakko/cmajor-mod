// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.binary.resource.ptr;

import std.core;

export namespace util {

enum class ResourceFlags
{
    none = 0, loadLibraryAsDataFile = 1 << 0
};

constexpr ResourceFlags operator|(ResourceFlags left, ResourceFlags right)
{
    return ResourceFlags(int(left) | int(right));
}

constexpr ResourceFlags operator&(ResourceFlags left, ResourceFlags right)
{
    return ResourceFlags(int(left) & int(right));
}

constexpr ResourceFlags operator~(ResourceFlags flags)
{
    return ResourceFlags(~int(flags));
}

class BinaryResourcePtr
{
public:
    BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_);
    BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_, ResourceFlags flags);
    const std::string& ResourceName() const { return resourceName; }
    uint8_t* Data() const { return data; }
    int64_t Size() const { return size; }
private:
    std::string resourceName;
    uint8_t* data;
    int64_t size;
};

} // util
