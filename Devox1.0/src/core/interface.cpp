#include "interface.h"


#include <stdexcept>
#include <format>

namespace interfaces {
	template <class T>
	T* capture_interface(const char* module_name, const char* interface_name) {
		const HMODULE module_handle = GetModuleHandleA(module_name);
		if (!module_handle) {
			throw std::runtime_error(std::format("Module nem található: {}", module_name));
		}


		using create_interface_fn = T * (*)(const char*, int*);
		const auto create_interface = 
			reinterpret_cast<create_interface_fn>(GetProcAddress(module_handle, "CreateInterface"));
		if (create_interface == nullptr) {
			throw std::runtime_error(std::format("CreateInterface nem található: {} in {}", interface_name, module_name));
		}
		T* interface_ptr = create_interface(interface_name, nullptr);
		if (interface_ptr == nullptr) {
			throw std::runtime_error(std::format("Interface nem található: {} in {}", interface_name, module_name));
		}
		return interface_ptr;
	}

	static void create_d3d11_resources() {
		{
			std::uint8_t* address = sdk::find_pattern("rendersystemdx11.dll", "48 89 2D ? ? ? ? 48 C7 05");

			dx11_csereinput = **reinterpret_cast<sdk::interface_dx11_csereinput***>(sdk::resolve_absolute_rip_address(address, 3, 7));

			if (dx11_csereinput == nullptr) {
				throw std::runtime_error("Nem található cs2_dx11_csereinput");
			}
		}

		if (dx11_csereinput->swap_chain == nullptr) {
			throw std::runtime_error("Swap chain nullptr");
		}


		IDXGISwapChain* swap_chain = dx11_csereinput->swap_chain;


		if (FAILED(swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&d3d11_device)))) {
			throw std::runtime_error("Nem sikerült lekérni a D3D11 Device-t");
		}

		d3d11_device->GetImmediateContext(&d3d11_device_context);

		if (d3d11_device == nullptr) {
			throw std::runtime_error("D3D11 Device nullptr");
		}

		{
			ID3D11Texture2D* back_buffer = nullptr;
			if (FAILED(swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)))) {
				throw std::runtime_error("Nem sikerült lekérni a back buffer-t");
			}

			if (!back_buffer) {
				throw std::runtime_error("Back buffer nullptr");
			}

			if (FAILED(d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &d3d11_render_target_view))) {
				back_buffer->Release();
				throw std::runtime_error("Nem sikerült létrehozni a render target view-t");
			}

			back_buffer->Release();
			if (d3d11_render_target_view == nullptr) {
				throw std::runtime_error("Render target view nullptr");
			}
		}

		{

		DXGI_SWAP_CHAIN_DESC swap_chain_desc;
		if (FAILED(swap_chain->GetDesc(&swap_chain_desc))) {
			throw std::runtime_error("Nem sikerült lekérni a swap chain leíróját");
		}

		hwnd = swap_chain_desc.OutputWindow;

		if (hwnd == nullptr) {
			throw std::runtime_error("HWND nullptr");
		}
	}
}
	 

	void create() {
		{
			std::uint8_t* address = sdk::find_pattern("client.dll", "48 8B 0D ? ? ? ? 4C 8D 8F ? ? ? ? 45 33 F6");

			cs2_input = *reinterpret_cast<sdk::interface_cs2_input**>(sdk::resolve_absolute_rip_address(address, 3, 7));

                if (cs2_input == nullptr) {
				throw std::runtime_error("Nem található cs2_input interface");
			}
		}


		input_rendszer = capture_interface<sdk::interface_input_rendszer>("inputsystem.dll",
			                                                              "InputSystemVersion001");
	
		create_d3d11_resources();
	}



	void destroy() {
		destroy_render_target();

		if (d3d11_device_context != nullptr) {
			d3d11_device_context->Release();
			d3d11_device_context = nullptr;
		}

		if (d3d11_device != nullptr) {
			d3d11_device->Release();
			d3d11_device = nullptr;
		}


		dx11_csereinput = nullptr;
		cs2_input = nullptr;
		input_rendszer = nullptr;
		hwnd = nullptr;

	}

	void create_render_target() {
		if (!d3d11_device || !dx11_csereinput || !dx11_csereinput->swap_chain) {
			throw std::runtime_error("Nem sikerült létrehozni a render target view-t");
			return;
		}

		// Use ID3D11Texture2D for D3D11 swap chain buffer
		ID3D11Texture2D* back_buffer = nullptr;
		if (FAILED(dx11_csereinput->swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer)))) {
			throw std::runtime_error("Nem sikerült lekérni a back buffer-t");
		}

		if (back_buffer == nullptr) {
			throw std::runtime_error("Back buffer nullptr");
		}

		if (FAILED(d3d11_device->CreateRenderTargetView(back_buffer, nullptr, &d3d11_render_target_view))) {
			back_buffer->Release();
			throw std::runtime_error("Nem sikerült létrehozni a render target view-t");
		}

		back_buffer->Release();

		if (d3d11_render_target_view == nullptr) {
			throw std::runtime_error("Render target view nullptr");
		}
	}

	void destroy_render_target() {
		if (d3d11_render_target_view != nullptr) {
			d3d11_render_target_view->Release();
			d3d11_render_target_view = nullptr;
		}
	}
}