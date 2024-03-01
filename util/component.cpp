// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.component;

namespace util {

Component::Component() : container(nullptr), nextSibling(nullptr), prevSibling(nullptr)
{
}

Component::~Component()
{
}

} // namespace util
