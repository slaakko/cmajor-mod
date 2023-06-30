// =================================
// Copyright (c) 2022 Seppo Laakko
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
