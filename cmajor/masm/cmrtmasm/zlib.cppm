// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.zlib;

import std.core;

export {

extern "C" int RtmInitZlib(int mode, int level, void** handle, int& errorId);
extern "C" void RtmDoneZlib(int mode, void* handle);
extern "C" void RtmSetInputZlib(void* inChunk, uint32_t inAvail, void* handle);
extern "C" int RtmInflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, uint32_t* inAvail, void* handle, int& errorId);
extern "C" int RtmDeflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, void* handle, int32_t flush, int& errorId);

}
