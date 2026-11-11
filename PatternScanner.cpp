#include "pch.h"
#include "PatternScan.h"
#include "DriverManager.h"


uintptr_t PatternScan::In_Scan(uint8_t* pattern, char* mask, uintptr_t begin, size_t size)
{
	size_t patternLength = strlen(mask);

	for (unsigned int i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (unsigned int j = 0; j < patternLength; j++)
		{
			if (mask[j] != '?' && pattern[j] != *(uint8_t*)(begin + i + j))
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			return (begin + i);
		}
	}
	return 0;
}

uintptr_t PatternScan::Ex_Scan(uint8_t* pattern, char* mask, const uintptr_t& begin, const uintptr_t& end, const SIZE_T& bufferSize)
{
	uintptr_t currentChunk = begin;
	size_t patternSize = strlen(mask);
	SIZE_T bytesRead = bufferSize;

	while (currentChunk < end)
	{
		std::unique_ptr<uint8_t[]> Buffer = std::make_unique<uint8_t[]>(bufferSize);

		ReadPM_withBuffer(currentChunk, Buffer.get(), bufferSize);
		//ReadProcessMemory(process->handle, currentChunk, &buffer, sizeof(buffer), &bytesRead);
		//Buffer = ReadPM<buff>(currentChunk);

		uintptr_t internalAddress = In_Scan(pattern, mask, reinterpret_cast<uintptr_t>(Buffer.get()), bytesRead);

		if (internalAddress != 0)
		{
			//calculate from internal to external
			uintptr_t offsetFromBuffer = internalAddress - reinterpret_cast<uintptr_t>(Buffer.get());
			return (currentChunk + offsetFromBuffer);
		}
		else
		{
			//advance to next chunk
			currentChunk += (bytesRead - patternSize);
			if ((currentChunk + bytesRead) > end && currentChunk != end)
			{
				currentChunk = end - (bytesRead - patternSize);
			}
		}
	}
	return 0;
}

uint64_t PatternScan::get_offset_from_instruction(uintptr_t instruction_address, uintptr_t base_of_module)
{
	if (!instruction_address)
		return 0;
	uint64_t offsetnextinstruction = (instruction_address + 0x3 + 0x4) - base_of_module;
	uint64_t lpoffset = ReadPM<uint32_t>(instruction_address + 0x3) + offsetnextinstruction;
	return lpoffset;
}

uint64_t PatternScan::get_offset_from_instruction_2(uintptr_t instruction_address, uintptr_t base_of_module)
{
	if (!instruction_address)
		return 0;
	uint64_t offsetnextinstruction = (instruction_address + 0x2 + 0x5) - base_of_module;
	uint64_t lpoffset = ReadPM<uint32_t>(instruction_address + 0x2) + offsetnextinstruction;
	return lpoffset;
}

uint64_t PatternScan::get_offset_from_sig(uint8_t* signature, char* mask, uintptr_t start, uintptr_t end)
{
	return get_offset_from_instruction((uintptr_t)Ex_Scan(signature, mask, start, end), (uintptr_t)start);
}

uint64_t PatternScan::get_offset_from_sig_2(uint8_t* signature, char* mask, uintptr_t start, uintptr_t end)
{
	return get_offset_from_instruction_2((uintptr_t)Ex_Scan(signature, mask, start, end), (uintptr_t)start);
}

uint64_t PatternScan::get_offset_wrapper(const Signature& sig, uint64_t start, uint64_t end)
{
	uintptr_t ex_scan_result = (uintptr_t)Ex_Scan((uint8_t*)sig.bytes.data(), (char*)sig.mask.c_str(), start, end);
	return get_offset_from_instruction(ex_scan_result, start);
}

uint64_t PatternScan::get_offset_wrapper_2(const Signature& sig, uint64_t start, uint64_t end)
{
	return get_offset_from_instruction_2((uintptr_t)Ex_Scan((uint8_t*)sig.bytes.data(), (char*)sig.mask.c_str(), start, end), start);
}
