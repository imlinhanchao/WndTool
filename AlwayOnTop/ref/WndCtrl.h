#pragma once

namespace Easy {

typedef struct _WNDPRO
{
	HWND hWnd;
	DWORD dwProcessId;

}WNDPRO;

class WndCtrl
{
public:
	WndCtrl(void);
	~WndCtrl(void);

	static DWORD FindProcess(CString sProcessName);
	static HWND FindWndFromProcess(DWORD dwProcessId);
	static HWND FindWindowLoop(TCHAR* lpClassName, TCHAR* lpWindowName, int nTime=10);

	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
	static HBITMAP Capture(HWND hWnd, CString sPath=_T(""));
	static HBITMAP ScreenCapture(CString sFilename, WORD bitCount, RECT rc);

	static bool KeyBoardPress(BYTE vk, bool bAlt, bool bShift, bool bCtrl);
	static bool MouseClick(CPoint pt) { return MouseClick(pt.x, pt.y); }
	static bool MouseClick(int nX, int nY);

	static bool GetText(HWND hWnd, TCHAR* szText, LRESULT& nMaxCount);
	static void Focus(HWND hWnd);
	static HWND GetFocus();

	static void SetWindowAlpha(float fAlpha, HWND hWnd);
	static void MouseThrough(bool bThrough, HWND hWnd);
	static bool IsMouseThrough(HWND hWnd);
	static bool IsWindowTopMost(HWND hWnd);
};

}