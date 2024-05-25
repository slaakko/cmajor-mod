// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/zlib.hpp>
#include <cmrt/error.hpp>
#include <util/zlib_interface.h>
#ifdef NDEBUG
#pragma comment(lib, "zlibstat.lib")
#else 
#pragma comment(lib, "zlibstatd.lib")
#endif

int RtmInitZlib(int mode, int level, void** handle, int& errorId)
{
    errorId = 0;
    int ret = zlib_init(mode, level, handle);
    if (ret < 0)
    {
        errorId = cmajor::cpp::rt::AllocateError("could not create deflate stream: zlib initialization returned error code " +
            std::to_string(ret) + ": " + zlib_retval_str(ret));
    }
    return ret;
}

void RtmDoneZlib(int mode, void* handle)
{
    zlib_done(mode, handle);
}

void RtmSetInputZlib(void* inChunk, uint32_t inAvail, void* handle)
{
    zlib_set_input(inChunk, inAvail, handle);
}

int RtmInflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, uint32_t* inAvail, void* handle, int& errorId)
{
    errorId = 0;
    int ret = zlib_inflate(outChunk, outChunkSize, have, outAvail, inAvail, handle);
    if (ret < 0)
    {
        std::string errorMessage = "deflate stream: could not decompress: inflate returned error code " + std::to_string(ret) + ": " + zlib_retval_str(ret);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    }
    return ret;
}

int RtmDeflateZlib(void* outChunk, uint32_t outChunkSize, uint32_t* have, uint32_t* outAvail, void* handle, int32_t flush, int& errorId)
{
    errorId = 0;
    int ret = zlib_deflate(outChunk, outChunkSize, have, outAvail, handle, flush);
    if (ret < 0)
    {
        std::string errorMessage = "deflate stream: could not compress: deflate returned error code " + std::to_string(ret) + ": " + zlib_retval_str(ret);
        errorId = cmajor::cpp::rt::AllocateError(errorMessage);
    }
    return ret;
}
