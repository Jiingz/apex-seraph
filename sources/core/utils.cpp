#include <core/utils.h>
#include <Windows.h>
#include <profileapi.h>
#include <sysinfoapi.h>

using namespace core;

uintptr_t Utils::GetMs()
{
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount64();
	}
}