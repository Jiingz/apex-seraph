#pragma once
#include <cstdint>
#include <core/game/entity.h>

namespace features
{
	class Aimbot
	{
	public:
		static void Execute();

	private:
		static void PredictPosition(uintptr_t target, Vector3 bone_pos);
		static uintptr_t GetBestTarget();

	private:
		static Vector3 old_punch_value_;
		static uintptr_t last_target_;
		static int  last_target_index_;
	};
}