#pragma once

#include <loader/app/resource_holder.h>

#include <imgui.h>


namespace loader
{
	class Locator
	{
	public:
		static void Finalize();

		static ResourceHolderNop<ImFont>* GetFonts();
	private:
		static std::unique_ptr<ResourceHolderNop<ImFont>> s_fonts_;
	};
}
