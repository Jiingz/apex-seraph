#pragma once
#include <wtypes.h>

namespace game
{
	namespace structs
	{
		enum GlowContext
		{
			Outline = 1,
			Visible = 5,
			BloodHunt = 7,
		};

		//GeneralGlowMode, BorderGlowMode, BoderSize, TransparentLevel
		struct GlowMode
		{
			int8_t GeneralGlowMode;
			int8_t	BorderGlowMode;
			int8_t	BorderSize;
			int8_t	TransparentLevel;
		};

		struct Bone
		{
			BYTE pad[0xCC];
			float x;
			BYTE pad2[0xC];
			float y;
			BYTE pad3[0xC];
			float z;
		};

		struct ViewMatrix
		{
			float matrix[16];
		}static viewMatrix;

		struct visibleTime
		{
			float last_vis_time[100]; // last visible time in float
			bool last_state[100]; // last visibility state
			uintptr_t lastCheck[100]; // last visibility check time
		}static lastVis;

		struct Color
		{
			float red;
			float green;
			float blue;
		};
	}

}