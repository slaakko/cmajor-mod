// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.node.operation;

import std.core;
import soul.xml.node;

export namespace soul::xml {

class NodeOperation
{
public:
    virtual ~NodeOperation();
    virtual void Apply(Node* node) = 0;
};

} // namespace soul::xml
