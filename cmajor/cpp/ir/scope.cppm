// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.ir.scope;

import cmajor.debug;
import util;
import std.core;

export namespace cmajor::cpp::ir {

class Scope
{
public:
    Scope(int16_t id_, int16_t parentScopeId_);
    Scope(const Scope&) = delete;
    Scope(Scope&&) = delete;
    Scope& operator=(const Scope&) = delete;
    Scope& operator=(Scope&&) = delete;
    int16_t Id() const { return id; }
    int16_t ParentScopeId() const { return parentScopeId; }
    void AddLocalVariable(cmajor::debug::DIVariable* localVariable);  
    cmajor::debug::DIVariable* GetLocalVariable(int32_t index) const; 
    const std::vector<std::unique_ptr<cmajor::debug::DIVariable>>& LocalVariables() const { return localVariables; } 
private:
    int16_t id;
    int16_t parentScopeId;
    std::vector<std::unique_ptr<cmajor::debug::DIVariable>> localVariables; 
};

} // namespace cmajor::cpp::ir
