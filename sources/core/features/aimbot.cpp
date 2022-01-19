#include <core/features/aimbot.h>
#include <core/service_locator.h>
#include <cfloat>
#include <iostream>
#include <core/globals.h>
#include <core/features/glow.h>
#include <math.h>

using namespace features;

Vector3 Aimbot::old_punch_value;

void features::Aimbot::PredictPosition(uintptr_t target, Vector3* bone_pos)
{
	uintptr_t weapon_id = driver::Read<uintptr_t>(core::ServiceLocator::GetWorld()->GetPlayer() + offsets::cai_base_npc::m_latestPrimaryWeapons) & 0xFFFF;
	uintptr_t current_weapon = driver::Read<uintptr_t>((driver::GetBaseAddress() + offsets::engine::cl_entitylist) + (weapon_id << 5));

	if (current_weapon != 0)
	{
		float bullet_speed = driver::Read<float>(current_weapon + offsets::weapon::m_flProjectileSpeed);
		float bullet_gravity = driver::Read<float>(current_weapon + offsets::weapon::bullet_gravity);

		if (bullet_speed > 1.f)
		{
			Vector3 muzzle = game::Entity::GetCameraAngle(core::ServiceLocator::GetWorld()->GetPlayer());

			if (bone_pos->DistanceTo(muzzle))
				return;

			float time = bone_pos->DistanceTo(muzzle) / bullet_speed;
			bone_pos->Z += (750.f * bullet_gravity * 0.5f) * (time * time);
			Vector3 vel_delta = (driver::Read<Vector3>(target + offsets::cai_base_npc::camer_origin - 0xC) * time);
			bone_pos->X += vel_delta.X;
			bone_pos->Y += vel_delta.Y;
			bone_pos->Z += vel_delta.Z;
			//bone_pos->Z += velDelta.Z + 100.f;//z轴+加法版
		//	bone_pos->Z += velDelta.Z - 100.f;//z轴-减法版
		}
	}
}

uintptr_t Aimbot::GetBestTarget()
{
	float old_dist = FLT_MAX;
	float new_dist = 0;
	uintptr_t target = NULL;

	int visible_index_ = 0; //index to check for visibility
	for (uintptr_t& entity : core::ServiceLocator::GetWorld()->GetEntities())
	{
		if (!game::Entity::IsAlive(entity))
			continue;

		bool visible = game::Entity::IsVisible(entity, visible_index_);
		visible_index_++;

		if (!visible) continue;

		if (game::Entity::GetTeamID(core::ServiceLocator::GetWorld()->GetPlayer()) == game::Entity::GetTeamID(entity))
			continue;

		Vector3 local_head = game::Entity::GetBonePos(core::ServiceLocator::GetWorld()->GetPlayer(), 8);

		Vector3 target_head = game::Entity::GetBonePos(entity, 8);

		Vector3 view_angle = game::Entity::GetViewAngle(core::ServiceLocator::GetWorld()->GetPlayer());

		/*	if (local_head.DistanceTo(target_head) / 32.62f > 200.f)
				continue;*/

		Vector2 target_head_screen;
		if (!core::ServiceLocator::GetWorld()->WorldToScreen(target_head, target_head_screen)) continue;
		new_dist = globals::cross_hair.DistanceTo(target_head_screen);

		if (new_dist < old_dist && new_dist <= 120.f/*FOV*/ /* max aimbot distance*/)
		{
			old_dist = new_dist;
			target = entity;
		}
	}
	return target;
}

Vector3 GetClosestBonePosition(uintptr_t target)
{
	Vector3 closest_bone = game::Entity::GetBonePos(target, 8);
	Vector3 view_angle = game::Entity::GetBonePos(core::ServiceLocator::GetWorld()->GetPlayer(), 8);
	for (size_t i = 0; i < 8; i++)
	{
		Vector3 bone = game::Entity::GetBonePos(target, i);

		if (bone.DistanceTo(view_angle) < closest_bone.DistanceTo(view_angle))
			closest_bone = bone;
	}
	return closest_bone;
}

void Aimbot::Execute()
{
	features::Glow::GlowOnFrame();

	uintptr_t local_player = core::ServiceLocator::GetWorld()->GetPlayer();

	if (!game::Entity::IsZooming(local_player)) // aimbot key
		return;

	if (!local_player)
		return;

	uintptr_t target = GetBestTarget();

	if (!target)
		return;

	Vector3 target_head = GetClosestBonePosition(target);//GetClosestBonePosition(target);//8是头，5是胸。8 is head,5 is chest 
	Vector3 local_head = game::Entity::GetBonePos(local_player, 8);
	PredictPosition(target, &target_head);

	Vector2 aim_pos;
	if (!core::ServiceLocator::GetWorld()->WorldToScreen(target_head, aim_pos))
		return;

	core::ServiceLocator::GetSerialController()->SendAimCommand(aim_pos.X, aim_pos.Y, 3.f);
}