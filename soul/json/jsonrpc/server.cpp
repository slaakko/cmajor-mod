// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.server;

import soul.json.rpc.error;
import soul.json.rpc.method;
import soul.json.rpc.method.registry;
import soul.json.rpc.io;
import soul.json.rpc.context;
import soul.json.rpc.config;

namespace soul::json::rpc {

void GetRequestFields(util::JsonObject* request, util::JsonString*& version, util::JsonValue*& id, util::JsonString*& method, util::JsonValue*& params)
{
    version = request->GetStringField(U"jsonrpc");
    id = request->GetField(U"id");
    method = request->GetStringField(U"method");
    params = request->GetField(U"params");
}

std::unique_ptr<util::JsonValue> HandleRequestMethod(ExecutionContext& context, util::JsonString* methodName, util::JsonValue* params)
{
    if (!methodName)
    {
        throw JsonRpcException(invalidRequest, "method name is empty");
    }
    if (params)
    {
        if (!params->IsStructuredValue())
        {
            throw JsonRpcException(invalidRequest, "params is not structured value");
        }
    }
    Method* method = GetMethod(util::ToUtf8(methodName->Value()));
    method->Validate(params);
    std::unique_ptr<util::JsonValue> result = method->Execute(context, params);
    return result;
}

void HandleNotificationMethod(ExecutionContext& context, util::JsonString* methodName, util::JsonValue* params)
{
    if (!methodName)
    {
        throw JsonRpcException(invalidRequest, "method name is empty");
    }
    if (params)
    {
        if (!params->IsStructuredValue())
        {
            throw JsonRpcException(invalidRequest, "params is not structured value");
        }
    }
    Method* method = GetMethod(util::ToUtf8(methodName->Value()));
    method->Validate(params);
    std::unique_ptr<util::JsonValue> result = method->Execute(context, params);
}

std::unique_ptr<util::JsonValue> HandleRequest(ExecutionContext& context, util::JsonValue* request)
{
    if (request->IsArray())
    {
        util::JsonArray* requestArray = static_cast<util::JsonArray*>(request);
        std::unique_ptr<util::JsonArray> responseArray(new util::JsonArray());
        int n = requestArray->Count();
        for (int i = 0; i < n; ++i)
        {
            util::JsonValue* req = requestArray->GetItem(i);
            std::unique_ptr<util::JsonValue> response = HandleRequest(context, req);
            if (response)
            {
                responseArray->AddItem(std::move(response));
            }
        }
        return std::unique_ptr<util::JsonValue>(responseArray.release());
    }
    else if (!request->IsObject())
    {
        throw JsonRpcException(invalidRequest, "request not an array or an object");
    }
    util::JsonObject* requestObject = static_cast<util::JsonObject*>(request);
    util::JsonString* version = nullptr;
    util::JsonValue* id = nullptr; 
    util::JsonString* method = nullptr;
    util::JsonValue* params = nullptr;
    try
    {
        GetRequestFields(requestObject, version, id, method, params);
        if (!version || version->Value() != U"2.0")
        {
            std::string versionStr;
            if (version)
            {
                versionStr = version->ToString();
            }
            throw JsonRpcException(invalidRequest, "unsupported JSON-RPC version '" + versionStr + "': version '2.0' expected");
        }
        if (id)
        {
            std::unique_ptr<util::JsonObject> response(new util::JsonObject());
            response->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
            response->AddField(U"id", std::unique_ptr<util::JsonValue>(id->Clone()));
            std::unique_ptr<util::JsonValue> result = HandleRequestMethod(context, method, params);
            response->AddField(U"result", std::move(result));
            return response;
        }
        else
        {
            HandleNotificationMethod(context, method, params);
            return std::unique_ptr<util::JsonObject>();
        }
    }
    catch (JsonRpcException& ex)
    {
        std::unique_ptr<util::JsonObject> response(new util::JsonObject());
        response->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
        if (id)
        {
            response->AddField(U"id", std::unique_ptr<util::JsonValue>(id->Clone()));
        }
        response->AddField(U"error", std::unique_ptr<util::JsonValue>(ex.ToErrorObject().release()));
        return response;
    }
    catch (const std::exception& ex)
    {
        std::unique_ptr<util::JsonObject> response(new util::JsonObject());
        response->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
        if (id)
        {
            response->AddField(U"id", std::unique_ptr<util::JsonValue>(id->Clone()));
        }
        JsonRpcException exception(internalError, "internal error: " + std::string(ex.what()));
        response->AddField(U"error", std::unique_ptr<util::JsonValue>(exception.ToErrorObject().release()));
        return response;
    }
}

bool ProcessRequest(util::Stream& stream, bool& wait)
{
    ExecutionContext context;
    if (wait)
    {
        wait = false;
        context.SetWait();
    }
    std::unique_ptr<util::JsonValue> response;
    try
    {
        std::vector<std::string> headers = ReadHeaders(stream);
        if (headers.empty())
        {
            return true;
        }
        Context inputContext;
        ParseHeaders(inputContext, headers);
        if (inputContext.ContentLength() == -1)
        {
            throw JsonRpcException(invalidRequest, "'Content-Length' JSON-RPC header required");
        }
        std::string requestContent = ReadContent(stream, inputContext);
        std::unique_ptr<util::JsonValue> request = ParseContent(requestContent);
        if (Config::Instance().GetFlag(Flags::log))
        {
            std::ofstream file(util::Path::Combine(Config::Instance().LogDir(), "request.json"));
            util::CodeFormatter formatter(file);
            request->Write(formatter);
        }
        response = HandleRequest(context, request.get());
    }
    catch (JsonRpcException& ex)
    {
        response.reset(new util::JsonObject());
        static_cast<util::JsonObject*>(response.get())->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
        static_cast<util::JsonObject*>(response.get())->AddField(U"error", std::unique_ptr<util::JsonValue>(ex.ToErrorObject().release()));
    }
    catch (const std::exception& ex)
    {
        response.reset(new util::JsonObject());
        static_cast<util::JsonObject*>(response.get())->AddField(U"jsonrpc", std::unique_ptr<util::JsonValue>(new util::JsonString(U"2.0")));
        JsonRpcException exception(internalError, "internal error: " + std::string(ex.what()));
        static_cast<util::JsonObject*>(response.get())->AddField(U"error", std::unique_ptr<util::JsonValue>(exception.ToErrorObject().release()));
    }
    if (response)
    {
        WriteJsonValue(response.get(), stream);
        if (Config::Instance().GetFlag(Flags::log))
        {
            std::ofstream file(util::Path::Combine(Config::Instance().LogDir(), "response.json"));
            util::CodeFormatter formatter(file);
            response->Write(formatter);
        }
    }
    return context.Exit();
}

} // namespace soul::json::rpc
