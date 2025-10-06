#pragma once

#include <d3d11.h>

#include "../sdk/interface/cs2_input.h"
#include "../sdk/interface/dx11_csereinput.h"
#include "../sdk/interface/input_rendszer.h"

namespace interfaces {
	void create();
	void destroy();

	inline sdk::interface_cs2_input* cs2_input = nullptr;
	inline sdk::interface_dx11_csereinput* dx11_csereinput = nullptr;
	inline sdk::interface_input_rendszer* input_rendszer = nullptr;


	inline ID3D11Device* d3d11_device = nullptr;
	inline ID3D11DeviceContext* d3d11_device_context = nullptr;
	inline ID3D11RenderTargetView* d3d11_render_target_view = nullptr;

	void create_render_target();
	void destroy_render_target();


	inline HWND hwnd = nullptr;
}