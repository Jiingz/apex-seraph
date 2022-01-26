#include <loader/views/app_view.h>
#include <loader/app/module_loader.h>
#include <loader/app/locator.h>

#include <wrl/client.h>

#include <IconsMaterialDesign.h>
#include <imgui.h>

#include <filesystem>
#include <iostream>
#include "../app/application.h"
#include "D:\dev\cryptolens-cpp-1.2.0\examples\VisualStudio\Example_Activate\license_check.h"
#include <core/core.h>

namespace ImGui {
	void CustomStyle() {
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(8.00f, 8.00f);
		style.FramePadding = ImVec2(4.00f, 3.00f);
		style.CellPadding = ImVec2(4.00f, 2.00f);
		style.ItemSpacing = ImVec2(8.00f, 4.00f);
		style.ItemInnerSpacing = ImVec2(4.00f, 4.00f);
		style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style.IndentSpacing = 21.00f;
		style.ScrollbarSize = 14.00f;
		style.GrabMinSize = 10.00f;
		style.WindowBorderSize = 1.00f;
		style.ChildBorderSize = 1.00f;
		style.PopupBorderSize = 1.00f;
		style.FrameBorderSize = 0.00f;
		style.TabBorderSize = 0.00f;
		style.WindowRounding = 5.00f;
		style.ChildRounding = 0.00f;
		style.FrameRounding = 5.00f;
		style.PopupRounding = 0.00f;
		style.ScrollbarRounding = 9.00f;
		style.GrabRounding = 5.00f;
		style.LogSliderDeadzone = 4.00f;
		style.TabRounding = 4.00f;
		style.WindowTitleAlign = ImVec2(0.00f, 0.50f);
		style.WindowMenuButtonPosition = 0;
		style.ColorButtonPosition = 1;
		style.ButtonTextAlign = ImVec2(0.50f, 0.50f);
		style.SelectableTextAlign = ImVec2(0.00f, 0.00f);
		style.DisplaySafeAreaPadding = ImVec2(3.00f, 3.00f);
	}
}


namespace ImGui {
	void CustomColor() {
		auto* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.48f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.28f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.39f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.59f, 0.00f, 0.00f, 0.77f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.47f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_Header] = ImVec4(0.28f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.47f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.28f, 0.00f, 0.00f, 0.50f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
}







using namespace Microsoft::WRL;
using namespace loader;


AppView::AppView()
	: license_valid_(false)
{
}

void AppView::Initialize()
{
	auto font_resources = Locator::GetFonts();

	md_main_font_ = &font_resources->Get(TEXT("md-main-font"));
	md_material_font_ = &font_resources->Get(TEXT("md-material-font"));
	lg_material_font_ = &font_resources->Get(TEXT("lg-material-font"));
}

const char* list[] = { "Outline","Visible","Bloodhound" };
void AppView::Render()
{
	if (!license_valid_)
		return;

	RunCore();

	ImGui::SetWindowSize({ 750.f,327.f });

	ImGui::CustomStyle();
	ImGui::CustomColor();

	ImGui::SetCursorPos({ 13.f,53.f });
	ImGui::PushItemWidth(546.000000);
	ImGui::SliderFloat("##Smoothing", &GetConfig()->smoothing, 0.f, 15.f);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 13.f,145.f });
	ImGui::PushItemWidth(546.000000);
	ImGui::SliderFloat("##MaxDistance", &GetConfig()->max_distance, 50, 300);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 13.f,101.f });
	ImGui::PushItemWidth(546.000000);
	ImGui::SliderInt("##FOV", &GetConfig()->fov, 0, 100);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 13.f,180.f });
	ImGui::Checkbox("Glow", &GetConfig()->glow_active);
	ImGui::SetCursorPos({ 83.f,180.f });
	ImGui::Checkbox("Glow through walls", &GetConfig()->glow_walls);
	ImGui::SetCursorPos({ 13.f,220.f });
	ImGui::PushItemWidth(120.000000);
	ImGui::ListBox("GlowType", &GetConfig()->glow_type, list, IM_ARRAYSIZE(list));
	//ImGui::SliderInt("Glow Type    ", &t, 1, 3);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 190.f,210.f });
	ImGui::PushItemWidth(110.000000);
	ImGui::ColorPicker3("Glow Color", GetConfig()->color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoAlpha);
	GetConfig()->color[4] = 1.f;

}
char buffer[256];
void loader::AppView::RenderLogin()
{
	if (license_valid_)
		return;

	ImGui::CustomColor();
	ImGui::CustomStyle();

	//	ImGui::SetNextWindowSize({ 521.f,118.f });

	ImGui::SetCursorPos({ 26.f,35.f });
	ImGui::PushItemWidth(444.000000);
	ImGui::InputText("##License", buffer, 255);
	ImGui::PopItemWidth();
	ImGui::SetCursorPos({ 213.f,76.f });
	if (ImGui::Button("Login", { 64.f,19.f }))
	{
		Product apex;
		apex.module_base = "sLHxRffwJGQBreHuM/BOw5eQQvr9P+xQ9NL+5mHmV7p0ZEn4kr6nl/ChoT5nldxg/XzyA2z81mW77qdFsraZ0aO/5K+bMRDAVgnLdNO/BD7ltdq/byTHWwy3xyUE9EF8q8SMNqRFunr5LP0CX+cL/DzQ8HKABK6xxL6+GAWWjqjhtibSaGelV3r++Izzr4mhwFQCh530ERjm3Cx5nOOQmejzr/SSprouNbDxUdTwY8ThRngZYj0chzgJ9repiYN+X/DX+uzhWRMatDQFVL2XhztK/TqgF6viorbMueNoEgGqOWZqFXu0WVqmyl5WV1Bugs6Yze32ENgNJBKei4fxPQ==";
		apex.product_id = 13790;
		apex.access_token = "WyIxMTE2NjQ3NSIsIm1aeFNEOFNnTzhXOFVJQnM2S2ZSRzVLTytPMFhQUzhISUF4eGt0QkciXQ==";
		apex.exponent_base = "AQAB";

		if (CheckLicense(apex, buffer))
		{
			license_valid_ = true;
			InitializeCore();
			SDL_SetWindowSize(loader::Application::GetWindow(), 624.f, 327.f);
		}
		else
			memset(&buffer, 0, sizeof(buffer));

	}
}

