// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.io;

import soul.json.rpc.context;
import util;
import std.core;

export namespace soul::json::rpc {

std::vector<std::string> ReadHeaders(util::Stream& stream);

void ParseHeaders(Context& context, const std::vector<std::string>& headers);

std::string ReadContent(util::Stream& stream, const Context& inputContext);

std::unique_ptr<util::JsonValue> ParseContent(const std::string& content);

void WriteHeaders(util::Stream& stream, const Context& outputContext);

void WriteContent(util::Stream& stream, const std::string& content);

void WriteJsonValue(util::JsonValue* value, util::Stream& stream);

} // namespace soul::json::rpc
