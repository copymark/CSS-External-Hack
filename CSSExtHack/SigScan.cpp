#include "SigScan.h"


CSigScan::CSigScan(HANDLE hGameHandle, DWORD dwModuleBase, DWORD dwSize)
{
	m_dwModuleBase = dwModuleBase + 0x1000;
	m_dwModuleSize = dwSize;
	m_hGameHandle = hGameHandle;

	m_pbModule = new BYTE[m_dwModuleSize];
	g_pNtFunctions->m_pNtReadVirtualMemory(m_hGameHandle, (void*)m_dwModuleBase, &m_pbModule, m_dwModuleSize, NULL);
}


CSigScan::~CSigScan(void)
{
	delete m_pbModule;
}

DWORD CSigScan::GetModuleSize(DWORD processID, char* module)
{

    HANDLE hSnap;
    MODULEENTRY32 xModule;
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);
    xModule.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &xModule)) {
        while (Module32Next(hSnap, &xModule)) {
            if (!strncmp((char*)xModule.szModule, module, 8)) {
                CloseHandle(hSnap);
                return (DWORD)xModule.modBaseSize;
            }
        }
    }
    CloseHandle(hSnap);
    return 0;
}

bool CSigScan::dataCompare(const BYTE *pData, const BYTE *pMask, const char *sMask)
{
	for (; *sMask; ++sMask, ++pData, ++pMask)
	{
		if (*sMask == 'x' && *pData != *pMask)
		{
			return false;
		}
	}

	return (*sMask == NULL);
}

DWORD CSigScan::findPattern(BYTE *bMask, char* szMask)
{
	DWORD dwAddress = m_dwModuleBase;
	for (DWORD i=0; i < m_dwModuleSize; i++)
	{
		if (dataCompare((BYTE*)(dwAddress+i),bMask,szMask))
		{
			return (DWORD)(dwAddress+i);
		}
	}
	return 0;
}