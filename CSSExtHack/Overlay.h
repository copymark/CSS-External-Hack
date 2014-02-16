//#pragma once
//
//#ifndef __OVERLAY_H__
//#define __OVERLAY_H__
//
//#include "Globals.h"
//
//#include "CSSHack.h"
//
//#include <d3d9.h>
//#include <d3dx9.h>
//
//#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"d3dx9.lib")
//
//class COverlay
//{
//public:
//	COverlay();
//	~COverlay();
//
//	void Create();
//	void Destroy();
//
//	void ResizeToTarget();
//
//	HWND WindowHandle();
//	IDirect3DDevice9* GetDevice();
//
//	static LRESULT CALLBACK OverlayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//private:
//	HWND m_hOverlayHwnd, m_hTargetHwnd;
//	COLORREF m_clrColorKey;
//	D3DCOLOR m_d3dColorKey;
//
//	IDirect3D9* m_pDirectX;
//
//	D3DPRESENT_PARAMETERS m_d3dParameters;
//	IDirect3DDevice9* m_pDevice;
//};
//
//extern COverlay* g_pOverlay;
//
//#endif // __OVERLAY_H__