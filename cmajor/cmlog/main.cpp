// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.log.log.server;
import util;
import std.core;

void InitApplication()
{
    util::Init();
    cmajor::log::InitLogServer();
}

void DoneApplication()
{
    cmajor::log::DoneLogServer();
    util::Done();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        cmajor::log::StartLogServer();
        std::cout << "type 'exit' to quit..." << std::endl;
        std::string line;
        std::cin >> line;
        while (line != "exit")
        {
            std::cin >> line;
        }
        cmajor::log::StopLogServer();
        std::cout << "exited" << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    DoneApplication();
    return 0;

}