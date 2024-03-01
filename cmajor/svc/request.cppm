// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.service.request;

import std.core;

export namespace cmajor::service {

class Request
{
public:
    virtual ~Request();
    virtual void Execute() = 0;
    virtual std::string Name() const = 0;
};

void PutRequest(Request* request);
std::unique_ptr<Request> GetRequest();
void Exit();
bool Exiting();

} // namespace cmajor::service
