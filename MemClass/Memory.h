#pragma once

#include <Windows.h>
#include <cstdint>

class Memory

{

Memory(LPCSTR windowName);

template <typename T>
T ReadMemory(uintptr_t addressToRead);

template <typename T>
void WriteMemory(uintptr_t addressToWrite, T desiredValue);

DWORD GetProcessId(LPCWSTR processName);

uintptr_t GetModuleBase(LPCWSTR moduleName);

};
