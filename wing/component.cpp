// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.component;

namespace wing {

Component::Component() : container(nullptr), nextSibling(nullptr), prevSibling(nullptr)
{
}

Component::~Component()
{
}

} // wing
