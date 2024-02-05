#pragma once

typedef struct CScreenSize_t
{
	int iScreenHeight;
	int iScreenWidth;

} CScreenSize;

typedef void(__fastcall* PaintTraverse_t)(void*, void*, unsigned int, bool, bool);
extern PaintTraverse_t oPaintTraverse;
void __fastcall Hooked_PaintTraverse(PVOID pPanels, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
void Intro();

extern CScreenSize gScreenSize;