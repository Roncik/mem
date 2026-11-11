#include "pch.h"
#include "Help.h"

void Help::handler()
{
	std::cout << "mem: CLI tool for reading/writing Windows Process's memory.\n";
	std::cout << "Options: Read, Write, Get module base address, Byte pattern scan\n\n";

	std::cout << "Usage: \"mem [OPTION] [PARAMETER1] [PARAMETER2]...\"\n\n";

	std::cout << "Help:\n -h, --help | Syntax: \"-h\" |\n"
		<< "Example: \"mem -h\"\n\n";

	std::cout << "Reading process memory:\n -r, --read | Syntax: \"mem -r [PID/processName] [VirtualAddress] [SizeToRead]\" |\n"
		<< "Example: \"mem -r cmd.exe 0x010BB2C8 5\" output: \"Read bytes: E8 AD 0B 00 00\"\n\n";

	std::cout << "Writing process memory:\n -w, --write | Syntax: \"mem -w [PID/processName] [VirtualAddress] [BytesToWrite] [SizeOfBytesToWrite]\" |\n"
		<< "Example: \"mem -w cmd.exe 0x010BB2C8 0x90,0x90,0x90 3\" output: \"Write success\"\n\n";

	std::cout << "Get base of a specific module within a process:\n -b, --base | Syntax: \"mem -b [PID/processName] [ModuleName]\" |\n"
		<< "Example: \"mem -b cmd.exe ntdll.dll\" output: \"ntdll.dll base: 2001993728\"\n\n";

	std::cout << "Scan for a byte pattern:\n -p, --pattern | Syntax: \"mem -p [PID/processName] [ModuleName] [Pattern] [PatternMask]\" |\n"
		<< "Example: \"mem -p cmd.exe cmd.exe 0x48,0x8b,0x05,0x00,0x00,0x00,0x00,0xf0,0xb6 xxx????xx\" output: \"Pattern found at: 20034663728\"\n\n";

	std::cout << "PS:\n";
	std::cout << " - Argument parser is written in a way that you can chain options\n   for example: "
		<< "\"mem -r launcher.exe 0x77540017 1 -w launcher.exe 0x77540017 0x90 1 -r launcher.exe 0x77540017 1\"\n\n";

	std::cout << " - Reading more than 10000 bytes will save the output in a binfile in the exec directory\n\n";

	std::cout << " - Writing will change page protection if it is not writable. This can crash the target process.\n\n";
}
