#pragma once

class UHANDLE
{
public:
	HANDLE handle{};



	~UHANDLE()
	{
		if (handle && handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);
	}

	UHANDLE(HANDLE handle) : handle{ handle } {}
	UHANDLE() = default;

	UHANDLE(const UHANDLE& other) = delete;
	UHANDLE& operator=(const UHANDLE& other) = delete;

	UHANDLE(UHANDLE&& other) noexcept
	{
		if (handle && handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);
		handle = other.handle;
		other.handle = NULL;
	}

	UHANDLE& operator=(UHANDLE&& other) noexcept
	{
		if (handle && handle != INVALID_HANDLE_VALUE)
			CloseHandle(handle);
		handle = other.handle;
		other.handle = NULL;
	}

	bool is_valid() const noexcept 
	{
		return handle != nullptr && handle != INVALID_HANDLE_VALUE;
	}

	explicit operator bool() const noexcept 
	{
		return is_valid();
	}

	operator HANDLE() const noexcept
	{
		return HANDLE{ handle };
	}
};

