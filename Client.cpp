#include "SDK.h"
#include "Client.h"
#include "Aimbot.h"
#include "Followbot.h"
#include "Client.h"
#include "HvH.h"
#include "Misc.h"
CreateMove_t oCreateMove;
//============================================================================================
bool __fastcall Hooked_CreateMove(PVOID ClientMode, void* edx, float input_sample_frametime, CUserCmd* pCommand)
{
	//If you want taunt slide, you will need to hook CHLClient::CreateMove and do it there. If you do it here, you'll just shimmy forward.
	//VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	//bool bReturn = hook.GetMethod<bool(__thiscall*)(PVOID, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.
	try
	{
		CBaseEntity* pBaseEntity = GetBaseEntity(me); //Grab the local player's entity pointer.

		if (pBaseEntity == NULL) //This should never happen, but never say never. 0xC0000005 is no laughing matter.
			return oCreateMove(ClientMode, edx, input_sample_frametime, pCommand);

		//Do your client hook stuff here. This function is called once per tick. For time-critical functions, run your code in PaintTraverse. For move specific functions, run them here.
		gAim.Run(pBaseEntity, pCommand);
		gFollow.Run(pBaseEntity, pCommand);
		gHvH.Run(pBaseEntity, pCommand);
		gMisc.Run(pBaseEntity, pCommand);
		oCreateMove(ClientMode, edx, input_sample_frametime, pCommand);
	}
	catch(...)
	{
		Log::Fatal("Failed Hooked_CreateMove");
	}
	return false; //If bReturn is true, CInput::CreateMove will call CEngine::SetViewAngles(pCommand->viewangles). If you want silent aim, return false, but make sure to call SetViewAngles manually.
}
//============================================================================================