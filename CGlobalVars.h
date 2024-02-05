#include "SDK.h"

class CPlayerVariables
{
public:
	int iWeaponID;
	bool bHasMeleeWeapon;
	bool bHasFlameThrower;
	int iClass;
	int iWeaponSlot;
	int iPlayerCond;
	int iPlayerCondEx;
	int iFlags;
	char chName[34];
};

class COffsets
{
public:
	int iCreateMoveOffset = 21, iPaintTraverseOffset = 41; // these are the same for TF2C & TF2, no need to worry :3
};