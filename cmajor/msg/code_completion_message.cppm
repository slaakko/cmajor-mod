// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/msg/code_completion_message.schema' by xmlsergen version 4.1.0

export module cmajor.msg.code.completion.message;

import std.core;
import util.uuid;
import util.time;
import soul.xml.element;

export class CodeCompletionRequest
{
public:
    CodeCompletionRequest();
    virtual ~CodeCompletionRequest();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
    std::chrono::steady_clock::time_point created;
    std::chrono::steady_clock::time_point received;
};

export class CodeCompletionReply
{
public:
    CodeCompletionReply();
    virtual ~CodeCompletionReply();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
    std::chrono::steady_clock::time_point requestCreated;
    std::chrono::steady_clock::time_point requestReceived;
    std::chrono::steady_clock::time_point created;
};

export class LoadEditModuleRequest : public CodeCompletionRequest
{
public:
    LoadEditModuleRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string projectFilePath;
    std::string backend;
    std::string config;
};

export class LoadEditModuleReply : public CodeCompletionReply
{
public:
    LoadEditModuleReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string key;
    std::string result;
    std::string error;
    std::chrono::steady_clock::time_point startLoading;
    std::chrono::steady_clock::time_point startParsing;
    std::chrono::steady_clock::time_point endParsing;
    std::chrono::steady_clock::time_point endLoading;
};

export class ResetEditModuleCacheRequest : public CodeCompletionRequest
{
public:
    ResetEditModuleCacheRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
};

export class ResetEditModuleCacheReply : public CodeCompletionReply
{
public:
    ResetEditModuleCacheReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string result;
    std::string error;
};

export class ParseSourceRequest : public CodeCompletionRequest
{
public:
    ParseSourceRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string projectFilePath;
    std::string backend;
    std::string config;
    std::string sourceFilePath;
    std::u32string sourceCode;
};

export class ParseSourceReply : public CodeCompletionReply
{
public:
    ParseSourceReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    bool ok;
    std::string error;
    std::vector<std::string> errors;
    bool synchronized;
    std::string cursorContainer;
    std::vector<int32_t> ruleContext;
    std::chrono::steady_clock::time_point startParsing;
    std::chrono::steady_clock::time_point endParsing;
};

export class GetCCListRequest : public CodeCompletionRequest
{
public:
    GetCCListRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string projectFilePath;
    std::string backend;
    std::string config;
    std::string sourceFilePath;
    std::u32string ccText;
    std::vector<int32_t> ruleContext;
    std::u32string cursorLine;
};

export class GetCCListReply : public CodeCompletionReply
{
public:
    GetCCListReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    bool ok;
    std::string error;
    std::string ccList;
    std::chrono::steady_clock::time_point startGetCCList;
    std::chrono::steady_clock::time_point endGetCCList;
};

export class GetParamHelpListRequest : public CodeCompletionRequest
{
public:
    GetParamHelpListRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    std::string projectFilePath;
    std::string backend;
    std::string config;
    std::string sourceFilePath;
    int32_t symbolIndex;
};

export class GetParamHelpListReply : public CodeCompletionReply
{
public:
    GetParamHelpListReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
    bool ok;
    std::string error;
    std::string list;
    std::chrono::steady_clock::time_point startGetParamHelpList;
    std::chrono::steady_clock::time_point endGetParamHelpList;
};

export class StopCCRequest : public CodeCompletionRequest
{
public:
    StopCCRequest();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
};

export class StopCCReply : public CodeCompletionReply
{
public:
    StopCCReply();
    std::string ClassName() const override;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    soul::xml::Element* ToXml(const std::string& fieldName) const override;
    void FromXml(soul::xml::Element* element) override;
public:
};

export class KeepAliveCCRequest
{
public:
    KeepAliveCCRequest();
    virtual ~KeepAliveCCRequest();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
};

export class KeepAliveCCReply
{
public:
    KeepAliveCCReply();
    virtual ~KeepAliveCCReply();
    virtual std::string ClassName() const;
    static std::string StaticClassName();
    static void* Create();
    static void Register();
    void WriteToXmlFile(const std::string& xmlFileName);
    std::string ToXmlString();
    void ReadFromXmlFile(const std::string& xmlFileName);
    void ReadFromXmlString(const std::string& xmlString);
    virtual soul::xml::Element* ToXml(const std::string& fieldName) const;
    virtual void FromXml(soul::xml::Element* element);
public:
};

