// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.init.done;

import cmajor.ast.node;

namespace cmajor::ast {

void Init()
{
    InitNode();
}

void Done()
{
    DoneNode();
}

} // namespace cmajor::ast
