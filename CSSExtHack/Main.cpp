#include "Globals.h"
#include "CSSHack.h"
#include "Overlay.h"

HINSTANCE g_hInstance = 0;
HWND g_hWindow = 0;

bool getCheckboxState(HWND hwndDlg, int iIDDlgItem)
{
	if(SendMessage(GetDlgItem(hwndDlg, iIDDlgItem), BM_GETCHECK, 0, 0)==BST_CHECKED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		{
			g_hWindow = hwndDlg;

			/*
			Icon vom Fenster setzen
			*/
			HICON hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
			SendMessage(hwndDlg, WM_SETICON, WPARAM (ICON_SMALL), LPARAM (hIcon));

			return TRUE;
		}
	case WM_COMMAND:
		{
			int wmId    = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

			switch (wmId)
			{
			case IDC_CHECK_BHOP:
				{
					g_pHack->setBhopState(getCheckboxState(hwndDlg, IDC_CHECK_BHOP));
					break;
				}
			case IDC_CHECK_TRIGGERBOT:
				{
					g_pHack->setTriggerbotState(getCheckboxState(hwndDlg, IDC_CHECK_TRIGGERBOT));
					break;
				}
			case IDC_CHECK_KNIFEBOT:
				{
					g_pHack->setKnifebotState(getCheckboxState(hwndDlg, IDC_CHECK_KNIFEBOT));
					break;
				}
			case IDC_CHECK_NOFLASH:
				{
					g_pHack->setNoFlashState(getCheckboxState(hwndDlg, IDC_CHECK_NOFLASH));
					break;
				}
			case IDC_BUTTON1:
				{
					//g_pOverlay->ResizeToTarget();
					break;
				}
			}
			return TRUE;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_CLOSE:
		PostQuitMessage(0);
		return TRUE;
	}
	return FALSE;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Verhindern, dass er öfters geöffnet wird
	HANDLE hMutex = CreateMutex(NULL, true, "CSSExtHack");
	if (GetLastError() == ERROR_ALREADY_EXISTS || hMutex == INVALID_HANDLE_VALUE)
		return FALSE;

	// Global machen
	g_hInstance = hInstance;

	// Hauptfenster erstellen und anzeigen
	CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);
	ShowWindow(g_hWindow, SW_SHOW);

	g_pHack = new CCSSHack();

	//Sleep(2000);
	//try
	//{
	//	g_pOverlay->Create();
	//}
	//catch (CError &e)
	//{
	//	e.setMsgHeader("ERR - CCSSHack::OnGameFound");
	//	e.showError(ERRPOS);
	//}

	// Hauptnachrichtenschleife:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

void add_log(const char * fmt, ...)
{
	try
	{
		struct tm current_tm;
		time_t current_time;

		time(&current_time);
		localtime_s(&current_tm, &current_time);

		char logbuf[256];
		sprintf_s(logbuf, sizeof(logbuf), "-> %02d:%02d:%02d  ", current_tm.tm_hour, current_tm.tm_min, current_tm.tm_sec);

		va_list va_alist;
		va_start(va_alist, fmt);
		_vsnprintf_s(logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end(va_alist);

		FILE *fp;
		fopen_s(&fp, "C:\\cssexthack.txt", "a");
		if (fp)
		{
			fprintf_s(fp, "%s\n", logbuf);
			fclose(fp);
		}
	}
	catch (...)
	{
	}
}