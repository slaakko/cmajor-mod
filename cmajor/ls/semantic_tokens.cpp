// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.language.server.semantic.tokens;

import cmajor.language.server.info;
import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import cmajor.fault.tolerant.binder;
import cmajor.fault.tolerant.build;
import util;

namespace cmajor::language::server {

std::vector<cmajor::fault::tolerant::symbols::TokenType> tokenTypes;
std::vector<cmajor::fault::tolerant::symbols::TokenModifiers> tokenModifiers;
std::vector<int> tokenTypeIndeces;
std::vector<int> tokenModifierIndeces;

std::string GetTokenTypeName(cmajor::fault::tolerant::symbols::TokenType tokenType)
{
    switch (tokenType)
    {
        case cmajor::fault::tolerant::symbols::TokenType::namespace_: return "namespace";
        case cmajor::fault::tolerant::symbols::TokenType::type: return "type";
        case cmajor::fault::tolerant::symbols::TokenType::class_: return "class";
        case cmajor::fault::tolerant::symbols::TokenType::enum_: return "enum";
        case cmajor::fault::tolerant::symbols::TokenType::interface: return "interface";
        case cmajor::fault::tolerant::symbols::TokenType::typeParameter: return "typeParameter";
        case cmajor::fault::tolerant::symbols::TokenType::parameter: return "parameter";
        case cmajor::fault::tolerant::symbols::TokenType::variable: return "variable";
        case cmajor::fault::tolerant::symbols::TokenType::enumMember: return "enumMember";
        case cmajor::fault::tolerant::symbols::TokenType::function: return "function";
        case cmajor::fault::tolerant::symbols::TokenType::method: return "method";
        case cmajor::fault::tolerant::symbols::TokenType::keyword: return "keyword";
        case cmajor::fault::tolerant::symbols::TokenType::modifier: return "modifier";
        case cmajor::fault::tolerant::symbols::TokenType::comment: return "comment";
        case cmajor::fault::tolerant::symbols::TokenType::string: return "string";
        case cmajor::fault::tolerant::symbols::TokenType::number: return "number";
        case cmajor::fault::tolerant::symbols::TokenType::operator_: return "operator";
    }
    return std::string();
}

std::string GetTokenModifierName(cmajor::fault::tolerant::symbols::TokenModifiers tokenModifier)
{
    switch (tokenModifier)
    {
        case cmajor::fault::tolerant::symbols::TokenModifiers::declaration: return "declaration";
        case cmajor::fault::tolerant::symbols::TokenModifiers::definition: return "definition";
        case cmajor::fault::tolerant::symbols::TokenModifiers::readonly: return "readonly";
        case cmajor::fault::tolerant::symbols::TokenModifiers::static_: return "static";
        case cmajor::fault::tolerant::symbols::TokenModifiers::abstract: return "abstract";
    }
    return std::string();
}

void InitSemanticTokens()
{
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::namespace_);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::type);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::class_);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::enum_);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::interface);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::typeParameter);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::parameter);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::variable);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::enumMember);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::function);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::method);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::keyword);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::modifier);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::comment);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::string);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::number);
    tokenTypes.push_back(cmajor::fault::tolerant::symbols::TokenType::operator_);
    int tokenTypeIndex = 0;
    for (auto tokenType : tokenTypes)
    {
        std::string tokenTypeName = GetTokenTypeName(tokenType);
        if (!tokenTypeName.empty())
        {
            if (Info::Instance().HasClientTokenType(tokenTypeName))
            {
                tokenTypeIndeces.push_back(tokenTypeIndex++);
                Info::Instance().AddServerTokenType(tokenTypeName);
            }
            else
            {
                tokenTypeIndeces.push_back(-1);
            }
        }
        else
        {
            tokenTypeIndeces.push_back(-1);
        }
    }
    tokenModifiers.push_back(cmajor::fault::tolerant::symbols::TokenModifiers::declaration);
    tokenModifiers.push_back(cmajor::fault::tolerant::symbols::TokenModifiers::definition);
    tokenModifiers.push_back(cmajor::fault::tolerant::symbols::TokenModifiers::readonly);
    tokenModifiers.push_back(cmajor::fault::tolerant::symbols::TokenModifiers::static_);
    tokenModifiers.push_back(cmajor::fault::tolerant::symbols::TokenModifiers::abstract);
    int tokenModifierIndex = 0;
    for (auto tokenModifier : tokenModifiers)
    {
        std::string tokenModifierName = GetTokenModifierName(tokenModifier);
        if (!tokenModifierName.empty())
        {
            if (Info::Instance().HasClientTokenModifier(tokenModifierName))
            {
                tokenModifierIndeces.push_back(tokenModifierIndex++);
                Info::Instance().AddServerTokenModifier(tokenModifierName);
            }
            else
            {
                tokenModifierIndeces.push_back(-1);
            }
        }
        else
        {
            tokenModifierIndeces.push_back(-1);
        }
    }
}

