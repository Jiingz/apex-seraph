#include "glow.h"
#include <core/service_locator.h>
#include <core/core.h>
#include <core/game/entity.h>

void features::Glow::GlowPlayer(uintptr_t entity, bool visible)
{
	game::structs::GlowContext glow_type = game::structs::GlowContext::Outline;

	switch (GetConfig()->glow_type)
	{
	case 0:
		glow_type = game::structs::GlowContext::Outline;
		break;
	case 1:
		glow_type = game::structs::GlowContext::Visible;
		break;
	case 2:
		glow_type = game::structs::GlowContext::BloodHunt;
		break;
	}

	driver::Write<int>(entity + offsets::engine::glow_context, glow_type);

	int glow_through_walls = GetConfig()->glow_walls ? 2 : 5;

	game::structs::Color color;
	//Draw in green if enemy visible
	if (visible)
	{
		color = { 0.f,200.f,0.f };
	}
	else
	{
		color = { GetConfig()->color[0],GetConfig()->color[1] ,GetConfig()->color[2] };
	}

	driver::Write<int>(entity + offsets::engine::glow_through_walls, glow_through_walls); //through walls //2 = enabled, 5 = disabled
	//driver::Write<game::structs::GlowMode>(entity + offsets::engine::m_highlightFunctionBits, glow_mode);
	driver::Write<game::structs::Color>(entity + offsets::engine::glow_color, color);
}

void features::Glow::GlowItem(uintptr_t entity)
{
}

void features::Glow::GlowOnFrame()
{
	if (!GetConfig()->glow_active)
		return;

	for (uintptr_t i = 0; i <= 100; i++)
	{
		uint64_t entity = driver::Read<uint64_t>((driver::GetBaseAddress() + offsets::engine::cl_entitylist) + (i << 5));
		if (entity == 0 || entity <= 0x1000) continue;

		if (game::Entity::GetTeamID(entity) == game::Entity::GetTeamID(core::ServiceLocator::GetWorld()->GetPlayer()))
			continue;

		if (game::Entity::IsPlayer(entity))
		{
			//if visible glow green, if not visible glow red
			Glow::GlowPlayer(entity, game::Entity::IsVisible(entity, i));
		}
	}
}
