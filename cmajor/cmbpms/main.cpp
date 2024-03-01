// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

// Cmajor binary port map server

import std.core;
import bpm.server;
import util;

void InitApplication()
{
    util::Init();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bpm::RunPortMapServer();
    }
    catch (const std::exception& ex)
    {
        std::cout << "binary-port-map-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        return 1;
    }
    return 0;
}