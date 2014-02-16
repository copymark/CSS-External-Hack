#include "ExternalProcess.h"


CExternalProcess::CExternalProcess(char *pcWindowName)
{
	/*
	Membervariablen mit Standartwerten initialisieren
	*/
	m_hGameHandle = NULL;
	m_hGameHwnd = NULL;
	m_pcWindowName = pcWindowName;
	m_dwProcessId = -1;
}


CExternalProcess::~CExternalProcess(void)
{
}


DWORD WINAPI CExternalProcess::findGameWindow(LPVOID lpParam)
{
	try
	{
		CExternalProcess *pExtProcess = (CExternalProcess*)lpParam;
		while (!pExtProcess->m_hGameHwnd)
		{
			pExtProcess->m_hGameHwnd = FindWindow(NULL, pExtProcess->m_pcWindowName);
			Sleep(1000);
		}

		GetWindowThreadProcessId(pExtProcess->m_hGameHwnd, &pExtProcess->m_dwProcessId);

		if (pExtProcess->m_dwProcessId == -1)
		{
			throw(CError("ExternalProcess::findGameWindow : GetWindowThreadProcessId failed so dwProcessID is invalid.", ERRPOS));
		}

		pExtProcess->m_hGameHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ, false, pExtProcess->m_dwProcessId);

		if (!pExtProcess->m_hGameHandle)
			throw(CError("ExternalProcess::findGameWindow : OpenProcess failed so m_hGameHandle is invalid.", ERRPOS));
		else
			pExtProcess->OnGameFound();
	}
	catch (CError &e)
	{
		e.setMsgHeader("CExternalProcess::findGameWindow");
		e.showError(ERRPOS);
	}
	return 1;
}

DWORD WINAPI CExternalProcess::findGameProcess(LPVOID lpParam)
{
	try
	{
		CExternalProcess *pExtProcess = (CExternalProcess*)lpParam;
		PROCESSENTRY32 pe32 = { sizeof(PROCESSENTRY32) };
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		int iLenProcess = strlen(pExtProcess->m_pcWindowName);

		if (Process32First(hSnapshot, &pe32))
		{
			do
			{
				if (!strncmp(pe32.szExeFile, pExtProcess->m_pcWindowName, iLenProcess))
				{
					pExtProcess->m_dwProcessId = pe32.th32ProcessID;
					break; // Steam lässt sowieso nur 1x hl2.exe zu
				}
			} while (Process32Next(hSnapshot, &pe32));
		}

		if (pExtProcess->m_dwProcessId == -1)
			throw(CError("ExternalProcess::findGameProcess : Couldn't find desired Process, so dwProcessID is invalid.", ERRPOS));

		pExtProcess->m_hGameHandle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ, false, pExtProcess->m_dwProcessId);

		if (!pExtProcess->m_hGameHandle)
			throw(CError("ExternalProcess::findGameProcess : OpenProcess failed so m_hGameHandle is invalid.", ERRPOS));
		else
			pExtProcess->OnGameFound();
	}
	catch (CError &e)
	{
		e.setMsgHeader("CExternalProcess::findGameProcess");
		e.showError(ERRPOS);
	}
	return 1;
}

void CExternalProcess::readMemory(PVOID baseAddress, PVOID buffer, ULONG bufferLength, PULONG numBytesRead = NULL)
{
	try
	{
		if (!m_hGameHandle)
		{
			throw(CError("Invalid Game Handle", ERRPOS));
		}

		g_pNtFunctions->m_pNtReadVirtualMemory(m_hGameHandle, baseAddress, buffer, bufferLength, numBytesRead);
	}
	catch (CError &e)
	{
		e.setMsgHeader("CExternalProcess::readMemory");
		e.showError(ERRPOS);
	}
}

void CExternalProcess::writeMemory(PVOID baseAddress, PVOID buffer, ULONG numBytesToWrite, PULONG numBytesWritten)
{
	try
	{
		if (!m_hGameHandle)
		{
			throw(CError("Invalid Game Handle", ERRPOS));
		}

		g_pNtFunctions->m_pNtWriteVirtualMemory(m_hGameHandle, baseAddress, buffer, numBytesToWrite, numBytesWritten);
	}
	catch (CError &e)
	{
		e.setMsgHeader("CExternalProcess::writeMemory");
		e.showError(ERRPOS);
	}
}

DWORD CExternalProcess::GetModuleBaseExtern(const char *szModuleName)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwProcessId);

    if (!hSnap)
	{
		return 0;
	}
 
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
 
    DWORD dwReturn = 0;
 
    if (Module32First(hSnap, &me32)) {
        while (Module32Next(hSnap, &me32)) {
            if (lstrcmpi(me32.szModule, szModuleName) == 0) {
                dwReturn = (DWORD)me32.modBaseAddr;
                break;
            }
        }
    }
 
    CloseHandle(hSnap);
   
    return dwReturn;
}

void CExternalProcess::startSearching(GAMELISTENERMODE mode)
{
	if (mode == FINDGAMEWINDOW)
	{
		CreateThread(NULL, NULL, findGameWindow, this, NULL, NULL);
	}
	else if (mode == FINDGAMEPROCESS)
	{
		CreateThread(NULL, NULL, findGameProcess, this, NULL, NULL);
	}
}

HWND CExternalProcess::getGameWindow(void)
{
	return m_hGameHwnd;
}

DWORD CExternalProcess::readPointer(DWORD dwAddress, unsigned int numOffsets, ...)
{
	if (m_hGameHandle)
		throw(CError("readPointer - Invalid Game Handle", ERRPOS));

	va_list vl;
	va_start(vl, numOffsets);

	for (unsigned int i = 0; i < numOffsets; i++)
	{
		readMemory((void*)dwAddress, &dwAddress, sizeof(DWORD));
		dwAddress += va_arg(vl, DWORD);
	}
	return dwAddress;
}