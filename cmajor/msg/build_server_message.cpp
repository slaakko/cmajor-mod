// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/msg/build_server_message.schema' by xmlsergen version 4.1.0

module cmajor.msg.build.server.message;

import soul.xml.document;
import soul.xml.dom.parser;
import soul.xml.serialization.class_registry;
import soul.xml.serialization.exp;
import soul.xml.serialization.imp;

StopBuildRequest::StopBuildRequest()
{
}

StopBuildRequest::~StopBuildRequest()
{
}

std::string StopBuildRequest::ClassName() const
{
    return "StopBuildRequest";
}

std::string StopBuildRequest::StaticClassName()
{
    return "StopBuildRequest";
}

void* StopBuildRequest::Create()
{
    return new StopBuildRequest();
}

void StopBuildRequest::Register()
{
    soul::xml::serialization::RegisterClass<StopBuildRequest>();
}

void StopBuildRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopBuildRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopBuildRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopBuildRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopBuildRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopBuildRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopBuildRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopBuildRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopBuildRequest::FromXml(soul::xml::Element* element)
{
}

StopBuildReply::StopBuildReply()
{
}

StopBuildReply::~StopBuildReply()
{
}

std::string StopBuildReply::ClassName() const
{
    return "StopBuildReply";
}

std::string StopBuildReply::StaticClassName()
{
    return "StopBuildReply";
}

void* StopBuildReply::Create()
{
    return new StopBuildReply();
}

void StopBuildReply::Register()
{
    soul::xml::serialization::RegisterClass<StopBuildReply>();
}

void StopBuildReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopBuildReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string StopBuildReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("StopBuildReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void StopBuildReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void StopBuildReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "StopBuildReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* StopBuildReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void StopBuildReply::FromXml(soul::xml::Element* element)
{
}

KeepAliveBuildRequest::KeepAliveBuildRequest()
{
}

KeepAliveBuildRequest::~KeepAliveBuildRequest()
{
}

std::string KeepAliveBuildRequest::ClassName() const
{
    return "KeepAliveBuildRequest";
}

std::string KeepAliveBuildRequest::StaticClassName()
{
    return "KeepAliveBuildRequest";
}

void* KeepAliveBuildRequest::Create()
{
    return new KeepAliveBuildRequest();
}

void KeepAliveBuildRequest::Register()
{
    soul::xml::serialization::RegisterClass<KeepAliveBuildRequest>();
}

void KeepAliveBuildRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveBuildRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KeepAliveBuildRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveBuildRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KeepAliveBuildRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KeepAliveBuildRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KeepAliveBuildRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KeepAliveBuildRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KeepAliveBuildRequest::FromXml(soul::xml::Element* element)
{
}

KeepAliveBuildReply::KeepAliveBuildReply()
{
}

KeepAliveBuildReply::~KeepAliveBuildReply()
{
}

std::string KeepAliveBuildReply::ClassName() const
{
    return "KeepAliveBuildReply";
}

std::string KeepAliveBuildReply::StaticClassName()
{
    return "KeepAliveBuildReply";
}

void* KeepAliveBuildReply::Create()
{
    return new KeepAliveBuildReply();
}

void KeepAliveBuildReply::Register()
{
    soul::xml::serialization::RegisterClass<KeepAliveBuildReply>();
}

void KeepAliveBuildReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveBuildReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string KeepAliveBuildReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("KeepAliveBuildReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void KeepAliveBuildReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void KeepAliveBuildReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "KeepAliveBuildReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* KeepAliveBuildReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void KeepAliveBuildReply::FromXml(soul::xml::Element* element)
{
}

BuildRequest::BuildRequest()
     : messageKind(), backend(), config(), optimizationLevel(), filePath(), defines(), numBuildThreads(), toolChain(), verbose(), quiet(), clean(), strictNothrow(), emitIR(), emitOptIR(), linkWithDebugRuntime(), linkUsingMsLink(), rebuild(), buildAllDependencies(), forceDebugInfo(), forceNoDebugInfo(), disableModuleCache(), singleThreadedCompile(), disableCodeGen(), justMyCode()
{
}

BuildRequest::~BuildRequest()
{
}

std::string BuildRequest::ClassName() const
{
    return "BuildRequest";
}

std::string BuildRequest::StaticClassName()
{
    return "BuildRequest";
}

void* BuildRequest::Create()
{
    return new BuildRequest();
}

void BuildRequest::Register()
{
    soul::xml::serialization::RegisterClass<BuildRequest>();
}

void BuildRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BuildRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BuildRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BuildRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BuildRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BuildRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(messageKind, "messageKind"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    element->AppendChild(soul::xml::serialization::ToXml(optimizationLevel, "optimizationLevel"));
    element->AppendChild(soul::xml::serialization::ToXml(filePath, "filePath"));
    element->AppendChild(soul::xml::serialization::ToXml(defines, "defines"));
    element->AppendChild(soul::xml::serialization::ToXml(numBuildThreads, "numBuildThreads"));
    element->AppendChild(soul::xml::serialization::ToXml(toolChain, "toolChain"));
    element->AppendChild(soul::xml::serialization::ToXml(verbose, "verbose"));
    element->AppendChild(soul::xml::serialization::ToXml(quiet, "quiet"));
    element->AppendChild(soul::xml::serialization::ToXml(clean, "clean"));
    element->AppendChild(soul::xml::serialization::ToXml(strictNothrow, "strictNothrow"));
    element->AppendChild(soul::xml::serialization::ToXml(emitIR, "emitIR"));
    element->AppendChild(soul::xml::serialization::ToXml(emitOptIR, "emitOptIR"));
    element->AppendChild(soul::xml::serialization::ToXml(linkWithDebugRuntime, "linkWithDebugRuntime"));
    element->AppendChild(soul::xml::serialization::ToXml(linkUsingMsLink, "linkUsingMsLink"));
    element->AppendChild(soul::xml::serialization::ToXml(rebuild, "rebuild"));
    element->AppendChild(soul::xml::serialization::ToXml(buildAllDependencies, "buildAllDependencies"));
    element->AppendChild(soul::xml::serialization::ToXml(forceDebugInfo, "forceDebugInfo"));
    element->AppendChild(soul::xml::serialization::ToXml(forceNoDebugInfo, "forceNoDebugInfo"));
    element->AppendChild(soul::xml::serialization::ToXml(disableModuleCache, "disableModuleCache"));
    element->AppendChild(soul::xml::serialization::ToXml(singleThreadedCompile, "singleThreadedCompile"));
    element->AppendChild(soul::xml::serialization::ToXml(disableCodeGen, "disableCodeGen"));
    element->AppendChild(soul::xml::serialization::ToXml(justMyCode, "justMyCode"));
    return element;
}

void BuildRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "messageKind", messageKind);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
    soul::xml::serialization::FromXml(element, "optimizationLevel", optimizationLevel);
    soul::xml::serialization::FromXml(element, "filePath", filePath);
    soul::xml::serialization::FromXml(element, "defines", defines);
    soul::xml::serialization::FromXml(element, "numBuildThreads", numBuildThreads);
    soul::xml::serialization::FromXml(element, "toolChain", toolChain);
    soul::xml::serialization::FromXml(element, "verbose", verbose);
    soul::xml::serialization::FromXml(element, "quiet", quiet);
    soul::xml::serialization::FromXml(element, "clean", clean);
    soul::xml::serialization::FromXml(element, "strictNothrow", strictNothrow);
    soul::xml::serialization::FromXml(element, "emitIR", emitIR);
    soul::xml::serialization::FromXml(element, "emitOptIR", emitOptIR);
    soul::xml::serialization::FromXml(element, "linkWithDebugRuntime", linkWithDebugRuntime);
    soul::xml::serialization::FromXml(element, "linkUsingMsLink", linkUsingMsLink);
    soul::xml::serialization::FromXml(element, "rebuild", rebuild);
    soul::xml::serialization::FromXml(element, "buildAllDependencies", buildAllDependencies);
    soul::xml::serialization::FromXml(element, "forceDebugInfo", forceDebugInfo);
    soul::xml::serialization::FromXml(element, "forceNoDebugInfo", forceNoDebugInfo);
    soul::xml::serialization::FromXml(element, "disableModuleCache", disableModuleCache);
    soul::xml::serialization::FromXml(element, "singleThreadedCompile", singleThreadedCompile);
    soul::xml::serialization::FromXml(element, "disableCodeGen", disableCodeGen);
    soul::xml::serialization::FromXml(element, "justMyCode", justMyCode);
}

CompileError::CompileError()
     : message(), project(), file(), line(), scol(), ecol()
{
}

CompileError::~CompileError()
{
}

std::string CompileError::ClassName() const
{
    return "CompileError";
}

std::string CompileError::StaticClassName()
{
    return "CompileError";
}

void* CompileError::Create()
{
    return new CompileError();
}

void CompileError::Register()
{
    soul::xml::serialization::RegisterClass<CompileError>();
}