int GetTokenTypeIndex(cmajor::fault::tolerant::symbols::TokenType tokenType)
{
    int index = static_cast<int>(tokenType);
    if (index < tokenTypeIndeces.size())
    {
        return tokenTypeIndeces[index];
    }
    return -1;
}

int GetTokenModifiersIndex(cmajor::fault::tolerant::symbols::TokenModifiers tokenModifiers)
{
    int value = static_cast<int>(tokenModifiers);
    int tokenModifiersValue = 0;
    for (int i = 0; i < cmajor::fault::tolerant::symbols::tokenModifierBitCount; ++i)
    {
        if ((tokenModifiers & cmajor::fault::tolerant::symbols::TokenModifiers(1 << i)) != cmajor::fault::tolerant::symbols::TokenModifiers::none)
        {
            if (tokenModifierIndeces[i] != -1)
            {
                tokenModifiersValue |= (1 << tokenModifierIndeces[i]);
            }
        }
    }
    return tokenModifiersValue;
}

TextDocumentSemanticTokensFullMethod::TextDocumentSemanticTokensFullMethod() : soul::json::rpc::Method("textDocument/semanticTokens/full")
{
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::object, "textDocument", false));
}

std::unique_ptr<util::JsonValue> TextDocumentSemanticTokensFullMethod::ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params)
{
    util::JsonValue* textDocumentParam = params->GetField(U"textDocument");
    if (textDocumentParam && textDocumentParam->IsObject())
    {
        util::JsonObject* textDocumentObject = static_cast<util::JsonObject*>(textDocumentParam);
        util::JsonValue* uriValue = textDocumentObject->GetField(U"uri");
        if (uriValue && uriValue->IsString())
        {
            util::JsonString* uriString = static_cast<util::JsonString*>(uriValue);
            util::Uri textDocumentUri(util::ToUtf8(uriString->Value()));
            std::string sourceFilePath = textDocumentUri.path;
            cmajor::fault::tolerant::build::Compilation* compilation = cmajor::fault::tolerant::build::CompilationTable().Instance().GetCompilationBySourceFile(sourceFilePath);
            if (compilation)
            {
                std::vector<cmajor::fault::tolerant::symbols::TokenInfo> tokenInfo = compilation->GetTokenInfo(sourceFilePath);
                if (!tokenInfo.empty())
                {
                    std::unique_ptr<util::JsonObject> semanticTokensObject(new util::JsonObject());
                    std::unique_ptr<util::JsonArray> semanticTokenData(new util::JsonArray());
                    for (const auto& token : tokenInfo)
                    {
                        if (token.lineColLen.IsValid())
                        {
                            int line = token.lineColLen.line;
                            int startChar = token.lineColLen.col;
                            int length = token.lineColLen.len;
                            int tokenType = GetTokenTypeIndex(token.tokenType);
                            if (tokenType != -1)
                            {
                                int tokenModifiers = GetTokenModifiersIndex(token.tokenModifiers);
                                semanticTokenData->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonNumber(line)));
                                semanticTokenData->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonNumber(startChar)));
                                semanticTokenData->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonNumber(length)));
                                semanticTokenData->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonNumber(tokenType)));
                                semanticTokenData->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonNumber(tokenModifiers)));
                            }
                        }
                    }
                    semanticTokensObject->AddField(U"data", std::move(semanticTokenData));
                    return std::unique_ptr<util::JsonValue>(semanticTokensObject.release());
                }
            }
            return std::unique_ptr<util::JsonValue>(new util::JsonNull());
        }
    }
    throw soul::json::rpc::JsonRpcException(soul::json::rpc::invalidParams, "method '" + Name() + "': invalid parameters");
}

} // cmajor::language::server
