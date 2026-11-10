#include "pch.h"
#include "Process.h"

bool Process::GetPidByProcessName(std::string_view process_name, DWORD& output)
{
	PROCESSENTRY32 processentry;
	HANDLE Th32Handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Th32Handle == INVALID_HANDLE_VALUE)
	{
		CloseHandle(Th32Handle);
		std::cerr << "Couldn't open Toolhelp32 handle\n";
		return false;
	}

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(Th32Handle, &processentry) == TRUE)
	{
		if (process_name.compare(processentry.szExeFile) == NULL)
		{
			output = processentry.th32ProcessID;
			CloseHandle(Th32Handle);
			return true;
		}
	}

	CloseHandle(Th32Handle);
	output = NULL;
	std::cout << "Process not found\n";
	return true;
}

HANDLE Process::OpenProcessAllAccess(DWORD PID)
{
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
}

std::uintptr_t Process::GetModuleBaseAddress(DWORD processId, std::string_view moduleName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32,
        processId
    );

    if (snapshot == INVALID_HANDLE_VALUE)
        return NULL;

    MODULEENTRY32 moduleEntry{};
    moduleEntry.dwSize = sizeof(moduleEntry);

    if (Module32First(snapshot, &moduleEntry))
    {
        do
        {
            if (_stricmp(moduleEntry.szModule, moduleName.data()) == 0)
            {
                CloseHandle(snapshot);
                return reinterpret_cast<std::uintptr_t>(
                    moduleEntry.modBaseAddr
                    );
            }
        } while (Module32Next(snapshot, &moduleEntry));
    }

    CloseHandle(snapshot);
    return NULL;
}