void CompileError::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CompileError"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CompileError::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CompileError"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CompileError::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CompileError::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CompileError");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CompileError::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(message, "message"));
    element->AppendChild(soul::xml::serialization::ToXml(project, "project"));
    element->AppendChild(soul::xml::serialization::ToXml(file, "file"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(scol, "scol"));
    element->AppendChild(soul::xml::serialization::ToXml(ecol, "ecol"));
    return element;
}

void CompileError::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "message", message);
    soul::xml::serialization::FromXml(element, "project", project);
    soul::xml::serialization::FromXml(element, "file", file);
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "scol", scol);
    soul::xml::serialization::FromXml(element, "ecol", ecol);
}

BuildReply::BuildReply()
     : requestValid(), requestErrorMessage(), logException(), time(), success(), errors()
{
}

BuildReply::~BuildReply()
{
}

std::string BuildReply::ClassName() const
{
    return "BuildReply";
}

std::string BuildReply::StaticClassName()
{
    return "BuildReply";
}

void* BuildReply::Create()
{
    return new BuildReply();
}

void BuildReply::Register()
{
    soul::xml::serialization::RegisterClass<BuildReply>();
}

void BuildReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BuildReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BuildReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BuildReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BuildReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BuildReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(requestValid, "requestValid"));
    element->AppendChild(soul::xml::serialization::ToXml(requestErrorMessage, "requestErrorMessage"));
    element->AppendChild(soul::xml::serialization::ToXml(logException, "logException"));
    element->AppendChild(soul::xml::serialization::ToXml(time, "time"));
    element->AppendChild(soul::xml::serialization::ToXml(success, "success"));
    element->AppendChild(soul::xml::serialization::ToXml(errors, "errors"));
    return element;
}

void BuildReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "requestValid", requestValid);
    soul::xml::serialization::FromXml(element, "requestErrorMessage", requestErrorMessage);
    soul::xml::serialization::FromXml(element, "logException", logException);
    soul::xml::serialization::FromXml(element, "time", time);
    soul::xml::serialization::FromXml(element, "success", success);
    soul::xml::serialization::FromXml(element, "errors", errors);
}

LogBuildMessageRequest::LogBuildMessageRequest()
     : message()
{
}

LogBuildMessageRequest::~LogBuildMessageRequest()
{
}

std::string LogBuildMessageRequest::ClassName() const
{
    return "LogBuildMessageRequest";
}

std::string LogBuildMessageRequest::StaticClassName()
{
    return "LogBuildMessageRequest";
}

void* LogBuildMessageRequest::Create()
{
    return new LogBuildMessageRequest();
}

void LogBuildMessageRequest::Register()
{
    soul::xml::serialization::RegisterClass<LogBuildMessageRequest>();
}

void LogBuildMessageRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogBuildMessageRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string LogBuildMessageRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogBuildMessageRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void LogBuildMessageRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void LogBuildMessageRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "LogBuildMessageRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* LogBuildMessageRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(message, "message"));
    return element;
}

void LogBuildMessageRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "message", message);
}

LogBuildMessageReply::LogBuildMessageReply()
     : ok()
{
}

LogBuildMessageReply::~LogBuildMessageReply()
{
}

std::string LogBuildMessageReply::ClassName() const
{
    return "LogBuildMessageReply";
}

std::string LogBuildMessageReply::StaticClassName()
{
    return "LogBuildMessageReply";
}

void* LogBuildMessageReply::Create()
{
    return new LogBuildMessageReply();
}

void LogBuildMessageReply::Register()
{
    soul::xml::serialization::RegisterClass<LogBuildMessageReply>();
}

void LogBuildMessageReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogBuildMessageReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string LogBuildMessageReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("LogBuildMessageReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void LogBuildMessageReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void LogBuildMessageReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "LogBuildMessageReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* LogBuildMessageReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(ok, "ok"));
    return element;
}

void LogBuildMessageReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "ok", ok);
}

BuildProgressMessage::BuildProgressMessage()
{
}

BuildProgressMessage::~BuildProgressMessage()
{
}

std::string BuildProgressMessage::ClassName() const
{
    return "BuildProgressMessage";
}

std::string BuildProgressMessage::StaticClassName()
{
    return "BuildProgressMessage";
}

void* BuildProgressMessage::Create()
{
    return new BuildProgressMessage();
}

void BuildProgressMessage::Register()
{
    soul::xml::serialization::RegisterClass<BuildProgressMessage>();
}

