#pragma once
#include <core/hardware/serial_controller.h>
#include <core/game/world.h>
#include <memory>

namespace core
{
	// Main, static object to get access to needed objects.
	class ServiceLocator
	{
	public:
		static hardware::SerialController* GetSerialController();
		static game::World* GetWorld();

	private:
		static std::unique_ptr<hardware::SerialController> s_serial_controller_;
		static std::unique_ptr<game::World> s_world_;
	};
}