// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module sxgmdb.themes;

import wing;

namespace sxgmdb {

void InitThemes()
{
    wing::LoadThemes();
    wing::SetTheme("light");
}

} // namespace sxgmdb
