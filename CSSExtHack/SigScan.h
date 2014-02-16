#pragma once
#include "Globals.h"
#include "NtFunctions.h"

class CSigScan
{
public:
	CSigScan(HANDLE hGameHandle, DWORD dwModuleBase, DWORD dwSize);
	~CSigScan(void);

	static DWORD GetModuleSize(DWORD processID, char* module);

	DWORD findPattern(BYTE *bMask, char* szMask);

private:
	BYTE *m_pbModule;
	DWORD m_dwModuleBase;
	DWORD m_dwModuleSize;
	HANDLE m_hGameHandle;

	bool dataCompare(const BYTE *pData, const BYTE *pMask, const char *sMask);

};

