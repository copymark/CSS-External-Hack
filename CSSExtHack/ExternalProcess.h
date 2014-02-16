#pragma once
#include <stdarg.h>

#include "Globals.h"
#include "NtFunctions.h"

enum GAMELISTENERMODE {
	FINDGAMEWINDOW,
	FINDGAMEPROCESS
};

class CExternalProcess
{
public:
	CExternalProcess(char *pcWindowName);
	~CExternalProcess(void);
public:
	HANDLE m_hGameHandle;
	HWND m_hGameHwnd;
	DWORD m_dwProcessId;
	char *m_pcWindowName;
private:
	/*
	GAMELISTENER Funktionen
	*/
	static DWORD WINAPI findGameWindow(LPVOID lpParam);
	static DWORD WINAPI findGameProcess(LPVOID lpParam);

public:
	void readMemory(PVOID baseAddress, PVOID buffer, ULONG bufferLength, PULONG numBytesRead);
	void writeMemory(PVOID baseAddress, PVOID buffer, ULONG numBytesToWrite, PULONG numBytesWritten);
	DWORD GetModuleBaseExtern(const char *szModuleName);
	virtual void OnGameFound(void) = NULL;
	void startSearching(GAMELISTENERMODE mode);
	DWORD readPointer(DWORD dwAddress, unsigned int numOffsets, ...);

	HWND getGameWindow(void);
};

