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

bool Process::WriteMemory(HANDLE hProcess, LPVOID lpBaseAddress, LPCVOID lpBuffer, SIZE_T nSize, BOOL force)
{
    // Helper to determine if a protection flag permits writing
    auto IsWritable = [](DWORD protect) -> bool
        {
            // Strip modifier flags like PAGE_GUARD or PAGE_NOCACHE
            DWORD baseProtect = protect & 0xFF;
            return (baseProtect == PAGE_READWRITE ||
                baseProtect == PAGE_EXECUTE_READWRITE ||
                baseProtect == PAGE_WRITECOPY ||
                baseProtect == PAGE_EXECUTE_WRITECOPY);
        };

    // Helper to determine if a protection flag permits execution
    auto IsExecutable = [](DWORD protect) -> bool
        {
            DWORD baseProtect = protect & 0xFF;
            return (baseProtect == PAGE_EXECUTE ||
                baseProtect == PAGE_EXECUTE_READ ||
                baseProtect == PAGE_EXECUTE_READWRITE ||
                baseProtect == PAGE_EXECUTE_WRITECOPY);
        };


    MEMORY_BASIC_INFORMATION mbi = { 0 };

    // 1. Query the current memory page state
    if (VirtualQueryEx(hProcess, lpBaseAddress, &mbi, sizeof(mbi)) == 0) {
        std::cerr << "VirtualQueryEx failed. Error: " << GetLastError() << std::endl;
        return false;
    }

    DWORD currentProtect = mbi.Protect;
    DWORD oldProtect = currentProtect;
    bool modifiedProtection = false;

    // 2. Check if the page is already writable
    if (!IsWritable(currentProtect)) {
        // Determine target protection based on executable status
        DWORD newProtect = IsExecutable(currentProtect) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;

        // Modify protection
        if (!VirtualProtectEx(hProcess, lpBaseAddress, nSize, newProtect, &oldProtect)) {
            std::cerr << "VirtualProtectEx (enable write) failed. Error: " << GetLastError() << std::endl;
            return false;
        }
        modifiedProtection = true;
    }

    // 3. Perform the write operation
    SIZE_T bytesWritten = 0;
    BOOL writeSuccess = WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, &bytesWritten);
    if (!writeSuccess || bytesWritten != nSize) {
        std::cerr << "WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
    }

    // 4. Restore original protection if it was modified
    if (modifiedProtection) {
        DWORD dummy = 0;
        if (!VirtualProtectEx(hProcess, lpBaseAddress, nSize, oldProtect, &dummy)) {
            std::cerr << "VirtualProtectEx (restore) failed. Error: " << GetLastError() << std::endl;
        }
    }

    return writeSuccess && (bytesWritten == nSize);
}
