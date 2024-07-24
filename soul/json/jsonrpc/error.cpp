// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.json.rpc.error;

namespace soul::json::rpc {

JsonRpcException::JsonRpcException(int code_, const std::string& message_) : code(code_), message(message_)
{
}

std::unique_ptr<util::JsonObject> JsonRpcException::ToErrorObject() 
{
    std::unique_ptr<util::JsonObject> error(new util::JsonObject());
    error->AddField(U"code", std::unique_ptr<util::JsonValue>(new util::JsonNumber(code)));
    error->AddField(U"message", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(message))));
    return error;
}

} // namespace soul::json::rpc
