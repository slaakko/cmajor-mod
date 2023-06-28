// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module wing.init.done;

//#include <wing/WingApi.hpp>
//#include <Windows.h>

export namespace wing {

void Init(HINSTANCE instance);
void Done();
} // wing