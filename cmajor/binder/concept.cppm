// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.concepts;

import std.core;
import cmajor.symbols;
import cmajor.ast;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;
class BoundFunction;
class BoundConstraint;

cmajor::symbols::ConceptSymbol* ResolveConceptId(cmajor::ast::ConceptIdNode* conceptIdNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope);

bool CheckConstraint(cmajor::ast::ConstraintNode* constraint, const cmajor::ast::NodeList<cmajor::ast::Node>& usingNodes, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    const std::vector<cmajor::symbols::TemplateParameterSymbol*>& templateParameters, 
    const std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*>& templateParameterMap,
    std::unique_ptr<BoundConstraint>& boundConstraint, const soul::ast::SourcePos& sourcedPos, const util::uuid& moduleId, cmajor::symbols::FunctionSymbol* viableFunction, 
    std::unique_ptr<cmajor::symbols::Exception>& conceptCheckException);

} // namespace cmajor::binder
