#include "CSSHack.h"

CCSSHack *g_pHack = NULL;

CCSSHack::CCSSHack(void) : CExternalProcess("Counter-Strike Source")
{
	m_dwClient = NULL;
	m_dwEngine = NULL;
	m_dwOverlay = NULL;
	m_dwMatSurface = NULL;

	m_pSigScan = NULL;
	m_bBhopStatus = false;
	m_bEntityTriggerbotStatus = false;
	m_bKnifeBotStatus = false;

	startSearching(FINDGAMEWINDOW);
}


CCSSHack::~CCSSHack(void)
{
}


DWORD WINAPI CCSSHack::ThreadStarter(LPVOID lpParam)
{
	CCSSHack *pHack = (CCSSHack*)lpParam;
	pHack->mainLoopThread();
	return 0;
}

void CCSSHack::mainLoopThread(void)
{
	DWORD dwOverlayStatusAddress = m_dwOverlay + 0x842C4;
	int iOverlayEnabled = 0;

	DWORD dwIngameChatStatusAddress = m_dwMatSurface + 0x140020;
	int iIngameChatEnabled = 0;

	while (true)
	{
		this->readMemory((void*)dwOverlayStatusAddress, &iOverlayEnabled, sizeof(iOverlayEnabled), NULL);
		this->readMemory((void*)dwIngameChatStatusAddress, &iIngameChatEnabled, sizeof(iIngameChatEnabled), NULL);

		if (!(iOverlayEnabled | iIngameChatEnabled))
		{
			if (m_bBhopStatus == true)
			{
				bhopHack();
			}
			if (m_bEntityTriggerbotStatus == true || m_bKnifeBotStatus == true)
			{
				triggerbotHack();
			}
			if (m_bNoFlashStatus == true)
			{
				noFlashHack();
			}
			if (GetKeyState(VK_INSERT) & (1<<15))
			{
				CPlayer localPlayer(1);
				CWeapon weapon;
				weapon.getActiveWeapon(localPlayer.getBaseAddress());
				int id = weapon.getWeaponId();
				add_log("ID: %d", id);
				char *name = weapon.getWeaponAlias(id);
				add_log("Name: %s", name);
				delete name;
			}
		}
		else
		{
			Sleep(100);
		}
		Sleep(10);
	}
}

void CCSSHack::OnGameFound(void)
{
	// client.dll erst ab Menu geladen.
	while (!m_dwClient)		{ m_dwClient = GetModuleBaseExtern("client.dll");				Sleep(100); }
	while (!m_dwEngine)		{ m_dwEngine = GetModuleBaseExtern("engine.dll");				Sleep(100); }
	while (!m_dwOverlay)	{ m_dwOverlay = GetModuleBaseExtern("gameoverlayrenderer.dll");	Sleep(100); }
	while (!m_dwMatSurface)	{ m_dwMatSurface = GetModuleBaseExtern("vguimatsurface.dll");	Sleep(100); }

	m_pSigScan = new CSigScan(m_hGameHandle, m_dwClient, CSigScan::GetModuleSize(m_dwProcessId, "client.dll"));

	CreateThread(NULL, NULL, &CCSSHack::ThreadStarter, this, NULL, NULL);
}


void CCSSHack::setBhopState(bool bEnabled)
{
	m_bBhopStatus = bEnabled;
}

void CCSSHack::setTriggerbotState(bool bEnabled)
{
	m_bEntityTriggerbotStatus = bEnabled;
}

void CCSSHack::setKnifebotState(bool bEnabled)
{
	m_bKnifeBotStatus = bEnabled;
}

void CCSSHack::setNoFlashState(bool bEnabled)
{
	m_bNoFlashStatus = bEnabled;
}

void CCSSHack::bhopHack(void)
{
	DWORD jumpState = m_dwClient + 0x58A68C;
	int valJump = 5;

	if (GetKeyState(VK_SPACE) & (1<<15)) // return type == short | check for high-order bit: if 1 down else up
	{
		CPlayer localPlayer(1);
		if (!localPlayer.getLifeState())
			return;

		int m_fFlags = localPlayer.getFlags();
		if (m_fFlags & FL_ONGROUND || m_fFlags & FL_INWATER)
		{
			this->writeMemory((void*)jumpState, (void*)&(valJump = 5), sizeof(valJump), NULL);
		}
		else
		{
			this->writeMemory((void*)jumpState, (void*)&(valJump = 4), sizeof(valJump), NULL);
		}
	}
}

void CCSSHack::triggerbotHack(void)
{
	CPlayer localPlayer(1);
	if (!localPlayer.getLifeState())
		return;

	int iTargetIndex = localPlayer.getTargetIndex();

	if (iTargetIndex > 1)
	{
		bool bUseRightClick = false;
		CWeapon myWeapon;
		myWeapon.getActiveWeapon(localPlayer.getBaseAddress());

		int iWeaponId = myWeapon.getWeaponId();

		if (iWeaponId == WEAPON_C4 || 
			iWeaponId == WEAPON_FLASHBANG ||
			iWeaponId == WEAPON_HEGRENADE ||
			iWeaponId == WEAPON_SMOKEGRENADE)
			return;
		else if (iWeaponId == WEAPON_KNIFE)
		{
			if (m_bKnifeBotStatus == false)
				return;

			bUseRightClick = true;
		}
		else
		{
			if (m_bEntityTriggerbotStatus == false)
				return;
		}
		CPlayer otherPlayer(iTargetIndex);

		if (!otherPlayer.isPlayer())
			return;

				
		int iLocalTeam = localPlayer.getTeam();
		int iAimTeam = otherPlayer.getTeam();

		if ((iLocalTeam == 2 && iAimTeam == 3) || (iLocalTeam == 3 && iAimTeam == 2))
		{
			if (!bUseRightClick)
			{
				DWORD dwAttack = m_dwClient + 0x58A680;
				int value = 5;
				this->writeMemory((void*)(dwAttack), &value, sizeof(value), NULL);
				Sleep(10);
				this->writeMemory((void*)(dwAttack), &(value=4), sizeof(value), NULL);
			}
			else
			{
				Vektor myPos = localPlayer.getPosition();
				Vektor otherPos = otherPlayer.getPosition();

				Vektor v;
				v.x = otherPos.x - myPos.x; v.y = otherPos.y - myPos.y; v.z = otherPos.z - myPos.z;

				float length = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);

				if (length <= 66.6f)
				{
					DWORD dwAttack2 = m_dwClient + 0x58A674;
					int value = 5;
					this->writeMemory((void*)(dwAttack2), &value, sizeof(value), NULL);
					Sleep(10);
					this->writeMemory((void*)(dwAttack2), &(value=4), sizeof(value), NULL);
				}
			}
		}
	}
}

void CCSSHack::noFlashHack(void)
{
	float flPercentage = 0.0f;
	float flFlashAlpha = (flPercentage * 2.55f);

	CPlayer localplayer(1);
	this->writeMemory((void*)(localplayer.getBaseAddress() + 0x1430), &flFlashAlpha, sizeof(flFlashAlpha), NULL);
}