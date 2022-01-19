#pragma once
#include <core/game/Offsets.h>
#include <core/driver/driver.h>
#include <core/game/structs.h>
#include <core/libs/vector.h>
#include <vector>
#include <string>

#define STR_MERGE_IMPL(x, y)				x##y
#define STR_MERGE(x,y)						STR_MERGE_IMPL(x,y)
#define MAKE_PAD(size)						BYTE STR_MERGE(pad_, __COUNTER__) [ size ]
#define DEFINE_MEMBER_0(x, y)				x
#define DEFINE_MEMBER_N(x,offset)			struct { MAKE_PAD(offset); x; }

namespace game
{

	class Entity
	{
	public:

		static int GetHealth(uintptr_t entity);
		static int GetMaxHealth(uintptr_t entity);
		static int GetBleedState(uintptr_t entity);
		static int GetLifeState(uintptr_t entity);
		static int GetTeamID(uintptr_t entity);
		static float GetZoomFOV(uintptr_t entity);
		static uintptr_t GetBones(uintptr_t entity);

		static bool IsAlive(uintptr_t entity);
		static bool IsVisible(uintptr_t entity, int index);
		static bool IsSprinting(uintptr_t entity);
		static bool IsZooming(uintptr_t entity);
		static bool IsPlayer(uintptr_t entity);

		static std::string GetEntityType(uintptr_t entity);

		static Vector3 GetOrigin(uintptr_t entity);
		static Vector3 GetCameraAngle(uintptr_t entity);
		static Vector3 GetViewAngle(uintptr_t entity);
		static Vector3 GetLocalAngles(uintptr_t entity);
		static Vector3 GetBreathAngle(uintptr_t entity);
		static Vector3 GetAimPunch(uintptr_t entity);
		static Vector3 GetBonePos(uintptr_t entity, int bone_id);
		static Vector3 GetVelocity(uintptr_t entity);
	};
}