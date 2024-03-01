// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.bitmaps;

import wing;
import std.core;

export namespace cmajor::view {

wing::Bitmap* GetBitmap(const std::string& bitmapName);
void ClearBitmaps();

} // namespace cmajor::view
