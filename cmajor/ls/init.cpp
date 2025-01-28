// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.language.server.init;

import cmajor.language.server.info;
import cmajor.language.server.semantic.tokens;
import soul.json.rpc;

namespace cmajor::language::server {

class InitializeMethod : public soul::json::rpc::Method
{
public:
    InitializeMethod();
    std::unique_ptr<util::JsonValue> ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params) override;
};

InitializeMethod::InitializeMethod() : soul::json::rpc::Method("initialize")
{
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::number | util::JsonValueType::null, "processId", false));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::object, "clientInfo", true));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::string, "locale", true));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::string | util::JsonValueType::null, "rootPath", true));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::string | util::JsonValueType::null, "rootUri", false));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::any, "initializationOptions", true));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::object, "capabilities", false));
    AddParameter(soul::json::rpc::Parameter(util::JsonValueType::string, "trace", true));
}

std::unique_ptr<util::JsonValue> InitializeMethod::ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params)
{
    Info::Instance().SetClientInfo(new ClientInfo(params));
    if (context.Wait())
    {
        std::this_thread::sleep_for(std::chrono::seconds{ 30 });
    }
    InitSemanticTokens();
    return std::unique_ptr<util::JsonValue>(Info::Instance().GetServerInfo()->ToJson().release());
}

class InitializedNotification : public soul::json::rpc::Method
{
public:
    InitializedNotification();
    std::unique_ptr<util::JsonValue> ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params) override;
};

InitializedNotification::InitializedNotification() : soul::json::rpc::Method("initialized")
{
}

std::unique_ptr<util::JsonValue> InitializedNotification::ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params)
{
    return std::unique_ptr<util::JsonValue>();
}

class ShutDownMethod : public soul::json::rpc::Method
{
public:
    ShutDownMethod();
    std::unique_ptr<util::JsonValue> ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params) override;
};

ShutDownMethod::ShutDownMethod() : soul::json::rpc::Method("shutdown")
{
}

std::unique_ptr<util::JsonValue> ShutDownMethod::ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params)
{
    return std::unique_ptr<util::JsonValue>(new util::JsonNull());
}

class ExitNotification : public soul::json::rpc::Method
{
public: 
    ExitNotification();
    std::unique_ptr<util::JsonValue> ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params) override;
};

ExitNotification::ExitNotification() : soul::json::rpc::Method("exit")
{
}

std::unique_ptr<util::JsonValue> ExitNotification::ExecuteNamed(soul::json::rpc::ExecutionContext& context, util::JsonObject* params)
{
    context.SetExit();
    return std::unique_ptr<util::JsonValue>();
}

void Initialize()
{
    soul::json::rpc::AddMethod(new InitializeMethod());
    soul::json::rpc::AddMethod(new InitializedNotification());
    soul::json::rpc::AddMethod(new TextDocumentSemanticTokensFullMethod());
    soul::json::rpc::AddMethod(new ShutDownMethod());
    soul::json::rpc::AddMethod(new ExitNotification());
}

} // cmajor::language::server
