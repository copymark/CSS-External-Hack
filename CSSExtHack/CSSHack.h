#pragma once
#include "ExternalProcess.h"
#include "SigScan.h"
#include "Player.h"
#include "Weapon.h"
#include "math.h"

//Flags
#define FL_ONGROUND	(1<<0)
#define FL_DUCKING	(1<<1)
#define	FL_CLIENT	(1<<8)
#define FL_INWATER	(1<<10) // Not 1<<9 ???

class CCSSHack :
	public CExternalProcess
{
public:
	CCSSHack(void);
	~CCSSHack(void);
	void mainLoopThread(void);

	static DWORD WINAPI ThreadStarter(LPVOID lpParam);
	virtual void OnGameFound(void);

	CSigScan *m_pSigScan;

	void bhopHack(void);
	void triggerbotHack(void);
	void noFlashHack(void);

	/*
	Module Bases
	*/
	DWORD m_dwClient;
	DWORD m_dwEngine;
	DWORD m_dwOverlay;
	DWORD m_dwMatSurface;

	/*
	Config
	*/
	bool m_bBhopStatus;
	bool m_bEntityTriggerbotStatus;
	bool m_bKnifeBotStatus;
	bool m_bNoFlashStatus;
public:
	void setBhopState(bool bEnabled);
	void setTriggerbotState(bool bEnabled);
	void setKnifebotState(bool bEnabled);
	void setNoFlashState(bool bEnabled);
};

extern CCSSHack *g_pHack;
