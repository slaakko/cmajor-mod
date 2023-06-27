// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.binary.message.test.server;
import bm_test_message;
import util;
import std.core;

void RunClient()
{
    util::TcpSocket client("127.0.0.1", "56789");
    std::unique_ptr<BmTest::SimpleMessage> message(new BmTest::SimpleMessage());
    cmajor::bmp::WriteMessage(client, message.get());
    std::unique_ptr<cmajor::bmp::BinaryMessage> reply(cmajor::bmp::ReadMessage(client));
    std::cout << "got reply " << reply->Id() << std::endl;
}

int main()
{
    try
    {
        util::Init();
        StartServer();
        //RunClient();
        std::string line;
        std::getline(std::cin, line);
        StopServer();
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}