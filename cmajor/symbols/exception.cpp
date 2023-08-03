// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.exception;

import cmajor.symbols.module_cache;
import cmajor.symbols.modules;

import std.core;

namespace cmajor::symbols {

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
    return Expand(errorMessage, sourcePos, moduleId, references);
}

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const util::uuid& primaryModuleId,
    const soul::ast::SourcePos& referenceSourcePos, const util::uuid& referenceModuleId)
{
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references(1, std::make_pair(referenceSourcePos, referenceModuleId));
    return Expand(errorMessage, primarySourcePos, primaryModuleId, references, "Error");
}

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const util::uuid& primaryModuleId,
    const soul::ast::SourcePos& referenceSourcePos, const util::uuid& referenceModuleId, const std::string& title)
{
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references(1, std::make_pair(referenceSourcePos, referenceModuleId));
    return Expand(errorMessage, primarySourcePos, primaryModuleId, references, title);
}

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references)
{
    return Expand(errorMessage, sourcePos, moduleId, references, "Error");
}

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references,
    const std::string& title)
{
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> referenceSourcePoss = references;
    referenceSourcePoss.erase(std::unique(referenceSourcePoss.begin(), referenceSourcePoss.end()), referenceSourcePoss.end());
    std::string expandedMessage = title + ": " + errorMessage;
    if (sourcePos.IsValid())
    {
        Module* module = GetModuleById(moduleId);
        if (module)
        {
            std::string fileName = module->GetFilePath(sourcePos.file);
            if (!fileName.empty())
            {
                expandedMessage.append(" (file '" + fileName + "', line " + std::to_string(sourcePos.line) + ")");
                expandedMessage.append(":\n").append(util::ToUtf8(module->GetErrorLines(sourcePos))); 
            }
        }
    }
    for (const std::pair<soul::ast::SourcePos, util::uuid>& referenceSourcePos : referenceSourcePoss)
    {
        if (!referenceSourcePos.first.IsValid()) continue;
        if (referenceSourcePos.first == sourcePos && referenceSourcePos.second == moduleId) continue;
        Module* module = GetModuleById(referenceSourcePos.second);
        if (module)
        {
            std::string fileName = module->GetFilePath(referenceSourcePos.first.file);
            if (!fileName.empty())
            {
                expandedMessage.append("\nsee reference to file '" + fileName + "', line " + std::to_string(referenceSourcePos.first.line));
                expandedMessage.append(":\n").append(util::ToUtf8(module->GetErrorLines(referenceSourcePos.first)));
            }
        }
    }
    return expandedMessage;
}

/*
std::unique_ptr<util::JsonObject> SourcePosToJson(Module* module, const soul::ast::SourcePos& sourcePos)
{
    if (!sourcePos.IsValid()) return std::unique_ptr<util::JsonObject>();
    if (!module)
    {
        throw std::runtime_error("module not set");
    }
    const std::string& fileName = module->GetFilePath(sourcePos.file);
    if (fileName.empty()) return std::unique_ptr<util::JsonObject>();
    std::unique_ptr<util::JsonObject> json(new util::JsonObject());
    json->AddField(U"file", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(fileName))));
    json->AddField(U"line", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(sourcePos.line)))));
    std::u32string text = module->GetErrorLines(sourcePos); 
    int32_t startCol = 0;
    int32_t endCol = 0;
    json->AddField(U"startCol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(startCol)))));
    json->AddField(U"endCol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(endCol)))));
    json->AddField(U"text", std::unique_ptr<util::JsonValue>(new util::JsonString(text))); 
    return json;
}
*/

