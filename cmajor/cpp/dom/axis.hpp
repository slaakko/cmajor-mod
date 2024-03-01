// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_AXIS_INCLUDED
#define DOM_AXIS_INCLUDED
#include <string>

namespace soul::xml {

enum class Axis
{
    child, descendant, parent, ancestor, followingSibling, precedingSibling, following, preceding, attribute, ns, self, descendantOrSelf, ancestorOrSelf
};

std::string AxisName(Axis axis);

} // namespace soul::xml

#endif // DOM_AXIS_INCLUDED
