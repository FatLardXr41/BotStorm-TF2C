#include "SDK.h"
#include "Followbot.h"
#include "CDrawManager.h"
#include "Vector.h"
#include "Menu.h"
//This "followBot" is trash but it kinda works I guess.

CFollow gFollow;
Vector scn;

std::pair<float, float> ComputeMove(const Vector& a, const Vector& b, CUserCmd* pCommand)
{
	Vector diff = (b - a);
	if (diff.Length() == 0)
		return { 0, 0 };
	const float x = diff.x;
	const float y = diff.y;
	Vector vsilent(x, y, 0);
	float speed = sqrt(vsilent.x * vsilent.x + vsilent.y * vsilent.y);
	Vector ang;
	VectorAngles(vsilent, ang);
	float yaw = DEG2RAD(ang.y - pCommand->viewangles.y);
	return { cos(yaw) * 450, -sin(yaw) * 450 };
}

void MoveTo(const Vector& vector, CBaseEntity* pLocal, CUserCmd* pCommand)
{
	auto result = ComputeMove(pLocal->GetAbsOrigin(), vector, pCommand);
	pCommand->forwardmove = result.first;
	pCommand->sidemove = result.second;
}

void CFollow::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!gCheatMenu.followbot_enabled)
			return;

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;


		if (pEntity->GetLifeState() != LIFE_ALIVE)
			continue;

		player_info_t pInfo;
		if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
			continue;

		if (!gCheatMenu.PlayerMode_followbot[i]) // mark bots s ignore so they dont follow each other.. 
			continue;

		//if (pInfo.name != "total engineer death")
	//		continue; // i know i shouldnt check the name, but whatever, this is temporary.
		if (pLocal->GetVecOrigin().DistTo(pEntity->GetVecOrigin()) > (float)gCheatMenu.followbot_val2) // 3400.0f
			continue;

		if (pLocal->GetVecOrigin().DistTo(pEntity->GetVecOrigin()) > (float)gCheatMenu.followbot_val1) // 175
			continue;


		MoveTo(pEntity->GetVecOrigin(), pLocal, pCommand);

	}
}