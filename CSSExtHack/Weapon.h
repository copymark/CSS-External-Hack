#pragma once

#include "Globals.h"
#include "CSSHack.h"
#include "EntityList.h"

#define WEAPON_P228 1
#define WEAPON_GLOCK 2
#define WEAPON_SCOUT 3
#define WEAPON_HEGRENADE 4
#define WEAPON_XM1014 5
#define WEAPON_C4 6
#define WEAPON_MAC10 7
#define WEAPON_AUG 8
#define WEAPON_SMOKEGRENADE 9
#define WEAPON_ELITE 10
#define WEAPON_FIVESEVEN 11
#define WEAPON_UMP45 12
#define WEAPON_SG550 13
#define WEAPON_GALIL 14
#define WEAPON_FAMAS 15
#define WEAPON_USP 16
#define WEAPON_AWP 17
#define WEAPON_MP5NAVY 18
#define WEAPON_M249 19
#define WEAPON_M3 20
#define WEAPON_M4A1 21
#define WEAPON_TMP 22
#define WEAPON_G3SG1 23
#define WEAPON_FLASHBANG 24
#define WEAPON_DEAGLE 25
#define WEAPON_SG552 26
#define WEAPON_AK47 27
#define WEAPON_KNIFE 28
#define WEAPON_P90 29
#define WEAPON_SHIELD 30
#define WEAPON_KEVLAR 31
#define WEAPON_ASSAULTSUIT 32
#define WEAPON_NIGHTVISION 33


class CWeapon
{
public:
	CWeapon(void);
	~CWeapon(void);

	int getWeaponId(void);
	char* getWeaponAlias(int iWeaponID);
	DWORD getActiveWeapon(DWORD dwPlayer);

private:
	DWORD m_dwBaseAddress;
};

