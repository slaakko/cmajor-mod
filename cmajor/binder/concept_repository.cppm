// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.concept_repository;

import std.core;
import cmajor.ast;
import cmajor.binder.bound.constraint;

export namespace cmajor::binder {

class ConceptRepository
{
public:
    BoundConcept* GetBoundConcept(const BoundConceptKey& key) const;
    void AddBoundConcept(const BoundConceptKey& key, std::unique_ptr<BoundConcept>&& boundConcept);
private:
    std::unordered_map<BoundConceptKey, BoundConcept*, BoundConceptKeyHash> boundConceptMap;
    std::vector<std::unique_ptr<BoundConcept>> boundConcepts;
};

} // namespace cmajor::binder
