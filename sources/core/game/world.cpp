#include <core/game/world.h>
#include <core/globals.h>
#include <core/service_locator.h>
#include <core/game/entity.h>

using namespace game;

World::World()
{
	//this->player_ = driver::Read<uintptr_t>(driver::GetBaseAddress() + offsets::engine::local_player);
}

std::vector<uintptr_t> World::GetEntities() const
{
	std::vector<uintptr_t> vec;
	for (uintptr_t i = 0; i <= 100; i++)
	{
		uint64_t entity = driver::Read<uint64_t>((driver::GetBaseAddress() + offsets::engine::cl_entitylist) + (i << 5));
		if (entity == 0 || entity <= 0x1000 || !game::Entity::IsPlayer(entity)) continue;
		vec.push_back(entity);
	}
	return vec;
}

bool game::World::WorldToScreen(Vector3 world, Vector2& screen) const
{
	uintptr_t view_renderer = driver::Read<uintptr_t>(driver::GetBaseAddress() + offsets::engine::view_render);
	float* m_vMatrix = driver::Read<game::structs::ViewMatrix>(driver::Read<uintptr_t>(view_renderer + offsets::engine::view_matrix)).matrix;
	float w = m_vMatrix[12] * world.X + m_vMatrix[13] * world.Y + m_vMatrix[14] * world.Z + m_vMatrix[15];

	if (w < 0.01f) return false;

	screen.X = m_vMatrix[0] * world.X + m_vMatrix[1] * world.Y + m_vMatrix[2] * world.Z + m_vMatrix[3];
	screen.Y = m_vMatrix[4] * world.X + m_vMatrix[5] * world.Y + m_vMatrix[6] * world.Z + m_vMatrix[7];

	float invw = 1.0f / w;
	screen.X *= invw;
	screen.Y *= invw;

	//Hardcoded Resolution
	float x = 1920 / 2;
	float y = 1080 / 2;

	//Hardcoded Resolution
	x += 0.5 * screen.X * 1920 + 0.5;
	y -= 0.5 * screen.Y * 1080 + 0.5;

	screen.X = x;
	screen.Y = y;

	if (screen.X > 1920 || screen.X < 0 || screen.Y > 1080 || screen.Y < 0)
		return false;

	return true;
}

uintptr_t World::GetPlayer() const
{
	return driver::Read<uintptr_t>(driver::GetBaseAddress() + offsets::engine::local_player);
}