#include "pch.h"
#include "Parameter.h"
#include "Process.h"

DWORD Parameter::parse_pid(const char* arg)
{
	DWORD PID{};
	//check whether arg1 is a PID or process name
	std::string_view sv{ arg };
	if (sv.find(".exe") != std::string_view::npos)
	{
		//process name
		Process::GetPidByProcessName(sv, PID);
		if (!PID)
		{
			std::cerr << "Couldn't find PID of this process name\n";
			return NULL;
		}
	}
	else
	{
		//PID
		PID = std::stoi(arg);
		if (!PID)
		{
			std::cerr << "Couldn't parse PID\n";
			return NULL;
		}
	}

	return PID;
}

std::uintptr_t Parameter::parse_address(const char* arg)
{
	std::uintptr_t address{};
	std::string_view sv2{ arg };
	if (sv2.find("0x") != std::string_view::npos)
	{
		//hexadecimal
		address = std::stoull(arg, nullptr, 16);
	}
	else
	{
		//decimal
		address = std::stoull(arg);
	}

	if (!address)
	{
		std::cerr << "Parsing the read address failed\n";
		return NULL;
	}

	return address;
}

std::uintptr_t Parameter::parse_size(const char* arg)
{
	size_t size{};
	std::string_view sv{ arg };
	if (sv.find("0x") != std::string_view::npos)
	{
		//hexadecimal
		size = std::stoull(arg, nullptr, 16);
	}
	else
	{
		//decimal
		size = std::stoull(arg);
	}

	if (!size || size > 300000) //cap read size at 300kb to avoid alloc errors
	{
		std::cerr << "Parsing of the read size failed or invalid value passed\n";
		return NULL;
	}

	return size;
}

bool Parameter::validate_arg_count()
{
	return arguments.size() == num_of_arguments;
}
