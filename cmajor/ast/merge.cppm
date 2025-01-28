export module cmajor.ast.merge;

// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export namespace cmajor::ast {

class CompileUnitNode;

void Merge(CompileUnitNode& source, CompileUnitNode& target);

} // namespace cmajor::ast