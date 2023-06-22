// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/msg/code_completion_message.schema' by xmlsergen version 4.1.0

module cmajor.msg.code.completion.message;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

CodeCompletionRequest::CodeCompletionRequest()
     : created(), received()
{
}

CodeCompletionRequest::~CodeCompletionRequest()
{
}

std::string CodeCompletionRequest::ClassName() const
{
    return "CodeCompletionRequest";
}

std::string CodeCompletionRequest::StaticClassName()
{
    return "CodeCompletionRequest";
}

void* CodeCompletionRequest::Create()
{
    return new CodeCompletionRequest();
}

void CodeCompletionRequest::Register()
{
    soul::xml::serialization::RegisterClass<CodeCompletionRequest>();
}

void CodeCompletionRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CodeCompletionRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CodeCompletionRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CodeCompletionRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CodeCompletionRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CodeCompletionRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CodeCompletionRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CodeCompletionRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(created, "created"));
    element->AppendChild(soul::xml::serialization::ToXml(received, "received"));
    return element;
}

void CodeCompletionRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "created", created);
    soul::xml::serialization::FromXml(element, "received", received);
}

CodeCompletionReply::CodeCompletionReply()
     : requestCreated(), requestReceived(), created()
{
}

CodeCompletionReply::~CodeCompletionReply()
{
}

std::string CodeCompletionReply::ClassName() const
{
    return "CodeCompletionReply";
}

std::string CodeCompletionReply::StaticClassName()
{
    return "CodeCompletionReply";
}

void* CodeCompletionReply::Create()
{
    return new CodeCompletionReply();
}

void CodeCompletionReply::Register()
{
    soul::xml::serialization::RegisterClass<CodeCompletionReply>();
}

void CodeCompletionReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CodeCompletionReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CodeCompletionReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CodeCompletionReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CodeCompletionReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CodeCompletionReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CodeCompletionReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CodeCompletionReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(requestCreated, "requestCreated"));
    element->AppendChild(soul::xml::serialization::ToXml(requestReceived, "requestReceived"));
    element->AppendChild(soul::xml::serialization::ToXml(created, "created"));
    return element;
}

void CodeCompletionReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "requestCreated", requestCreated);
    soul::xml::serialization::FromXml(element, "requestReceived", requestReceived);
    soul::xml::serialization::FromXml(element, "created", created);
}

LoadEditModuleRequest::LoadEditModuleRequest()
     : CodeCompletionRequest(), projectFilePath(), backend(), config()
{
}

std::string LoadEditModuleRequest::ClassName() const
{
    return "LoadEditModuleRequest";
}

std::string LoadEditModuleRequest::StaticClassName()
{
    return "LoadEditModuleRequest";
}

void* LoadEditModuleRequest::Create()
{
    return new LoadEditModuleRequest();
}

void LoadEditModuleRequest::Register()
{
    soul::xml::serialization::RegisterClass<LoadEditModuleRequest>();
}

soul::xml::Element* LoadEditModuleRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "LoadEditModuleRequest");
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    return element;
}

void LoadEditModuleRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
}

LoadEditModuleReply::LoadEditModuleReply()
     : CodeCompletionReply(), key(), result(), error(), startLoading(), startParsing(), endParsing(), endLoading()
{
}

std::string LoadEditModuleReply::ClassName() const
{
    return "LoadEditModuleReply";
}

std::string LoadEditModuleReply::StaticClassName()
{
    return "LoadEditModuleReply";
}

void* LoadEditModuleReply::Create()
{
    return new LoadEditModuleReply();
}

void LoadEditModuleReply::Register()
{
    soul::xml::serialization::RegisterClass<LoadEditModuleReply>();
}

soul::xml::Element* LoadEditModuleReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "LoadEditModuleReply");
    element->AppendChild(soul::xml::serialization::ToXml(key, "key"));
    element->AppendChild(soul::xml::serialization::ToXml(result, "result"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    element->AppendChild(soul::xml::serialization::ToXml(startLoading, "startLoading"));
    element->AppendChild(soul::xml::serialization::ToXml(startParsing, "startParsing"));
    element->AppendChild(soul::xml::serialization::ToXml(endParsing, "endParsing"));
    element->AppendChild(soul::xml::serialization::ToXml(endLoading, "endLoading"));
    return element;
}

void LoadEditModuleReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
    soul::xml::serialization::FromXml(element, "key", key);
    soul::xml::serialization::FromXml(element, "result", result);
    soul::xml::serialization::FromXml(element, "error", error);
    soul::xml::serialization::FromXml(element, "startLoading", startLoading);
    soul::xml::serialization::FromXml(element, "startParsing", startParsing);
    soul::xml::serialization::FromXml(element, "endParsing", endParsing);
    soul::xml::serialization::FromXml(element, "endLoading", endLoading);
}

