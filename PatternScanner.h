#pragma once
namespace PatternScanner
{
	constexpr SIZE_T BUFFER_SIZE = 1000000; // 1MB buffer
	
	struct Signature
	{
		std::vector<uint8_t> bytes;
		std::string mask;
	};

	uintptr_t scan_range(const HANDLE& handle, const uint8_t* pattern, const char* mask, const uintptr_t begin, const uintptr_t end, const size_t bufferSize = BUFFER_SIZE);
};

