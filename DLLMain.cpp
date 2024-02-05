#include "SDK.h"
#include "Client.h"
#include "Panels.h"
#include "MinHook/MinHook.h"
#include "HookingShit.h"
#include <thread>
COffsets gOffsets;
CPlayerVariables gPlayerVars;
CInterfaces gInts;

DWORD WINAPI dwMainThread(LPVOID lpArguments)
{
	gHook::GetInterface<CHLClient*>(gInts.Client, "client.dll", "VClient017");
	//gUtil::GetInterface<CPrediction*>(gInts.Prediction, "client.dll", "VClientPrediction001");
	gHook::GetInterface<CEntList*>(gInts.EntList, "client.dll", "VClientEntityList003");
	gHook::GetInterface<EngineClient*>(gInts.Engine, "engine.dll", "VEngineClient013");
	gHook::GetInterface<ISurface*>(gInts.Surface, "vguimatsurface.dll", "VGUI_Surface030");
	gHook::GetInterface<IPanel*>(gInts.Panels, "vgui2.dll", "VGUI_Panel009");
	gHook::GetInterface<IEngineTrace*>(gInts.EngineTrace, "engine.dll", "EngineTraceClient003");
	gHook::GetInterface<IVModelInfo*>(gInts.ModelInfo, "engine.dll", "VModelInfoClient006");
	/*
	*					Hook shit yo
	*/

	gInts.ClientMode = **(ClientModeShared***)((*(DWORD**)gInts.Client)[10] + 7); // 0x5 for TF2, 7 for TF2C (i believe?)

	//gInts.globals = *reinterpret_cast<CGlobals**>(gSignatures.GetEngineSignature("A1 ? ? ? ? 8B 11 68") + 8);
	MH_Initialize(); 
	{
		MH_CreateHook(gHook::GetVirtual(gInts.ClientMode, gOffsets.iCreateMoveOffset), &Hooked_CreateMove, reinterpret_cast<void**>(&oCreateMove));
		MH_CreateHook(gHook::GetVirtual(gInts.Panels, gOffsets.iPaintTraverseOffset), &Hooked_PaintTraverse, reinterpret_cast<void**>(&oPaintTraverse));
	}
	MH_EnableHook(MH_ALL_HOOKS);
	gInts.Engine->ClientCmd_Unrestricted("echo waltuh.. put yo dick away waltuh..");

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));



	gInts.Engine->ClientCmd_Unrestricted("echo waltuh.. im uninjecting waltuh");
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	MH_DisableHook(MH_ALL_HOOKS);
	//CL_Move_Detour.Shutdown();
	MH_RemoveHook(MH_ALL_HOOKS);

	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, dwMainThread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}