#pragma once

#include "SDK.h"

typedef bool(__fastcall* CreateMove_t)(void*, void*, float, CUserCmd*);
extern CreateMove_t oCreateMove;
bool __fastcall Hooked_CreateMove(PVOID ClientMode, void* edx, float input_sample_frametime, CUserCmd* pCommand);