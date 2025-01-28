// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_NODE_OPERATION_INCLUDED
#define DOM_NODE_OPERATION_INCLUDED
#include <dom/node.hpp>

namespace soul::xml {

class NodeOperation
{
public:
    virtual ~NodeOperation();
    virtual void Apply(Node* node) = 0;
};

} // namespace soul::xml

#endif // DOM_NODE_OPERATION_INCLUDED
