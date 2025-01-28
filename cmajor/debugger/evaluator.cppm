// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.evaluator;

import cmajor.debugger.container;
import cmajor.info;
import cmajor.debug;
import soul.xml.dom;
import std.core;

export namespace cmajor::debugger {

const int maxPointerLevel = 3;
class Debugger;

void ClearBrowsingData();

Container* GetOrInsertContainer(cmajor::debug::ContainerClassTemplateKind containerKind, const std::string& containerVarExpr, Debugger* debugger);

cmajor::info::db::EvaluateReply Evaluate(const std::string& expression, Debugger* debugger);

std::unique_ptr<soul::xml::Element> EvaluateToXml(const std::string& expression, Debugger* debugger);

std::string GetValue(const std::string& expression, cmajor::debug::DIType* type, int64_t& count, cmajor::debug::DIType*& dynType, std::set<uint64_t>& printedPointers,
    int& level, int maxLevel, Debugger* debugger);

std::string GetValue(const std::string& parentExpr, cmajor::debug::DIVariable* variable, int64_t& count, cmajor::debug::DIType*& dynType, std::set<uint64_t>& printedPointers, 
    int& level, int maxLevel, Debugger* debugger);

void EvaluateChildRequest(cmajor::debug::Project* project, const std::string& expression, int start, int count, cmajor::info::db::EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

void EvaluateSpecializationTypeChildRequest(cmajor::debug::DIClassTemplateSpecializationType* specializationType, const std::string& expression, int start, int count,
    cmajor::info::db::EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

void EvaluateClassTypeChildRequest(cmajor::debug::DIClassType* classType, const std::string& expression, int start, int count, cmajor::info::db::EvaluateChildReply& reply,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

void EvaluateContainerTypeChildRequest(cmajor::debug::DIClassTemplateSpecializationType* containerType, const std::string& expression, int start, int count,
    cmajor::info::db::EvaluateChildReply& reply, std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

std::string GetEnumeratedTypeValue(uint64_t value, cmajor::debug::DIEnumType* enumType);

std::string GetSpecializationValue(bool initialized, const std::string& expression, cmajor::debug::DIClassTemplateSpecializationType* specializationType, int64_t& count,
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

std::string GetClassValue(const std::string& expression, cmajor::debug::DIClassType* classType, int64_t& count, std::set<uint64_t>& printedPointers, int& level, int maxLevel, 
    Debugger* debugger);

std::string GetPointedValue(const std::string& expression, cmajor::debug::DIType* derefType, cmajor::debug::DIType* dynamicType, int64_t& count, 
    std::set<uint64_t>& printedPointers, int& level, int maxLevel, Debugger* debugger);

std::string GetClassDelegateValue(const std::string& expression, cmajor::debug::DIClassDelegateType* classDelegateType, std::set<uint64_t>& printedPointers, int& level, int maxLevel,
    Debugger* debugger);

std::string GetStringValue(const std::string& expression, Debugger* debugger);

std::string GetWStringValue(const std::string& expression, Debugger* debugger);

std::string GetUStringValue(const std::string& expression, Debugger* debugger);

} // namespace cmajor::debugger

