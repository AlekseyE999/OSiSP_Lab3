#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <iostream>
#include <libloaderapi.h>
#include <TlHelp32.h>
#include <winnt.h>
#include  <io.h>
#include <vector>

bool FileExist(char* name)
{
    return _access(name, 0) != -1;
}

bool Inject(DWORD pID, const char* path)
{
    HANDLE proc_handle;
    LPVOID RemoteString;
    LPCVOID LoadLibAddy;
    if (pID == 0)
        return false;

    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
    if (proc_handle == 0)
        return false;

    LoadLibAddy = GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    RemoteString = VirtualAllocEx(proc_handle, NULL, strlen(path) * sizeof(char),
        MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    SIZE_T written;
    WriteProcessMemory(proc_handle, RemoteString, path, strlen(path) * sizeof(char), &written);
    HANDLE threadId = CreateRemoteThread(proc_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, RemoteString, NULL, NULL);
    CloseHandle(proc_handle);
    return true;
}

DWORD GetProcessIdByName(const char* processName)
{
    DWORD processId = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 processEntry;
    ZeroMemory(&processEntry, sizeof(processEntry));
    processEntry.dwSize = sizeof(processEntry);
    std::vector<char*> vd;
    while (Process32Next(hSnapshot, &processEntry))
    {

        if (!strcmp(processEntry.szExeFile, processName))
        {
            processId = processEntry.th32ProcessID;
            break;
        }
    }

    return processId;
}

int main()
{
    std::string process_name = "ProcessToInjectInto.exe";
    std::string dll_name = "Dll.dll";
    std::string path = "c:\Downloads\Dll1.dll";
    DWORD pID = GetProcessIdByName(process_name.c_str());
    std::cout << "process name : " + process_name << std::endl;
    std::cout << "dll name : " + dll_name << std::endl;

    if (Inject(pID, path.c_str()))
    {
        std::cout << "DLL successfully injected!" << std::endl;
    }
    else
    {
        std::cout << "CRITICAL ERROR!" << std::endl << " Destroying window..." << std::endl;
    }

    return 0;
}
