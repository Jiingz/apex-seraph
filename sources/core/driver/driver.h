#pragma once
#include <WinSock2.h>
#include <cstdint>

namespace driver
{
	static DWORD process_id = 0;
	static SOCKET driver_socket = 0;

	void	Initialize();
	void	Deinitialize();

	SOCKET	CreateSocketConnection();
	void	Disconnect();

	uint32_t ReadMemory(uintptr_t address, uintptr_t buffer, size_t size);
	uint32_t WriteMemory(uintptr_t address, uintptr_t buffer, size_t size);
	uint64_t GetBaseAddress();

	template <typename T>
	T Read(const uintptr_t address)
	{
		T buffer{ };
		ReadMemory(address, uint64_t(&buffer), sizeof(T));
		return buffer;
	}

	template <typename T>
	void Write(const uintptr_t address, const T& buffer)
	{
		WriteMemory(address, uint64_t(&buffer), sizeof(T));
	}
}