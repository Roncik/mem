#pragma once
namespace Process
{
	bool GetPidByProcessName(std::string_view process_name, DWORD& output);

	HANDLE OpenProcessAllAccess(DWORD PID);

	std::uintptr_t GetModuleBaseAddress(DWORD processId, std::string_view moduleName);
};