void BuildProgressMessage::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildProgressMessage"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string BuildProgressMessage::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("BuildProgressMessage"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void BuildProgressMessage::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void BuildProgressMessage::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "BuildProgressMessage");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* BuildProgressMessage::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    return element;
}

void BuildProgressMessage::FromXml(soul::xml::Element* element)
{
}

GenericBuildErrorReply::GenericBuildErrorReply()
     : error()
{
}

GenericBuildErrorReply::~GenericBuildErrorReply()
{
}

std::string GenericBuildErrorReply::ClassName() const
{
    return "GenericBuildErrorReply";
}

std::string GenericBuildErrorReply::StaticClassName()
{
    return "GenericBuildErrorReply";
}

void* GenericBuildErrorReply::Create()
{
    return new GenericBuildErrorReply();
}

void GenericBuildErrorReply::Register()
{
    soul::xml::serialization::RegisterClass<GenericBuildErrorReply>();
}

void GenericBuildErrorReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GenericBuildErrorReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GenericBuildErrorReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GenericBuildErrorReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GenericBuildErrorReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GenericBuildErrorReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GenericBuildErrorReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GenericBuildErrorReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void GenericBuildErrorReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "error", error);
}

CacheModuleRequest::CacheModuleRequest()
     : projectName(), projectFilePath(), backend(), config()
{
}

CacheModuleRequest::~CacheModuleRequest()
{
}

std::string CacheModuleRequest::ClassName() const
{
    return "CacheModuleRequest";
}

std::string CacheModuleRequest::StaticClassName()
{
    return "CacheModuleRequest";
}

void* CacheModuleRequest::Create()
{
    return new CacheModuleRequest();
}

void CacheModuleRequest::Register()
{
    soul::xml::serialization::RegisterClass<CacheModuleRequest>();
}

void CacheModuleRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CacheModuleRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CacheModuleRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CacheModuleRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CacheModuleRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CacheModuleRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CacheModuleRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CacheModuleRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(projectName, "projectName"));
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    return element;
}

void CacheModuleRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "projectName", projectName);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
}

CacheModuleReply::CacheModuleReply()
     : error()
{
}

CacheModuleReply::~CacheModuleReply()
{
}

std::string CacheModuleReply::ClassName() const
{
    return "CacheModuleReply";
}

std::string CacheModuleReply::StaticClassName()
{
    return "CacheModuleReply";
}

void* CacheModuleReply::Create()
{
    return new CacheModuleReply();
}

void CacheModuleReply::Register()
{
    soul::xml::serialization::RegisterClass<CacheModuleReply>();
}

void CacheModuleReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CacheModuleReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string CacheModuleReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("CacheModuleReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void CacheModuleReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void CacheModuleReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "CacheModuleReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* CacheModuleReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void CacheModuleReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "error", error);
}

DefinitionSourceLocation::DefinitionSourceLocation()
     : file(), line(), scol()
{
}

DefinitionSourceLocation::~DefinitionSourceLocation()
{
}

std::string DefinitionSourceLocation::ClassName() const
{
    return "DefinitionSourceLocation";
}

std::string DefinitionSourceLocation::StaticClassName()
{
    return "DefinitionSourceLocation";
}

void* DefinitionSourceLocation::Create()
{
    return new DefinitionSourceLocation();
}

void DefinitionSourceLocation::Register()
{
    soul::xml::serialization::RegisterClass<DefinitionSourceLocation>();
}

void DefinitionSourceLocation::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DefinitionSourceLocation"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string DefinitionSourceLocation::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("DefinitionSourceLocation"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void DefinitionSourceLocation::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void DefinitionSourceLocation::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "DefinitionSourceLocation");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* DefinitionSourceLocation::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(file, "file"));
    element->AppendChild(soul::xml::serialization::ToXml(line, "line"));
    element->AppendChild(soul::xml::serialization::ToXml(scol, "scol"));
    return element;
}

void DefinitionSourceLocation::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "file", file);
    soul::xml::serialization::FromXml(element, "line", line);
    soul::xml::serialization::FromXml(element, "scol", scol);
}

GetDefinitionRequest::GetDefinitionRequest()
     : projectName(), projectFilePath(), backend(), config(), identifier(), identifierLocation()
{
}

GetDefinitionRequest::~GetDefinitionRequest()
{
}

std::string GetDefinitionRequest::ClassName() const
{
    return "GetDefinitionRequest";
}

std::string GetDefinitionRequest::StaticClassName()
{
    return "GetDefinitionRequest";
}

