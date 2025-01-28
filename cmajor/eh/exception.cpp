// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.eh.exception;

void* EhGetExceptionTypeId()
{
    const std::type_info& exceptionTypeId = typeid(cmajor::eh::Exception);
    const void* ti = &exceptionTypeId;
    return const_cast<void*>(ti);
}
