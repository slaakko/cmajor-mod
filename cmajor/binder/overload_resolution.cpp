// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.overload.resolution;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.bound.function;
import cmajor.binder.bound.constraint;
import cmajor.binder.concepts;
import cmajor.binder.type.binder;
import cmajor.binder.bound_class;
import cmajor.binder.bound.expression;
import util;

namespace cmajor::binder {

bool BetterFunctionMatch::operator()(const FunctionMatch& left, const FunctionMatch& right) const
{
    int leftBetterArgumentMatches = 0;
    int rightBetterArgumentMatches = 0;
    int n = std::max(int(left.argumentMatches.size()), int(right.argumentMatches.size()));
    for (int i = 0; i < n; ++i)
    {
        ArgumentMatch leftMatch;
        if (i < int(left.argumentMatches.size()))
        {
            leftMatch = left.argumentMatches[i];
        }
        ArgumentMatch rightMatch;
        if (i < int(right.argumentMatches.size()))
        {
            rightMatch = right.argumentMatches[i];
        }
        if (BetterArgumentMatch(leftMatch, rightMatch))
        {
            ++leftBetterArgumentMatches;
        }
        else if (BetterArgumentMatch(rightMatch, leftMatch))
        {
            ++rightBetterArgumentMatches;
        }
    }
    if (leftBetterArgumentMatches > rightBetterArgumentMatches)
    {
        return true;
    }
    if (rightBetterArgumentMatches > leftBetterArgumentMatches)
    {
        return false;
    }
    if (left.numConversions < right.numConversions)
    {
        return true;
    }
    if (left.numConversions > right.numConversions)
    {
        return false;
    }
    if (left.numQualifyingConversions < right.numQualifyingConversions)
    {
        return true;
    }
    if (left.numQualifyingConversions > right.numQualifyingConversions)
    {
        return false;
    }
    if (!left.fun->IsFunctionTemplate() && right.fun->IsFunctionTemplate())
    {
        return true;
    }
    if (left.fun->IsFunctionTemplate() && !right.fun->IsFunctionTemplate())
    {
        return false;
    }
    if (!left.fun->IsTemplateSpecialization() && right.fun->IsTemplateSpecialization())
    {
        return true;
    }
    if (left.fun->IsTemplateSpecialization() && !right.fun->IsTemplateSpecialization())
    {
        return false;
    }
    if (left.boundConstraint && !right.boundConstraint)
    {
        return true;
    }
    if (!left.boundConstraint && right.boundConstraint)
    {
        return false;
    }
    if (left.boundConstraint && right.boundConstraint)
    {
        bool leftSubsumeRight = left.boundConstraint->Subsume(right.boundConstraint);
        bool rightSubsumeLeft = right.boundConstraint->Subsume(left.boundConstraint);
        if (leftSubsumeRight && !rightSubsumeLeft)
        {
            return true;
        }
        if (rightSubsumeLeft && !leftSubsumeRight)
        {
            return false;
        }
    }
    return false;
}

bool FindQualificationConversion(cmajor::symbols::TypeSymbol* sourceType, cmajor::symbols::TypeSymbol* targetType, BoundExpression* argument, cmajor::symbols::ConversionType conversionType, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    FunctionMatch& functionMatch, ArgumentMatch& argumentMatch)
{
    int distance = 0;
    if (argumentMatch.conversionFun)
    {
        distance = argumentMatch.conversionFun->ConversionDistance();
    }
    if (targetType->IsRvalueReferenceType() && !sourceType->IsRvalueReferenceType())
    {
        ++functionMatch.numQualifyingConversions;
    }
    if (sourceType->IsConstType())
    {
        if (targetType->IsConstType() || !targetType->IsReferenceType())
        {
            ++distance;
        }
        else if (conversionType == cmajor::symbols::ConversionType::implicit_)
        {
            if (sourceType->PointerCount() < 1)
            {
                functionMatch.cannotBindConstArgToNonConstParam = true;
                functionMatch.sourceType = sourceType;
                functionMatch.targetType = targetType;
                return false;
            }
            else
            {
                ++distance;
            }
        }
        else
        {
            distance = 255;
        }
    }
    else
    {
        if (targetType->IsConstType())
        {
            distance += 2;
        }
        else
        {
            distance += 3;
        }
    }
    if (sourceType->IsReferenceType() && !targetType->IsReferenceType())
    {
        argumentMatch.postReferenceConversionFlags = cmajor::ir::OperationFlags::deref;
        argumentMatch.conversionDistance = distance;
        ++functionMatch.numQualifyingConversions;
        return true;
    }
    else if (!sourceType->IsReferenceType() && (targetType->IsReferenceType() || targetType->IsClassTypeSymbol() || targetType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol))
    {
        if (targetType->IsConstType() || targetType->IsClassTypeSymbol() || targetType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
        {
            argumentMatch.postReferenceConversionFlags = cmajor::ir::OperationFlags::addr;
            argumentMatch.conversionDistance = distance;
            ++functionMatch.numQualifyingConversions;
            return true;
        }
        else if ((!sourceType->IsConstType() || sourceType->PointerCount() >= 1) && argument->IsLvalueExpression())
        {
            if (targetType->IsRvalueReferenceType() && !sourceType->IsRvalueReferenceType())
            {
                if (argument->GetFlag(BoundExpressionFlags::bindToRvalueReference))
                {
                    distance = 0;
                }
                else
                {
                    distance += 10;
                }
            }
            argumentMatch.postReferenceConversionFlags = cmajor::ir::OperationFlags::addr;
            argumentMatch.conversionDistance = distance;
            ++functionMatch.numQualifyingConversions;
            return true;
        }
        else
        {
            functionMatch.cannotBindConstArgToNonConstParam = true;
            functionMatch.sourceType = sourceType;
            functionMatch.targetType = targetType;
        }
    }
    else if (sourceType->IsConstType() && !targetType->IsConstType())
    {
        ++functionMatch.numQualifyingConversions;
        ++distance;
        if (sourceType->IsLvalueReferenceType() && targetType->IsRvalueReferenceType())
        {
            ++distance;
            ++functionMatch.numQualifyingConversions;
        }
        argumentMatch.conversionDistance = distance;
        return true;
    }
    else if (!sourceType->IsConstType() && targetType->IsConstType())
    {
        ++functionMatch.numQualifyingConversions;
        ++distance;
        if (sourceType->IsLvalueReferenceType() && targetType->IsRvalueReferenceType())
        {
            ++distance;
            ++functionMatch.numQualifyingConversions;
        }
        argumentMatch.conversionDistance = distance;
        return true;
    }
    else if (sourceType->IsLvalueReferenceType() && targetType->IsRvalueReferenceType())
    {
        ++distance;
        ++functionMatch.numQualifyingConversions;
        argumentMatch.conversionDistance = distance;
        return true;
    }
    else if (sourceType->IsRvalueReferenceType() && targetType->IsLvalueReferenceType())
    {
        ++distance;
        ++functionMatch.numQualifyingConversions;
        argumentMatch.conversionDistance = distance;
        return true;
    }
    else if (argumentMatch.conversionFun)
    {
        argumentMatch.conversionDistance = distance;
        return true;
    }
    return false;
}

bool FindTemplateParameterMatch(cmajor::symbols::TypeSymbol* sourceType, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ConversionType conversionType, BoundExpression* argument,
    BoundCompileUnit& boundCompileUnit, FunctionMatch& functionMatch, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, 
    const util::uuid& moduleId)
{
    if (targetType->BaseType()->GetSymbolType() != cmajor::symbols::SymbolType::templateParameterSymbol) return false;
    cmajor::symbols::TemplateParameterSymbol* templateParameter = static_cast<cmajor::symbols::TemplateParameterSymbol*>(targetType->BaseType());
    cmajor::symbols::TypeSymbol* templateArgumentType = nullptr;
    auto it = functionMatch.templateParameterMap.find(templateParameter);
    if (it == functionMatch.templateParameterMap.cend())
    {
        templateArgumentType = sourceType->RemoveDerivations(targetType->DerivationRec(), sourcePos, moduleId);
        if (templateArgumentType)
        {
            functionMatch.templateParameterMap[templateParameter] = templateArgumentType;
        }
        else
        {
            return false;
        }
    }
    else
    {
        templateArgumentType = it->second;
    }
    targetType = targetType->Unify(templateArgumentType, sourcePos, moduleId);
    if (!targetType)
    {
        return false;
    }
    if (cmajor::symbols::TypesEqual(sourceType, targetType))
    {
        functionMatch.argumentMatches.push_back(ArgumentMatch());
        return true;
    }
    else
    {
        bool qualificationConversionMatch = false;
        ArgumentMatch argumentMatch;
        if (cmajor::symbols::TypesEqual(sourceType->PlainType(sourcePos, moduleId), targetType->PlainType(sourcePos, moduleId)))
        {
            qualificationConversionMatch = FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch);
        }
        if (qualificationConversionMatch)
        {
            functionMatch.argumentMatches.push_back(argumentMatch);
            return true;
        }
        else
        {
            cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(sourceType, targetType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (conversionFun)
            {
                if (conversionFun->GetConversionType() == conversionType || conversionFun->GetConversionType() == cmajor::symbols::ConversionType::implicit_)
                {
                    ++functionMatch.numConversions;
                    argumentMatch.conversionFun = conversionFun;
                    //if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::none) bug???
                    if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::none)
                    {
                        if (FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (FindQualificationConversion(conversionFun->ConversionSourceType(), targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool FindClassTemplateSpecializationMatch(cmajor::symbols::TypeSymbol* sourceType, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ConversionType conversionType, BoundExpression* argument,
    BoundCompileUnit& boundCompileUnit, FunctionMatch& functionMatch, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (targetType->BaseType()->GetSymbolType() != cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        return false;
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* targetClassTemplateSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(targetType->BaseType());
    int n = targetClassTemplateSpecialization->TemplateArgumentTypes().size();
    int numArgumentMatches = functionMatch.argumentMatches.size();
    if (sourceType->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        cmajor::symbols::ClassTemplateSpecializationSymbol* sourceClassTemplateSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(sourceType->BaseType());
        int m = sourceClassTemplateSpecialization->TemplateArgumentTypes().size();
        if (n != m) return false;
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TypeSymbol* sourceArgumentType = sourceClassTemplateSpecialization->TemplateArgumentTypes()[i];
            cmajor::symbols::TypeSymbol* targetArgumentType = targetClassTemplateSpecialization->TemplateArgumentTypes()[i];
            if (FindTemplateParameterMatch(sourceArgumentType, targetArgumentType, conversionType, argument, boundCompileUnit, functionMatch, containerScope, currentFunction, sourcePos, moduleId))
            {
                continue;
            }
            else if (FindClassTemplateSpecializationMatch(sourceArgumentType, targetArgumentType, conversionType, argument, boundCompileUnit, functionMatch, containerScope, currentFunction, sourcePos, moduleId))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    functionMatch.argumentMatches.resize(numArgumentMatches);
    std::vector<cmajor::symbols::TypeSymbol*> targetTemplateArguments;
    for (int i = 0; i < n; ++i)
    {
        cmajor::symbols::TypeSymbol* templateArgumentType = targetClassTemplateSpecialization->TemplateArgumentTypes()[i]->BaseType()->UnifyTemplateArgumentType(
            boundCompileUnit.GetSymbolTable(), functionMatch.templateParameterMap, sourcePos, moduleId);
        if (templateArgumentType)
        {
            targetTemplateArguments.push_back(templateArgumentType);
        }
        else
        {
            return false;
        }
    }
    cmajor::symbols::TypeSymbol* plainTargetType = boundCompileUnit.GetSymbolTable().MakeClassTemplateSpecialization(targetClassTemplateSpecialization->GetClassTemplate(), targetTemplateArguments, sourcePos,
        cmajor::symbols::GetRootModuleForCurrentThread()->Id());
    targetType = boundCompileUnit.GetSymbolTable().MakeDerivedType(plainTargetType, targetType->DerivationRec(), sourcePos, moduleId);
    if (cmajor::symbols::TypesEqual(sourceType, targetType))
    {
        functionMatch.argumentMatches.push_back(ArgumentMatch());
        return true;
    }
    else
    {
        bool qualificationConversionMatch = false;
        ArgumentMatch argumentMatch;
        if (cmajor::symbols::TypesEqual(sourceType->PlainType(sourcePos, moduleId), targetType->PlainType(sourcePos, moduleId)))
        {
            qualificationConversionMatch = FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch);
        }
        if (qualificationConversionMatch)
        {
            functionMatch.argumentMatches.push_back(argumentMatch);
            return true;
        }
        else
        {
            cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(sourceType, targetType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
            if (conversionFun)
            {
                if (conversionFun->GetConversionType() == conversionType || conversionFun->GetConversionType() == cmajor::symbols::ConversionType::implicit_)
                {
                    argumentMatch.conversionFun = conversionFun;
                    ++functionMatch.numConversions;
                    if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::none)
                    {
                        if (FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        if (FindQualificationConversion(conversionFun->ConversionSourceType(), targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                        {
                            functionMatch.argumentMatches.push_back(argumentMatch);
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool FindConversions(BoundCompileUnit& boundCompileUnit, cmajor::symbols::FunctionSymbol* function, std::vector<std::unique_ptr<BoundExpression>>& arguments, FunctionMatch& functionMatch,
    cmajor::symbols::ConversionType conversionType, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (!currentFunction)
    {
        if (function->IsProject() && !function->IsBound())
        {
            cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(function);
            if (node)
            {
                TypeBinder typeBinder(boundCompileUnit);
                typeBinder.SetContainerScope(containerScope);
                typeBinder.SetCurrentFunctionSymbol(function);
                node->Accept(typeBinder);
            }
        }
    }
    int arity = arguments.size();
    if (arity == 1 && function->GroupName() == U"@constructor" && arguments[0]->GetType()->IsReferenceType())
    {
        functionMatch.referenceMustBeInitialized = true;
        return false;
    }
    int n = std::min(arity, function->Arity());
    if (!function->IsVarArg())
    {
        // Assert(arity == function->Arity(), "wrong arity"); TODO
    }
    for (int i = 0; i < n; ++i)
    {
        BoundExpression* argument = arguments[i].get();
        cmajor::symbols::TypeSymbol* sourceType = argument->GetType();
        if (sourceType->RemoveConst(sourcePos, moduleId)->IsBasicTypeSymbol())
        {
            sourceType = sourceType->RemoveConst(sourcePos, moduleId);
        }
        cmajor::symbols::ParameterSymbol* parameter = function->Parameters()[i];
        cmajor::symbols::TypeSymbol* targetType = parameter->GetType();
        if (arity == 2 && function->GroupName() == U"operator=")
        {
            if (targetType->IsRvalueReferenceType() && !sourceType->IsRvalueReferenceType() && !argument->GetFlag(BoundExpressionFlags::bindToRvalueReference))
            {
                ++functionMatch.numQualifyingConversions;
            }
            if (i == 0)
            {
                if (targetType->IsConstType() && targetType->PointerCount() <= 1)
                {
                    functionMatch.cannotAssignToConstObject = true;
                    return false;
                }
            }
            if (i == 0 && cmajor::symbols::TypesEqual(sourceType, targetType))    // exact match
            {
                if (sourceType->IsReferenceType() && !function->IsConstructorDestructorOrNonstaticMemberFunction())
                {
                    functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, nullptr, cmajor::ir::OperationFlags::deref, 1));
                    ++functionMatch.numConversions;
                    continue;
                }
            }
            else if (i == 1 && sourceType->IsReferenceType() && targetType->IsLvalueReferenceType() && cmajor::symbols::TypesEqual(sourceType->RemoveReference(sourcePos, moduleId)->RemoveConst(sourcePos, moduleId), targetType->RemoveReference(sourcePos, moduleId)))
            {
                if (!function->IsConstructorDestructorOrNonstaticMemberFunction())
                {
                    functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, nullptr, cmajor::ir::OperationFlags::deref, 1));
                    ++functionMatch.numConversions;
                    continue;
                }
            }
            else if (i == 1 && sourceType->IsReferenceType() && targetType->IsRvalueReferenceType() && cmajor::symbols::TypesEqual(sourceType->RemoveReference(sourcePos, moduleId), targetType->RemoveReference(sourcePos, moduleId)))
            {
                if (!function->IsConstructorDestructorOrNonstaticMemberFunction())
                {
                    functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, nullptr, cmajor::ir::OperationFlags::none, 0));
                    continue;
                }
            }
            else if (i == 1 && !sourceType->IsReferenceType() && argument->IsLvalueExpression() && targetType->IsReferenceType() && cmajor::symbols::TypesEqual(sourceType, targetType->RemoveReference(sourcePos, moduleId)))
            {
                if (!function->IsConstructorDestructorOrNonstaticMemberFunction())
                {
                    functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, nullptr, cmajor::ir::OperationFlags::none, 0));
                    continue;
                }
            }
            else if (i == 1 && function->IsLvalueReferenceCopyAssignment() && cmajor::symbols::TypesEqual(sourceType, targetType->RemoveReference(sourcePos, moduleId)))
            {
                functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, nullptr, cmajor::ir::OperationFlags::none, 0));
                continue;
            }
            else if (i == 1 && function->IsLvalueReferenceCopyAssignment())
            {
                ArgumentMatch argumentMatch;
                cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(sourceType, targetType->RemoveReference(sourcePos, moduleId), containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (conversionFun && (conversionFun->GetConversionType() == conversionType || conversionFun->GetConversionType() == cmajor::symbols::ConversionType::implicit_))
                {
                    ++functionMatch.numConversions;
                    functionMatch.argumentMatches.push_back(ArgumentMatch(cmajor::ir::OperationFlags::none, conversionFun, cmajor::ir::OperationFlags::none, 1));
                    continue;
                }
            }
        }
        if (cmajor::symbols::TypesEqual(sourceType, targetType))    // exact match
        {
            functionMatch.argumentMatches.push_back(ArgumentMatch());
        }
        else
        {
            if (arity == 2 && (function->GroupName() == U"@constructor" || function->GroupName() == U"operator="))
            {
                if (i == 0)
                {
                    return false;
                }
            }
            bool qualificationConversionMatch = false;
            ArgumentMatch argumentMatch;
            if (cmajor::symbols::TypesEqual(sourceType->PlainType(sourcePos, moduleId), targetType->PlainType(sourcePos, moduleId)))
            {
                qualificationConversionMatch = FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch);
                functionMatch.argumentMatches.push_back(argumentMatch);
            }
            if (!qualificationConversionMatch)
            {
                cmajor::symbols::FunctionSymbol* conversionFun = boundCompileUnit.GetConversion(sourceType, targetType, containerScope, currentFunction, sourcePos, moduleId, argumentMatch);
                if (conversionFun)
                {
                    if (conversionFun->GetConversionType() == conversionType || conversionFun->GetConversionType() == cmajor::symbols::ConversionType::implicit_)
                    {
                        argumentMatch.conversionFun = conversionFun;
                        ++functionMatch.numConversions;
                        if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::none)
                        {
                            if (FindQualificationConversion(sourceType, targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                            {
                                functionMatch.argumentMatches.push_back(argumentMatch);
                                continue;
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            if (FindQualificationConversion(conversionFun->ConversionSourceType(), targetType, argument, conversionType, sourcePos, moduleId, functionMatch, argumentMatch))
                            {
                                functionMatch.argumentMatches.push_back(argumentMatch);
                                continue;
                            }
                            else
                            {
                                return false;
                            }
                        }
                    }
                    else
                    {
                        if (arity == 2 && i == 1 && conversionType == cmajor::symbols::ConversionType::implicit_ && conversionFun->GetConversionType() == cmajor::symbols::ConversionType::explicit_)
                        {
                            functionMatch.castRequired = true;
                            functionMatch.sourceType = sourceType;
                            functionMatch.targetType = targetType;
                        }
                        return false;
                    }
                }
                else
                {
                    if (function->IsFunctionTemplate())
                    {
                        if (FindTemplateParameterMatch(sourceType, targetType, conversionType, argument, boundCompileUnit, functionMatch, containerScope, currentFunction, sourcePos, moduleId))
                        {
                            continue;
                        }
                        if (FindClassTemplateSpecializationMatch(sourceType, targetType, conversionType, argument, boundCompileUnit, functionMatch, containerScope, currentFunction, sourcePos, moduleId))
                        {
                            continue;
                        }
                    }
                    return false;
                }
            }
        }
    }
    return true;
}

std::string MakeOverloadName(const std::u32string& groupName, const std::vector<std::unique_ptr<BoundExpression>>& arguments, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    std::string overloadName = util::ToUtf8(groupName);
    overloadName.append(1, '(');
    bool first = true;
    for (const std::unique_ptr<BoundExpression>& argument : arguments)
    {
        bool wasFirst = first;
        if (first)
        {
            first = false;
        }
        else
        {
            overloadName.append(", ");
        }
        if (wasFirst && (groupName == U"@constructor" || groupName == U"operator="))
        {
            overloadName.append(util::ToUtf8(argument->GetType()->RemovePointer(sourcePos, moduleId)->FullName()));
        }
        else
        {
            overloadName.append(util::ToUtf8(argument->GetType()->FullName()));
        }
    }
    overloadName.append(1, ')');
    return overloadName;
}

std::unique_ptr<BoundFunctionCall> FailWithNoViableFunction(const std::u32string& groupName, const std::vector<std::unique_ptr<BoundExpression>>& arguments,
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, OverloadResolutionFlags flags, std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    std::string overloadName = MakeOverloadName(groupName, arguments, sourcePos, moduleId);
    int arity = arguments.size();
    if (groupName == U"@constructor" && arity == 1 && arguments[0]->GetType()->IsReferenceType())
    {
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::NoViableFunctionException("overload resolution failed: '" + overloadName + "' not found. Note: reference must be initialized.", sourcePos, moduleId,
                arguments[0]->GetSourcePos(), arguments[0]->ModuleId()));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::NoViableFunctionException("overload resolution failed: '" + overloadName + "' not found. Note: reference must be initialized.", sourcePos, moduleId,
                arguments[0]->GetSourcePos(), arguments[0]->ModuleId());
        }
    }
    else
    {
        std::string note;
        if (exception)
        {
            note.append(": Note: ").append(exception->Message());
        }
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::NoViableFunctionException("overload resolution failed: '" + overloadName + "' not found. " +
                "No viable functions taking " + std::to_string(arity) + " arguments found in function group '" + util::ToUtf8(groupName) + "'" + note, sourcePos, moduleId));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::NoViableFunctionException("overload resolution failed: '" + overloadName + "' not found. " +
                "No viable functions taking " + std::to_string(arity) + " arguments found in function group '" + util::ToUtf8(groupName) + "'" + note, sourcePos, moduleId);
        }
    }
}

std::unique_ptr<BoundFunctionCall> FailWithOverloadNotFound(cmajor::symbols::Module* module, const cmajor::symbols::ViableFunctionSet& viableFunctions, const std::u32string& groupName,
    const std::vector<std::unique_ptr<BoundExpression>>& arguments, const std::vector<FunctionMatch>& failedFunctionMatches, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    OverloadResolutionFlags flags, std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    std::string overloadName = MakeOverloadName(groupName, arguments, sourcePos, moduleId);
    bool referenceMustBeInitialized = false;
    bool castRequired = false;
    bool cannotBindConstArgToNonConstParam = false;
    bool cannotAssignToConstObject = false;
    cmajor::symbols::TypeSymbol* sourceType = nullptr;
    cmajor::symbols::TypeSymbol* targetType = nullptr;
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
    std::string note;
    if (exception)
    {
        note.append(" Note: ").append(exception->What());
    }
    if (!failedFunctionMatches.empty())
    {
        int n = failedFunctionMatches.size();
        for (int i = 0; i < n; ++i)
        {
            if (failedFunctionMatches[i].referenceMustBeInitialized)
            {
                referenceMustBeInitialized = true;
                break;
            }
        }
        if (!referenceMustBeInitialized)
        {
            for (int i = 0; i < n; ++i)
            {
                if (failedFunctionMatches[i].castRequired)
                {
                    castRequired = true;
                    sourceType = failedFunctionMatches[i].sourceType;
                    targetType = failedFunctionMatches[i].targetType;
                    references.push_back(std::make_pair(failedFunctionMatches[i].fun->GetSourcePos(), failedFunctionMatches[i].fun->SourceModuleId()));
                    break;
                }
            }
        }
        if (!referenceMustBeInitialized && !castRequired)
        {
            for (int i = 0; i < n; ++i)
            {
                if (failedFunctionMatches[i].cannotBindConstArgToNonConstParam)
                {
                    cannotBindConstArgToNonConstParam = true;
                    sourceType = failedFunctionMatches[i].sourceType;
                    targetType = failedFunctionMatches[i].targetType;
                    references.push_back(std::make_pair(failedFunctionMatches[i].fun->GetSourcePos(), failedFunctionMatches[i].fun->SourceModuleId()));
                    break;
                }
            }
        }
        if (!referenceMustBeInitialized && !castRequired && !cannotBindConstArgToNonConstParam)
        {
            for (int i = 0; i < n; ++i)
            {
                if (failedFunctionMatches[i].cannotAssignToConstObject)
                {
                    cannotAssignToConstObject = true;
                    references.push_back(std::make_pair(failedFunctionMatches[i].fun->GetSourcePos(), failedFunctionMatches[i].fun->SourceModuleId()));
                    break;
                }
            }
        }
        if (!referenceMustBeInitialized && !castRequired && !cannotBindConstArgToNonConstParam && !cannotAssignToConstObject)
        {
            for (int i = 0; i < n; ++i)
            {
                if (failedFunctionMatches[i].conceptCheckException)
                {
                    if (!note.empty())
                    {
                        note.append(".");
                    }
                    note.append(" Note: concept check failed: " + failedFunctionMatches[i].conceptCheckException->Message());
                    references.insert(references.end(), failedFunctionMatches[i].conceptCheckException->References().begin(), failedFunctionMatches[i].conceptCheckException->References().end());
                    break;
                }
            }
        }
    }
    if (referenceMustBeInitialized || groupName == U"@constructor" && arguments.size() == 1 && arguments[0]->GetType()->IsReferenceType())
    {
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            references.push_back(std::make_pair(arguments[0]->GetSourcePos(), arguments[0]->ModuleId()));
            exception.reset(new cmajor::symbols::Exception("overload resolution failed: '" + overloadName + "' not found. Note: reference must be initialized.", sourcePos, moduleId, references));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            references.push_back(std::make_pair(arguments[0]->GetSourcePos(), arguments[0]->ModuleId()));
            throw cmajor::symbols::Exception("overload resolution failed: '" + overloadName + "' not found. Note: reference must be initialized.", sourcePos, moduleId, references);
        }
    }
    else if (castRequired)
    {
        // Assert(sourceType, "source type not set"); TODO
        // Assert(targetType, "target type not set"); TODO
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::CastOverloadException("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot convert implicitly from '" +
                util::ToUtf8(sourceType->FullName()) + "' to '" + util::ToUtf8(targetType->FullName()) + "' but explicit conversion (cast) exists.", sourcePos, moduleId, references));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::CastOverloadException("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot convert implicitly from '" +
                util::ToUtf8(sourceType->FullName()) + "' to '" + util::ToUtf8(targetType->FullName()) + "' but explicit conversion (cast) exists.", sourcePos, moduleId, references);
        }
    }
    else if (cannotBindConstArgToNonConstParam)
    {
        // Assert(sourceType, "source type not set"); TODO
        // Assert(targetType, "target type not set"); TODO
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::CannotBindConstToNonconstOverloadException("overload resolution failed: '" + overloadName +
                "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot bind constant '" + util::ToUtf8(sourceType->FullName()) + "' argument " +
                " to nonconstant '" + util::ToUtf8(targetType->FullName()) + "' parameter", sourcePos, moduleId, references));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::CannotBindConstToNonconstOverloadException("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot bind constant '" + util::ToUtf8(sourceType->FullName()) + "' argument " +
                " to nonconstant '" + util::ToUtf8(targetType->FullName()) + "' parameter", sourcePos, moduleId, references);
        }
    }
    else if (cannotAssignToConstObject)
    {
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::CannotAssignToConstOverloadException("overload resolution failed: '" + overloadName +
                "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot assign to const object.", sourcePos, moduleId, references));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::CannotAssignToConstOverloadException("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined. Note: cannot assign to const object. ", sourcePos, moduleId, references);
        }
    }
    else
    {
        if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
        {
            exception.reset(new cmajor::symbols::Exception("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined." + note, sourcePos, moduleId, references));
            return std::unique_ptr<BoundFunctionCall>();
        }
        else
        {
            throw cmajor::symbols::Exception("overload resolution failed: '" + overloadName + "' not found, or there are no acceptable conversions for all argument types. " +
                std::to_string(viableFunctions.Get().size()) + " viable functions examined." + note, sourcePos, moduleId, references);
        }
    }
}

std::unique_ptr<BoundFunctionCall> FailWithAmbiguousOverload(const std::u32string& groupName, std::vector<std::unique_ptr<BoundExpression>>& arguments,
    std::vector<FunctionMatch>& functionMatches, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, OverloadResolutionFlags flags, std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    std::string overloadName = MakeOverloadName(groupName, arguments, sourcePos, moduleId);
    std::string matchedFunctionNames;
    bool first = true;
    FunctionMatch equalMatch = std::move(functionMatches[0]);
    std::vector<FunctionMatch> equalMatches;
    equalMatches.push_back(std::move(equalMatch));
    int n = int(functionMatches.size());
    for (int i = 1; i < n; ++i)
    {
        FunctionMatch match = std::move(functionMatches[i]);
        if (!BetterFunctionMatch()(equalMatches[0], match))
        {
            equalMatches.push_back(std::move(match));
        }
    }
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
    for (const FunctionMatch& match : equalMatches)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            matchedFunctionNames.append(", or ");
        }
        matchedFunctionNames.append(util::ToUtf8(match.fun->FullName()));
        references.push_back(std::make_pair(match.fun->GetSourcePos(), match.fun->SourceModuleId()));
    }
    if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
    {
        exception.reset(new cmajor::symbols::Exception("overload resolution for overload name '" + overloadName + "' failed: call is ambiguous: \n" + matchedFunctionNames, sourcePos, moduleId, references));
        return std::unique_ptr<BoundFunctionCall>();
    }
    else
    {
        throw cmajor::symbols::Exception("overload resolution for overload name '" + overloadName + "' failed: call is ambiguous: \n" + matchedFunctionNames, sourcePos, moduleId, references);
    }
}

