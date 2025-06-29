#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <cwchar>

typedef HMODULE(*PFUNC)(LPCSTR);
DWORD GetProcessId(LPCWSTR processName);

int main()
{
	//extract path
	CHAR cAbsPath[MAX_PATH];
	DWORD path_size = GetFullPathNameA("injector.dll", MAX_PATH, cAbsPath, nullptr) + 1;
	if (path_size == 0)
	{
		std::cout << "injector.dll is not in the local path" << std::endl;
		return -1;
	}

	//extract LoadLibrary from kernel32.dll.
	HMODULE hDLL = GetModuleHandleA("kernel32.dll");
	PFUNC pfLoadLibFunc = (PFUNC)GetProcAddress(hDLL, "LoadLibraryA");

	// get the Visual Studio process ID
	DWORD dwVSProcID = GetProcessId(L"devenv.exe");
	if (0 == dwVSProcID)
	{
		std::cout << "Visual Studio is not open" << std::endl;
		return -1;
	}

	
	//open VS
	HANDLE vsProc = OpenProcess(PROCESS_ALL_ACCESS, false, dwVSProcID);
	if (nullptr == vsProc)
	{
		std::cout << "failed opening proces" << std::endl;
		return -1;
	}

	// Allocate memory
	PVOID lpAllocMem = (PVOID)VirtualAllocEx(vsProc, nullptr, path_size, MEM_COMMIT, PAGE_READWRITE);
	if (nullptr == lpAllocMem)
	{
		CloseHandle(vsProc);
		std::cout << "VirtualAllocEx() Failed." << std::endl;
		return -1;
	}
	
	//write DLL file address
	if (0 == WriteProcessMemory(vsProc, lpAllocMem, cAbsPath, path_size, nullptr))
	{
		VirtualFreeEx(vsProc, lpAllocMem, 0, MEM_RELEASE);
		CloseHandle(vsProc);
		std::cout << "failed writing process to memory." << std::endl;
		return -1;
	}

	
	//create a remote thread 
	HANDLE remote_thread = CreateRemoteThread(vsProc, nullptr, 0, (LPTHREAD_START_ROUTINE)pfLoadLibFunc, lpAllocMem, 0, nullptr);

	// check if the thread was created successfully  
	if (nullptr == remote_thread)
	{
		VirtualFreeEx(vsProc, lpAllocMem, 0, MEM_RELEASE);//free memory

		CloseHandle(vsProc);//close handle

		std::cout << "failed creating remote" << std::endl;
		return -1;
	}

	//wait for remote thread
	WaitForSingleObject(remote_thread, INFINITE);

	CloseHandle(remote_thread);
	VirtualFreeEx(vsProc, lpAllocMem, 0, MEM_RELEASE);
	CloseHandle(vsProc);
	return 0;
}

DWORD GetProcessId(LPCWSTR name)
{
	PROCESSENTRY32W input;
	input.dwSize = sizeof(PROCESSENTRY32W);

	//create a snapshot of all running processes.
	HANDLE curr_processes = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (curr_processes == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	//search for the process id by its name.
	DWORD process_id = 0;
	if (Process32FirstW(curr_processes, &input))
	{
		do
		{
			//compare the current process name with the requested name.
			if (0 == std::wcscmp(input.szExeFile, name))
			{
				process_id = input.th32ProcessID;
				break;
			}
		} while (Process32NextW(curr_processes, &input));
	}

	//close the snapshot handle after searching.
	CloseHandle(curr_processes);
	return process_id;
}
