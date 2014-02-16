#include "Player.h"


CPlayer::CPlayer(int iIndex)
{
	m_dwBaseAddress = getPlayer(iIndex);
}


CPlayer::~CPlayer(void)
{
}

DWORD CPlayer::getBaseAddress(void)
{
	return m_dwBaseAddress;
}

DWORD CPlayer::getPlayer(int iIndex)
{
	DWORD dwEntityListAddressPointer = g_pHack->m_dwClient + 0x56B3E4;
	DWORD dwPlayer = 0;

	/*
	Das funktioniert so gut wie immer. Manchmal (online) besitzt das Array allerdings lücken :(
	*/
	g_pHack->readMemory((void*)(dwEntityListAddressPointer + 0x10 * (iIndex-1)), &dwPlayer, sizeof(dwPlayer), NULL);

	if (dwPlayer != NULL)
	{
		return dwPlayer;
	}

	/*
	Sollten wir auf solch eine Lücke gestoßen sein, iterieren wir durch das komplette Array und suchen nach dem richtigen Spieler
	Index 1 lassen wir aus, da unser eigener Spieler dort gespeichert ist.
	*/
	int iPlayerIndex;
	for (int i = 1; i < 64; i++)
	{
		g_pHack->readMemory((void*)(dwEntityListAddressPointer + 0x10*i), &dwPlayer, sizeof(dwPlayer), NULL);
		g_pHack->readMemory((void*)(dwPlayer + 0x50), &iPlayerIndex, sizeof(iPlayerIndex), NULL);
		if (iPlayerIndex == iIndex)
		{
			return dwPlayer;
		}
	}

	/*
	Wenn voriges auch nicht geholfen hat, so gibt es diesen Spieler Index anscheinend nicht
	*/
	return NULL;
}

int CPlayer::getHealth(void)
{
	int iHealth;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x90), &iHealth, sizeof(iHealth), NULL);
	return iHealth;
}

int CPlayer::getTeam(void)
{
	int iTeam;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x98), &iTeam, sizeof(iTeam), NULL);
	return iTeam;
}

int CPlayer::getFlags(void)
{
	int fFlags;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x34C), &fFlags, sizeof(fFlags), NULL);
	return fFlags;
}

int CPlayer::getTargetIndex(void)
{
	int iTargetIndex;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x14D4), &iTargetIndex, sizeof(iTargetIndex), NULL);
	return iTargetIndex;
}

bool CPlayer::getLifeState(void)
{
	bool bIsAlive;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x8F), &bIsAlive, sizeof(bIsAlive), NULL);
	if (!bIsAlive)
		return true;
	return false;
}

bool CPlayer::isPlayer(void)
{
	int iFlags = getFlags();
	if (iFlags & FL_CLIENT)
	{
		return true;
	}
	return false;
}

Vektor CPlayer::getPosition(void)
{
	Vektor sPosition;
	g_pHack->readMemory((void*)(m_dwBaseAddress + 0x274), &sPosition, sizeof(sPosition), NULL);
	sPosition.z += 64.0f;
	return sPosition;
}