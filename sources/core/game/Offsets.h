#pragma once

namespace offsets
{
	namespace engine
	{
		constexpr auto cl_entitylist = 0x1972ea8;

		constexpr auto m_highlightFunctionBits = 0x02c0 + 4; // m_highlightFunctionBits + 4
		constexpr auto glow_context = 0x3c8; //m_highlightFunctionBits + 0x4
		constexpr auto glow_through_walls = 0x3d0; //2 = enabled, 5 = disabled
		constexpr auto glow_color = 0x1d0; // m_highlightParams + 0x18

		constexpr auto local_player = 0x1d22708;
		constexpr auto view_matrix = 0x1b3bd0; //gane dumper website gives some other shit here.
		constexpr auto view_render = 0x7512c88;
	}


	namespace cai_base_npc
	{
		constexpr auto m_iHealth = 0x438;
		constexpr auto m_iMaxHealth = 0x578;
		constexpr auto m_bleedoutState = 0x2718;
		constexpr auto m_lifeState = 0x798;
		constexpr auto m_fIsSprinting = 0x29d4; // int
		constexpr auto m_iTeamNum = 0x448;
		constexpr auto m_iName = 0x589;
		constexpr auto m_iSignifierName = 0x0580;
		constexpr auto m_customScriptInt = 0x1698;
		constexpr auto m_visible_time = 0x1af4; //float
		constexpr auto m_latestPrimaryWeapons = 0x1a8c;
		constexpr auto m_bones = 0xF38; // m_bConstrainBetweenEndpoints || m_nForceBone + 0x48
		constexpr auto m_bZooming = 0x1c31; //bool
		constexpr auto m_localAngles = 0x490; //Vector3
		constexpr auto camer_origin = 0x014C; //Vector3
		constexpr auto camera_angles = 0x1f20; //Vector3 CPlayer!camera_origin
		constexpr auto m_view_angles = 0x257c - 0x14; //m_ammoPoolCapacity - 0x14
		constexpr auto m_breath_angles = m_view_angles - 0x10; //Vector3
		constexpr auto m_iObserverMode = 0x33dc;
		constexpr auto m_hObserverTarget = 0x33e8;
		constexpr auto m_vecPunchWeapon_Angle = 0x2480; //Vector3 // m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
		constexpr auto m_vecAbsVelocity = 0x0140;
	}

	namespace weapon
	{
		constexpr auto m_flProjectileSpeed = 0x1f18;// 0x257c;
		constexpr auto bullet_gravity = m_flProjectileSpeed + 0x8;
	}
};