// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>

module cmajor.masm.win.rt.error;

import cmajor.masm.rt;
import util;

namespace cmajor::masm::win::rt {

void FatalError(const std::string& errorMessage)
{
    int errorId = cmajor::masm::rt::AllocateError(errorMessage);
    const char* msg = RtmGetErrorMessage(errorId);
    std::u16string text = util::ToUtf16(msg);
    MessageBoxW(nullptr, (LPWSTR)text.c_str(), (LPWSTR)u"fatal error", 0);
    exit(1);
}

void FatalGdiPlusError(Gdiplus::Status status)
{
    switch (status)
    {
        case Gdiplus::Status::GenericError:
        {
            FatalError("GDI+: generic error");
            break;
        }
        case Gdiplus::Status::InvalidParameter:
        {
            FatalError("GDI+: invalid parameter");
            break;
        }
        case Gdiplus::Status::OutOfMemory:
        {
            FatalError("GDI+: out of memory");
            break;
        }
        case Gdiplus::Status::ObjectBusy:
        {
            FatalError("GDI+: object busy");
            break;
        }
        case Gdiplus::Status::InsufficientBuffer:
        {
            FatalError("GDI+: insufficien buffer");
            break;
        }
        case Gdiplus::Status::NotImplemented:
        {
            FatalError("GDI+: not implemented");
            break;
        }
        case Gdiplus::Status::Win32Error:
        {
            FatalError("GDI+: WIN32 error");
            break;
        }
        case Gdiplus::Status::WrongState:
        {
            FatalError("GDI+: wrong state");
            break;
        }
        case Gdiplus::Status::Aborted:
        {
            FatalError("GDI+: aborted");
            break;
        }
        case Gdiplus::Status::FileNotFound:
        {
            FatalError("GDI+: file not found");
            break;
        }
        case Gdiplus::Status::ValueOverflow:
        {
            FatalError("GDI+: value overflow");
            break;
        }
        case Gdiplus::Status::AccessDenied:
        {
            FatalError("GDI+: access denied");
            break;
        }
        case Gdiplus::Status::UnknownImageFormat:
        {
            FatalError("GDI+: unknown image format");
            break;
        }
        case Gdiplus::Status::FontFamilyNotFound:
        {
            FatalError("GDI+: font family not found");
            break;
        }
        case Gdiplus::Status::FontStyleNotFound:
        {
            FatalError("GDI+: font style not found");
            break;
        }
        case Gdiplus::Status::NotTrueTypeFont:
        {
            FatalError("GDI+: not TrueType font");
            break;
        }
        case Gdiplus::Status::UnsupportedGdiplusVersion:
        {
            FatalError("GDI+: unsupported GDI+ version");
            break;
        }
        case Gdiplus::Status::GdiplusNotInitialized:
        {
            FatalError("GDI+: GDI+ not initialized");
            break;
        }
        case Gdiplus::Status::PropertyNotFound:
        {
            FatalError("GDI+: property not found");
            break;
        }
        case Gdiplus::Status::PropertyNotSupported:
        {
            FatalError("GDI+: property not supported");
            break;
        }
        default:
        {
            FatalError("GDI+: unknown error");
            break;
        }
    }
}

} // namespace cmajor::masm::win::rt

int WinAllocateWindowsError(const char* errorMessage, uint64_t errorCode)
{
    std::unique_ptr<char16_t> buffer(new char16_t[4096]);
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, (DWORD)errorCode, LANG_SYSTEM_DEFAULT,
        (LPWSTR)buffer.get(), 4096, nullptr);
    std::u16string text(buffer.get());
    std::string msg(std::string(errorMessage) + ": " + util::ToUtf8(text));
    return RtmAllocateError(msg.c_str());
}

int WinAllocateGdiPlusError(const char* errorMessage, int gdiplusStatus)
{
    switch (gdiplusStatus)
    {
    case Gdiplus::Status::GenericError:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: generic error";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::InvalidParameter:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: invalid parameter";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::OutOfMemory:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: out of memory";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::ObjectBusy:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: object busy";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::InsufficientBuffer:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: insufficient buffer";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::NotImplemented:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: not implemented";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::Win32Error:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: WIN32 error";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::WrongState:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: wrong state";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::Aborted:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: aborted";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::FileNotFound:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: file not found";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::ValueOverflow:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: value overflow";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::AccessDenied:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: access denied";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::UnknownImageFormat:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: unknown image format";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::FontFamilyNotFound:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: font family not found";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::FontStyleNotFound:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: font style not found";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::NotTrueTypeFont:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: not TrueType font";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::UnsupportedGdiplusVersion:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: unsupported GDI+ version";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::GdiplusNotInitialized:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: GDI+ not initialized";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::PropertyNotFound:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: GDI+ property not found";
        return RtmAllocateError(msg.c_str());
    }
    case Gdiplus::Status::PropertyNotSupported:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: GDI+ property not supported ";
        return RtmAllocateError(msg.c_str());
    }
    default:
    {
        std::string msg = std::string(errorMessage) + ": GDI+ error: unknown error";
        return RtmAllocateError(msg.c_str());
    }
    }
}

void WinFatalError(const char* errorMessage)
{
    cmajor::masm::win::rt::FatalError(errorMessage);
}

void WinFatalGdiplusError(int gdiplusStatus)
{
    cmajor::masm::win::rt::FatalGdiPlusError(Gdiplus::Status(gdiplusStatus));
}

uint64_t WinGetLastError()
{
    return GetLastError();
}

void WinFormatMessage(uint64_t errorCode, char16_t* buffer)
{
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, (DWORD)errorCode, LANG_SYSTEM_DEFAULT,
        (LPWSTR)buffer, 4096, nullptr);
}

void WinFatalWindowsError(uint64_t errorCode)
{
    std::unique_ptr<char16_t> buffer(new char16_t[4096]);
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, (DWORD)errorCode, LANG_SYSTEM_DEFAULT,
        (LPWSTR)buffer.get(), 4096, nullptr);
    std::u16string text(buffer.get());
    cmajor::masm::win::rt::FatalError(util::ToUtf8(text));
}

