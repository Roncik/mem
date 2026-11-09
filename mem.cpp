#include "pch.h"


namespace
{
	const char* gCurrentPath{};
}

// arguments:
// process name/PID
// virtual address
// size
// read/write/patternscan
// write data
// pattern
// dump game module
// class informer?
// persistent history?
// --help

// how to return the read data? hexstring? binary file?
// pattern scanning should have an option to only scan pages that are executable
// there should be an option to select operation on game module only or on all modules loaded in the process
// claude should be able to build the vs project


bool GetPidByProcessName(std::string_view process_name, DWORD& output)
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

int main(int argc, char* argv[])
{


    
    
    
    
    return 0;
}