ResetEditModuleCacheRequest::ResetEditModuleCacheRequest()
     : CodeCompletionRequest()
{
}

std::string ResetEditModuleCacheRequest::ClassName() const
{
    return "ResetEditModuleCacheRequest";
}

std::string ResetEditModuleCacheRequest::StaticClassName()
{
    return "ResetEditModuleCacheRequest";
}

void* ResetEditModuleCacheRequest::Create()
{
    return new ResetEditModuleCacheRequest();
}

void ResetEditModuleCacheRequest::Register()
{
    soul::xml::serialization::RegisterClass<ResetEditModuleCacheRequest>();
}

soul::xml::Element* ResetEditModuleCacheRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "ResetEditModuleCacheRequest");
    return element;
}

void ResetEditModuleCacheRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
}

ResetEditModuleCacheReply::ResetEditModuleCacheReply()
     : CodeCompletionReply(), result(), error()
{
}

std::string ResetEditModuleCacheReply::ClassName() const
{
    return "ResetEditModuleCacheReply";
}

std::string ResetEditModuleCacheReply::StaticClassName()
{
    return "ResetEditModuleCacheReply";
}

void* ResetEditModuleCacheReply::Create()
{
    return new ResetEditModuleCacheReply();
}

void ResetEditModuleCacheReply::Register()
{
    soul::xml::serialization::RegisterClass<ResetEditModuleCacheReply>();
}

soul::xml::Element* ResetEditModuleCacheReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "ResetEditModuleCacheReply");
    element->AppendChild(soul::xml::serialization::ToXml(result, "result"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void ResetEditModuleCacheReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
    soul::xml::serialization::FromXml(element, "result", result);
    soul::xml::serialization::FromXml(element, "error", error);
}

ParseSourceRequest::ParseSourceRequest()
     : CodeCompletionRequest(), projectFilePath(), backend(), config(), sourceFilePath(), sourceCode()
{
}

std::string ParseSourceRequest::ClassName() const
{
    return "ParseSourceRequest";
}

std::string ParseSourceRequest::StaticClassName()
{
    return "ParseSourceRequest";
}

void* ParseSourceRequest::Create()
{
    return new ParseSourceRequest();
}

void ParseSourceRequest::Register()
{
    soul::xml::serialization::RegisterClass<ParseSourceRequest>();
}

soul::xml::Element* ParseSourceRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "ParseSourceRequest");
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    element->AppendChild(soul::xml::serialization::ToXml(sourceFilePath, "sourceFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(sourceCode, "sourceCode"));
    return element;
}

void ParseSourceRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
    soul::xml::serialization::FromXml(element, "sourceFilePath", sourceFilePath);
    soul::xml::serialization::FromXml(element, "sourceCode", sourceCode);
}

ParseSourceReply::ParseSourceReply()
     : CodeCompletionReply(), ok(), error(), errors(), synchronized(), cursorContainer(), ruleContext(), startParsing(), endParsing()
{
}

std::string ParseSourceReply::ClassName() const
{
    return "ParseSourceReply";
}

std::string ParseSourceReply::StaticClassName()
{
    return "ParseSourceReply";
}

void* ParseSourceReply::Create()
{
    return new ParseSourceReply();
}

void ParseSourceReply::Register()
{
    soul::xml::serialization::RegisterClass<ParseSourceReply>();
}

soul::xml::Element* ParseSourceReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "ParseSourceReply");
    element->AppendChild(soul::xml::serialization::ToXml(ok, "ok"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    element->AppendChild(soul::xml::serialization::ToXml(errors, "errors"));
    element->AppendChild(soul::xml::serialization::ToXml(synchronized, "synchronized"));
    element->AppendChild(soul::xml::serialization::ToXml(cursorContainer, "cursorContainer"));
    element->AppendChild(soul::xml::serialization::ToXml(ruleContext, "ruleContext"));
    element->AppendChild(soul::xml::serialization::ToXml(startParsing, "startParsing"));
    element->AppendChild(soul::xml::serialization::ToXml(endParsing, "endParsing"));
    return element;
}

void ParseSourceReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
    soul::xml::serialization::FromXml(element, "ok", ok);
    soul::xml::serialization::FromXml(element, "error", error);
    soul::xml::serialization::FromXml(element, "errors", errors);
    soul::xml::serialization::FromXml(element, "synchronized", synchronized);
    soul::xml::serialization::FromXml(element, "cursorContainer", cursorContainer);
    soul::xml::serialization::FromXml(element, "ruleContext", ruleContext);
    soul::xml::serialization::FromXml(element, "startParsing", startParsing);
    soul::xml::serialization::FromXml(element, "endParsing", endParsing);
}

