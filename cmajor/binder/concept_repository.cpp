// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.concept_repository;

namespace cmajor::binder {

BoundConcept* ConceptRepository::GetBoundConcept(const BoundConceptKey& key) const
{
    auto it = boundConceptMap.find(key);
    if (it != boundConceptMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void ConceptRepository::AddBoundConcept(const BoundConceptKey& key, std::unique_ptr<BoundConcept>&& boundConcept)
{
    boundConceptMap[key] = boundConcept.get();
    boundConcepts.push_back(std::move(boundConcept));
}

} // namespace cmajor::binder
