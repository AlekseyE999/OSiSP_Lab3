#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <vector>

#include "DLL.h"

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
	std::string newString = "Helo danik";
	std::cout << "Start \"oldString\" value: " << oldString1 << std::endl;
	ReplaceString(GetProcessIdByName("UsingStaticDll.exe"), oldString.c_str(), newString.c_str());
	std::cout << "End \"oldString\" value: " << oldString1 << std::endl;
	return 0;
}