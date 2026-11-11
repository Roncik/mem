#include "pch.h"
#include "Write.h"
#include "UHANDLE.h"
#include "Process.h"

void Write::handler()
{
	if (!validate_arg_count())
	{
		std::cerr << "Invalid number of arguments passed. Expected: " << this->num_of_arguments << ". Got: " << arguments.size() << ".\n";
		return;
	}

	DWORD PID{ parse_pid(arguments[0]) };
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

	std::uintptr_t write_address{parse_address(arguments[1])};
	if (!write_address)
	{
		std::cerr << "Failed to parse write address\n";
		return;
	}

	//parse the data-to-write (0x10,0x20,0x30)

	std::vector<std::uint8_t> bytes{};
	std::stringstream ss;
	ss << arguments[2];
	while (ss.good())
	{
		std::string entry;
		std::getline(ss, entry, ',');
		bytes.push_back(static_cast<std::uint8_t>(std::stoul(entry, nullptr, 16)));
	}

	std::size_t write_size{ parse_size(arguments[3]) };
	if (!write_size || write_size != bytes.size())
	{
		std::cerr << "Failed to parse write size, or 0 passed, or size doesn't match the number of bytes-to-write passed\n";
		return;
	}

	if (!Process::WriteMemory(handle, reinterpret_cast<void*>(write_address), bytes.data(), write_size, TRUE))
	{
		std::cerr << "Writing process memory failed\n";
		return;
	}

	std::cout << "Write success\n";
}
