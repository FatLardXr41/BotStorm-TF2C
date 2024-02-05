#pragma once
// pasted from potassium tf2 
// credits: gir and kalvin for this and source file

struct CMenuItems_t
{
	char szTitle[30];
	float* value;
	float flMin;
	float flMax;
	float flStep;
	bool isClassSwitch;
};

class CCheatMenu
{
public:
	void Render(void);
	void DrawMenu(void);
	int	 AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassHead);
	void HandleControls();
	void DrawInfo(int speedValue);
	//private:
	bool bMenuActive;
	int iMenuIndex;

	int iMenuItems;
	CMenuItems_t pMenu[120];
	int iAimbotIndex = -1;

	float settings_switch;
	float iMenu_Pos_X = 300;
	float iMenu_Pos_Y = 75;
	float iInfo_Pos_X = 1150;
	float iInfo_Pos_Y = 600;
	float szsubstest;

	float aimbot_switch;
	float aimbot_silent;
	float aimbot_active;
	//float aimbot_key;
	float aimbot_hitscan;
	float aimbot_hitbox;
	float aimbot_autoshoot;
	float aimbot_zoomedonly;
	float aimbot_autoscope;
	float aimbot_fov;

	float triggerbot_switch;
	float triggerbot_active;
	float triggerbot_key;
	float triggerbot_headonly;

	float esp_switch;
	float esp_active;
	float esp_enemyonly;
	float esp_box;
	float esp_name;
	float esp_cond;
	float esp_class;
	float esp_health;
	float esp_bones;
	float esp_local;
	float building_esp;

	float misc_switch;
	float misc_speedcrouch;
	float misc_chatspam;
	// i'll do kill say later

	float misc_saveconfig;
	float misc_loadconfig;

	float hvh_switch;
	float hvh_enable;
	float hvh_yaw;
	float hvh_pitch;

	float followbot_enabled;
	float followbot_val1;
	float followbot_val2;

	//float autobackstab;
	//float autobackstab_silent;
	bool isThirdPersonEnabled = false;


	float playerlist_switch;
	float PlayerMode[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	float playerlist_switch_followbot;
	float PlayerMode_followbot[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	float priorties_switch;
	float priorties_ignorecloaked;
	float priorties_ignoretaunting;
	float priorties_ignoredisguised;
};

extern CCheatMenu gCheatMenu;