// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.language.server.info;

namespace cmajor::language::server {

SemanticTokensClientCapabilities::SemanticTokensClientCapabilities() : 
    dynamicRegistration(false), range(false), full(false), delta(false), overlappingTokenSupport(false), multilineTokenSupport(false), serverCancelSupport(false),
    augmentsSyntaxTokens(false)
{
}

SemanticTokensClientCapabilities::SemanticTokensClientCapabilities(util::JsonObject* object) : 
    dynamicRegistration(false), range(false), full(false), delta(false), overlappingTokenSupport(false), multilineTokenSupport(false), serverCancelSupport(false),
    augmentsSyntaxTokens(false)
{
    util::JsonValue* dynamicRegistrationValue = object->GetField(U"dynamicRegistration");
    if (dynamicRegistrationValue && dynamicRegistrationValue->IsBoolean())
    {
        util::JsonBool* dynamicRegistrationBoolean = static_cast<util::JsonBool*>(dynamicRegistrationValue);
        dynamicRegistration = dynamicRegistrationBoolean->Value();
    }
    util::JsonValue* requestsValue = object->GetField(U"requests");
    if (requestsValue && requestsValue->IsObject())
    {
        util::JsonObject* requestsObject = static_cast<util::JsonObject*>(requestsValue);
        util::JsonValue* rangeValue = requestsObject->GetField(U"range");
        if (rangeValue && rangeValue->IsBoolean())
        {
            util::JsonBool* rangeBoolean = static_cast<util::JsonBool*>(rangeValue);
            range = rangeBoolean->Value();
        }
        util::JsonValue* fullValue = requestsObject->GetField(U"full");
        if (fullValue && fullValue->IsBoolean())
        {
            util::JsonBool* fullBoolean = static_cast<util::JsonBool*>(fullValue);
            full = fullBoolean->Value();
        }
        if (fullValue && fullValue->IsObject())
        {
            full = true;
            util::JsonObject* fullObject = static_cast<util::JsonObject*>(fullValue);
            util::JsonValue* deltaValue = fullObject->GetField(U"delta");
            if (deltaValue && deltaValue->IsBoolean())
            {
                util::JsonBool* deltaBoolean = static_cast<util::JsonBool*>(deltaValue);
                delta = deltaBoolean->Value();
            }
        }
    }
    util::JsonValue* tokenTypesValue = object->GetField(U"tokenTypes");
    if (tokenTypesValue && tokenTypesValue->IsArray())
    {
        util::JsonArray* tokenTypesArray = static_cast<util::JsonArray*>(tokenTypesValue);
        int n = tokenTypesArray->Count();
        for (int i = 0; i < n; ++i)
        {
            util::JsonValue* tokenTypeValue = tokenTypesArray->GetItem(i);
            if (tokenTypeValue && tokenTypeValue->IsString())
            {
                util::JsonString* tokenTypeString = static_cast<util::JsonString*>(tokenTypeValue);
                tokenTypes.push_back(util::ToUtf8(tokenTypeString->Value()));
            }
        }
    }
    util::JsonValue* tokenModifiersValue = object->GetField(U"tokenModifiers");
    if (tokenModifiersValue && tokenModifiersValue->IsArray())
    {
        util::JsonArray* tokenModifiersArray = static_cast<util::JsonArray*>(tokenModifiersValue);
        int n = tokenModifiersArray->Count();
        for (int i = 0; i < n; ++i)
        {
            util::JsonValue* tokenModifierValue = tokenModifiersArray->GetItem(i);
            if (tokenModifierValue && tokenModifierValue->IsString())
            {
                util::JsonString* tokenModifierString = static_cast<util::JsonString*>(tokenModifierValue);
                tokenModifiers.push_back(util::ToUtf8(tokenModifierString->Value()));
            }
        }
    }
    util::JsonValue* overlappingTokenSupportValue = object->GetField(U"overlappingTokenSupport");
    if (overlappingTokenSupportValue && overlappingTokenSupportValue->IsBoolean())
    {
        util::JsonBool* overlappingTokenSupportBoolean = static_cast<util::JsonBool*>(overlappingTokenSupportValue);
        overlappingTokenSupport = overlappingTokenSupportBoolean->Value();
    }
    util::JsonValue* multilineTokenSupportValue = object->GetField(U"multilineTokenSupport");
    if (multilineTokenSupportValue && multilineTokenSupportValue->IsBoolean())
    {
        util::JsonBool* multilineTokenSupportBoolean = static_cast<util::JsonBool*>(multilineTokenSupportValue);
        multilineTokenSupport = multilineTokenSupportBoolean->Value();
    }
    util::JsonValue* serverCancelSupportValue = object->GetField(U"serverCancelSupport");
    if (serverCancelSupportValue && serverCancelSupportValue->IsBoolean())
    {
        util::JsonBool* serverCancelSupportBoolean = static_cast<util::JsonBool*>(serverCancelSupportValue);
        serverCancelSupport = serverCancelSupportBoolean->Value();
    }
    util::JsonValue* augmentsSyntaxTokensValue = object->GetField(U"augmentsSyntaxTokens");
    if (augmentsSyntaxTokensValue && augmentsSyntaxTokensValue->IsBoolean())
    {
        util::JsonBool* augmentsSyntaxTokensBoolean = static_cast<util::JsonBool*>(augmentsSyntaxTokensValue);
        augmentsSyntaxTokens = augmentsSyntaxTokensBoolean->Value();
    }
}

bool SemanticTokensClientCapabilities::HasTokenType(const std::string& tokenTypeName) const
{
    for (const auto& tokenType : tokenTypes)
    {
        if (tokenType == tokenTypeName) return true;
    }
    return false;
}

bool SemanticTokensClientCapabilities::HasTokenModifier(const std::string& tokenModifierName) const
{
    for (const auto& tokenModifier : tokenModifiers)
    {
        if (tokenModifier == tokenModifierName) return true;
    }
    return false;
}

TextDocumentClientCapabilities::TextDocumentClientCapabilities()
{
}

ClientCapabilities::ClientCapabilities()
{
}

WorkspaceFolder::WorkspaceFolder()
{
}

ClientInfo::ClientInfo(util::JsonObject* object) : processId(-1)
{
    util::JsonValue* processIdNumber = object->GetField(U"processId");
    if (processIdNumber && processIdNumber->IsNumber())
    {
        processId = static_cast<int>(static_cast<util::JsonNumber*>(processIdNumber)->Value());
    }
    util::JsonValue* clientInfoValue = object->GetField(U"clientInfo");
    if (clientInfoValue && clientInfoValue->IsObject())
    {
        util::JsonObject* clientInfoObject = static_cast<util::JsonObject*>(clientInfoValue);
        util::JsonValue* clientNameValue = clientInfoObject->GetField(U"name");
        if (clientNameValue && clientNameValue->IsString())
        {
            clientName = util::ToUtf8(static_cast<util::JsonString*>(clientNameValue)->Value());
        }
        util::JsonValue* clientVersionValue = clientInfoObject->GetField(U"version");
        if (clientVersionValue && clientVersionValue->IsString())
        {
            clientVersion = util::ToUtf8(static_cast<util::JsonString*>(clientVersionValue)->Value());
        }
    }
    util::JsonValue* localeValue = object->GetField(U"locale");
    if (localeValue && localeValue->IsString())
    {
        locale = util::ToUtf8(static_cast<util::JsonString*>(localeValue)->Value());
    }
    util::JsonValue* rootPathValue = object->GetField(U"rootPath");
    if (rootPathValue && rootPathValue->IsString())
    {
        rootPath = util::ToUtf8(static_cast<util::JsonString*>(rootPathValue)->Value());
    }
    util::JsonValue* rootUriValue = object->GetField(U"rootUri");
    if (rootUriValue && rootUriValue->IsString())
    {
        rootUri = util::Uri(util::ToUtf8(static_cast<util::JsonString*>(rootUriValue)->Value()));
    }
    util::JsonValue* capabilitiesValue = object->GetField(U"capabilities");
    if (capabilitiesValue && capabilitiesValue->IsObject())
    {
        util::JsonObject* clientCapabilitiesObject = static_cast<util::JsonObject*>(capabilitiesValue);
        util::JsonValue* textDocumentCapabilitiesValue = clientCapabilitiesObject->GetField(U"textDocument");
        if (textDocumentCapabilitiesValue && textDocumentCapabilitiesValue->IsObject())
        {
            util::JsonObject* textDocumentObject = static_cast<util::JsonObject*>(textDocumentCapabilitiesValue);
            util::JsonValue* semanticTokensCapabilitiesValue = textDocumentObject->GetField(U"semanticTokens");
            if (semanticTokensCapabilitiesValue && semanticTokensCapabilitiesValue->IsObject())
            {
                util::JsonObject* semanticTokenCapabilitiesObject = static_cast<util::JsonObject*>(semanticTokensCapabilitiesValue);
                clientCapabilities.textDocumentCapabilities.semanticTokensCapabilities = SemanticTokensClientCapabilities(semanticTokenCapabilitiesObject);
            }
        }
    }
    util::JsonValue* workspaceFoldersValue = object->GetField(U"workspaceFolders");
    if (workspaceFoldersValue && workspaceFoldersValue->IsArray())
    {
        util::JsonArray* workspaceFoldersArray = static_cast<util::JsonArray*>(workspaceFoldersValue);
        int n = workspaceFoldersArray->Count();
        for (int i = 0; i < n; ++i)
        {
            util::JsonValue* workspaceFolderValue = workspaceFoldersArray->GetItem(i);
            if (workspaceFolderValue && workspaceFolderValue->IsObject())
            {
                util::JsonObject* workspaceFolderObject = static_cast<util::JsonObject*>(workspaceFolderValue);
                WorkspaceFolder workspaceFolder;
                util::JsonValue* uriValue = workspaceFolderObject->GetField(U"uri");
                if (uriValue && uriValue->IsString())
                {
                    util::JsonString* uriStr = static_cast<util::JsonString*>(uriValue);
                    util::Uri uri(util::ToUtf8(uriStr->Value()));
                    workspaceFolder.uri = uri;
                    util::JsonValue* nameValue = workspaceFolderObject->GetField(U"name");
                    if (nameValue && nameValue->IsString())
                    {
                        util::JsonString* nameStr = static_cast<util::JsonString*>(nameValue);
                        workspaceFolder.name = util::ToUtf8(nameStr->Value());
                    }
                }
                workspaceFolders.push_back(workspaceFolder);
            }
        }
    }
}

SemanticTokensLegend::SemanticTokensLegend()
{
}

std::unique_ptr<util::JsonObject> SemanticTokensLegend::ToJson() const
{
    std::unique_ptr<util::JsonObject> result(new util::JsonObject());
    util::JsonArray* tokenTypesArray = new util::JsonArray();
    for (const auto& tokenType : tokenTypes)
    {
        tokenTypesArray->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(tokenType))));
    }
    result->AddField(U"tokenTypes", std::unique_ptr<util::JsonValue>(tokenTypesArray));
    util::JsonArray* tokenModifiersArray = new util::JsonArray();
    for (const auto& tokenModifier : tokenModifiers)
    {
        tokenModifiersArray->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(tokenModifier))));
    }
    result->AddField(U"tokenModifiers", std::unique_ptr<util::JsonValue>(tokenModifiersArray));
    return result;
}

