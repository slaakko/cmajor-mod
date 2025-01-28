// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.language.server.info;

import util;
import std.core;

export namespace cmajor::language::server {

std::string Version()
{
    return "0.1.0";
}

struct SemanticTokensClientCapabilities
{
    SemanticTokensClientCapabilities();
    SemanticTokensClientCapabilities(util::JsonObject* object);
    bool HasTokenType(const std::string& tokenTypeName) const;
    bool HasTokenModifier(const std::string& tokenModifierName) const;
    bool dynamicRegistration;
    bool range;
    bool full;
    bool delta;
    std::vector<std::string> tokenTypes;
    std::vector<std::string> tokenModifiers;
    bool overlappingTokenSupport;
    bool multilineTokenSupport;
    bool serverCancelSupport;
    bool augmentsSyntaxTokens;
};

struct TextDocumentClientCapabilities
{
    TextDocumentClientCapabilities();
    SemanticTokensClientCapabilities semanticTokensCapabilities;
};

struct ClientCapabilities
{
    ClientCapabilities();
    TextDocumentClientCapabilities textDocumentCapabilities;
};

struct WorkspaceFolder
{
    WorkspaceFolder();
    util::Uri uri;
    std::string name;
};

struct ClientInfo
{
    ClientInfo(util::JsonObject* object);
    int processId;
    std::string clientName;
    std::string clientVersion;
    std::string locale;
    std::string rootPath;
    util::Uri rootUri;
    std::vector<WorkspaceFolder> workspaceFolders;
    ClientCapabilities clientCapabilities;
};

struct SemanticTokensLegend
{
    SemanticTokensLegend();
    std::unique_ptr<util::JsonObject> ToJson() const;
    std::vector<std::string> tokenTypes;
    std::vector<std::string> tokenModifiers;
};

struct SemanticTokensOptions
{
    SemanticTokensOptions();
    std::unique_ptr<util::JsonObject> ToJson() const;
    SemanticTokensLegend legend;
    bool range;
    bool full;
};

struct ServerCapabilities
{
    ServerCapabilities();
    std::unique_ptr<util::JsonObject> ToJson() const;
    std::string positionEncoding;
    SemanticTokensOptions semanticTokensProvider;
};

struct ServerInfo
{
    ServerInfo();
    std::unique_ptr<util::JsonObject> ToJson() const;
    ServerCapabilities serverCapabilities;
    std::string name;
    std::string version;
};

class Info
{
public:
    static Info& Instance();
    void SetClientInfo(ClientInfo* clientInfo_);
    ClientInfo* GetClientInfo() const { return clientInfo.get(); }
    ServerInfo* GetServerInfo() const { return serverInfo.get(); }
    bool HasClientTokenType(const std::string& tokenTypeName) const;
    bool HasClientTokenModifier(const std::string& tokenModifierName) const;
    void AddServerTokenType(const std::string& tokenTypeName);
    void AddServerTokenModifier(const std::string& tokenModifierName);
private:
    Info();
    std::unique_ptr<ClientInfo> clientInfo;
    std::unique_ptr<ServerInfo> serverInfo;
};

} // cmajor::language::server
