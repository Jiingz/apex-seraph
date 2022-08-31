#include <loader/app/application.h>
#include <loader/app/locator.h>
#include <loader/app/module_loader.h>
#include <loader/app/resource_holder.h>

#include <loader/glad/glad.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <IconsMaterialDesign.h>

#include <filesystem>

using namespace loader;

SDL_Window* Application::window_ = nullptr;

Application::Application()
	: is_closed_(false)
{
	this->Initialize();
}


Application::~Application()
{
	ImGui_ImplSDL2_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();

	SDL_GL_DeleteContext(context_);

	SDL_DestroyWindow(window_);
	SDL_Quit();

	Locator::Finalize();

	ImGui::DestroyContext();
}


INT Application::Run()
{
	while (!is_closed_)
	{
		this->HandleEvents();
		this->Render();
	}

	return 0;
}

void Application::HandleEvents()
{
	ImGuiIO& io = ImGui::GetIO();
	SDL_Event event;

	INT mouse_wheel = 0;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			is_closed_ = true;
		}

		if (event.type == SDL_QUIT)
		{
			is_closed_ = true;
		}
		else if (event.type == SDL_WINDOWEVENT)
		{
			if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				io.DisplaySize.x = static_cast<FLOAT>(event.window.data1);
				io.DisplaySize.y = static_cast<FLOAT>(event.window.data2);
			}
		}
		else if (event.type == SDL_MOUSEWHEEL)
		{
			mouse_wheel = event.wheel.y;
		}
	}
}


void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_);
	ImGui::NewFrame();

	{
		//	ImGui::SetNextWindowPos(ImVec2(0, 0));
		//ImGui::SetNextWindowSize(ImVec2(250, 150));
		ImGui::Begin("MainWindow", 0,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove | ImGuiConfigFlags_None);

		app_view_.RenderLogin();
		app_view_.Render();

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window_);
}

SDL_Window* loader::Application::GetWindow()
{
	return Application::window_;
}


void Application::Initialize()
{
	// TODO We need error checks
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	this->InitializeGraphics();
	this->InitializeResources();
	this->InitializeViews();

}


void Application::InitializeGraphics()
{
	// Initialize SDL, OpenGL and ImGui
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	window_ = SDL_CreateWindow("Vault7", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 250, 150, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL);

	context_ = SDL_GL_CreateContext(window_);
	SDL_GL_MakeCurrent(window_, context_);

	SDL_GL_SetSwapInterval(1);

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(window_, context_);
	ImGui_ImplOpenGL3_Init("#version 130");
}


void Application::InitializeResources()
{
	auto& style = ImGui::GetStyle();

	style.ChildRounding = 5.0f;
	style.FrameRounding = 5.0f;

	static const ImWchar icons_ranges[] = { ICON_MIN_MD, ICON_MAX_MD, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = false;
	icons_config.PixelSnapH = true;

	auto fonts_storage = Locator::GetFonts();

	// Load and store fonts.
	ImGuiIO& io = ImGui::GetIO();

	auto md_main_font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Candara.ttf", 13.0f);
	auto md_material_font = io.Fonts->AddFontFromFileTTF("resources\\fonts\\MaterialIcons-Regular.ttf", 13.0f, &icons_config, icons_ranges);
	auto lg_material_font = io.Fonts->AddFontFromFileTTF("resources\\fonts\\MaterialIcons-Regular.ttf", 32.0f, &icons_config, icons_ranges);

	fonts_storage->Store(LPCWSTR("md-main-font"), ResourcePtrNop<ImFont>(md_main_font));
	fonts_storage->Store(LPCWSTR("md-material-font"), ResourcePtrNop<ImFont>(md_material_font));
	fonts_storage->Store(LPCWSTR("lg-material-font"), ResourcePtrNop<ImFont>(lg_material_font));

	io.Fonts->Build();
}


void Application::InitializeViews()
{
	app_view_.Initialize();
}

void loader::Application::UpdateFormResolution(int x, int y)
{
	SDL_SetWindowSize(this->window_, x, y);
}
