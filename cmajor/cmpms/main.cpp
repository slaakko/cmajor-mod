// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.portmap;
import soul.xml.serialization;
import soul.xml.xpath;
import util;
import std.core;

void InitApplication()
{
    util::Init();
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        cmajor::portmap::RunPortMapServer();
    }
    catch (const std::exception& ex)
    {
        std::cout << "port-map-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        return 1;
    }
    return 0;
}
