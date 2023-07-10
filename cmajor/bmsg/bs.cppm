// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bmsg/bs.schema' by cmajor binary protocol message generator version 5.0.0

export module cmajor.build.server.message;

import std.core;
import util;
import cmajor.binary.message.protocol;
import cmajor.binary.message.number;
import cmajor.common.message;


using SourceLoc = common::SourceLoc;
export namespace bs {

const uint32_t bmpStopBuildRequestId = 2706131288;

class StopBuildRequest : public cmajor::bmp::BinaryMessage
{
public:
    StopBuildRequest();
    StopBuildRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpStopBuildReplyId = 3174723854;

class StopBuildReply : public cmajor::bmp::BinaryMessage
{
public:
    StopBuildReply();
    StopBuildReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpKeepAliveBuildRequestId = 4286217037;

class KeepAliveBuildRequest : public cmajor::bmp::BinaryMessage
{
public:
    KeepAliveBuildRequest();
    KeepAliveBuildRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpKeepAliveBuildReplyId = 3486254892;

class KeepAliveBuildReply : public cmajor::bmp::BinaryMessage
{
public:
    KeepAliveBuildReply();
    KeepAliveBuildReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpBuildRequestId = 3665051473;

class BuildRequest : public cmajor::bmp::BinaryMessage
{
public:
    BuildRequest();
    BuildRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string backend;
    std::string config;
    std::string optimizationLevel;
    std::string filePath;
    std::vector<std::string> defines;
    bool verbose;
    bool quiet;
    bool clean;
    bool rebuild;
    bool emitIR;
    bool emitOptIR;
    bool linkWithDebugRuntime;
    bool disableModuleCache;
    bool singleThreadedCompile;
    bool buildAllDependencies;
};

const uint32_t bmpCompileErrorId = 4220890353;

class CompileError : public cmajor::bmp::BinaryMessage
{
public:
    CompileError();
    CompileError(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string message;
    std::string project;
    std::string file;
    int32_t line;
    int32_t scol;
    int32_t ecol;
};

const uint32_t bmpBuildReplyId = 2816165042;

class BuildReply : public cmajor::bmp::BinaryMessage
{
public:
    BuildReply();
    BuildReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool requestValid;
    std::string requestErrorMessage;
    std::string logException;
    std::string time;
    bool success;
    std::vector<CompileError> errors;
};

const uint32_t bmpLogBuildMessageRequestId = 3492289225;

class LogBuildMessageRequest : public cmajor::bmp::BinaryMessage
{
public:
    LogBuildMessageRequest();
    LogBuildMessageRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string message;
};

const uint32_t bmpLogBuildMessageReplyId = 4207502656;

class LogBuildMessageReply : public cmajor::bmp::BinaryMessage
{
public:
    LogBuildMessageReply();
    LogBuildMessageReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    bool ok;
};

const uint32_t bmpBuildProgressMessageId = 3054626391;

class BuildProgressMessage : public cmajor::bmp::BinaryMessage
{
public:
    BuildProgressMessage();
    BuildProgressMessage(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
};

const uint32_t bmpGenericBuildErrorReplyId = 2404960351;

class GenericBuildErrorReply : public cmajor::bmp::BinaryMessage
{
public:
    GenericBuildErrorReply();
    GenericBuildErrorReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string error;
};

const uint32_t bmpCacheModuleRequestId = 1602532038;

class CacheModuleRequest : public cmajor::bmp::BinaryMessage
{
public:
    CacheModuleRequest();
    CacheModuleRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string projectName;
    std::string projectFilePath;
    std::string backend;
    std::string config;
};

const uint32_t bmpCacheModuleReplyId = 156523240;

class CacheModuleReply : public cmajor::bmp::BinaryMessage
{
public:
    CacheModuleReply();
    CacheModuleReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string error;
};

const uint32_t bmpDefinitionSourceLocationId = 1941400111;

class DefinitionSourceLocation : public cmajor::bmp::BinaryMessage
{
public:
    DefinitionSourceLocation();
    DefinitionSourceLocation(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string file;
    int32_t line;
    int32_t scol;
};

const uint32_t bmpGetDefinitionRequestId = 2265013640;

class GetDefinitionRequest : public cmajor::bmp::BinaryMessage
{
public:
    GetDefinitionRequest();
    GetDefinitionRequest(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    std::string projectName;
    std::string projectFilePath;
    std::string backend;
    std::string config;
    std::string identifier;
    DefinitionSourceLocation identifierLocation;
};

const uint32_t bmpGetDefinitionReplyId = 1607165785;

class GetDefinitionReply : public cmajor::bmp::BinaryMessage
{
public:
    GetDefinitionReply();
    GetDefinitionReply(uint32_t id_);
    static std::string ClassName();
    static void Register();
    static cmajor::bmp::BinaryMessage* Create(uint32_t messageId);
    uint32_t Length() const override;
    void Write(util::MemoryWriter& writer) const override;
    void Read(util::MemoryReader& reader) override;
public:
    DefinitionSourceLocation definitionLocation;
    bool ok;
    std::string error;
};


} // namespace bs
