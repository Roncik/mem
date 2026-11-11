#include "pch.h"
#include "Read.h"
#include "Process.h"
#include "UHANDLE.h"

void Read::handler()
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

	std::uintptr_t read_address{ parse_address(arguments[1])};
	if (!read_address)
	{
		std::cerr << "Failed to parse read address\n";
		return;
	}

	std::size_t read_size{ parse_size(arguments[2])};
	if (!read_size)
	{
		std::cerr << "Failed to parse size or 0 passed\n";
		return;
	}

	std::vector<char> read_buffer(read_size);
	//read_buffer.reserve(read_size);

	SIZE_T bytes_read{};
	if (!ReadProcessMemory(handle, reinterpret_cast<void*>(read_address), read_buffer.data(), read_size, &bytes_read))
	{
		std::cerr << "ReadProcessMemory failed with error code: " << GetLastError() << '\n';
		return;
	}

	if (bytes_read != read_size)
	{
		std::cerr << "The full size couldn't be read. Passed size: " << read_size << " Read size: " << bytes_read << '\n';
	}

	std::ostream
	if (bytes_read <= 10000)
	
	std::cout << "Read bytes: ";

	for (char c : read_buffer)
		std::cout << std::format("{:02X}", static_cast<uint8_t>(c)) << ' ';

	std::cout << '\n';

}
