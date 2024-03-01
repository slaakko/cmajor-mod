// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.client;

import soul.json.rpc.context;
import soul.json.rpc.io;

namespace soul::json::rpc {

std::unique_ptr<util::JsonValue> MakePositionalParamRequest(const std::string& method, util::JsonArray* params, int id)
{
    std::unique_ptr<util::JsonObject> request(new util::JsonObject());
    request->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
    request->AddField(U"method", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(method))));
    if (params)
    {
        request->AddField(U"params", std::unique_ptr<util::JsonValue>(params->Clone()));
    }
    else
    {
        request->AddField(U"params", std::unique_ptr<util::JsonValue>(new util::JsonArray()));
    }
    request->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonNumber(id)));
    return std::unique_ptr<util::JsonValue>(request.release());
}

std::unique_ptr<util::JsonValue> MakeNamedParamRequest(const std::string& method, util::JsonObject* params, int id)
{
    std::unique_ptr<util::JsonObject> request(new util::JsonObject());
    request->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
    request->AddField(U"method", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(method))));
    if (params)
    {
        request->AddField(U"params", std::unique_ptr<util::JsonValue>(params->Clone()));
    }
    else
    {
        request->AddField(U"params", std::unique_ptr<util::JsonValue>(new util::JsonObject()));
    }
    request->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonNumber(id)));
    return std::unique_ptr<util::JsonValue>(request.release());
}

std::unique_ptr<util::JsonValue> MakePositionalParamNotification(const std::string& method, util::JsonArray* params)
{
    std::unique_ptr<util::JsonObject> notification(new util::JsonObject());
    notification->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
    notification->AddField(U"method", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(method))));
    if (params)
    {
        notification->AddField(U"params", std::unique_ptr<util::JsonValue>(params->Clone()));
    }
    else
    {
        notification->AddField(U"params", std::unique_ptr<util::JsonValue>(new util::JsonArray()));
    }
    return std::unique_ptr<util::JsonValue>(notification.release());
}

std::unique_ptr<util::JsonValue> MakeNamedParamNotification(const std::string& method, util::JsonObject* params)
{
    std::unique_ptr<util::JsonObject> notification(new util::JsonObject());
    notification->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
    notification->AddField(U"method", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(method))));
    if (params)
    {
        notification->AddField(U"params", std::unique_ptr<util::JsonValue>(params->Clone()));
    }
    else
    {
        notification->AddField(U"params", std::unique_ptr<util::JsonValue>(new util::JsonObject()));
    }
    return std::unique_ptr<util::JsonValue>(notification.release());
}

std::unique_ptr<util::JsonValue> Transact(util::JsonValue* request, util::Stream& stream)
{
    WriteJsonValue(request, stream);
    std::vector<std::string> headers = ReadHeaders(stream);
    Context context;
    ParseHeaders(context, headers);
    std::string responseContent = ReadContent(stream, context);
    std::unique_ptr<util::JsonValue> response = ParseContent(responseContent);
    return response;
}

} // namespace soul::json::rpc
