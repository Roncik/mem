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

	UHANDLE handle{ Process::OpenProcessAllAccess(PID) };
	if (!handle)
	{
		std::cerr << "Failed to obtain the process handle\n";
		return;
	}

	const auto& module_name = arguments[1];
	const auto base = Process::GetModuleBaseAddress(PID, module_name);

	if (!base)
	{
		std::cerr << "Failed to get base for the module name: " << module_name << '\n';
		return;
	}

	std::cout << module_name << " Base: " << base << '\n';
}
