#pragma once
#include <cstdint>
#include <core/game/Offsets.h>
#include <core/game/world.h>
#include <core/game/entity.h>

namespace features
{
	class Glow
	{
	public:
		static void GlowOnFrame();

	private:
		static void GlowPlayer(uintptr_t entity,bool visible);
		static void GlowItem(uintptr_t entity);
	};
}