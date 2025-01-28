// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <eh/exception.hpp>
#include <typeinfo>

void* EhGetExceptionTypeId()
{
    const std::type_info& exceptionTypeId = typeid(cmajor::eh::Exception);
    const void* ti = &exceptionTypeId;
    return const_cast<void*>(ti);
}
