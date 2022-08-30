#include <core/core.h>
#include <core/hardware/serial.h>
#include <core/service_locator.h>
#include <core/features/aimbot.h>
#include <core/features/glow.h>
#include <core/game/entity.h>

#include <iostream>
#include <string>
#include <thread>

std::shared_ptr<Config> config_;

void FunctionExecution()
{
	features::Aimbot::Execute();
	features::Glow::GlowOnFrame();

	if (GetAsyncKeyState(VK_F10))
	{
		exit(0);
	}

}

void RunCore()
{
	FunctionExecution();
}

Config* GetConfig()
{
	if (!config_)
	{
		config_ = std::make_shared<Config>();
	}
	return config_.get();
}

void InitializeCore()
{
	driver::Initialize();
	hardware::SerialProvider::Setup();
}
