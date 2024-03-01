// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.binary.resource.ptr;

import std.core;

export namespace wing {

class BinaryResourcePtr
{
public:
    BinaryResourcePtr(const std::string& moduleName, const std::string& resourceName_);
    const std::string& ResourceName() const { return resourceName; }
    uint8_t* Data() const { return data; }
    int64_t Size() const { return size; }
private:
    std::string resourceName;
    uint8_t* data;
    int64_t size;
};

} // wing
