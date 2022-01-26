#pragma once

struct Config
{
	float smoothing;
	int fov;
	float max_distance;

	bool glow_active;
	bool glow_walls;

	int glow_type;//1,5,7

	float color[4] = { 200.f,0.f,0.f,0.f };

	Config()
	{
		smoothing = 2.f;
		fov = 65.f;
		max_distance = 300.f;
		glow_active = true;
		glow_walls = true;
		glow_type = 0;
	}
};