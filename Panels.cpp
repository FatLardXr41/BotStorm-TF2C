#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Menu.h"
#include "CNetVars.h"

CScreenSize gScreenSize;
PaintTraverse_t oPaintTraverse;
//===================================================================================
void __fastcall Hooked_PaintTraverse( PVOID pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		//VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		//hook.GetMethod<void(__thiscall*)(PVOID, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiMatSystemTopPanel;

		static unsigned int vguiFocusOverlayPanel;

		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if (szName[0] == 'F' && szName[5] == 'O' && szName[12] == 'P')
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}
		
		oPaintTraverse(pPanels, edx, vguiPanel, forceRepaint, allowForce);

		if (vguiFocusOverlayPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			gCheatMenu.HandleControls(); 
			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}
		}
	}
	catch(...)
	{
		Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.
		gNetVars.Initialize(); // WOO IM RETARDED LOOOOOOOOOOOOOOOOOL
		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.
	}
	catch(...)
	{
		Log::Fatal("Failed Intro");
	}
}