#include "pch.h"
#include "Patternscan.h"
#include "Process.h"
#include "UHANDLE.h"
#include "PatternScanner.h"

void Patternscan::handler()
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
	const auto module = [&]() -> std::pair<std::uintptr_t, size_t>
		{
			std::uintptr_t base{};
			size_t size{};
			base = Process::GetModuleBaseAddress(PID, module_name, &size);
			return { base, size };
		}();
	const auto& module_base = module.first;
	const auto& module_size = module.second;

	//pattern (0x10,0x20,0x30)
	std::vector<std::uint8_t> pattern_bytes{};
	std::stringstream ss;
	ss << arguments[2];
	while (ss.good())
	{
		std::string entry;
		std::getline(ss, entry, ',');
		pattern_bytes.push_back(static_cast<std::uint8_t>(std::stoul(entry, nullptr, 16)));
	}

	//pattern mask(xxx????xx)
	const char* pattern_mask = arguments[3];

	std::uintptr_t result = PatternScanner::Ex_Scan(handle, pattern_bytes.data(), pattern_mask, module_base, module_base + module_size);
	if (!result)
	{
		std::cerr << "Pattern was not found in the given module\n";
		return;
	}

	std::cout << "Pattern found at: " << result << '\n';
}
