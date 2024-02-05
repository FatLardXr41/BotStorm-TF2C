#pragma once
#include "SDK.h"

namespace gHook
{

	template <typename T>
	inline void GetInterface(T& Interface, const char* szModuleName, const char* szInterfaceName)
	{
		if (HMODULE Module = GetModuleHandleA(szModuleName))
		{
			if (CreateInterfaceFn CreateInterface = (CreateInterfaceFn)GetProcAddress(Module, "CreateInterface"))
				Interface = reinterpret_cast<T>(CreateInterface(szInterfaceName, NULL));
			else
				MessageBoxA(NULL, szInterfaceName, "Failed to find interface!", NULL);

			return;
		}

		MessageBoxA(NULL, szInterfaceName, "Failed to find find module!", NULL);
	}

	inline void* GetVirtual(void* iClass, int iIndex)
	{
		return (void*)((DWORD)(*(PVOID**)iClass)[iIndex]);
	}

	template< typename Fn >
	inline Fn CallVirtual(void* iClass, size_t iIndex)
	{
		return reinterpret_cast<Fn>(GetVirtual(iClass, iIndex));
	}

};