// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.exception;

import cmajor.symbols.module_cache;
import cmajor.symbols.modules;

import std.core;

namespace cmajor::symbols {

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan)
{
    std::vector<soul::ast::FullSpan> references;
    return Expand(errorMessage, fullSpan, references);
}

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& primarySpan, const soul::ast::FullSpan& referenceSpan)
{
    std::vector<soul::ast::FullSpan> references(1, referenceSpan);
    return Expand(errorMessage, primarySpan, references, "Error");
}

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& primarySpan, const soul::ast::FullSpan& referenceSpan, const std::string& title)
{
    std::vector<soul::ast::FullSpan> references(1, referenceSpan);
    return Expand(errorMessage, primarySpan, references, title);
}

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan, const std::vector<soul::ast::FullSpan>& references)
{
    return Expand(errorMessage, fullSpan, references, "Error");
}

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan, const std::vector<soul::ast::FullSpan>& references, const std::string& title)
{
    std::vector<soul::ast::FullSpan> referenceSpans = references;
    referenceSpans.erase(std::unique(referenceSpans.begin(), referenceSpans.end()), referenceSpans.end());
    std::string expandedMessage = title + ": " + errorMessage;
    if (fullSpan.IsValid())
    {
        Module* module = GetModuleById(fullSpan.moduleId);
        if (module)
        {
            std::string fileName = module->GetFilePath(fullSpan.fileIndex);
            if (!fileName.empty())
            {
                soul::ast::LineColLen lineColLen;
                std::string errorLines = module->GetErrorLines(fullSpan.span, fullSpan.fileIndex, lineColLen);
                expandedMessage.append(" (file '" + fileName + "', line " + std::to_string(lineColLen.line) + ")");
                expandedMessage.append(":\n").append(errorLines);
            }
        }
    }
    for (const soul::ast::FullSpan& referenceSpan : referenceSpans)
    {
        if (!referenceSpan.IsValid()) continue;
        if (referenceSpan == fullSpan) continue;
        Module* module = GetModuleById(referenceSpan.moduleId);
        if (module)
        {
            std::string fileName = module->GetFilePath(referenceSpan.fileIndex);
            if (!fileName.empty())
            {
                soul::ast::LineColLen lineColLen;
                std::string errorLines = module->GetErrorLines(referenceSpan.span, referenceSpan.fileIndex, lineColLen);
                expandedMessage.append("\nsee reference to file '" + fileName + "', line " + std::to_string(lineColLen.line));
                expandedMessage.append(":\n").append(errorLines);
            }
        }
    }
    return expandedMessage;
}

/*
std::unique_ptr<util::JsonObject> SourcePosToJson(Module* module, const soul::ast::Span& span)
{
    if (!span.IsValid()) return std::unique_ptr<util::JsonObject>();
    if (!module)
    {
        throw std::runtime_error("module not set");
    }
    const std::string& fileName = module->GetFilePath(span.file);
    if (fileName.empty()) return std::unique_ptr<util::JsonObject>();
    std::unique_ptr<util::JsonObject> json(new util::JsonObject());
    json->AddField(U"file", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(fileName))));
    json->AddField(U"line", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(span.line)))));
    std::u32string text = module->GetErrorLines(span); 
    int32_t startCol = 0;
    int32_t endCol = 0;
    json->AddField(U"startCol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(startCol)))));
    json->AddField(U"endCol", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(endCol)))));
    json->AddField(U"text", std::unique_ptr<util::JsonValue>(new util::JsonString(text))); 
    return json;
}
*/

