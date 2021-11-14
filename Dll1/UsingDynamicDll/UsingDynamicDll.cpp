#include <windows.h>
#include <iostream>
#include <libloaderapi.h>
#include <TlHelp32.h>
#include <vector>

typedef int(__cdecl* replaceMemoryFunc)(DWORD processPid, const char*, const char*);

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

int main()
{
    std::string oldString = "Hello world";
    std::string oldString1 = "Hello world";
    std::string newString = "Helo Danik";
    std::cout << "Start \"oldString\" value: " << oldString1 << std::endl;
    HINSTANCE hlib = LoadLibrary("C:\Downloads\Dll1.dll");
    replaceMemoryFunc replaceMemory = NULL;
    int result = -1;

    if (hlib)
    {
        replaceMemory = (replaceMemoryFunc)GetProcAddress(hlib, "ReplaceString");
        if (replaceMemory)
        {
            result = replaceMemory(GetProcessIdByName("UsingDynamicDll.exe"), oldString.c_str(), newString.c_str());
        }
        FreeLibrary(hlib);
    }

    std::cout << "End \"oldString\" value: " << oldString1 << std::endl;
}
