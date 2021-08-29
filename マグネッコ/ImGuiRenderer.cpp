#include "ImGuiRenderer.h"
#include "config.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "myd3d.h"
#include <string>
#include <strsafe.h>
#include "StringConvert.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool isDBCS = false;
BYTE befByte;

void ImGuiRenderer::Init(HWND hWnd) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(GetD3DDevice());

	io.IniFilename = NULL;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\meiryo.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	isDBCS = false;
}
void ImGuiRenderer::Uninit() {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}


LRESULT CALLBACK ImGuiRenderer::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	if (msg == WM_CHAR) {
		BYTE c = (BYTE)wParam;

		if (c > 128|| isDBCS)
		{
			if (!isDBCS) {
				isDBCS = true;
				befByte = c;
				return false;
			}
			else {
				short s = (befByte | c << 8);//２つの文字をあわせて漢字に変換
				char ch[sizeof(short) + 1];
				ZeroMemory(ch, sizeof(ch));
				StringCchCat(ch, sizeof(ch), (const char*)&s);


				auto wstr = StringConvert::MultiToWide(std::string(ch));
				wParam = wstr[0];
			}
		}

	}
	isDBCS = false;
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}
void ImGuiRenderer::DrawBegin() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}
void ImGuiRenderer::DrawEnd() {
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