std::unique_ptr<BoundFunctionCall> CreateBoundFunctionCall(cmajor::symbols::FunctionSymbol* bestFun, std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundCompileUnit& boundCompileUnit,
    BoundFunction* boundFunction, const FunctionMatch& bestMatch, cmajor::symbols::ContainerScope* containerScope, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    std::unique_ptr<BoundFunctionCall> boundFunctionCall(new BoundFunctionCall(sourcePos, moduleId, bestFun));
    int arity = arguments.size();
    int n = std::min(arity, bestFun->Arity());
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<BoundExpression>& argument = arguments[i];
        if (i == 0 && !bestFun->IsConstructorDestructorOrNonstaticMemberFunction() &&
            (bestFun->GroupName() == U"@constructor" || bestFun->GroupName() == U"operator=" || bestFun->GroupName() == U"operator->"))
        {
            if (argument->GetBoundNodeType() == BoundNodeType::boundAddressOfExpression)
            {
                BoundAddressOfExpression* addrOf = static_cast<BoundAddressOfExpression*>(argument.get());
                std::unique_ptr<BoundExpression> subject(std::move(addrOf->Subject()));
                addrOf->MoveTemporaryDestructorCallsTo(*subject);
                argument.reset(subject.release());
            }
        }
        const ArgumentMatch& argumentMatch = bestMatch.argumentMatches[i];
        if (argumentMatch.preReferenceConversionFlags != cmajor::ir::OperationFlags::none)
        {
            if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
            {
                if (!argument->IsLvalueExpression())
                {
                    BoundLocalVariable* backingStore = nullptr;
                    if (boundFunction)
                    {
                        backingStore = new BoundLocalVariable(sourcePos, moduleId, boundFunction->GetFunctionSymbol()->CreateTemporary(argument->GetType(), sourcePos, moduleId));
                    }
                    argument.reset(new BoundTemporary(std::move(argument), std::unique_ptr<BoundLocalVariable>(backingStore)));
                }
                cmajor::symbols::TypeSymbol* type = nullptr;
                if (argument->GetType()->IsClassTypeSymbol() && argument->GetFlag(BoundExpressionFlags::bindToRvalueReference))
                {
                    type = argument->GetType()->AddRvalueReference(sourcePos, moduleId);
                }
                else
                {
                    type = argument->GetType()->AddLvalueReference(sourcePos, moduleId);
                }
                BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(argument), type);
                argument.reset(addressOfExpression);
            }
            else if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
            {
                cmajor::symbols::TypeSymbol* type = argument->GetType()->RemoveReference(sourcePos, moduleId);
                BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(argument), type);
                argument.reset(dereferenceExpression);
            }
        }
        if (argumentMatch.conversionFun)
        {
            cmajor::symbols::FunctionSymbol* conversionFun = argumentMatch.conversionFun;
            if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
            {
                if (!boundFunction)
                {
                    return std::unique_ptr<BoundFunctionCall>();
                }
                BoundFunctionCall* constructorCall = new BoundFunctionCall(sourcePos, moduleId, conversionFun);
                cmajor::symbols::TypeSymbol* conversionTargetType = conversionFun->ConversionTargetType();
                if (conversionTargetType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
                {
                    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(conversionTargetType);
                    if (!specialization->IsBound())
                    {
                        boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
                    }
                }
                cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(conversionTargetType, sourcePos, moduleId);
                constructorCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)),
                    conversionTargetType->AddPointer(sourcePos, moduleId))));
                if (conversionTargetType->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(conversionTargetType);
                    if (classType->Destructor())
                    {
                        std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                        destructorCall->AddArgument(std::unique_ptr<BoundExpression>(constructorCall->Arguments()[0]->Clone()));
                        boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
                    }
                }
                constructorCall->AddArgument(std::move(argument));
                std::vector<cmajor::symbols::LocalVariableSymbol*> temporaryLocalVariables = conversionFun->CreateTemporariesTo(boundFunction->GetFunctionSymbol());
                for (cmajor::symbols::LocalVariableSymbol* temporaryLocalVariable : temporaryLocalVariables)
                {
                    constructorCall->AddTemporary(std::unique_ptr<BoundLocalVariable>(new BoundLocalVariable(sourcePos, moduleId, temporaryLocalVariable)));
                }
                BoundConstructAndReturnTemporaryExpression* conversion = new BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>(constructorCall),
                    std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)));
                argument.reset(conversion);
            }
            else if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol && conversionFun->ReturnsClassInterfaceOrClassDelegateByValue())
            {
                if (!boundFunction)
                {
                    return std::unique_ptr<BoundFunctionCall>();
                }
                BoundFunctionCall* conversionFunctionCall = new BoundFunctionCall(sourcePos, moduleId, conversionFun);
                conversionFunctionCall->AddArgument(std::move(argument));
                cmajor::symbols::TypeSymbol* conversionTargetType = conversionFun->ConversionTargetType();
                if (conversionTargetType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
                {
                    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(conversionTargetType);
                    if (!specialization->IsBound())
                    {
                        boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(specialization, containerScope, sourcePos, moduleId);
                    }
                }
                cmajor::symbols::LocalVariableSymbol* temporary = boundFunction->GetFunctionSymbol()->CreateTemporary(conversionTargetType, sourcePos, moduleId);
                conversionFunctionCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(sourcePos, moduleId, temporary)),
                    conversionTargetType->AddPointer(sourcePos, moduleId))));
                std::vector<cmajor::symbols::LocalVariableSymbol*> temporaryLocalVariables = conversionFun->CreateTemporariesTo(boundFunction->GetFunctionSymbol());
                for (cmajor::symbols::LocalVariableSymbol* temporaryLocalVariable : temporaryLocalVariables)
                {
                    conversionFunctionCall->AddTemporary(std::unique_ptr<BoundLocalVariable>(new BoundLocalVariable(sourcePos, moduleId, temporaryLocalVariable)));
                }
                BoundLocalVariable* conversionResult = new BoundLocalVariable(sourcePos, moduleId, temporary);
                if (conversionTargetType->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(conversionTargetType);
                    if (classType->Destructor())
                    {
                        std::unique_ptr<BoundFunctionCall> destructorCall(new BoundFunctionCall(sourcePos, moduleId, classType->Destructor()));
                        cmajor::symbols::TypeSymbol* type = conversionResult->GetType()->AddPointer(sourcePos, moduleId);
                        destructorCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(conversionResult->Clone()), type)));
                        boundFunction->AddTemporaryDestructorCall(std::move(destructorCall), boundFunction, containerScope, sourcePos, moduleId);
                    }
                }
                BoundClassOrClassDelegateConversionResult* conversion = new BoundClassOrClassDelegateConversionResult(std::unique_ptr<BoundExpression>(conversionResult),
                    std::unique_ptr<BoundFunctionCall>(conversionFunctionCall));
                argument.reset(conversion);
            }
            else
            {
                BoundConversion* conversion = new BoundConversion(std::move(argument), conversionFun);
                std::vector<cmajor::symbols::LocalVariableSymbol*> temporaryLocalVariables = conversionFun->CreateTemporariesTo(boundFunction->GetFunctionSymbol());
                for (cmajor::symbols::LocalVariableSymbol* temporaryLocalVariable : temporaryLocalVariables)
                {
                    conversion->AddTemporary(std::unique_ptr<BoundLocalVariable>(new BoundLocalVariable(sourcePos, moduleId, temporaryLocalVariable)));
                }
                argument.reset(conversion);
            }
        }
        if (argumentMatch.postReferenceConversionFlags != cmajor::ir::OperationFlags::none)
        {
            if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
            {
                if (!argument->IsLvalueExpression())
                {
                    BoundLocalVariable* backingStore = nullptr;
                    if (boundFunction)
                    {
                        backingStore = new BoundLocalVariable(sourcePos, moduleId, boundFunction->GetFunctionSymbol()->CreateTemporary(argument->GetType(), sourcePos, moduleId));
                    }
                    argument.reset(new BoundTemporary(std::move(argument), std::unique_ptr<BoundLocalVariable>(backingStore)));
                }
                cmajor::symbols::TypeSymbol* type = nullptr;
                if (argument->GetType()->IsClassTypeSymbol() && argument->GetFlag(BoundExpressionFlags::bindToRvalueReference))
                {
                    type = argument->GetType()->AddRvalueReference(sourcePos, moduleId);
                }
                else
                {
                    type = argument->GetType()->AddLvalueReference(sourcePos, moduleId);
                }
                BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(argument), type);
                argument.reset(addressOfExpression);
            }
            else if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
            {
                cmajor::symbols::TypeSymbol* type = argument->GetType()->RemoveReference(sourcePos, moduleId);
                BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(argument), type);
                argument.reset(dereferenceExpression);
            }
        }
        if (argument->GetType()->IsClassTypeSymbol() || argument->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol || argument->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            if (argument->GetType()->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(argument->GetType());
                if (!classType->CopyConstructor())
                {
                    try
                    {
                        boundCompileUnit.GenerateCopyConstructorFor(classType, containerScope, boundFunction, sourcePos, moduleId);
                    }
                    catch (const cmajor::symbols::Exception& ex)
                    {
                        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
                        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
                        references.insert(references.end(), ex.References().begin(), ex.References().end());
                        throw cmajor::symbols::Exception("cannot pass class '" + util::ToUtf8(classType->FullName()) + "' by value because: " + ex.Message(), argument->GetSourcePos(), argument->ModuleId(), references);
                    }
                }
                cmajor::symbols::TypeSymbol* type = classType->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId);
                argument.reset(new BoundAddressOfExpression(std::move(argument), type));
            }
            else if (argument->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
            {
                cmajor::symbols::TypeSymbol* type = argument->GetType()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId);
                argument.reset(new BoundAddressOfExpression(std::move(argument), type));
            }
            else if (argument->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
            {
                cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(argument->GetType());
                if (!interfaceTypeSymbol->CopyConstructor())
                {
                    boundCompileUnit.GenerateCopyConstructorFor(interfaceTypeSymbol, containerScope, sourcePos, moduleId);
                }
                cmajor::symbols::TypeSymbol* type = argument->GetType()->AddConst(sourcePos, moduleId)->AddLvalueReference(sourcePos, moduleId);
                argument.reset(new BoundAddressOfExpression(std::move(argument), type));
            }
        }
        boundFunctionCall->AddArgument(std::move(argument));
    }
    for (int i = n; i < arity; ++i)
    {
        std::unique_ptr<BoundExpression>& argument = arguments[i];
        boundFunctionCall->AddArgument(std::move(argument));
    }
    if (boundFunction)
    {
        std::vector<cmajor::symbols::LocalVariableSymbol*> temporaryLocalVariables = bestFun->CreateTemporariesTo(boundFunction->GetFunctionSymbol());
        for (cmajor::symbols::LocalVariableSymbol* temporaryLocalVariable : temporaryLocalVariables)
        {
            boundFunctionCall->AddTemporary(std::unique_ptr<BoundLocalVariable>(new BoundLocalVariable(sourcePos, moduleId, temporaryLocalVariable)));
        }
    }
    cmajor::symbols::FunctionSymbol* functionSymbol = boundFunctionCall->GetFunctionSymbol();
    if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
    {
        cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(functionSymbol);
        if (destructorSymbol->IsGeneratedFunction() && !GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
        {
            if (destructorSymbol->Parent()->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(destructorSymbol->Parent());
                if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
                {
                    boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
                    std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                    GenerateDestructorImplementation(boundClass.get(), destructorSymbol, boundCompileUnit, containerScope, boundFunction, sourcePos, moduleId);
                    boundCompileUnit.AddBoundNode(std::move(boundClass));
                }
            }
        }
    }
    return boundFunctionCall;
}

