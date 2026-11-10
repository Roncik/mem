#include "pch.h"
#include "Read.h"
#include "Process.h"
#include "UHANDLE.h"

void Read::handler()
{
	if (arguments.size() != this->num_of_arguments)
	{
		std::cerr << "Invalid number of arguments passed. Expected: " << this->num_of_arguments << ". Got: " << arguments.size() << ".\n";
		return;
	}

	DWORD PID{};
	{
		const auto& arg1 = arguments[0];
		//check whether arg1 is a PID or process name
		std::string_view sv{ arg1 };
		if (sv.find(".exe") != std::string_view::npos)
		{
			//process name
			Process::GetPidByProcessName(sv, PID);
			if (!PID)
			{
				std::cerr << "Couldn't find PID of this process name\n";
				return;
			}
		}
		else
		{
			//PID
			PID = std::stoi(arg1);
			if (!PID)
			{
				std::cerr << "Couldn't parse PID\n";
				return;
			}
		}
	}

	UHANDLE handle{ Process::OpenProcessAllAccess(PID) };

	if (!handle)
	{
		std::cerr << "Failed to obtain the process handle\n";
		return;
	}

	std::uintptr_t read_address{};
	{
		const auto& arg2 = arguments[1];
		std::string_view sv2{ arg2 };
		if (sv2.find("0x") != std::string_view::npos)
		{
			//hexadecimal
			read_address = std::stoull(arg2, nullptr, 16);
		}
		else
		{
			//decimal
			read_address = std::stoull(arg2);
		}

		if (!read_address)
		{
			std::cerr << "Parsing the read address failed\n";
			return;
		}
	}

	std::size_t read_size{};
	{
		const auto& arg3 = arguments[2];
		std::string_view sv3{ arg3 };
		if (sv3.find("0x") != std::string_view::npos)
		{
			//hexadecimal
			read_size = std::stoull(arg3, nullptr, 16);
		}
		else
		{
			//decimal
			read_size = std::stoull(arg3);
		}

		if (!read_size || read_size > 300000) //cap read size at 300kb to avoid alloc errors
		{
			std::cerr << "Parsing of the read size failed or invalid value passed\n";
			return;
		}
	}

	std::vector<char> read_buffer(read_size);
	//read_buffer.reserve(read_size);

	SIZE_T bytes_read{};
	if (!ReadProcessMemory(handle, reinterpret_cast<void*>(read_address), read_buffer.data(), read_size, &bytes_read))
	{
		std::cerr << "ReadProcessMemory failed with: " << GetLastError() << '\n';
		return;
	}

	if (bytes_read != read_size)
	{
		std::cerr << "The full size couldn't be read. Passed size: " << read_size << " Read size: " << bytes_read << '\n';
	}

	std::cout << "Read bytes: ";

	for (char c : read_buffer)
		std::cout << std::format("{:02X}", static_cast<uint8_t>(c)) << ' ';

	std::cout << '\n';

}
