#pragma once
#include "../memoria_utils.h"

class IDXGISwapChain;

namespace sdk {
	class interface_dx11_csereinput {
	private:
		padding<0x170> m_pad;

	public:
		IDXGISwapChain* swap_chain;
	};
}