void* GetDefinitionRequest::Create()
{
    return new GetDefinitionRequest();
}

void GetDefinitionRequest::Register()
{
    soul::xml::serialization::RegisterClass<GetDefinitionRequest>();
}

void GetDefinitionRequest::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetDefinitionRequest"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GetDefinitionRequest::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetDefinitionRequest"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GetDefinitionRequest::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GetDefinitionRequest::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GetDefinitionRequest");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GetDefinitionRequest::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(projectName, "projectName"));
    element->AppendChild(soul::xml::serialization::ToXml(projectFilePath, "projectFilePath"));
    element->AppendChild(soul::xml::serialization::ToXml(backend, "backend"));
    element->AppendChild(soul::xml::serialization::ToXml(config, "config"));
    element->AppendChild(soul::xml::serialization::ToXml(identifier, "identifier"));
    element->AppendChild(soul::xml::serialization::ToXml(identifierLocation, "identifierLocation"));
    return element;
}

void GetDefinitionRequest::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "projectName", projectName);
    soul::xml::serialization::FromXml(element, "projectFilePath", projectFilePath);
    soul::xml::serialization::FromXml(element, "backend", backend);
    soul::xml::serialization::FromXml(element, "config", config);
    soul::xml::serialization::FromXml(element, "identifier", identifier);
    soul::xml::serialization::FromXml(element, "identifierLocation", identifierLocation);
}

GetDefinitionReply::GetDefinitionReply()
     : definitionLocation(), ok(), error()
{
}

GetDefinitionReply::~GetDefinitionReply()
{
}

std::string GetDefinitionReply::ClassName() const
{
    return "GetDefinitionReply";
}

std::string GetDefinitionReply::StaticClassName()
{
    return "GetDefinitionReply";
}

void* GetDefinitionReply::Create()
{
    return new GetDefinitionReply();
}

void GetDefinitionReply::Register()
{
    soul::xml::serialization::RegisterClass<GetDefinitionReply>();
}

void GetDefinitionReply::WriteToXmlFile(const std::string& xmlFileName)
{
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetDefinitionReply"));
    std::ofstream xmlFile(xmlFileName);
    util::CodeFormatter formatter(xmlFile);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
}

std::string GetDefinitionReply::ToXmlString()
{
    std::stringstream stringStream;
    soul::xml::Document xmlDoc;
    xmlDoc.AppendChild(ToXml("GetDefinitionReply"));
    util::CodeFormatter formatter(stringStream);
    formatter.SetIndentSize(2);
    xmlDoc.Write(formatter);
    return stringStream.str();
}

void GetDefinitionReply::ReadFromXmlFile(const std::string& xmlFileName)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlFile(xmlFileName);
    FromXml(xmlDoc->DocumentElement());
}

void GetDefinitionReply::ReadFromXmlString(const std::string& xmlString)
{
    std::unique_ptr<soul::xml::Document> xmlDoc = soul::xml::ParseXmlContent(xmlString, "GetDefinitionReply");
    FromXml(xmlDoc->DocumentElement());
}

soul::xml::Element* GetDefinitionReply::ToXml(const std::string& fieldName) const
{
    soul::xml::Element* element = soul::xml::MakeElement(fieldName);
    element->AppendChild(soul::xml::serialization::ToXml(definitionLocation, "definitionLocation"));
    element->AppendChild(soul::xml::serialization::ToXml(ok, "ok"));
    element->AppendChild(soul::xml::serialization::ToXml(error, "error"));
    return element;
}

void GetDefinitionReply::FromXml(soul::xml::Element* element)
{
    soul::xml::serialization::FromXml(element, "definitionLocation", definitionLocation);
    soul::xml::serialization::FromXml(element, "ok", ok);
    soul::xml::serialization::FromXml(element, "error", error);
}


namespace {

struct Reg
{
    Reg()
    {
        StopBuildRequest::Register();
        StopBuildReply::Register();
        KeepAliveBuildRequest::Register();
        KeepAliveBuildReply::Register();
        BuildRequest::Register();
        CompileError::Register();
        BuildReply::Register();
        LogBuildMessageRequest::Register();
        LogBuildMessageReply::Register();
        BuildProgressMessage::Register();
        GenericBuildErrorReply::Register();
        CacheModuleRequest::Register();
        CacheModuleReply::Register();
        DefinitionSourceLocation::Register();
        GetDefinitionRequest::Register();
        GetDefinitionReply::Register();
    }
};

Reg reg;

} // namespace