SemanticTokensOptions::SemanticTokensOptions() : legend(), range(false), full(true)
{
}

std::unique_ptr<util::JsonObject> SemanticTokensOptions::ToJson() const
{
    std::unique_ptr<util::JsonObject> result(new util::JsonObject());
    result->AddField(U"legend", std::unique_ptr<util::JsonValue>(legend.ToJson().release()));
    result->AddField(U"range", std::unique_ptr<util::JsonValue>(new util::JsonBool(range)));
    result->AddField(U"full", std::unique_ptr<util::JsonValue>(new util::JsonBool(full)));
    return result;
}

ServerCapabilities::ServerCapabilities() : positionEncoding("utf-16")
{
}

std::unique_ptr<util::JsonObject> ServerCapabilities::ToJson() const
{
    std::unique_ptr<util::JsonObject> result(new util::JsonObject());
    result->AddField(U"positionEncoding", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(positionEncoding))));
    result->AddField(U"semanticTokensProvider", std::unique_ptr<util::JsonValue>(semanticTokensProvider.ToJson().release()));
    return result;
}

ServerInfo::ServerInfo() : name("cmajor-language-server"), version(Version())
{
}

std::unique_ptr<util::JsonObject> ServerInfo::ToJson() const
{
    std::unique_ptr<util::JsonObject> result(new util::JsonObject());
    result->AddField(U"capabilities", std::unique_ptr<util::JsonValue>(serverCapabilities.ToJson().release()));
    std::unique_ptr<util::JsonObject> serverInfo(new util::JsonObject());
    serverInfo->AddField(U"name", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(name))));
    serverInfo->AddField(U"version", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(version))));
    result->AddField(U"serverInfo", std::unique_ptr<util::JsonValue>(serverInfo.release()));
    return result;
}

