// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.client;

import util;
import std.core;

export namespace soul::json::rpc {

std::unique_ptr<util::JsonValue> MakePositionalParamRequest(const std::string& method, util::JsonArray* params, int id);

std::unique_ptr<util::JsonValue> MakeNamedParamRequest(const std::string& method, util::JsonObject* params, int id);

std::unique_ptr<util::JsonValue> MakePositionalParamNotification(const std::string& method, util::JsonArray* params);

std::unique_ptr<util::JsonValue> MakeNamedParamNotification(const std::string& method, util::JsonObject* params);

std::unique_ptr<util::JsonValue> Transact(util::JsonValue* request, util::Stream& stream);

} // namespace soul::json::rpc
