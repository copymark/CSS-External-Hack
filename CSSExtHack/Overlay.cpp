//#include "Overlay.h"
//
//COverlay* g_pOverlay = new COverlay();
//
//COverlay::COverlay()
//{
//	m_clrColorKey = RGB(255, 0, 255);
//	m_d3dColorKey = D3DCOLOR_ARGB(255, 255, 0, 255);
//}
//
//COverlay::~COverlay()
//{
//	Destroy();
//}
//
//void COverlay::Create()
//{
//	m_hTargetHwnd = g_pHack->getGameWindow();
//	
//	if(!g_hInstance || !m_hTargetHwnd)
//		throw(CError("Overlay::Create : g_hInstance and/or m_hOverlayHwnd are/is invalid.", ERRPOS));
//
//	WNDCLASSEX wndex;
//	memset(&wndex, NULL, sizeof(wndex));
//	wndex.cbSize = sizeof(WNDCLASSEX);
//	wndex.style = CS_HREDRAW | CS_VREDRAW;
//	wndex.lpfnWndProc = COverlay::OverlayProc;
//	wndex.cbClsExtra = 0;
//	wndex.cbWndExtra = 0;
//	wndex.hInstance = g_hInstance;
//	wndex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wndex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wndex.hbrBackground = (HBRUSH)CreateSolidBrush(m_clrColorKey);
//	wndex.lpszMenuName = NULL;
//	wndex.lpszClassName = "WINDOWOVERLAY";
//	wndex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//
//	if(!RegisterClassEx(&wndex))
//		throw(CError("Overlay::Create : RegisterClassEx() failed.", ERRPOS));
//
//	RECT rectTargetWindowSize;
//	if(!GetWindowRect(m_hTargetHwnd, &rectTargetWindowSize))
//		throw(CError("Overlay::Create : GetWindowRect() failed.", ERRPOS));
//
//	// LAYERED:		Um bestimmte Teile des Fensters durchsichtig zu machen (Colorkey)
//	// TRANSPARENT:	Events wie Mausklicks und Tastatureingaben gehen "durchs fenster durch"
//	// COMPOSITED:	Bin mir net genau sicher, erlaubt aber wohl schnelleres zeichnen^^
//	// TOPMOST:		Ist immer vor allen anderen fenstern (außer andere topmost windows, ofc)
//	m_hOverlayHwnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_COMPOSITED | WS_EX_TOPMOST, "WINDOWOVERLAY", "Overlay", WS_VISIBLE | WS_POPUP, rectTargetWindowSize.left, rectTargetWindowSize.top, (rectTargetWindowSize.right - rectTargetWindowSize.left), (rectTargetWindowSize.bottom - rectTargetWindowSize.top), NULL, NULL, g_hInstance, NULL);
//	if(!m_hOverlayHwnd)
//		throw(CError("Overlay::Create : CreateWindowEx() failed.", ERRPOS));
//	ShowWindow(m_hOverlayHwnd, SW_SHOW);
//	UpdateWindow(m_hOverlayHwnd);
//	
//	if(!SetLayeredWindowAttributes(m_hOverlayHwnd, m_clrColorKey, 0, LWA_COLORKEY))
//		throw(CError("Overlay::Create : SetLayeredWindowAttributes() failed.", ERRPOS));
//
//	// D3D9 initialisieren
//	m_pDirectX = Direct3DCreate9(D3D_SDK_VERSION);
//	if(!m_pDirectX)
//		throw(CError("Overlay::Create : Direct3DCreate9() failed.", ERRPOS));
//
//	m_d3dParameters.BackBufferWidth				= (rectTargetWindowSize.right - rectTargetWindowSize.left);
//	m_d3dParameters.BackBufferHeight			= (rectTargetWindowSize.bottom - rectTargetWindowSize.top);
//	m_d3dParameters.BackBufferFormat			= D3DFMT_A8R8G8B8;
//	m_d3dParameters.BackBufferCount				= 1;
//	m_d3dParameters.MultiSampleType				= D3DMULTISAMPLE_NONE;
//	m_d3dParameters.MultiSampleQuality			= 0;
//	m_d3dParameters.SwapEffect					= D3DSWAPEFFECT_DISCARD;
//	m_d3dParameters.hDeviceWindow				= m_hOverlayHwnd;
//	m_d3dParameters.Windowed					= TRUE;
//	m_d3dParameters.EnableAutoDepthStencil		= FALSE;
//	m_d3dParameters.AutoDepthStencilFormat		= D3DFMT_UNKNOWN;
//	m_d3dParameters.Flags						= 0;
//	m_d3dParameters.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
//	m_d3dParameters.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE; // D3DPRESENT_INTERVAL_DEFAULT = VSync an
//
//	if(FAILED(m_pDirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hOverlayHwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3dParameters, &m_pDevice)))
//		throw(CError("Overlay::Create : IDirect3D9::CreateDevice() failed.", ERRPOS));
//}
//
//void COverlay::Destroy()
//{
//	if(m_pDevice)
//	{
//		m_pDevice->Release();
//		m_pDevice = NULL;
//	}
//
//	if(m_pDirectX)
//	{
//		m_pDirectX->Release();
//		m_pDirectX = NULL;
//	}
//
//	if(m_hOverlayHwnd)
//	{
//		DestroyWindow(m_hOverlayHwnd);
//	}
//}
//
//void COverlay::ResizeToTarget()
//{
//	if(!m_hOverlayHwnd || !m_hTargetHwnd)
//		return;
//
//	RECT rectTargetWindowSize;
//	if(!GetWindowRect(m_hTargetHwnd, &rectTargetWindowSize))
//		return;
//
//	if(!MoveWindow(m_hOverlayHwnd, rectTargetWindowSize.left, rectTargetWindowSize.top, (rectTargetWindowSize.right - rectTargetWindowSize.left), (rectTargetWindowSize.bottom - rectTargetWindowSize.top), TRUE))
//		return;
//
//	m_d3dParameters.BackBufferWidth		= (rectTargetWindowSize.right - rectTargetWindowSize.left);
//	m_d3dParameters.BackBufferHeight	= (rectTargetWindowSize.bottom - rectTargetWindowSize.top);
//
//	if(FAILED(m_pDevice->Reset(&m_d3dParameters)))
//		return;
//}
//
//HWND COverlay::WindowHandle()
//{
//	return m_hOverlayHwnd;
//}
//
//IDirect3DDevice9* COverlay::GetDevice()
//{
//	return m_pDevice;
//}
//
//LRESULT CALLBACK COverlay::OverlayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	static D3DRECT myRect = { 100, 100, 200, 200 };
//	IDirect3DDevice9* pDevice = g_pOverlay->GetDevice();
//
//	switch(uMsg)
//	{
//	case WM_PAINT:
//		pDevice->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 255), 1.0f, NULL);
//		
//		if (SUCCEEDED(pDevice->BeginScene()))
//		{
//			pDevice->Clear(1, &myRect, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 1.0f, 0);
//
//			pDevice->EndScene();
//		}
//
//		pDevice->Present(NULL, NULL, NULL, NULL);
//		ValidateRect(hwnd, NULL);
//		break;
//	case WM_ERASEBKGND:
//		break;
//	case WM_CLOSE:
//	case WM_DESTROY:
//		g_pOverlay->Destroy();
//	default:
//		return DefWindowProc(hwnd, uMsg, wParam, lParam);
//	}
//
//	return 0;
//}