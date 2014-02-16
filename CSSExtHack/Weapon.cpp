#include "Weapon.h"

CWeapon::CWeapon(void)
{
	m_dwBaseAddress = 0;
}


CWeapon::~CWeapon(void)
{

}

DWORD CWeapon::getActiveWeapon(DWORD dwPlayer)
{
	CBaseHandle hActiveWeapon;
	g_pHack->readMemory((void*)(dwPlayer + 0xD70), &hActiveWeapon, sizeof(hActiveWeapon), NULL);
	
	m_dwBaseAddress = CEntityList::LookupEntity(hActiveWeapon);
	return m_dwBaseAddress;
}

int CWeapon::getWeaponId(void)
{
	if (!m_dwBaseAddress)
		return 0;

	DWORD dwVTable = 0;
	g_pHack->readMemory((void*)m_dwBaseAddress, &dwVTable, sizeof(dwVTable), NULL);
	DWORD dwGetWeaponIdFunc = 0;
	g_pHack->readMemory((void*)(dwVTable + 0x5A0), &dwGetWeaponIdFunc, sizeof(dwGetWeaponIdFunc), NULL);
	int iWeaponID = 0;
	g_pHack->readMemory((void*)(dwGetWeaponIdFunc + 0x1), &iWeaponID, sizeof(iWeaponID), NULL);
	return iWeaponID;
}

char* CWeapon::getWeaponAlias(int iWeaponID)
{
	DWORD dwWeaponListAddress = g_pHack->m_dwClient + 0x2A2300 + 0x2761B0;
	for (int i = 0; i < 0x26; i++)
	{
		int iWeaponIDinArray = 0;
		g_pHack->readMemory((void*)(dwWeaponListAddress+8*i), &iWeaponIDinArray, sizeof(iWeaponIDinArray), NULL);
		if (iWeaponID == iWeaponIDinArray)
		{
			dwWeaponListAddress += 0x4;
			DWORD dwWeaponNameAddress = 0;
			g_pHack->readMemory((void*)(dwWeaponListAddress+8*i), &dwWeaponNameAddress, sizeof(dwWeaponNameAddress), NULL);

			char cLetter = 1;
			DWORD dwCurPos = dwWeaponNameAddress;
			int iLength = 0;
			while (cLetter != NULL)
			{
				g_pHack->readMemory((void*)dwCurPos, &cLetter, sizeof(cLetter), NULL);
				iLength++;
				dwCurPos++;
			}
			char *pcWeaponName = new char[iLength];
			g_pHack->readMemory((void*)dwWeaponNameAddress, pcWeaponName, iLength, NULL);
			return pcWeaponName;
		}
	}
	return NULL;
}