std::unique_ptr<BoundFunctionCall> SelectViableFunction(const cmajor::symbols::ViableFunctionSet& viableFunctions, const std::u32string& groupName,
    std::vector<std::unique_ptr<BoundExpression>>& arguments, cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit, BoundFunction* boundFunction,
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    OverloadResolutionFlags flags, std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes, std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    cmajor::symbols::Module* module = &boundCompileUnit.GetModule();
    std::vector<FunctionMatch> functionMatches;
    std::vector<FunctionMatch> failedFunctionMatches;
    std::vector<std::unique_ptr<cmajor::symbols::Exception>> conceptCheckExceptions;
    std::vector<std::unique_ptr<BoundConstraint>> boundConstraints;
    for (cmajor::symbols::FunctionSymbol* viableFunction : viableFunctions.Get())
    {
        if (viableFunction->GetFlag(cmajor::symbols::FunctionSymbolFlags::dontReuse))
        {
            if (!boundCompileUnit.CanReuse(viableFunction))
            {
                continue;
            }
        }
        if (viableFunction->IsSystemDefault())
        {
            if ((flags & OverloadResolutionFlags::includeSystemDefaultFunctions) == OverloadResolutionFlags::none)
            {
                continue;
            }
        }
        FunctionMatch functionMatch(viableFunction);
        if (viableFunction->IsFunctionTemplate())
        {
            if ((flags & OverloadResolutionFlags::noTemplates) != OverloadResolutionFlags::none)
            {
                continue;
            }
            int n = templateArgumentTypes.size();
            if (n > viableFunction->TemplateParameters().size())
            {
                continue;
            }
            else
            {
                for (int i = 0; i < n; ++i)
                {
                    cmajor::symbols::TemplateParameterSymbol* templateParameter = viableFunction->TemplateParameters()[i];
                    functionMatch.templateParameterMap[templateParameter] = templateArgumentTypes[i];
                }
            }
        }
        else
        {
            if (!templateArgumentTypes.empty())
            {
                continue;
            }
        }
        if (FindConversions(boundCompileUnit, viableFunction, arguments, functionMatch, cmajor::symbols::ConversionType::implicit_, containerScope, boundFunction, sourcePos, moduleId))
        {
            if (viableFunction->IsFunctionTemplate())
            {
                bool allTemplateParametersFound = true;
                int n = viableFunction->TemplateParameters().size();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = viableFunction->TemplateParameters()[i];
                    auto it = functionMatch.templateParameterMap.find(templateParameterSymbol);
                    if (it == functionMatch.templateParameterMap.cend())
                    {
                        allTemplateParametersFound = false;
                        break;
                    }
                }
                if (allTemplateParametersFound)
                {
                    if (!viableFunction->Constraint())
                    {
                        cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(viableFunction);
                        if (node)
                        {
                            // Assert(node->GetNodeType() == cmajor::ast::NodeType::functionNode, "function node expected"); TODO
                            cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
                            cmajor::ast::ConstraintNode* constraint = functionNode->WhereConstraint();
                            if (constraint)
                            {
                                cmajor::ast::CloneContext cloneContext;
                                viableFunction->SetConstraint(static_cast<cmajor::ast::ConstraintNode*>(constraint->Clone(cloneContext)));
                            }
                        }
                    }
                    if (viableFunction->Constraint())
                    {
                        std::unique_ptr<cmajor::symbols::Exception> conceptCheckException;
                        std::unique_ptr<BoundConstraint> boundConstraint;
                        bool candidateFound = CheckConstraint(viableFunction->Constraint(), viableFunction->UsingNodes(), boundCompileUnit, containerScope, boundFunction,
                            viableFunction->TemplateParameters(), functionMatch.templateParameterMap, boundConstraint, sourcePos, moduleId, viableFunction, conceptCheckException);
                        if (candidateFound)
                        {
                            functionMatch.boundConstraint = boundConstraint.get();
                            functionMatches.push_back(functionMatch);
                            boundConstraints.push_back(std::move(boundConstraint));
                        }
                        else
                        {
                            functionMatch.conceptCheckException = conceptCheckException.get();
                            failedFunctionMatches.push_back(functionMatch);
                            conceptCheckExceptions.push_back(std::move(conceptCheckException));
                        }
                    }
                    else
                    {
                        functionMatches.push_back(functionMatch);
                    }
                }
                else
                {
                    failedFunctionMatches.push_back(functionMatch);
                }
            }
            else
            {
                functionMatches.push_back(functionMatch);
            }
        }
        else
        {
            failedFunctionMatches.push_back(functionMatch);
        }
    }
    if (functionMatches.empty())
    {
        if ((flags & OverloadResolutionFlags::includeSystemDefaultFunctions) == OverloadResolutionFlags::none)
        {
            return SelectViableFunction(viableFunctions, groupName, arguments, containerScope, boundCompileUnit, boundFunction, sourcePos, moduleId,
                (flags | OverloadResolutionFlags::includeSystemDefaultFunctions), templateArgumentTypes, exception);
        }
        return FailWithOverloadNotFound(module, viableFunctions, groupName, arguments, failedFunctionMatches, sourcePos, moduleId, flags, exception);
    }
    else if (functionMatches.size() > 1)
    {
        std::sort(functionMatches.begin(), functionMatches.end(), BetterFunctionMatch());
        if (BetterFunctionMatch()(functionMatches[0], functionMatches[1]))
        {
            const FunctionMatch& bestMatch = functionMatches[0];
            cmajor::symbols::FunctionSymbol* bestFun = bestMatch.fun;
            if (bestFun->IsSuppressed())
            {
                if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
                {
                    exception.reset(new cmajor::symbols::Exception("cannot call a suppressed member function '" + util::ToUtf8(bestFun->FullName()) + "'", sourcePos, moduleId, bestFun->GetSourcePos(), bestFun->SourceModuleId()));
                    return std::unique_ptr<BoundFunctionCall>();
                }
                else
                {
                    throw cmajor::symbols::Exception("cannot call a suppressed member function '" + util::ToUtf8(bestFun->FullName()) + "'", sourcePos, moduleId, bestFun->GetSourcePos(), bestFun->SourceModuleId());
                }
            }
            bool instantiate = (flags & OverloadResolutionFlags::dontInstantiate) == OverloadResolutionFlags::none;
            if (bestFun->IsFunctionTemplate())
            {
                if (instantiate)
                {
                    bestFun = boundCompileUnit.InstantiateFunctionTemplate(bestFun, bestMatch.templateParameterMap, sourcePos, moduleId);
                }
                else
                {
                    templateArgumentTypes.clear();
                    for (cmajor::symbols::TemplateParameterSymbol* templateParameter : bestFun->TemplateParameters())
                    {
                        auto it = bestMatch.templateParameterMap.find(templateParameter);
                        if (it != bestMatch.templateParameterMap.cend())
                        {
                            cmajor::symbols::TypeSymbol* templateArgumentType = it->second;
                            templateArgumentTypes.push_back(templateArgumentType);
                        }
                    }
                }
            }
            else if (!bestFun->IsGeneratedFunction() && bestFun->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                if (instantiate)
                {
                    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(bestFun->Parent());
                    if (specialization->GetModule() != module)
                    {
                        specialization = boundCompileUnit.GetSymbolTable().GetCurrentClassTemplateSpecialization(specialization);
                        int index = bestFun->GetIndex();
                        bestFun = specialization->GetFunctionByIndex(index);
                    }
                    bool firstTry = boundCompileUnit.InstantiateClassTemplateMemberFunction(bestFun, containerScope, boundFunction, sourcePos, moduleId);
                    if (!firstTry)
                    {
                        cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(bestFun->Parent());
                        std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
                        cmajor::symbols::ClassTemplateSpecializationSymbol* copy = boundCompileUnit.GetSymbolTable().CopyClassTemplateSpecialization(specialization);
                        boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(
                            copy, boundCompileUnit.GetSymbolTable().GlobalNs().GetContainerScope(), sourcePos, moduleId);
                        int index = bestFun->GetIndex();
                        bestFun = copy->GetFunctionByIndex(index);
                        bool secondTry = boundCompileUnit.InstantiateClassTemplateMemberFunction(bestFun, containerScope, boundFunction, sourcePos, moduleId);
                        if (!secondTry)
                        {
                            throw cmajor::symbols::Exception("internal error: could not instantiate member function of a class template specialization '" + util::ToUtf8(specialization->FullName()) + "'",
                                specialization->GetSourcePos(), specialization->SourceModuleId());
                        }
                    }
                }
            }
            else if (!bestFun->IsGeneratedFunction() && GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && bestFun->IsInline())
            {
                if (instantiate)
                {
                    if (bestFun->IsTemplateSpecialization())
                    {
                        cmajor::symbols::FunctionSymbol* functionTemplate = bestFun->FunctionTemplate();
                        std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*> templateParameterMap;
                        int n = functionTemplate->TemplateParameters().size();
                        for (int i = 0; i < n; ++i)
                        {
                            cmajor::symbols::TemplateParameterSymbol* templateParameter = functionTemplate->TemplateParameters()[i];
                            cmajor::symbols::TypeSymbol* templateArgumentType = bestFun->TemplateArgumentTypes()[i];
                            templateParameterMap[templateParameter] = templateArgumentType;
                        }
                        bestFun = boundCompileUnit.InstantiateFunctionTemplate(functionTemplate, templateParameterMap, sourcePos, moduleId);
                    }
                    else
                    {
                        bestFun = boundCompileUnit.InstantiateInlineFunction(bestFun, containerScope, sourcePos, moduleId);
                    }
                }
            }
            if (boundFunction && boundFunction->GetFunctionSymbol()->DontThrow() && !boundFunction->GetFunctionSymbol()->HasTry() && !bestFun->DontThrow())
            {
                std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
                references.push_back(std::make_pair(boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId()));
                references.push_back(std::make_pair(bestFun->GetSourcePos(), bestFun->SourceModuleId()));
                if (GetGlobalFlag(cmajor::symbols::GlobalFlags::strictNothrow))
                {
                    if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
                    {
                        exception.reset(new cmajor::symbols::Exception("a nothrow function cannot call a function that can throw unless it handles exceptions (compiled with --strict-nothrow)", sourcePos, moduleId, references));
                        return std::unique_ptr<BoundFunctionCall>();
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("a nothrow function cannot call a function that can throw unless it handles exceptions (compiled with --strict-nothrow)", sourcePos, moduleId, references);
                    }
                }
                else
                {
                    cmajor::symbols::Warning warning(module->GetCurrentProjectName(), "a nothrow function calls a function that can throw and does not handle exceptions");
                    warning.SetDefined(sourcePos, moduleId);
                    warning.SetReferences(references);
                    module->WarningCollection().AddWarning(warning);
                }
            }
            return CreateBoundFunctionCall(bestFun, arguments, boundCompileUnit, boundFunction, bestMatch, containerScope, sourcePos, moduleId);
        }
        else
        {
            return FailWithAmbiguousOverload(groupName, arguments, functionMatches, sourcePos, moduleId, flags, exception);
        }
    }
    else
    {
        const FunctionMatch& bestMatch = functionMatches[0];
        cmajor::symbols::FunctionSymbol* singleBest = bestMatch.fun;
        if (singleBest->IsSuppressed())
        {
            if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
            {
                exception.reset(new cmajor::symbols::Exception("cannot call a suppressed member function '" + util::ToUtf8(singleBest->FullName()) + "'", sourcePos, moduleId, singleBest->GetSourcePos(), singleBest->SourceModuleId()));
                return std::unique_ptr<BoundFunctionCall>();
            }
            else
            {
                throw cmajor::symbols::Exception("cannot call a suppressed member function '" + util::ToUtf8(singleBest->FullName()) + "'", sourcePos, moduleId, singleBest->GetSourcePos(), singleBest->SourceModuleId());
            }
        }
        bool instantiate = (flags & OverloadResolutionFlags::dontInstantiate) == OverloadResolutionFlags::none;
        if (singleBest->IsFunctionTemplate())
        {
            if (instantiate)
            {
                singleBest = boundCompileUnit.InstantiateFunctionTemplate(singleBest, bestMatch.templateParameterMap, sourcePos, moduleId);
            }
            else
            {
                templateArgumentTypes.clear();
                for (cmajor::symbols::TemplateParameterSymbol* templateParameter : singleBest->TemplateParameters())
                {
                    auto it = bestMatch.templateParameterMap.find(templateParameter);
                    if (it != bestMatch.templateParameterMap.cend())
                    {
                        cmajor::symbols::TypeSymbol* templateArgumentType = it->second;
                        templateArgumentTypes.push_back(templateArgumentType);
                    }
                }
            }
        }
        else if (!singleBest->IsGeneratedFunction() && singleBest->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            if (instantiate)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(singleBest->Parent());
                if (specialization->GetModule() != module)
                {
                    specialization = boundCompileUnit.GetSymbolTable().GetCurrentClassTemplateSpecialization(specialization);
                    int index = singleBest->GetIndex();
                    singleBest = specialization->GetFunctionByIndex(index);
                }
                bool firstTry = boundCompileUnit.InstantiateClassTemplateMemberFunction(singleBest, containerScope, boundFunction, sourcePos, moduleId);
                if (!firstTry)
                {
                    std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().Lock());
                    cmajor::symbols::ClassTemplateSpecializationSymbol* copy = boundCompileUnit.GetSymbolTable().CopyClassTemplateSpecialization(specialization);
                    boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(copy, boundCompileUnit.GetSymbolTable().GlobalNs().GetContainerScope(), sourcePos, moduleId);
                    int index = singleBest->GetIndex();
                    singleBest = copy->GetFunctionByIndex(index);
                    bool secondTry = boundCompileUnit.InstantiateClassTemplateMemberFunction(singleBest, containerScope, boundFunction, sourcePos, moduleId);
                    if (!secondTry)
                    {
                        throw cmajor::symbols::Exception("internal error: could not instantiate member function of a class template specialization '" + util::ToUtf8(specialization->FullName()) + "'",
                            specialization->GetSourcePos(), specialization->SourceModuleId());
                    }
                }
            }
        }
        else if (!singleBest->IsGeneratedFunction() && GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && singleBest->IsInline())
        {
            if (instantiate)
            {
                if (singleBest->IsTemplateSpecialization())
                {
                    cmajor::symbols::FunctionSymbol* functionTemplate = singleBest->FunctionTemplate();
                    std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*> templateParameterMap;
                    int n = functionTemplate->TemplateParameters().size();
                    for (int i = 0; i < n; ++i)
                    {
                        cmajor::symbols::TemplateParameterSymbol* templateParameter = functionTemplate->TemplateParameters()[i];
                        cmajor::symbols::TypeSymbol* templateArgumentType = singleBest->TemplateArgumentTypes()[i];
                        templateParameterMap[templateParameter] = templateArgumentType;
                    }
                    singleBest = boundCompileUnit.InstantiateFunctionTemplate(functionTemplate, templateParameterMap, sourcePos, moduleId);
                }
                else
                {
                    singleBest = boundCompileUnit.InstantiateInlineFunction(singleBest, containerScope, sourcePos, moduleId);
                }
            }
        }
        if (boundFunction && boundFunction->GetFunctionSymbol()->DontThrow() && !boundFunction->GetFunctionSymbol()->HasTry() && !singleBest->DontThrow())
        {
            std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
            references.push_back(std::make_pair(boundFunction->GetFunctionSymbol()->GetSourcePos(), boundFunction->GetFunctionSymbol()->SourceModuleId()));
            references.push_back(std::make_pair(singleBest->GetSourcePos(), singleBest->SourceModuleId()));
            if (GetGlobalFlag(cmajor::symbols::GlobalFlags::strictNothrow))
            {
                if ((flags & OverloadResolutionFlags::dontThrow) != OverloadResolutionFlags::none)
                {
                    exception.reset(new cmajor::symbols::Exception("a nothrow function cannot call a function that can throw unless it handles exceptions (compiled with --strict-nothrow)", sourcePos, moduleId, references));
                    return std::unique_ptr<BoundFunctionCall>();
                }
                else
                {
                    throw cmajor::symbols::Exception("a nothrow function cannot call a function that can throw unless it handles exceptions (compiled with --strict-nothrow)", sourcePos, moduleId, references);
                }
            }
            else
            {
                cmajor::symbols::Warning warning(module->GetCurrentProjectName(), "a nothrow function calls a function that can throw and does not handle exceptions");
                warning.SetDefined(sourcePos, moduleId);
                warning.SetReferences(references);
                module->WarningCollection().AddWarning(warning);
            }
        }
        return CreateBoundFunctionCall(singleBest, arguments, boundCompileUnit, boundFunction, bestMatch, containerScope, sourcePos, moduleId);
    }
}

