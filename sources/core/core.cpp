
#include <core/hardware/serial.h>
#include <core/service_locator.h>
#include <core/features/aimbot.h>
#include <core/game/entity.h>

#include <iostream>
#include <string>

void FunctionExecution()
{
	features::Aimbot::Execute();

	if (GetAsyncKeyState(VK_F10))
	{
		exit(0);
	}

}


int main()
{
	driver::Initialize();
	hardware::SerialProvider::Setup();

	while (true)
		FunctionExecution();

	return 0;
}