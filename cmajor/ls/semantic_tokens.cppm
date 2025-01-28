// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.language.server.semantic.tokens;

import cmajor.fault.tolerant.symbols.token.info;
import soul.json.rpc;
import std.core;

export namespace cmajor::language::server {

void InitSemanticTokens();

std::string GetTokenTypeName(cmajor::fault::tolerant::symbols::TokenType tokenType);
std::string GetTokenModifierName(cmajor::fault::tolerant::symbols::TokenModifiers tokenModifier);

int GetTokenTypeIndex(cmajor::fault::tolerant::symbols::TokenType tokenType);
int GetTokenModifiersIndex(cmajor::fault::tolerant::symbols::TokenModifiers tokenModifiers);

class TextDocumentSemanticTokensFullMethod : public soul::json::rpc::Method
{
public:
    TextDocumentSemanticTokensFullMethod();
    std::unique_ptr<util::JsonValue> ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params) override;
};

} // cmajor::language::server
