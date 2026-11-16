#include "pch.h"
#include "ModuleBase.h"
#include "UHANDLE.h"
#include "Process.h"

void ModuleBase::handler()
{
	if (!validate_arg_count())
	{
		std::cerr << "Invalid number of arguments passed. Expected: " << this->num_of_arguments << ". Got: " << arguments.size() << ".\n";
		return;
	}

	DWORD PID = parse_pid(arguments[0]);
	if (!PID)
	{
		std::cerr << "Failed to parse PID/process name\n";
		return;
	}

	UHANDLE handle{ Process::OpenProcessAllAccess(PID) };
	if (!handle)
	{
		std::cerr << "Failed to obtain the process handle\n";
		return;
	}

	const auto& module_name = arguments[1];
	size_t module_size{};
	const auto base = Process::GetModuleBaseAddress(PID, module_name, &module_size);
	if (!base)
	{
		std::cerr << "Failed to get base address for the module name: " << module_name << '\n';
		return;
	}

	std::cout << "Name: " << module_name << " | Base: " << std::hex << "0x" << base << " | Size: " << std::hex << "0x" << module_size << '\n';
}
