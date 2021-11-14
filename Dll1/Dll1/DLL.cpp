#include "pch.h"
#include "framework.h"
#include "Dll.h"
#include <vector>

void ReplaceString(DWORD processPid, const char* replacedString, const char* replacementString)
{
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processPid);
    if (process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        MEMORY_BASIC_INFORMATION info;
        std::vector<char> procMem;
        char* border = 0;
        while (border < si.lpMaximumApplicationAddress)
        {
            if (VirtualQueryEx(process, border, &info, sizeof(info)))
            {
                if (info.State == MEM_COMMIT && info.AllocationProtect == PAGE_READWRITE)
                {
                    border = (char*)info.BaseAddress;
                    procMem.resize(info.RegionSize);
                    SIZE_T bytesRead;
                    try
                    {
                        if (ReadProcessMemory(process, border, &procMem[0], info.RegionSize, &bytesRead))
                        {
                            for (size_t i = 0; i < (bytesRead - strlen(replacedString)); ++i)
                            {
                                int len = strlen(replacedString);
                                if ((memcmp(replacedString, &procMem[i], len) == 0) && (replacedString != (char*)border + i))
                                {
                                    char* ref = (char*)border + i;

                                    for (int j = 0; j < strlen(replacementString); j++)
                                        ref[j] = replacementString[j];

                                    ref[strlen(replacementString)] = 0;
                                }
                            }
                        }
                    }
                    catch (std::bad_alloc& e)
                    {

                    }
                }

                border += info.RegionSize;
            }
        }
    }
}
