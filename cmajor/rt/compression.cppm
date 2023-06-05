// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.compression;

import std.core;

export extern "C" RT_API int32_t RtInitZlib(int32_t  mode, int32_t  level, void** handle);
export extern "C" RT_API void RtDoneZlib(int32_t  mode, void* handle);
export extern "C" RT_API void RtSetInputZlib(void* inChunk, uint32_t inAvail, void* handle);
export extern "C" RT_API int32_t RtDeflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t * have, uint32_t * outAvail, void* handle, int32_t flush);
export extern "C" RT_API int32_t RtInflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t * have, uint32_t * outAvail, uint32_t * inAvail, void* handle);
export extern "C" RT_API const char* RtRetvalStrZlib(int32_t retVal);
