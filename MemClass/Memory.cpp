#include "Memory.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>

class Memory

{

public:

	DWORD processID;
	HANDLE processHandle;
	LPCSTR windowName;

	Memory(LPCSTR windowName)

	{

		this->windowName = windowName;
		GetWindowThreadProcessId(FindWindowA(NULL, this->windowName), &this->processID);
		this->processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, this->processID);

	}

	template <class T>

	T ReadMemory(uintptr_t addressToRead)

	{

		T value;
		ReadProcessMemory(this->processHandle, (LPCVOID)addressToRead, &value, sizeof(value), NULL);
		return value;

	}

	template <class T>

	void WriteMemory(uintptr_t addressToWrite, T desiredValue)

	{

		WriteProcessMemory(this->processHandle, (LPVOID)addressToWrite, &desiredValue, sizeof(desiredValue), NULL);
		return;

	}

	DWORD GetProcessID(LPCWSTR processName)

	{
		this->processID = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

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

						processID = entry.th32ProcessID;
						break;

					}

				} while(Process32NextW(hSnap, &entry));
			}
		}

		CloseHandle(hSnap);
		return processID;

	}

	uintptr_t GetModuleBase(LPCWSTR moduleName)

	{

		uintptr_t moduleBase = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, this->processID);

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