Info& Info::Instance()
{
    static Info instance;
    return instance;
}

Info::Info() : serverInfo(new ServerInfo())
{
}

void Info::SetClientInfo(ClientInfo* clientInfo_)
{
    clientInfo.reset(clientInfo_);
}

bool Info::HasClientTokenType(const std::string& tokenTypeName) const
{
    if (clientInfo)
    {
        return clientInfo->clientCapabilities.textDocumentCapabilities.semanticTokensCapabilities.HasTokenType(tokenTypeName);
    }
    return false;
}

bool Info::HasClientTokenModifier(const std::string& tokenModifierName) const
{
    if (clientInfo)
    {
        return clientInfo->clientCapabilities.textDocumentCapabilities.semanticTokensCapabilities.HasTokenModifier(tokenModifierName);
    }
    return false;
}

void Info::AddServerTokenType(const std::string& tokenTypeName)
{
    if (serverInfo)
    {
        serverInfo->serverCapabilities.semanticTokensProvider.legend.tokenTypes.push_back(tokenTypeName);
    }
}

void Info::AddServerTokenModifier(const std::string& tokenModifierName)
{
    if (serverInfo)
    {
        serverInfo->serverCapabilities.semanticTokensProvider.legend.tokenModifiers.push_back(tokenModifierName);
    }
}

} // cmajor::language::server
