#pragma once
#include <Windows.h>
#include <string>

struct Product
{
	std::string module_base;
	std::string exponent_base;
	std::string machine_code;

	std::string skm_token;
	INT pid;
};

extern "C"
{
	__declspec(dllexport) INT CheckLicense(Product p, const char* license_key);
}
