#pragma once
class PatternScanner
{
	static inline constexpr SIZE_T BUFFER_SIZE = 1000000; // 1MB buffer
	
public:
	struct Signature
	{
		std::vector<uint8_t> bytes;
		std::string mask;
	};

	static uintptr_t In_Scan(const uint8_t* pattern, const char* mask, uintptr_t begin, size_t size);

	static uintptr_t Ex_Scan(const HANDLE& handle, const uint8_t* pattern, const char* mask, const uintptr_t& begin, const uintptr_t& end, const SIZE_T& bufferSize = BUFFER_SIZE);

	static uint64_t get_offset_from_instruction(uintptr_t instruction_address, uintptr_t base_of_module);

	static uint64_t get_offset_from_instruction_2(uintptr_t instruction_address, uintptr_t base_of_module);

	static uint64_t get_offset_from_sig(uint8_t* signature, char* mask, uintptr_t start, uintptr_t end);

	static uint64_t get_offset_from_sig_2(uint8_t* signature, char* mask, uintptr_t start, uintptr_t end);

	static uint64_t get_offset_wrapper(const Signature& sig, uint64_t start, uint64_t end);

	static uint64_t get_offset_wrapper_2(const Signature& sig, uint64_t start, uint64_t end);
};

