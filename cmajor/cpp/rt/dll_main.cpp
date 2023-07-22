#ifdef _WIN32
#include <Windows.h>
#include <rt/module.hpp>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        cmajor::rt::SetRTModuleHandle(hinstDLL);
    }
    return true;
}

#endif
