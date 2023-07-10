// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.request.dispatcher;

import cmajor.service.request;
import cmajor.service.message;

namespace cmajor::service {

class RequestDispatcher
{
public:
    static RequestDispatcher& Instance();
    void Start();
    void Stop();
    void Run();
    void HandleRequest(Request* request);
private:
    std::thread thread;
};

RequestDispatcher& RequestDispatcher::Instance()
{
    static RequestDispatcher instance;
    return instance;
}

void RunDispatcher()
{
    RequestDispatcher::Instance().Run();
}

void RequestDispatcher::Start()
{
    thread = std::thread{ RunDispatcher };
}

void RequestDispatcher::Stop()
{
    try
    {
        Exit();
        thread.join();
    }
    catch (...)
    {
    }
}

void RequestDispatcher::Run()
{
    try
    {
        std::unique_ptr<Request> request = GetRequest();
        while (request)
        {
            try
            {
                request->Execute();
            }
            catch (const std::exception& ex)
            {
                PutOutputServiceMessage("request dispatcher: error executing " + request->Name() + ": " + std::string(ex.what()));
            }
            request = GetRequest();
        }
    }
    catch (...)
    {
    }
}

void StartRequestDispatcher()
{
    RequestDispatcher::Instance().Start();
}

void StopRequestDispatcher()
{
    RequestDispatcher::Instance().Stop();
}

} // namespace cmajor::service
