// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.llvm.link;

import cmajor.llvm.link.windows;

namespace cmajor::llvm {

void Link(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        {
#ifdef _WIN32
            LinkWindows(project, rootModule);
#else
            // todo
#endif 
            break;
        }
    }
}

} // cmajor::llvm
