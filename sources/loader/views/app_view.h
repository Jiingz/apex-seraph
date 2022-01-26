#pragma once

#include <ShlObj_core.h>
#include <ShObjIdl.h>
#include <imgui.h>

#include <loader/app/resource_holder.h>

#include <vector>

namespace loader
{
	class AppView
	{
	public:
		AppView();

		void Initialize();

		void Render();
		void RenderLogin();

	private:
		bool license_valid_;

		ImFont* md_main_font_;
		ImFont* md_material_font_;
		ImFont* lg_material_font_;
	};
}
