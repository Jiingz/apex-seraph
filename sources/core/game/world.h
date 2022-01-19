#pragma once
#include <core/game/entity.h>
#include <core/game/structs.h>

namespace game
{
	class World
	{
	public:

		World();

		uintptr_t GetPlayer() const;
		std::vector<uintptr_t> GetEntities() const;
		bool WorldToScreen(Vector3 world, Vector2& screen) const;

	private:
		uintptr_t player_;
		std::vector<uintptr_t> entities_;
	};
}

