#include <core/hardware/serial_controller.h>
#include <core/libs/ArduSerial.h>
#include <core/libs/vector.h>
#include <core/globals.h>

using namespace hardware;

void NormalizeAngle(Vector2& Angle)
{
	while (Angle.X <= -M_PI)
		Angle.X += 2 * M_PI;
	while (Angle.X > M_PI)
		Angle.X -= 2 * M_PI;
	while (Angle.Y <= -M_PI)
		Angle.Y += 2 * M_PI;
	while (Angle.Y > M_PI)
		Angle.Y -= 2 * M_PI;
}

void SmoothAngle(Vector2& From, Vector2& To, float Percent)
{
	Vector2 VecDelta = From - To;
	NormalizeAngle(VecDelta);
	VecDelta.X *= Percent;
	VecDelta.Y *= Percent;
	To = From - VecDelta;
}

void hardware::SerialController::SendAimCommand(float x, float y, float smoothing)
{
	Vector2 aim_pos = { x,y };

	aim_pos.X -= globals::cross_hair.X;
	aim_pos.X /= smoothing;

	aim_pos.Y -= globals::cross_hair.Y;
	aim_pos.Y /= smoothing;

	this->aim_pos_[0] = aim_pos.X;
	this->aim_pos_[1] = aim_pos.Y;
	Serial5.write(this->aim_pos_, sizeof(this->aim_pos_));
}
