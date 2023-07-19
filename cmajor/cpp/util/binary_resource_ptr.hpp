// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_BINARY_RESOURCE_PTR_INCLUDED
#define UTIL_BINARY_RESOURCE_PTR_INCLUDED
#include <string>
#include <stdint.h>

namespace util {

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

} // util

#endif // UTIL_BINARY_RESOURCE_PTR_INCLUDED
