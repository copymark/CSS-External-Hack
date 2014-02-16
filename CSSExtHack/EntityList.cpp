#include "EntityList.h"


CEntityList::CEntityList()
{
}


CEntityList::~CEntityList()
{
}

DWORD CEntityList::LookupEntity(const CBaseHandle handle)
{
	if (!handle.IsValid())
		return NULL;

	DWORD offset = (handle.GetEntryIndex() * 0x10) + 4;
	DWORD pInfo[2];

	DWORD dwEntityListAddress = g_pHack->m_dwClient + 0x52E02C;
	DWORD dwEntityList;
	g_pHack->readMemory((void*)(dwEntityListAddress), &dwEntityList, sizeof(dwEntityList), NULL);

	g_pHack->readMemory((void*)(dwEntityList + offset), &pInfo, sizeof(DWORD) * 2, NULL);

	if (pInfo[1] == handle.GetSerialNumber())
		return pInfo[0];
	else
		return NULL;	
}