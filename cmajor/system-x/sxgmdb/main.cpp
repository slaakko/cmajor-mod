// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <Windows.h>

import sxgmdb.main.window;
import cmajor.systemx.guiterm;
import wing;
import cmajor.systemx.kernel;
import cmajor.systemx.machine;
import cmajor.systemx.sxutil;
import sxgmdb.themes;
import util;
import std.core;

void InitApplication(HINSTANCE instance)
{
    util::Init();
    wing::Init(instance);
    cmajor::systemx::kernel::Init(false);
    sxgmdb::InitThemes();
}

void DoneApplication()
{
    cmajor::systemx::kernel::Done();
    wing::Done();
    util::Done();
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    try
    {
        InitApplication(instance);
        std::string filePath;
        std::vector<std::string> args;
        if (cmdLine)
        {
            std::string commandLine(cmdLine);
            args = cmajor::systemx::ParseArgs(commandLine);
            if (!args.empty())
            {
                filePath = cmajor::systemx::SearchBin(args[0]);
            }
        }
        sxgmdb::MainWindow mainWindow(filePath, args);
        mainWindow.SetIcon(wing::Application::GetResourceManager().GetIcon("bug.icon"));
        mainWindow.SetSmallIcon(wing::Application::GetResourceManager().GetIcon("bug.icon"));
        wing::Application::Run(mainWindow);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(nullptr, ex.what());
        return 1;
    }
    DoneApplication();
    return 0;
}
