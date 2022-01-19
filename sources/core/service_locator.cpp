#include <core/service_locator.h>

using namespace core;

std::unique_ptr<hardware::SerialController> ServiceLocator::s_serial_controller_;
std::unique_ptr<game::World> ServiceLocator::s_world_;

hardware::SerialController* ServiceLocator::GetSerialController()
{
	if (!s_serial_controller_)
	{
		s_serial_controller_ = std::make_unique<hardware::SerialController>();
	}

	return s_serial_controller_.get();
}

game::World* ServiceLocator::GetWorld()
{
	if (!s_world_)
	{
		s_world_ = std::make_unique<game::World>();
	}

	return s_world_.get();
}