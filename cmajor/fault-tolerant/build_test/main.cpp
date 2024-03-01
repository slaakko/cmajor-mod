// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.parser;
import cmajor.fault.tolerant.symbols;
import cmajor.fault.tolerant.binder;
import cmajor.fault.tolerant.build;
import util;
import soul.ast.span;
import std.core;

int main()
{
    try
    {
        util::Init();
        cmajor::fault::tolerant::build::BuildSystem();
        //cmajor::fault::tolerant::build::ReadSystem();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    util::Done();
    return 0;
}
