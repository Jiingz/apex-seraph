#include "driver.h"
#include "server_shared.h"
#include <iostream>
#include <core/libs/xor.h>

// Link to winsock.
#pragma comment(lib, "Ws2_32")

// Send request packet and wait for completion.
static bool send_packet(
	const SOCKET	connection,
	const Packet& packet,
	uint64_t& out_result)
{
	Packet completion_packet{ };

	if (send(connection, (const char*)&packet, sizeof(Packet), 0) == SOCKET_ERROR)
		return false;

	const auto result = recv(connection, (char*)&completion_packet, sizeof(Packet), 0);
	if (result < sizeof(PacketHeader) ||
		completion_packet.header.magic != packet_magic ||
		completion_packet.header.type != PacketType::packet_completed)
		return false;

	out_result = completion_packet.data.completed.result;
	return true;
}

static uint32_t copy_memory(
	const SOCKET	connection,
	const uint32_t	src_process_id,
	const uintptr_t src_address,
	const uint32_t	dest_process_id,
	const uintptr_t	dest_address,
	const size_t	size)
{
	Packet packet{ };

	packet.header.magic = packet_magic;
	packet.header.type = PacketType::packet_copy_memory;

	auto& data = packet.data.copy_memory;
	data.src_process_id = src_process_id;
	data.src_address = uint64_t(src_address);
	data.dest_process_id = dest_process_id;
	data.dest_address = uint64_t(dest_address);
	data.size = uint64_t(size);

	uint64_t result = 0;
	if (send_packet(connection, packet, result))
		return uint32_t(result);

	return 0;
}

void driver::Initialize()
{
	//Set Process ID
	HWND handle_to_window = FindWindowA(NULL, xor ("Apex Legends"));

	if (!handle_to_window)
	{
		system(xor ("color c"));
		std::cout << xor ("Couldn't find Apex. Make sure Apex is opened before you start Seraph. Seraph will close now.") << std::endl;
		Sleep(5000);
		exit(0);
	}

	GetWindowThreadProcessId(handle_to_window, &driver::process_id);

	//Initiate winsock
	try
	{
		WSADATA wsa_data;
		WSAStartup(MAKEWORD(2, 2), &wsa_data);
		driver::driver_socket = CreateSocketConnection();
	}
	catch (...) //its fine to catch all exceptions in this case
	{
		system(xor ("color c"));
		std::cout << xor ("Connetion to Driver failed.") << std::endl;
		Sleep(5000);
		exit(0);
	}
}

void driver::Deinitialize()
{
	WSACleanup();
}

SOCKET driver::CreateSocketConnection()
{

	SOCKADDR_IN address{ };

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(server_ip);
	address.sin_port = htons(server_port);

	const auto connection = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(connection, (SOCKADDR*)&address, sizeof(address)) == SOCKET_ERROR)
	{
		closesocket(connection);
		system(xor ("color c"));
		std::printf(xor ("Connection failed. %o"), SOCKET_ERROR);
		Sleep(5000);
		return INVALID_SOCKET;
	}

	return connection;
}

void driver::Disconnect()
{
	closesocket(driver::driver_socket);
}

uint32_t  driver::ReadMemory(const uintptr_t address, const uintptr_t buffer, const size_t	size)
{
	return copy_memory(driver::driver_socket, process_id, address, GetCurrentProcessId(), buffer, size);
}

uint32_t  driver::WriteMemory(const uintptr_t address, const uintptr_t buffer, const size_t	size)
{
	return copy_memory(driver::driver_socket, GetCurrentProcessId(), buffer, process_id, address, size);
}

uint64_t  driver::GetBaseAddress()
{
	Packet packet{ };

	packet.header.magic = packet_magic;
	packet.header.type = PacketType::packet_get_base_address;

	auto& data = packet.data.get_base_address;
	data.process_id = process_id;

	uint64_t result = 0;
	if (send_packet(driver::driver_socket, packet, result))
		return result;

	return 0;
}