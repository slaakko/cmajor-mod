// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.visit;

import std.core;

export namespace cmajor::systemx::intermediate {

class Function;
class BasicBlock;

std::vector<BasicBlock*> Preorder(Function& function);
std::vector<BasicBlock*> Postorder(Function& function);
std::vector<BasicBlock*> ReversePostorder(Function& function);

} // cmajor::systemx::intermediate
