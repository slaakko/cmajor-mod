// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.error;

import util;
import std.core;

export namespace soul::json::rpc {

const int parseError = -32700;
const int invalidRequest = -32600;
const int methodNotFound = -32601;
const int invalidParams = -32602;
const int internalError = -32603;

// const int serverError = -32000 ... -32099

class JsonRpcException
{
public:
    JsonRpcException(int code_, const std::string& message_);
    const int Code() const { return code; }
    const std::string& Message() const { return message; }
    std::unique_ptr<util::JsonObject> ToErrorObject();
private:
    int code;
    std::string message;
};

} // namespace soul::json::rpc