/*
std::unique_ptr<soul::xml::Element> SourcePosToDomElement(Module* module_, const soul::ast::SourcePos& sourcePos)
{
    if (!sourcePos.IsValid()) return std::unique_ptr<soul::xml::Element>();
    if (!module_)
    {
        throw std::runtime_error("module not set");
    }
    const std::string& fileName = module_->GetFilePath(sourcePos.file);
    if (fileName.empty()) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> sourcePosElement(soul::xml::MakeElement("sourcePos"));
    std::unique_ptr<soul::xml::Element> fileElement(soul::xml::MakeElement("file"));
    std::unique_ptr<soul::xml::Text> fileText(soul::xml::MakeText((fileName)));
    fileElement->AppendChild(std::unique_ptr<soul::xml::Node>(fileText.release()));
    std::unique_ptr<soul::xml::Element> lineElement(soul::xml::MakeElement("line"));
    std::unique_ptr<soul::xml::Text> lineText(soul::xml::MakeText((std::to_string(sourcePos.line))));
    lineElement->AppendChild(std::unique_ptr<soul::xml::Node>(lineText.release()));
    std::u32string text = module_->GetErrorLines(sourcePos);
    int32_t startCol = 0;
    int32_t endCol = 0;
    module_->GetColumns(sourcePos, startCol, endCol);
    sourcePosElement->AppendChild(std::unique_ptr<soul::xml::Node>(fileElement.release()));
    sourcePosElement->AppendChild(std::unique_ptr<soul::xml::Node>(lineElement.release()));
    std::unique_ptr<soul::xml::Element> startColElement(soul::xml::MakeElement("startCol"));
    std::unique_ptr<soul::xml::Text> startColText(soul::xml::MakeText((std::to_string(startCol))));
    startColElement->AppendChild(std::unique_ptr<soul::xml::Node>(startColText.release()));
    sourcePosElement->AppendChild(std::unique_ptr<soul::xml::Node>(startColElement.release()));
    std::unique_ptr<soul::xml::Element> endColElement(soul::xml::MakeElement("endCol"));
    std::unique_ptr<soul::xml::Text> endColText(soul::xml::MakeText((std::to_string(endCol))));
    endColElement->AppendChild(std::unique_ptr<soul::xml::Node>(endColText.release()));
    sourcePosElement->AppendChild(std::unique_ptr<soul::xml::Node>(endColElement.release()));
    std::unique_ptr<soul::xml::Element> textElement(soul::xml::MakeElement("text"));
    std::unique_ptr<soul::xml::Text> textText(soul::xml::MakeText(text));
    textElement->AppendChild(std::unique_ptr<soul::xml::Node>(textText.release()));
    sourcePosElement->AppendChild(std::unique_ptr<soul::xml::Node>(textElement.release()));
    return sourcePosElement;
}
*/

Exception::Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) :
    what(Expand(message_, defined_, definedModuleId_)), message(message_), defined(defined_), definedModuleId(definedModuleId_)
{
}

Exception::Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_) :
    what(Expand(message_, defined_, definedModuleId_, referenced_, referencedModuleId_)), message(message_), defined(defined_), definedModuleId(definedModuleId_)
{
    references.push_back(std::make_pair(referenced_, referencedModuleId_));
}

Exception::Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_) :
    what(Expand(message_, defined_, definedModuleId_, references_)), message(message_), defined(defined_), definedModuleId(definedModuleId_), references(references_)
{
}

Exception::~Exception()
{
}

void Exception::SetProject(const std::string& projectName_)
{
    projectName = projectName_;
    what = "project: " + projectName + " " + what;
}

std::vector<cmajor::info::bs::CompileError> Exception::ToErrors() const
{
    std::vector<cmajor::info::bs::CompileError> errors;
    Module* module = GetModuleById(definedModuleId);
    if (module)
    {
        cmajor::info::bs::CompileError mainError;
        mainError.message = message;
        mainError.project = projectName;
        mainError.file = module->GetFilePath(defined.file);
        mainError.line = defined.line;
        mainError.scol = defined.col;
        errors.push_back(mainError);
        for (const auto& ref : references)
        {
            Module* module = GetModuleById(ref.second);
            if (module)
            {
                const soul::ast::SourcePos& sourcePos = ref.first;
                cmajor::info::bs::CompileError refError;
                refError.message = "See reference to: ";
                refError.file = module->GetFilePath(sourcePos.file);
                refError.line = sourcePos.line;
                refError.scol = sourcePos.col;
                errors.push_back(refError);
            }
        }
    }
    return errors;
}

