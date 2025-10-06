#include "../src/core/interface.h"
#include "../src/core/Hooks.h"
#include "../src/core/Menu.h"
#include <windows.h>
#include <exception>
#include <stdexcept>

DWORD WINAPI cheat_thread(LPVOID instance) {
	MessageBox(nullptr, L"Devox 1.0 injectalva! END az unload-hoz", L"Devox 1.0",MB_ICONHAND);

	try {
		interfaces::create();
		menu::create();
		hooks::create();
} catch (const std::exception& e) {
	    hooks::destroy();
		menu::destroy();
	    interfaces::destroy();
		MessageBoxA(nullptr, e.what(), "Hiba az interfacek betoltese kozben", MB_ICONERROR);
		FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(instance), 1);
}

	while (!(GetAsyncKeyState(VK_END) & 1)) {
		Sleep(100);
	}

	hooks::destroy();	
	menu::destroy();
	interfaces::destroy();

	MessageBoxA(nullptr, "Devox unloadolva!!", "Info", MB_ICONINFORMATION);

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(instance), 0);	
	return 0;
}

DWORD APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID) {

if (reason == DLL_PROCESS_ATTACH) {
			DisableThreadLibraryCalls(instance);

			HANDLE thread = CreateThread(nullptr, 0, cheat_thread, instance, 0, nullptr);	
			if (thread == INVALID_HANDLE_VALUE) {
				CloseHandle(thread);
				return FALSE;
			}
}

}