GetCCListRequest::GetCCListRequest()
     : CodeCompletionRequest(), projectFilePath(), backend(), config(), sourceFilePath(), ccText(), ruleContext(), cursorLine()
{
}

std::string GetCCListRequest::ClassName() const
{
    return "GetCCListRequest";
}

std::string GetCCListRequest::StaticClassName()
{
    return "GetCCListRequest";
}

void* GetCCListRequest::Create()
{
    return new GetCCListRequest();
}

void GetCCListRequest::Register()
{
    soul::xml::serialization::RegisterClass<GetCCListRequest>();
}

soul::xml::Element* GetCCListRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "GetCCListRequest");
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    element->AppendChild(soul::xml::serialization::ToXml(sourceFilePath, "sourceFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(ccText, "ccText"));
    element->AppendChild(soul::xml::serialization::ToXml(ruleContext, "ruleContext"));
    element->AppendChild(soul::xml::serialization::ToXml(cursorLine, "cursorLine"));
    return element;
}

void GetCCListRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
    soul::xml::serialization::FromXml(element, "sourceFilePath", sourceFilePath);
    soul::xml::serialization::FromXml(element, "ccText", ccText);
    soul::xml::serialization::FromXml(element, "ruleContext", ruleContext);
    soul::xml::serialization::FromXml(element, "cursorLine", cursorLine);
}

GetCCListReply::GetCCListReply()
     : CodeCompletionReply(), ok(), error(), ccList(), startGetCCList(), endGetCCList()
{
}

std::string GetCCListReply::ClassName() const
{
    return "GetCCListReply";
}

std::string GetCCListReply::StaticClassName()
{
    return "GetCCListReply";
}

void* GetCCListReply::Create()
{
    return new GetCCListReply();
}

void GetCCListReply::Register()
{
    soul::xml::serialization::RegisterClass<GetCCListReply>();
}

soul::xml::Element* GetCCListReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "GetCCListReply");
    element->AppendChild(soul::xml::serialization::ToXml(ok, "ok"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    element->AppendChild(soul::xml::serialization::ToXml(ccList, "ccList"));
    element->AppendChild(soul::xml::serialization::ToXml(startGetCCList, "startGetCCList"));
    element->AppendChild(soul::xml::serialization::ToXml(endGetCCList, "endGetCCList"));
    return element;
}

void GetCCListReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
    soul::xml::serialization::FromXml(element, "ok", ok);
    soul::xml::serialization::FromXml(element, "error", error);
    soul::xml::serialization::FromXml(element, "ccList", ccList);
    soul::xml::serialization::FromXml(element, "startGetCCList", startGetCCList);
    soul::xml::serialization::FromXml(element, "endGetCCList", endGetCCList);
}

GetParamHelpListRequest::GetParamHelpListRequest()
     : CodeCompletionRequest(), projectFilePath(), backend(), config(), sourceFilePath(), symbolIndex()
{
}

std::string GetParamHelpListRequest::ClassName() const
{
    return "GetParamHelpListRequest";
}

std::string GetParamHelpListRequest::StaticClassName()
{
    return "GetParamHelpListRequest";
}

void* GetParamHelpListRequest::Create()
{
    return new GetParamHelpListRequest();
}

void GetParamHelpListRequest::Register()
{
    soul::xml::serialization::RegisterClass<GetParamHelpListRequest>();
}

soul::xml::Element* GetParamHelpListRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "GetParamHelpListRequest");
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    element->AppendChild(soul::xml::serialization::ToXml(sourceFilePath, "sourceFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(symbolIndex, "symbolIndex"));
    return element;
}

void GetParamHelpListRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
    soul::xml::serialization::FromXml(element, "sourceFilePath", sourceFilePath);
    soul::xml::serialization::FromXml(element, "symbolIndex", symbolIndex);
}

GetParamHelpListReply::GetParamHelpListReply()
     : CodeCompletionReply(), ok(), error(), list(), startGetParamHelpList(), endGetParamHelpList()
{
}

std::string GetParamHelpListReply::ClassName() const
{
    return "GetParamHelpListReply";
}

std::string GetParamHelpListReply::StaticClassName()
{
    return "GetParamHelpListReply";
}

void* GetParamHelpListReply::Create()
{
    return new GetParamHelpListReply();
}

void GetParamHelpListReply::Register()
{
    soul::xml::serialization::RegisterClass<GetParamHelpListReply>();
}

