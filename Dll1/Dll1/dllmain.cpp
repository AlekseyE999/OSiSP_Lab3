#include "pch.h"
#include <chrono>
#include <fstream>
#include <TlHelp32.h>
#include <vector>

#include "Dll.h"

DWORD GetProcessIdByName(const char* processName)
{
    DWORD processId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 processEntry;
    ZeroMemory(&processEntry, sizeof(processEntry));
    processEntry.dwSize = sizeof(processEntry);
    std::vector<std::string> vd;
    while (Process32Next(hSnapshot, &processEntry))
    {

        if (!strcmp(processEntry.szExeFile, processName))
        {
            processId = processEntry.th32ProcessID;
            break;
        }

        vd.push_back(processEntry.szExeFile);
    }

    return processId;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        ReplaceString(GetProcessIdByName("ProcessToInjectInto.exe"), "Hello World!", "Hello dddsdd");
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

