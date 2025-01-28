// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.overload.resolution;

import cmajor.ast;
import std.core;
import cmajor.symbols;
import cmajor.ir;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundExpression;
class BoundFunctionCall;
class BoundCompileUnit;
class BoundFunction;
class BoundConstraint;

enum class OverloadResolutionFlags : uint8_t
{
    none = 0,
    dontThrow = 1 << 0,
    dontInstantiate = 1 << 1,
    noTemplates = 1 << 2,
    noRvalueRef = 1 << 3,
    includeSystemDefaultFunctions = 1 << 4
};

constexpr OverloadResolutionFlags operator&(OverloadResolutionFlags left, OverloadResolutionFlags right)
{
    return OverloadResolutionFlags(uint8_t(left) & uint8_t(right));
}

constexpr OverloadResolutionFlags operator|(OverloadResolutionFlags left, OverloadResolutionFlags right)
{
    return OverloadResolutionFlags(uint8_t(left) | uint8_t(right));
}

struct FunctionScopeLookup
{
    FunctionScopeLookup(cmajor::symbols::ScopeLookup scopeLookup_) : scopeLookup(scopeLookup_), scope(nullptr) {}
    FunctionScopeLookup(cmajor::symbols::ScopeLookup scopeLookup_, cmajor::symbols::ContainerScope* scope_) : scopeLookup(scopeLookup_), scope(scope_) {}
    cmajor::symbols::ScopeLookup scopeLookup;
    cmajor::symbols::ContainerScope* scope;
};

struct ArgumentMatch
{
    ArgumentMatch() : preReferenceConversionFlags(cmajor::ir::OperationFlags::none), conversionFun(nullptr), 
        postReferenceConversionFlags(cmajor::ir::OperationFlags::none), conversionDistance(0) {}
    ArgumentMatch(cmajor::ir::OperationFlags preReferenceConversionFlags_, cmajor::symbols::FunctionSymbol* conversionFun_, cmajor::ir::OperationFlags postReferenceConversionFlags_, 
        int conversionDistance_) :
        preReferenceConversionFlags(preReferenceConversionFlags_), conversionFun(conversionFun_), postReferenceConversionFlags(postReferenceConversionFlags_), 
        conversionDistance(conversionDistance_) {}
    cmajor::ir::OperationFlags preReferenceConversionFlags;
    cmajor::symbols::FunctionSymbol* conversionFun;
    cmajor::ir::OperationFlags postReferenceConversionFlags;
    int conversionDistance;
};

inline bool BetterArgumentMatch(const ArgumentMatch& left, const ArgumentMatch& right)
{
    if (left.preReferenceConversionFlags == cmajor::ir::OperationFlags::none && right.preReferenceConversionFlags != cmajor::ir::OperationFlags::none) return true;
    if (left.preReferenceConversionFlags != cmajor::ir::OperationFlags::none && right.preReferenceConversionFlags == cmajor::ir::OperationFlags::none) return false;
    if (left.conversionFun == nullptr && right.conversionFun != nullptr) return true;
    if (right.conversionFun == nullptr && left.conversionFun != nullptr) return false;
    if (left.postReferenceConversionFlags == cmajor::ir::OperationFlags::none && right.postReferenceConversionFlags != cmajor::ir::OperationFlags::none) return true;
    if (left.postReferenceConversionFlags != cmajor::ir::OperationFlags::none && right.postReferenceConversionFlags == cmajor::ir::OperationFlags::none) return false;
    if (left.conversionDistance < right.conversionDistance) return true;
    if (left.conversionDistance > right.conversionDistance) return false;
    return false;
}

struct FunctionMatch
{
    FunctionMatch(cmajor::symbols::FunctionSymbol* fun_) :
        fun(fun_), numConversions(0), numQualifyingConversions(0), referenceMustBeInitialized(false), castRequired(false), cannotBindConstArgToNonConstParam(false), 
        cannotAssignToConstObject(false), sourceType(nullptr), targetType(nullptr), conceptCheckException(nullptr), boundConstraint(nullptr) {}
    cmajor::symbols::FunctionSymbol* fun;
    std::vector<ArgumentMatch> argumentMatches;
    int numConversions;
    int numQualifyingConversions;
    bool referenceMustBeInitialized;
    bool castRequired;
    bool cannotBindConstArgToNonConstParam;
    bool cannotAssignToConstObject;
    cmajor::symbols::TypeSymbol* sourceType;
    cmajor::symbols::TypeSymbol* targetType;
    std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*> templateParameterMap;
    cmajor::symbols::Exception* conceptCheckException;
    BoundConstraint* boundConstraint;
};

struct BetterFunctionMatch
{
    bool operator()(const FunctionMatch& left, const FunctionMatch& right) const;
};

bool FindConversions(BoundCompileUnit& boundCompileUnit, cmajor::symbols::FunctionSymbol* function, std::vector<std::unique_ptr<BoundExpression>>& arguments, 
    FunctionMatch& functionMatch, cmajor::symbols::ConversionType conversionType, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
    cmajor::ast::Node* node);

std::unique_ptr<BoundFunctionCall> ResolveOverload(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, 
    const std::vector<FunctionScopeLookup>& functionScopeLookups,
    std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundCompileUnit& boundCompileUnit, BoundFunction* currentFunction, 
    cmajor::ast::Node* node);

std::unique_ptr<BoundFunctionCall> ResolveOverload(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, 
    const std::vector<FunctionScopeLookup>& functionScopeLookups,
    std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundCompileUnit& boundCompileUnit, BoundFunction* currentFunction, 
    cmajor::ast::Node* node,
    OverloadResolutionFlags flags, std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes, std::unique_ptr<cmajor::symbols::Exception>& exception);

} // namespace cmajor::binder
