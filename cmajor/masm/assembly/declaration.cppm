// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.declaration;

import std.core;
import util.code.formatter;

export namespace cmajor::masm::assembly {

class Declaration
{
public:
    Declaration(const std::string& name_);
    virtual ~Declaration();
    const std::string& Name() const { return name; }
    virtual void Write(util::CodeFormatter& formatter) = 0;
private:
    std::string name;
};

} // namespace cmajor::masm::assembly
