#include <iostream>
#include <Windows.h>
#define DLL_EXPORT
#include "inlineHook.h"

CHAR original_bytes[6];
FARPROC hooked_addr;



HANDLE __stdcall new_func(
	LPCSTR lpFileName,
	DWORD dwDesiredAccess,
	DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD  dwCreationDisposition,
	DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile
) {

	//unhook function
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hooked_addr, original_bytes, 6, NULL);
	constexpr char msg[] = { "finished hooking function" };
	HANDLE file = CreateFileA(
		"..\\hooked.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		4,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	return file;
}

void log(const char msg[]) {
	HANDLE file = CreateFileA(
		"..\\log.txt",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		4,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (file == INVALID_HANDLE_VALUE) {
		std::cout << "nope\n";
	}
	std::cout << WriteFile(file, msg, 100, NULL, NULL) << '\n';
}

void set_up_hook(HANDLE mod) {
	HINSTANCE hLib;
	VOID* myFncAdrr;
	CHAR patch[6] = { 0 };

	hLib = LoadLibraryA("kernel32.dll");
	hooked_addr = GetProcAddress(hLib, "CreateFileA");

	ReadProcessMemory(GetCurrentProcess(), (LPCVOID)hooked_addr, original_bytes, 6, NULL);
	myFncAdrr = &new_func;

	memcpy_s(patch, 1, "\x68", 1);
	memcpy_s(patch + 1, 4, &myFncAdrr, 4);
	memcpy_s(patch + 5, 1, "\xC3", 1); 

	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hooked_addr, patch, 6, NULL);

	
}

BOOL APIENTRY DllMain(HANDLE hModule, 
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// A process is loading the DLL.
		set_up_hook(hModule);
	case DLL_THREAD_ATTACH:
		// A process is creating a new thread.
		break;
	case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;
	case DLL_PROCESS_DETACH:
		// A process unloads the DLL.
		break;
	}
	return TRUE;
}