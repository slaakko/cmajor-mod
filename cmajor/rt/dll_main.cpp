#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

import cmajor.rt.mod;
import util;

BOOL APIENTRY DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        cmajor::rt::SetRTModuleHandle(inst);
    }
    return TRUE;
}
