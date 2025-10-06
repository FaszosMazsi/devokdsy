#include "Menu.h"

#include "globals.h"
#include "Hooks.h"
#include "interface.h"

#include "../Devox1.0/repos/imgui/imgui.h"
#include "../Devox1.0/repos/imgui/imgui_impl_dx11.h"
#include "../Devox1.0/repos/imgui/imgui_impl_win32.h"

#include <stdexcept>

static WNDPROC original_wndproc = nullptr;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hook_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (wParam == WM_KEYDOWN && lParam == VK_INSERT) {
		
		globals::menu_opened = !globals::menu_opened;
		//ImGui::GetIO().MouseDrawCursor = globals::menu_nyitva;

		hooks::original_set_relative_mouse_mode(
			interfaces::input_rendszer, globals::menu_opened ? false : globals::relative_mouse_mode);

	}

	if (globals::menu_opened) {
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
		return true;
	} else {
		return CallWindowProcA(original_wndproc, hwnd, msg, wParam, lParam);
	}
}


namespace menu {
	void create() {
		if (!interfaces::d3d11_device  || !interfaces::d3d11_device_context || !interfaces::hwnd) {
			throw std::runtime_error("D3D11 device vagy context nullptr");
		}

		original_wndproc = reinterpret_cast<WNDPROC>(
			SetWindowLongPtrA(interfaces::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hook_wndproc))
			);

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		if (!ImGui_ImplWin32_Init(interfaces::hwnd)) {
			throw std::runtime_error("Failed to get IDXGIFactory from IDXGIAdapter.");
		}
		if (!ImGui_ImplDX11_Init(interfaces::d3d11_device, interfaces::d3d11_device_context)) {
			throw std::runtime_error("Failed to initialize ImGui_ImplDX11.");
		}
	}

	void destroy() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		if (original_wndproc) {
			SetWindowLongPtrA(interfaces::hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original_wndproc));
			original_wndproc = nullptr;
		}
	}

	void render() {
		if (!globals::menu_opened){
		  return;
		}

		ImGui::Begin("Devox Menu", &globals::menu_opened);
		ImGui::Text("Hello, world!");
		ImGui::End();
	}
}