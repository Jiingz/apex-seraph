#pragma once

namespace hardware
{
	// Controller class for the hardware. Sends data to execute algorithms on the hardware.
	class SerialController
	{
	public:
		// Sends coordinates to the hardware to aim at the specified position.
		void SendAimCommand(float x, float y, float smoothing = 0.3f);

	private:
		char aim_pos_[2];
	};
}