void CollectViableFunctionsFromSymbolTable(int arity, const std::u32string& groupName, const std::vector<FunctionScopeLookup>& functionScopeLookups, BoundCompileUnit& boundCompileUnit,
    cmajor::symbols::ViableFunctionSet& viableFunctions)
{
    std::unordered_set<cmajor::symbols::ContainerScope*> scopesLookedUp;
    bool fileScopesLookedUp = false;
    for (const FunctionScopeLookup& functionScopeLookup : functionScopeLookups)
    {
        if (functionScopeLookup.scopeLookup == cmajor::symbols::ScopeLookup::fileScopes && !fileScopesLookedUp)
        {
            fileScopesLookedUp = true;
            for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
            {
                fileScope->CollectViableFunctions(arity, groupName, scopesLookedUp, viableFunctions, &boundCompileUnit.GetModule());
            }
        }
        else
        {
            cmajor::symbols::ContainerScope* scope = functionScopeLookup.scope;
            scope->CollectViableFunctions(arity, groupName, scopesLookedUp, functionScopeLookup.scopeLookup, viableFunctions, &boundCompileUnit.GetModule());
        }
    }
}

std::unique_ptr<BoundFunctionCall> ResolveOverload(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, const std::vector<FunctionScopeLookup>& functionScopeLookups,
    std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundCompileUnit& boundCompileUnit, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    std::unique_ptr<cmajor::symbols::Exception> exception;
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    return ResolveOverload(groupName, containerScope, functionScopeLookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId,
        OverloadResolutionFlags::none, templateArgumentTypes, exception);
}

