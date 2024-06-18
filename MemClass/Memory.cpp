#include "Memory.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

class Memory

{

private:

	DWORD dwProcessId;
	HANDLE hProc;
	LPCSTR windowName;

public:

	Memory(LPCSTR windowName)
	{
		this->windowName = windowName;
		HWND hWindow = FindWindowA(NULL, windowName);
		GetWindowThreadProcessId(hWindow, &this->dwProcessId);
		this->hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->dwProcessId);
	}

	template <typename T>

	T ReadMemory(uintptr_t addressToRead)
	{
		T value;
		ReadProcessMemory(this->hProc, (LPCVOID)addressToRead, &value, sizeof(value), NULL);
		return value;
	}

	template <typename T>

	void WriteMemory(uintptr_t addressToWrite, T desiredValue)
	{
		WriteProcessMemory(this->hProc, (LPVOID)addressToWrite, &desiredValue, sizeof(desiredValue), NULL);
		return;
	}

	DWORD GetProcessId(LPCWSTR processName)
	{
		this->dwProcessId = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwProcessId);

		if (hSnap != INVALID_HANDLE_VALUE)
		{

			PROCESSENTRY32W entry;
			entry.dwSize = sizeof(entry);

			if (Process32FirstW(hSnap, &entry))
			{

				do
				{

					if (!_wcsicmp(entry.szExeFile, processName))
					{

						dwProcessId = entry.th32ProcessID;
						break;

					}

				} while (Process32NextW(hSnap, &entry));
			}
		}

		CloseHandle(hSnap);
		return dwProcessId;

	}

	uintptr_t GetModuleBase(LPCWSTR moduleName)
	{
		uintptr_t moduleBase = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId); 

		if (hSnap != INVALID_HANDLE_VALUE)
		{

			MODULEENTRY32W entry;
			entry.dwSize = sizeof(entry);

			if (Module32FirstW(hSnap, &entry))
			{

				do

				{

					if (!_wcsicmp(entry.szModule, moduleName))
					{

						moduleBase = (uintptr_t)entry.modBaseAddr;
						break;

					}

				} while (Module32NextW(hSnap, &entry));
			}
		}

		CloseHandle(hSnap);
		return moduleBase;

	}
};
