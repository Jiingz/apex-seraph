#pragma once
#include <core/config.h>
#include <memory>

extern std::shared_ptr<Config> config_;

extern "C"
{
	__declspec(dllexport) void InitializeCore();
	__declspec(dllexport) void RunCore();
	__declspec(dllexport) Config* GetConfig();
}
