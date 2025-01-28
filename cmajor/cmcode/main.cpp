// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <Windows.h>

import cmcode.main.window;
import cmcode.themes;
import cmcode.config;
import cmajor.service;
import cmajor.view;
import wing;
import util;
import std.core;

void InitApplication(HINSTANCE instance)
{
    util::Init();
    wing::Init(instance);
    cmajor::service::Init();
    cmajor::view::Init();
    cmcode::LoadConfiguration();
    cmcode::InitThemes();
}

void FinalizeApplication()
{
    cmajor::view::Done();
    cmajor::service::Done();
    wing::Done();
    util::Done();
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
    try
    {
        InitApplication(instance);
        std::string filePath;
        std::string commandLine(cmdLine);
        if (!commandLine.empty())
        {
            filePath = commandLine;
        }
        cmcode::MainWindow mainWindow(filePath);
        mainWindow.SetIcon(wing::Application::GetResourceManager().GetIcon("cmcode.small.icon"));
        mainWindow.SetSmallIcon(wing::Application::GetResourceManager().GetIcon("cmcode.small.icon"));
        wing::Application::Run(mainWindow);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(nullptr, ex.what());
        return 1;
    }
    FinalizeApplication();
    return 0;
}
