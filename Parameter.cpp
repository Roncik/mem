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

bool Parameter::validate_arg_count()
{
	return arguments.size() == num_of_arguments;
}
