// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API int32_t RtInitZlib(int32_t  mode, int32_t  level, void** handle);
extern "C" RT_API void RtDoneZlib(int32_t  mode, void* handle);
extern "C" RT_API void RtSetInputZlib(void* inChunk, uint32_t inAvail, void* handle);
extern "C" RT_API int32_t RtDeflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t * have, uint32_t * outAvail, void* handle, int32_t flush);
extern "C" RT_API int32_t RtInflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t * have, uint32_t * outAvail, uint32_t * inAvail, void* handle);
extern "C" RT_API const char* RtRetvalStrZlib(int32_t retVal);

#endif // COMPRESSION_HPP
