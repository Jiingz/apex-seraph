#include <core/game/entity.h>
#include <core/utils.h>
#include <core/service_locator.h>

using namespace game;

int Entity::GetHealth(uintptr_t player)
{
	return driver::Read<int>(player + offsets::cai_base_npc::m_iHealth);
}

int Entity::GetMaxHealth(uintptr_t player)
{
	return driver::Read<int>(player + offsets::cai_base_npc::m_iMaxHealth);
}

int Entity::GetBleedState(uintptr_t player)
{
	return driver::Read<int>(player + offsets::cai_base_npc::m_bleedoutState);
}

int game::Entity::GetLifeState(uintptr_t player)
{
	return driver::Read<int>(player + offsets::cai_base_npc::m_lifeState);
}

int Entity::GetTeamID(uintptr_t player)
{
	return driver::Read<int>(player + offsets::cai_base_npc::m_iTeamNum);
}

float game::Entity::GetZoomFOV(uintptr_t player)
{
	return driver::Read<float>(player + 0x1708 + 0x00b8);
}

uintptr_t Entity::GetBones(uintptr_t player)
{
	return driver::Read<uintptr_t>(player + offsets::cai_base_npc::m_bones);
}

bool game::Entity::IsAlive(uintptr_t player)
{
	return GetBleedState(player) == 0 && GetLifeState(player) == 0;
}

bool game::Entity::IsVisible(uintptr_t player, int index)
{
	//if (GetTickCount64() >= (game::structs::lastVis.lastCheck[index] + 6)) // only check every 10ms else its gonna flicker
	//{
		float vis_time = driver::Read<float>(player + offsets::cai_base_npc::m_visible_time);
		game::structs::lastVis.last_state[index] = vis_time > 0.f && vis_time > game::structs::lastVis.last_vis_time[index];
		game::structs::lastVis.last_vis_time[index] = vis_time;
		//game::structs::lastVis.lastCheck[index] = GetTickCount64();
	//}
	return game::structs::lastVis.last_state[index];
}

bool Entity::IsSprinting(uintptr_t player)
{
	return driver::Read<uint64_t>(player + offsets::cai_base_npc::m_fIsSprinting) > 0;
}

bool game::Entity::IsZooming(uintptr_t player)
{
	return driver::Read<bool>(player + offsets::cai_base_npc::m_bZooming);
}

bool Entity::IsPlayer(uintptr_t player)
{
	return driver::Read<uintptr_t>(player + offsets::cai_base_npc::m_iName) == 125780153691248;
}

std::string game::Entity::GetEntityType(uintptr_t entity)
{
	return driver::Read<std::string>(entity + offsets::cai_base_npc::m_iSignifierName);
}

Vector3 game::Entity::GetCameraAngle(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::camera_angles);
}

Vector3 game::Entity::GetOrigin(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::camer_origin);
}

Vector3 Entity::GetViewAngle(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::m_view_angles);
}

Vector3 game::Entity::GetLocalAngles(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::m_localAngles);
}

Vector3 game::Entity::GetBreathAngle(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::m_breath_angles);
}

Vector3 game::Entity::GetAimPunch(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::m_vecPunchWeapon_Angle);
}

Vector3 game::Entity::GetBonePos(uintptr_t player, int bone_id)
{
	Vector3 pos = GetOrigin(player);
	uintptr_t bones = GetBones(player);
	Vector3 bone = {};
	UINT32 boneloc = (bone_id * 0x30);
	game::structs::Bone bo = {};
	bo = driver::Read<game::structs::Bone>(bones + boneloc);

	bone.X = bo.x + pos.X;
	bone.Y = bo.y + pos.Y;
	bone.Z = bo.z + pos.Z;

	return bone;
}

Vector3 game::Entity::GetVelocity(uintptr_t player)
{
	return driver::Read<Vector3>(player + offsets::cai_base_npc::m_vecAbsVelocity);
}
