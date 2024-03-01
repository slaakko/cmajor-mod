// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DOM_ENTITY_REFERENCE_INCLUDED
#define DOM_ENTITY_REFERENCE_INCLUDED
#include <dom/character_data.hpp>

namespace soul::xml {

class EntityReference : public CharacterData
{
public:
    EntityReference(const soul::ast::SourcePos& sourcePos_);
    EntityReference(const soul::ast::SourcePos& sourcePos_, const std::string& entityRef_);
    void Accept(Visitor& visitor) override;
    void Write(util::CodeFormatter& formatter) override;
};

EntityReference* MakeEntityReference(const std::string& entityRef);

} // namespace soul::xml

#endif // DOM_ENTITY_REFERENCE_INCLUDED
