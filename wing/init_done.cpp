// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.init.done;

import wing.base;
import wing.application;
import wing.metrics;
import wing.shell;
import wing.theme;

namespace wing {

void Init(HINSTANCE instance)
{
    WingInit(instance);
    ApplicationInit();
    LoadMetrics();
    ShellInit();
    ThemeInit();
}

void Done()
{
    ThemeDone();
    ShellDone();
    ApplicationDone();
    WingDone();
}

} // wing