/*
std::unique_ptr<util::JsonValue> Exception::ToJson() const
{
    Module* module_ = cmajor::symbols::GetModuleById(definedModuleId);
    std::unique_ptr<util::JsonObject> json(new util::JsonObject());
    json->AddField(U"tool", std::unique_ptr<util::JsonValue>(new util::JsonString(module_->GetCurrentToolName())));
    json->AddField(U"kind", std::unique_ptr<util::JsonValue>(new util::JsonString(U"error")));
    json->AddField(U"project", std::unique_ptr<util::JsonValue>(new util::JsonString(module_->GetCurrentProjectName())));
    json->AddField(U"message", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(message))));
    std::unique_ptr<JsonArray> refs(new JsonArray());
    std::unique_ptr<util::JsonObject> ref = SourcePosToJson(module_, defined);
    if (ref)
    {
        refs->AddItem(std::move(ref));
    }
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> referenceSourcePoss = references;
    referenceSourcePoss.erase(std::unique(referenceSourcePoss.begin(), referenceSourcePoss.end()), referenceSourcePoss.end());
    for (const std::pair<soul::ast::SourcePos, util::uuid>& referenceSourcePosModuleId : referenceSourcePoss)
    {
        if (!referenceSourcePosModuleId.first.Valid()) continue;
        if (referenceSourcePosModuleId.first == defined && referenceSourcePosModuleId.second == definedModuleId) continue;
        Module* mod = cmajor::symbols::GetModuleById(referenceSourcePosModuleId.second);
        std::unique_ptr<util::JsonObject> ref = SourcePosToJson(mod, referenceSourcePosModuleId.first);
        if (ref)
        {
            refs->AddItem(std::move(ref));
        }
    }
    json->AddField(U"references", std::move(refs));
    return std::unique_ptr<util::JsonValue>(json.release());
    return std::unique_ptr<util::JsonValue>();
}

void Exception::AddToDiagnosticsElement(soul::xml::Element* diagnosticsElement) const
{
    std::unique_ptr<soul::xml::Element> diagnosticElement(soul::xml::MakeElement("diagnostic"));
    std::unique_ptr<soul::xml::Element> categoryElement(soul::xml::MakeElement("category"));
    std::unique_ptr<soul::xml::Text> categoryText(soul::xml::MakeText(U"error"));
    categoryElement->AppendChild(std::unique_ptr<soul::xml::Node>(categoryText.release()));
    std::unique_ptr<soul::xml::Element> subcategoryElement(soul::xml::MakeElement("subcategory"));
    std::unique_ptr<soul::xml::Text> subcategoryText(soul::xml::MakeText(U"error"));
    subcategoryElement->AppendChild(std::unique_ptr<soul::xml::Node>(subcategoryText.release()));
    std::unique_ptr<soul::xml::Element> messageElement(soul::xml::MakeElement("message"));
    std::unique_ptr<soul::xml::Text> messageText(soul::xml::MakeText((message)));
    messageElement->AppendChild(std::unique_ptr<soul::xml::Node>(messageText.release()));
    diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(categoryElement.release()));
    diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(subcategoryElement.release()));
    diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(messageElement.release()));
    Module* module_ = cmajor::symbols::GetModuleById(definedModuleId);
    std::unique_ptr<soul::xml::Element> sourcePosElement = SourcePosToDomElement(module, defined);
    if (sourcePosElement)
    {
        diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(sourcePosElement.release()));
    }
    diagnosticsElement->AppendChild(std::unique_ptr<soul::xml::Node>(diagnosticElement.release()));
    for (const std::pair<soul::ast::SourcePos, util::uuid>& sourcePosModuleId : references)
    {
        if (!sourcePosModuleId.first.Valid()) continue;
        Module* mod = cmajor::symbols::GetModuleById(sourcePosModuleId.second);
        std::unique_ptr<soul::xml::Element> diagnosticElement(soul::xml::MakeElement("diagnostic"));
        std::unique_ptr<soul::xml::Element> categoryElement(soul::xml::MakeElement("category"));
        std::unique_ptr<soul::xml::Text> categoryText(soul::xml::MakeText(U"info"));
        categoryElement->AppendChild(std::unique_ptr<soul::xml::Node>(categoryText.release()));
        std::unique_ptr<soul::xml::Element> messageElement(soul::xml::MakeElement("message"));
        std::unique_ptr<soul::xml::Text> messageText(soul::xml::MakeText(("see reference to")));
        messageElement->AppendChild(std::unique_ptr<soul::xml::Node>(messageText.release()));
        diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(categoryElement.release()));
        diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(messageElement.release()));
        std::unique_ptr<soul::xml::Element> sourcePosElement = SourcePosToDomElement(mod, sourcePosModuleId.first);
        if (sourcePosElement)
        {
            diagnosticElement->AppendChild(std::unique_ptr<soul::xml::Node>(sourcePosElement.release()));
            diagnosticsElement->AppendChild(std::unique_ptr<soul::xml::Node>(diagnosticElement.release()));
        }
    }
}
*/

ModuleImmutableException::ModuleImmutableException(Module* module, Module* immutableModule, const soul::ast::SourcePos& defined_, const soul::ast::SourcePos& referenced_) :
    Exception("attempt to add a symbol to an immutable module' " + util::ToUtf8(immutableModule->Name()) + "'", defined_, module->Id(), referenced_, immutableModule->Id())
{
}

SymbolCheckException::SymbolCheckException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& moduleId_) : Exception(message_, defined_, moduleId_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) : Exception(message_, defined_, definedModuleId_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_) :
    Exception(message_, defined_, definedModuleId_, referenced_, referencedModuleId_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_) : Exception(message_, defined_, definedModuleId_, references_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) :
    Exception(message_, defined_, definedModuleId_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_) :
    Exception(message_, defined_, definedModuleId_, referenced_, referencedModuleId_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_) :
    Exception(message_, defined_, definedModuleId_, references_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) :
    Exception(message_, defined_, definedModuleId_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_) :
    Exception(message_, defined_, definedModuleId_, referenced_, referencedModuleId_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_) :
    Exception(message_, defined_, definedModuleId_, references_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) :
    Exception(message_, defined_, definedModuleId_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_) :
    Exception(message_, defined_, definedModuleId_, referenced_, referencedModuleId_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_) :
    Exception(message_, defined_, definedModuleId_, references_)
{
}
} // namespace cmajor::symbols
