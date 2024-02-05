#include "HvH.h"


/*
*   Pasted From NaCl.
*   Very Simple Anti-Aim System i guess
*   23-01-2023
*/



CHvH gHvH;

void movementfix(CUserCmd* pCmd, Vector vOldAngles, float fOldSidemove, float fOldForward)
{
	int e = rand() % 3;

	Vector curAngs = pCmd->viewangles;
	float fRot = 90;
	//Vector nwANG = Vector(-89, curAngs.y + fRot, 0);
	//pCmd->viewangles = nwANG;

	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

float RandFloatRange(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}


void CHvH::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	CUserCmd* pCmd = pCommand;
	Vector vOldAngles = pCommand->viewangles;
	float fOldForward = pCommand->forwardmove;
	float fOldSidemove = pCommand->sidemove;
	Vector/*&*/ angles = pCommand->viewangles;
	static bool flip = false;
	bool clamp = false;

	//Yea yea, use case instead, I don't care enough to actually change it right now, I'm only doing this update cuz I'm a bored.

	if (!gCheatMenu.hvh_enable)
		return;

	if (pCommand->buttons & IN_ATTACK)
		return;

	//Pitch Anti-Aims
	if (gCheatMenu.hvh_pitch > 0)
	{
		if (gCheatMenu.hvh_pitch == 1)//Fake Up
			angles.x = -271.0;
		if (gCheatMenu.hvh_pitch == 2)//Up
			angles.x = -89.0f;
		if (gCheatMenu.hvh_pitch == 3)//Fake Down
			angles.x = 271.0;
		if (gCheatMenu.hvh_pitch == 4)//Down
			angles.x = 89.0f;
	}

	//Yaw Anti-Aims
	if (gCheatMenu.hvh_yaw)
	{
		if (gCheatMenu.hvh_yaw == 1)//Right
			angles.y -= 90.0f;


		if (gCheatMenu.hvh_yaw == 2)//Left
			angles.y += 90.0f;

		if (gCheatMenu.hvh_yaw == 3)//Back
			angles.y -= 180;

		if (gCheatMenu.hvh_yaw == 4)//Emotion
			angles.y = (float)-89.99985719438652715;

		if (gCheatMenu.hvh_yaw == 5)//Random
			angles.y = RandFloatRange(-180.0f, 180.0f);
		/*
		if (gCheatMenu.hvh_yaw == 6) //Fake Sideways Right
		{
			if (*g.sendpacket)
				angles.y += 90.0f;
			else
				angles.y += -90.0f;
		}

		if (gCheatMenu.hvh_yaw == 7) //Fake Sideways Left
		{
			if (*g.sendpacket)
				angles.y += -90.0f;
			else
				angles.y += 90.0f;
		}

		if (gCheatMenu.hvh_yaw == 8) //Fake Right
		{
			if (*g.sendpacket) angles.y += -90.0f;
			else angles.y += 0.0f;
		}

		if (gCheatMenu.hvh_yaw == 9)//Half Backwards Left
		{
			if (*g.sendpacket)
				angles.y += 135.0f;
			else
				angles.y += -135.0f;
		}

		if (gCheatMenu.hvh_yaw == 10)//Half Backwards Right
		{
			if (*g.sendpacket)
				angles.y += -135.0f;
			else
				angles.y += 135.0f;
		}

		if (gCheatMenu.hvh_yaw == 11) //Fake Left
		{
			if (*g.sendpacket)
				angles.y += 90.0f;
			else
				angles.y += 0.0f;
		}
		*/ // i dont have g.sendpacket working yet.
	}

	//	if (clamp) fClampAngle(angles);

	//Util->SilentMovementFix(pCommand, angles);
	pCommand->viewangles = angles;
	movementfix(pCmd, vOldAngles, fOldSidemove, fOldForward);
}