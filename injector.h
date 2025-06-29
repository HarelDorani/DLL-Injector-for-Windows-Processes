#pragma once

#include <Windows.h>
// check if DLL_EXPORT is defined  
#if defined(DLL_EXPORT)// if defined, export the symbol for use in other modules
	#define DECLDIR __declspec(dllexport)
#else// if not defined, import the symbol from another module  
	#define DECLDIR __declspec(dllimport)
#endif


extern "C"
{
	BOOL APIENTRY DllMain(HANDLE hModule, DWORD call_reason, LPVOID lpReserved);
}