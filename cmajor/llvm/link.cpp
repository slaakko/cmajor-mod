// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.llvm.link;

import cmajor.llvm.link.windows;

namespace cmajor::llvmlink {

void Link(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        case cmajor::ast::Target::winapp:
        case cmajor::ast::Target::winguiapp:
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

} // cmajor::llvmlink