std::unique_ptr<BoundFunctionCall> ResolveOverload(const std::u32string& groupName, cmajor::symbols::ContainerScope* containerScope, const std::vector<FunctionScopeLookup>& functionScopeLookups,
    std::vector<std::unique_ptr<BoundExpression>>& arguments, BoundCompileUnit& boundCompileUnit, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    OverloadResolutionFlags flags, std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes, std::unique_ptr<cmajor::symbols::Exception>& exception)
{
    cmajor::symbols::Module* module = &boundCompileUnit.GetModule();
    int arity = arguments.size();
    cmajor::symbols::ViableFunctionSet viableFunctions;
    if (currentFunction)
    {
        CollectFlags collectFlags = CollectFlags::none;
        if ((flags & OverloadResolutionFlags::noRvalueRef) != OverloadResolutionFlags::none)
        {
            collectFlags = collectFlags | CollectFlags::noRvalueRef;
        }
        boundCompileUnit.CollectViableFunctions(groupName, containerScope, arguments, currentFunction, viableFunctions, exception, sourcePos, moduleId, collectFlags);
    }
    if (viableFunctions.Get().empty())
    {
        if ((flags & OverloadResolutionFlags::dontThrow) == OverloadResolutionFlags::none && exception)
        {
            throw* exception;
        }
        CollectViableFunctionsFromSymbolTable(arity, groupName, functionScopeLookups, boundCompileUnit, viableFunctions);
    }
    if (viableFunctions.Get().empty())
    {
        return FailWithNoViableFunction(groupName, arguments, sourcePos, moduleId, flags, exception);
    }
    else
    {
        return SelectViableFunction(viableFunctions, groupName, arguments, containerScope, boundCompileUnit, currentFunction, sourcePos, moduleId, flags, templateArgumentTypes, exception);
    }
}

} // namespace cmajor::binder