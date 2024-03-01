// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.json.rpc.context;

import std.core;

export namespace soul::json::rpc {

class Context
{
public:
    Context();
    int ContentLength() const { return contentLength; }
    void SetContentLength(int contentLength_) { contentLength = contentLength_; }
    const std::string& ContentType() const { return contentType; }
    void SetContentType(const std::string& contentType_) { contentType = contentType_; }
private:
    int contentLength;
    std::string contentType;
};

} // namespace soul::json::rpc
