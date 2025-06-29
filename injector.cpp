#define DLL_EXPORT
#include "injector.h"

extern "C"
{
	BOOL APIENTRY DllMain(HANDLE hModule, DWORD call_reason, LPVOID lpReserved)
	{
		switch (call_reason)
		{
		case DLL_THREAD_ATTACH:
			// triggered when a new thread is created in the process
			break;

		case DLL_THREAD_DETACH:
			// triggered when a thread exits cleanly
			break;

		case DLL_PROCESS_DETACH:
			// triggered when the dll is unloaded from the process
			break;

		case DLL_PROCESS_ATTACH:
			// triggered when the dll is loaded into a process
			MessageBoxA(nullptr, "DLL Injected!", "x(", MB_OK | MB_ICONINFORMATION);
			break;
		}

		return true; // return true to indicate successful execution
	}
}
