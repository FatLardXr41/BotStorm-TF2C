#include "Misc.h"
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <cstdlib>
#include <chrono>
#include <random>
#include "XorStr.h"
#include "Menu.h"
/*
*	probably only gonna use this shit for spamming cuz im a cunt l0l
*/

CMisc gMisc;


std::string GetSpam(const int nIndex) { // PASTA
	std::string str;

	switch (nIndex)
	{
	case 0: str = XorStr("say Nigger").str(); break;
	case 1: str = XorStr("say Nigger").str(); break;
	}

	return str;
}

void CMisc::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	if (gCheatMenu.misc_chatspam) 
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		// this is not pasted xde
		if (flCurTime > flNextSend) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 1);

			int randomIndex = dis(gen);

			gInts.Engine->ClientCmd_Unrestricted(GetSpam(randomIndex).c_str());
			flNextSend = (flCurTime + 8.0f); 
		}
	}
	if (gCheatMenu.misc_speedcrouch && !(pCmd->buttons & IN_ATTACK) && (pCmd->buttons & IN_DUCK)) // this doesnt work but whatever lol
	{
		Vector vLocalAngles = pCmd->viewangles;
		float speed = pCmd->forwardmove;
		if (fabs(speed) > 0.0f) {
			pCmd->forwardmove = -speed;
			pCmd->sidemove = 0.0f;
			pCmd->viewangles.y = vLocalAngles.y;
			pCmd->viewangles.y -= 180.0f;
			if (pCmd->viewangles.y < -180.0f) pCmd->viewangles.y += 360.0f;
			pCmd->viewangles.z = 90.0f;
		}
	}
}