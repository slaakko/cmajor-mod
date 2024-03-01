// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.write.pass;

import cmajor.systemx.intermediate.writer;
import cmajor.systemx.intermediate.context;
import util;

namespace cmajor::systemx::intermediate {

WritePass::WritePass() : ModulePass("write")
{
}

void WritePass::Run(Context* context)
{
    Writer writer(context, util::Path::ChangeExtension(context->FilePath(), ".si"));
    context->GetCode().VisitFunctions(writer);
}

} // cmsx::intermediate
