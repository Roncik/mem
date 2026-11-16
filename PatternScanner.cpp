#include "pch.h"
#include "PatternScanner.h"

uintptr_t PatternScanner::scan_range(const HANDLE& handle, const uint8_t* pattern, const char* mask, const uintptr_t begin, const uintptr_t end, const size_t bufferSize)
{
	//check whether memory boundaries are readable
	if (char byte{}; !ReadProcessMemory(handle, reinterpret_cast<void*>(begin), &byte, 1, nullptr) ||
		!ReadProcessMemory(handle, reinterpret_cast<void*>(end), &byte, 1, nullptr))
	{
		std::cerr << "Invalid address range for pattern scan passed\n";
		return NULL;
	}

	std::vector<std::uint8_t> buffer{};
	buffer.reserve(bufferSize);

	const size_t patternLen = strlen(mask);

	for (size_t chunk_start{ begin }; chunk_start <= end; chunk_start += bufferSize - patternLen)
	{
		SIZE_T read_count{};
		bool read_result = ReadProcessMemory(handle, reinterpret_cast<void*>(chunk_start), buffer.data(), bufferSize, &read_count);
		if (!read_result && !read_count)
			return NULL;

		size_t pattern_id{ 0 };

		for (size_t i{}; i < read_count; ++i)
		{
			const auto& current_byte{ buffer.data()[i] };
			
			if (current_byte == pattern[pattern_id] || mask[pattern_id] == '?')
			{
				if (pattern_id == patternLen - 1)
					return chunk_start + i + 1 - patternLen;
				
				++pattern_id;
			}
			else
			{
				pattern_id = 0;
			}
		}

	}

	return NULL;
}
