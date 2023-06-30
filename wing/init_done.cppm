// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.init.done;

export namespace wing {

void Init(HINSTANCE instance);
void Done();

} // wing
