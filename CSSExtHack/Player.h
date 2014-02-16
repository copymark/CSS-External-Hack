#pragma once

#include "Globals.h"
#include "CSSHack.h"

struct Vektor
{
	float x;
	float y;
	float z;
};

class CPlayer
{
public:
	CPlayer(int iIndex = 1);
	~CPlayer(void);

	DWORD getBaseAddress(void);
	int getHealth(void);
	int getTeam(void);
	int getFlags(void);
	int getTargetIndex(void);
	bool getLifeState(void);
	bool isPlayer(void);
	Vektor getPosition(void);

private:
	DWORD m_dwBaseAddress;

	DWORD getPlayer(int iIndex);
};