/*
std::unique_ptr<soul::xml::Element> SourcePosToDomElement(Module* module_, const soul::ast::Span& span)
{
    if (!span.IsValid()) return std::unique_ptr<soul::xml::Element>();
    if (!module_)
    {
        throw std::runtime_error("module not set");
    }
    const std::string& fileName = module_->GetFilePath(span.file);
    if (fileName.empty()) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> sourcePosElement(soul::xml::MakeElement("span"));
    std::unique_ptr<soul::xml::Element> fileElement(soul::xml::MakeElement("file"));
    std::unique_ptr<soul::xml::Text> fileText(soul::xml::MakeText((fileName)));
    fileElement->AppendChild(std::unique_ptr<soul::xml::Node>(fileText.release()));
    std::unique_ptr<soul::xml::Element> lineElement(soul::xml::MakeElement("line"));
    std::unique_ptr<soul::xml::Text> lineText(soul::xml::MakeText((std::to_string(span.line))));
    lineElement->AppendChild(std::unique_ptr<soul::xml::Node>(lineText.release()));
    std::u32string text = module_->GetErrorLines(span);
    int32_t startCol = 0;
    int32_t endCol = 0;
    module_->GetColumns(span, startCol, endCol);
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

Exception::Exception(const std::string& message_, const soul::ast::FullSpan& defined_) : what(Expand(message_, defined_)), message(message_), defined(defined_)
{
}

Exception::Exception(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_) : 
    what(Expand(message_, defined_, referenced_)), message(message_), defined(defined_)
{
    references.push_back(referenced_);
}

Exception::Exception(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_) :
    what(Expand(message_, defined_, references_)), message(message_), defined(defined_), references(references_)
{
}

Exception::~Exception()
{
}

void Exception::SetProject(const std::string& projectName_)
{
    projectName = projectName_;
    what = "project: " + projectName + ": " + what;
}

std::vector<cmajor::info::bs::CompileError> Exception::ToErrors() const
{
    std::vector<cmajor::info::bs::CompileError> errors;
    Module* module = GetModuleById(defined.moduleId);
    if (module)
    {
        cmajor::info::bs::CompileError mainError;
        mainError.message = message;
        mainError.project = projectName;
        mainError.file = module->GetFilePath(defined.fileIndex);
        soul::ast::LineColLen lineColLen = module->GetLineColLen(defined.span, defined.fileIndex);
        mainError.line = lineColLen.line;
        mainError.scol = lineColLen.col;
        mainError.ecol = lineColLen.col + lineColLen.len;
        errors.push_back(mainError);
        for (const auto& ref : references)
        {
            Module* module = GetModuleById(ref.moduleId);
            if (module)
            {
                cmajor::info::bs::CompileError refError;
                refError.message = "See reference to: ";
                refError.file = module->GetFilePath(ref.fileIndex);
                soul::ast::LineColLen lineColLen = module->GetLineColLen(ref.span, ref.fileIndex);
                refError.line = lineColLen.line;
                refError.scol = lineColLen.col;
                refError.ecol = lineColLen.col + lineColLen.len;
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

ModuleImmutableException::ModuleImmutableException(Module* module, Module* immutableModule, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_) :
    Exception("attempt to add a symbol to an immutable module' " + util::ToUtf8(immutableModule->Name()) + "'", defined_, referenced_)
{
}

SymbolCheckException::SymbolCheckException(const std::string& message_, const soul::ast::FullSpan& defined_) : Exception(message_, defined_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_) : Exception(message_, defined_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_) :
    Exception(message_, defined_, referenced_)
{
}

CastOverloadException::CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_) : 
    Exception(message_, defined_, references_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_) : Exception(message_, defined_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, 
    const soul::ast::FullSpan& referenced_) : Exception(message_, defined_, referenced_)
{
}

CannotBindConstToNonconstOverloadException::CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_,
    const std::vector<soul::ast::FullSpan>& references_) : Exception(message_, defined_, references_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_) : Exception(message_, defined_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_) :
    Exception(message_, defined_, referenced_)
{
}

CannotAssignToConstOverloadException::CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, 
    const std::vector<soul::ast::FullSpan>& references_) : Exception(message_, defined_, references_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_) : Exception(message_, defined_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_) :
    Exception(message_, defined_, referenced_)
{
}

NoViableFunctionException::NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_) :
    Exception(message_, defined_, references_)
{
}
} // namespace cmajor::symbols