soul::xml::Element* GetParamHelpListReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "GetParamHelpListReply");
    element->AppendChild(soul::xml::serialization::ToXml(ok, "ok"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    element->AppendChild(soul::xml::serialization::ToXml(list, "list"));
    element->AppendChild(soul::xml::serialization::ToXml(startGetParamHelpList, "startGetParamHelpList"));
    element->AppendChild(soul::xml::serialization::ToXml(endGetParamHelpList, "endGetParamHelpList"));
    return element;
}

void GetParamHelpListReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
    soul::xml::serialization::FromXml(element, "ok", ok);
    soul::xml::serialization::FromXml(element, "error", error);
    soul::xml::serialization::FromXml(element, "list", list);
    soul::xml::serialization::FromXml(element, "startGetParamHelpList", startGetParamHelpList);
    soul::xml::serialization::FromXml(element, "endGetParamHelpList", endGetParamHelpList);
}

StopCCRequest::StopCCRequest()
     : CodeCompletionRequest()
{
}

std::string StopCCRequest::ClassName() const
{
    return "StopCCRequest";
}

std::string StopCCRequest::StaticClassName()
{
    return "StopCCRequest";
}

void* StopCCRequest::Create()
{
    return new StopCCRequest();
}

void StopCCRequest::Register()
{
    soul::xml::serialization::RegisterClass<StopCCRequest>();
}

soul::xml::Element* StopCCRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionRequest::ToXml(fieldName);
    element->SetAttribute("className", "StopCCRequest");
    return element;
}

void StopCCRequest::FromXml(soul::xml::Element* element)
{
    CodeCompletionRequest::FromXml(element);
}

StopCCReply::StopCCReply()
     : CodeCompletionReply()
{
}

std::string StopCCReply::ClassName() const
{
    return "StopCCReply";
}

std::string StopCCReply::StaticClassName()
{
    return "StopCCReply";
}

void* StopCCReply::Create()
{
    return new StopCCReply();
}

void StopCCReply::Register()
{
    soul::xml::serialization::RegisterClass<StopCCReply>();
}

soul::xml::Element* StopCCReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = CodeCompletionReply::ToXml(fieldName);
    element->SetAttribute("className", "StopCCReply");
    return element;
}

void StopCCReply::FromXml(soul::xml::Element* element)
{
    CodeCompletionReply::FromXml(element);
}

KeepAliveCCRequest::KeepAliveCCRequest()
{
}

KeepAliveCCRequest::~KeepAliveCCRequest()
{
}

std::string KeepAliveCCRequest::ClassName() const
{
    return "KeepAliveCCRequest";
}

std::string KeepAliveCCRequest::StaticClassName()
{
    return "KeepAliveCCRequest";
}

void* KeepAliveCCRequest::Create()
{
    return new KeepAliveCCRequest();
}

void KeepAliveCCRequest::Register()
{
    soul::xml::serialization::RegisterClass<KeepAliveCCRequest>();
}

void KeepAliveCCRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveCCRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KeepAliveCCRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveCCRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KeepAliveCCRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KeepAliveCCRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KeepAliveCCRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KeepAliveCCRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KeepAliveCCRequest::FromXml(soul::xml::Element* element)
{
}

KeepAliveCCReply::KeepAliveCCReply()
{
}

KeepAliveCCReply::~KeepAliveCCReply()
{
}

std::string KeepAliveCCReply::ClassName() const
{
    return "KeepAliveCCReply";
}

std::string KeepAliveCCReply::StaticClassName()
{
    return "KeepAliveCCReply";
}

void* KeepAliveCCReply::Create()
{
    return new KeepAliveCCReply();
}

void KeepAliveCCReply::Register()
{
    soul::xml::serialization::RegisterClass<KeepAliveCCReply>();
}

void KeepAliveCCReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveCCReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KeepAliveCCReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveCCReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KeepAliveCCReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KeepAliveCCReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KeepAliveCCReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KeepAliveCCReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KeepAliveCCReply::FromXml(soul::xml::Element* element)
{
}


namespace {

struct Reg
{
    Reg()
    {
        CodeCompletionRequest::Register();
        CodeCompletionReply::Register();
        LoadEditModuleRequest::Register();
        LoadEditModuleReply::Register();
        ResetEditModuleCacheRequest::Register();
        ResetEditModuleCacheReply::Register();
        ParseSourceRequest::Register();
        ParseSourceReply::Register();
        GetCCListRequest::Register();
        GetCCListReply::Register();
        GetParamHelpListRequest::Register();
        GetParamHelpListReply::Register();
        StopCCRequest::Register();
        StopCCReply::Register();
        KeepAliveCCRequest::Register();
        KeepAliveCCReply::Register();
    }
};

Reg reg;

} // namespace

