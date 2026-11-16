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
		<< "Example:\nmem -r explorer.exe 0x7FF72EBA7113 5\nRead bytes: 0xCC 0xCC 0xCC 0xCC 0xCC\n\n";

	std::cout << "Writing process memory:\n -w, --write | Syntax: \"mem -w [PID/processName] [VirtualAddress] [BytesToWrite] [SizeOfBytesToWrite]\" |\n"
		<< "Example:\nmem -w explorer.exe 0x7FF72EBA7113 0x10,0x20,0x30,0x40 4\nWrite success\n\n";

	std::cout << "Get base of a specific module within a process:\n -b, --base | Syntax: \"mem -b [PID/processName] [ModuleName]\" |\n"
		<< "Example:\nmem -b explorer.exe ntdll.dll\nName: ntdll.dll | Base : 0x7fffafe40000 | Size : 0x269000\n\n";

	std::cout << "Scan for a byte pattern:\n -p, --pattern | Syntax: \"mem -p [PID/processName] [ModuleName] [Pattern] [PatternMask]\" |\n"
		<< "Example:\nmem -p explorer.exe explorer.exe 0x10,0x20,0x30,0x40 xx?x\nPattern found at : 0x7ff72eba7113\n\n";

	std::cout << "PS:\n";
	std::cout << " - Argument parser is written in a way that you can chain options\n   for example: "
		<< "mem -r explorer.exe 0x7FF72EBA7113 5 -w explorer.exe 0x7FF72EBA7113 0x10,0x20,0x30,0x40 4 -r explorer.exe 0x7FF72EBA7113 5\nRead bytes : 0xCC 0xCC 0xCC 0xCC 0xCC\nWrite success\nRead bytes : 0x10 0x20 0x30 0x40 0xCC\n\n";
	
	std::cout << " - Reading more than 10000 bytes will save the output in a binfile in the mem directory\n\n";

	std::cout << " - Writing will change page protection if it is not writable. This can crash the target process.\n\n";
}
