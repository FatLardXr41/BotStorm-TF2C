#include "Aimbot.h"
#include "Menu.h"

CAimbot gAim;

#define VEC_ZERO ((Vector){ 0.f, 0.f, 0.f })
#define HITBOX_SET 0
#define MAXSTUDIOBONES 128

Vector calc_angle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw) * tmp);
	vector[2] = float(-sin(pitch));
}

float GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = calc_angle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}



float flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0f)
		return 1.0f;

	return m_fDistance;
}


void FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov) 
{
	float deltaView = cmd->viewangles.x - mov.angle.y;
	float f1;
	float f2;

	if (mov.angle.y < 0.f)
		f1 = 360.0f + mov.angle.y;
	else
		f1 = mov.angle.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * mov.fwd + cos(DEG2RAD(deltaView + 90.f)) * mov.sdm;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * mov.fwd + sin(DEG2RAD(deltaView + 90.f)) * mov.sdm;
}

void CAimbot::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	old_movement_t old_mov = old_movement_t();

	if (gCheatMenu.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCheatMenu.iAimbotIndex = -1;

	if (!gCheatMenu.aimbot_active)
		return;


	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	VectorAngles((vEntity - vLocal), vAngs);

	ClampAngle(vAngs);
	gCheatMenu.iAimbotIndex = pEntity->GetIndex();

	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCheatMenu.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCheatMenu.aimbot_silent) { // apply our movement fix if silent aim is enabled.
		FixMovementForUserCmd(pCommand, old_mov);
	}
	if (gCheatMenu.aimbot_autoscope && !pLocal->IsScoped() && pLocal->GetClassNum() == TF2_Sniper)
	{
		pCommand->buttons |= IN_ATTACK2;
	}

	if (gCheatMenu.aimbot_autoshoot)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (pLocal->GetClassNum() == TF2_Sniper)
		{
			if (gCheatMenu.aimbot_zoomedonly)
			{
				if (pLocal->IsScoped())
				{
					if (flCurTime > flNextSend)
					{
						pCommand->buttons |= IN_ATTACK;
						flNextSend = (flCurTime + 0.2f);
					}
				}
			}
		}
		if (pLocal->szGetClass() != "Sniper") // yey
		{
			pCommand->buttons |= IN_ATTACK;
		}
	}
}

int CAimbot::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCmd)
{
	int iBestTarget = -1;
	//this num could be smaller 
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (pEntity->GetLifeState() != LIFE_ALIVE ||
			pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->GetHitboxPosition(iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCheatMenu.PlayerMode[i]) // ignore the ppl that are marked as "ignore"
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_UberchargeFading ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;

		float flDistToTarget = (vLocal - vEntity).Length();
		float flFOV = GetFOV(pCmd->viewangles, vLocal, vEntity);
		if (flFOV < gCheatMenu.aimbot_fov)
		{
			if (gCheatMenu.PlayerMode[i] == 2)
				return i;
			//flDistToBest = flDistToTarget;
			//flDistToBest = flFOV;
			gCheatMenu.iAimbotIndex = i;
			iBestTarget = i;
		}
	}

	return iBestTarget;
}

bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	trace_t Trace;
	Ray_t Ray;			 // the future of variable naming
	CTraceFilter Filter;

	Filter.pSkip = pLocal;

	Ray.Init(vStart, vEnd);

	gInts.EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity);
}

int CAimbot::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCheatMenu.aimbot_hitbox)
	{
		if (pLocal->GetClassNum() == TF2_Sniper) // dumb but whatever, i cant be fucking bothered to implement CBaseCombatWeapon
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
	}
	else
	{
		iBestHitbox = gCheatMenu.aimbot_hitbox - 1;
	}

	if (gCheatMenu.aimbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(i)))
				return i;
		}
	}

	if (pEntity->GetHitboxPosition(iBestHitbox).IsZero())
		return -1;

	if (!IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPosition(iBestHitbox)))
		return -1;

	return iBestHitbox;
}