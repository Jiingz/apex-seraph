#include "glow.h"
#include <core/service_locator.h>

void features::Glow::GlowPlayer(uintptr_t entity, game::structs::GlowContext glow_context, game::structs::Color color)
{
	driver::Write<int>(entity + offsets::engine::glow_context, glow_context);
	driver::Write<int>(entity + offsets::engine::glow_through_walls, 2); //through walls //2 = enabled, 5 = disabled
	//driver::Write<game::structs::GlowMode>(entity + offsets::engine::m_highlightFunctionBits, glow_mode);
	driver::Write<game::structs::Color>(entity + offsets::engine::glow_color, color);
}

void features::Glow::GlowItem(uintptr_t entity)
{
}

void features::Glow::GlowOnFrame()
{
	for (uintptr_t i = 0; i <= 100; i++)
	{
		uint64_t entity = driver::Read<uint64_t>((driver::GetBaseAddress() + offsets::engine::cl_entitylist) + (i << 5));
		if (entity == 0 || entity <= 0x1000) continue;

		if (game::Entity::GetTeamID(entity) == game::Entity::GetTeamID(core::ServiceLocator::GetWorld()->GetPlayer()))
			continue;

		if (game::Entity::IsPlayer(entity))
		{
			//if visible glow green, if not visible glow red
			if (game::Entity::IsVisible(entity, i))
				Glow::GlowPlayer(entity,
					game::structs::GlowContext::Outline,
					game::structs::Color{ 0.f,15.f,0.f });
			else
				Glow::GlowPlayer(
					entity,
					game::structs::GlowContext::Outline,
					game::structs::Color{ 15.f,0.f,0.f });
		}
	}
}
