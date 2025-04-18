// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module wing.cursor;

import wing.base;
import util;

namespace wing {

Cursor::Cursor() : handle(nullptr), standard(false)
{
}

Cursor::Cursor(HCURSOR handle_) : handle(handle_), standard(false)
{
}

Cursor::Cursor(HCURSOR handle_, bool standard_) : handle(handle_), standard(standard_)
{
}

Cursor::Cursor(Cursor&& that) noexcept : handle(that.handle), standard(that.standard)
{
    that.handle = nullptr;
    that.standard = false;
}

Cursor& Cursor::operator=(Cursor&& that) noexcept
{
    std::swap(handle, that.handle);
    std::swap(standard, that.standard);
    return *this;
}

Cursor::~Cursor()
{
    if (handle && !standard)
    {
        DestroyCursor(handle);
    }
}

Cursor LoadStandardCursor(StandardCursorId standardCursorId)
{
    HCURSOR handle = LoadCursor(nullptr, MAKEINTRESOURCE(static_cast<int>(standardCursorId)));
    if (!handle)
    {
        throw WindowsException(GetLastError());
    }
    return Cursor(handle, true);
}

Cursor LoadCursorByName(const std::string& cursorName)
{
    std::u16string name = util::ToUtf16(cursorName);
    HANDLE handle = LoadImage(Instance(), LPCWSTR(name.c_str()), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
    if (!handle)
    {
        throw WindowsException(GetLastError());
    }
    return Cursor(static_cast<HCURSOR>(handle), false);
}

void SetCursor(const Cursor& cursor)
{
    ::SetCursor(cursor.Handle());
}

} // wing
