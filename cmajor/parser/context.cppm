// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.parser.context;

import std.core;
import util.uuid;

export namespace cmajor::parser::context {

class Context
{
public:
    Context();
    const util::uuid& ModuleId() const { return moduleId; }
    void SetModuleId(const util::uuid& moduleId_) { moduleId = moduleId_; }
private:
    util::uuid moduleId;
};

} // namespace cmajor::parser::context
