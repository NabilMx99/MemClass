#pragma once

#include <Windows.h>
#include <cstdint>

class Memory

{

Memory(LPCSTR windowName);

template <class T>
T ReadMemory(uintptr_t addressToRead);

template <class T>
void WriteMemory(uintptr_t addressToWrite, T desiredValue);

DWORD GetProcessID(LPCWSTR processName);

uintptr_t GetModuleBase(LPCWSTR moduleName);

};