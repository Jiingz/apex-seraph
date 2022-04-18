#include <core/hardware/serial_controller.h>
#include <core/libs/ArduSerial.h>
#include <core/libs/vector.h>
#include <core/globals.h>
#include <iostream>

using namespace hardware;

void hardware::SerialController::SendAimCommand(float x, float y, float smoothing)
{
	Vector2 aim_pos = { x,y };

	aim_pos.X -= globals::cross_hair.X;
	aim_pos.X /= smoothing;

	aim_pos.Y -= globals::cross_hair.Y;
	aim_pos.Y /= smoothing;
	this->aim_pos_[0] = aim_pos.X;
	this->aim_pos_[1] = aim_pos.Y;
	Serial3.write(this->aim_pos_, sizeof(this->aim_pos_));
}
