#pragma once

#include <windows.h>

#include <sdl.h>
#include <imgui.h>

#include <loader/views/app_view.h>


namespace loader
{
	class Application
	{
	public:
		Application();
		~Application();

		INT Run();

		void HandleEvents();
		void Render();
		static SDL_Window* GetWindow();

	private:
		void Initialize();
		void InitializeGraphics();
		void InitializeResources();
		void InitializeViews();
		void UpdateFormResolution(int x, int y);

	private:

		static SDL_Window* window_;
		SDL_GLContext context_;

		AppView app_view_;

		bool is_closed_;
	};
}
