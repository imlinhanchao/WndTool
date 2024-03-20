#include "pch.h"
#include "WndCtrl.h"
#include "tlhelp32.h"

namespace Easy {

WndCtrl::WndCtrl(void)
{
}

WndCtrl::~WndCtrl(void)
{
}

DWORD WndCtrl::FindProcess( CString sProcessName )
{
	HANDLE hProcessSnap = NULL;
	PROCESSENTRY32 pe32= {0};
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == (HANDLE)-1)
	{
		return FALSE;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hProcessSnap, &pe32))
	{
		do
		{   
			if (sProcessName.Compare(pe32.szExeFile) == 0)
			{
				return pe32.th32ProcessID;
			}
		}while (Process32Next(hProcessSnap, &pe32));
	}
	CloseHandle (hProcessSnap);

	return 0;
}

HWND WndCtrl::FindWndFromProcess(DWORD dwProcessId)
{
	WNDPRO wndProcess;
	wndProcess.dwProcessId = dwProcessId;
	if (EnumWindows(EnumWindowsProc, (LPARAM)&wndProcess)) return NULL;
	return wndProcess.hWnd;
}

HWND WndCtrl::FindWindowLoop(TCHAR* lpClassName, TCHAR* lpWindowName, int nTime)
{
	HWND hWnd = NULL;
	do
	{
		Sleep(1000);
		hWnd = ::FindWindow(lpClassName, lpWindowName);
	} while (hWnd == NULL && nTime-- > 0);
	return hWnd;
}

BOOL WndCtrl::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	WNDPRO* pData = (WNDPRO*)lParam;

	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hWnd, &dwProcessId);
	if(dwProcessId == pData->dwProcessId && CWnd::FromHandle(hWnd)->IsWindowVisible())
	{
		HWND h = GetParent(hWnd);
		while(GetParent(h)!=NULL)
		{
			h = GetParent(h);
			return FALSE;
		}
		pData->hWnd = hWnd;
		return FALSE;
	}
	return TRUE;
}

HBITMAP WndCtrl::Capture(HWND hWnd, CString sPath)
{	
	RECT rc = {0};
	if(hWnd != NULL) GetWindowRect(hWnd, &rc);
	return ScreenCapture(sPath, 16, rc);
}

HBITMAP WndCtrl::ScreenCapture(CString sFilename, WORD bitCount, RECT rc)
{
	HBITMAP hBitmap;
	HDC hScreenDC = CreateDCA("DISPLAY", NULL, NULL, NULL);	 //通过指定DISPLAY来获取一个显示设备上下文环境
	HDC hMemDC = CreateCompatibleDC(hScreenDC);			 //该函数创建一个与指定设备兼容的内存设备上下文环境（DC）
	int ScreenWidth = GetDeviceCaps(hScreenDC, HORZRES);	 //获取指定设备的性能参数（此处获取屏幕宽度）
	int ScreenHeight = GetDeviceCaps(hScreenDC, VERTRES);	 //获取指定设备的性能参数（此处获取屏幕高度）
	HBITMAP hOldBM;
	PVOID lpData;	//内存分配成功返回的指向所分配内存块的首地址指针
	int startX;		//截图x位置
	int startY;		//截图y位置
	int width;		//截图宽度
	int height;		//截图高度
	long BitmapSize;
	DWORD BitsOffset;
	DWORD ImageSize;
	DWORD FileSize;
	BITMAPINFOHEADER bmInfo; //BITMAPINFOHEADER结构所包含的成员表明了图像的尺寸、原始设备的颜色格式、以及数据压缩方案
	BITMAPFILEHEADER bmFileHeader;
	HANDLE bmFile, hpal, holdpal = NULL;;
	DWORD dwWritten;

	if (CRect(rc).Width() > 0) {
		startX = rc.left;
		startY = rc.top;
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
	}
	else {
		startX = 0;
		startY = 0;
		width = ScreenWidth;
		height = ScreenHeight;
	}

	//创建一张长width，宽height的画布，用于后面绘制图形
	hBitmap = CreateCompatibleBitmap(hScreenDC, width, height); 
	//该函数选择一对象到指定的设备上下文环境中，该新对象替换先前的相同类型的对象。
	hOldBM = (HBITMAP)SelectObject(hMemDC, hBitmap);		
	//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境。
	BitBlt(hMemDC, 0, 0, width, height, hScreenDC, startX, startY, SRCCOPY);

	POINT pt;
	HWND hWndPt;
	DWORD dwTid = 0, dwCurTid = 0;
	HCURSOR hCursor = NULL;
	GetCursorPos(&pt) ; //获取鼠标坐标 
	hWndPt = WindowFromPoint(pt) ; //根据坐标找窗口句柄 
	dwTid = GetWindowThreadProcessId(hWndPt, NULL) ; //获取该窗口所属的线程id 
	dwCurTid = GetCurrentThreadId() ; //获取当前线程id 
	if(dwTid != dwCurTid)
	{
		AttachThreadInput(dwCurTid, dwTid, TRUE) ; //线程输入附加 
		hCursor = GetCursor() ; //获取到鼠标光标句柄 
		AttachThreadInput(dwCurTid, dwTid, FALSE) ;
	}
	else hCursor = GetCursor() ;
	DrawIconEx(hMemDC, pt.x - startX, pt.y - startY, hCursor, 0, 0, 0, NULL, DI_NORMAL | DI_COMPAT | DI_DEFAULTSIZE); //把鼠标光标画出来 

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBM);

	if(sFilename.IsEmpty())
	{
		DeleteDC(hScreenDC);
		DeleteDC(hMemDC);
		return hBitmap;
	}

	BitmapSize = ((((width * 32) + 32) / 32) * 4) * height;
	//用来在指定的堆上分配内存，并且分配后的内存不可移动(HEAP_NO_SERIALIZE 不使用连续存取)
	lpData = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, BitmapSize); 
	ZeroMemory(lpData, BitmapSize);
	ZeroMemory(&bmInfo, sizeof(BITMAPINFOHEADER));
	bmInfo.biSize = sizeof(BITMAPINFOHEADER); //位图信息结构长度 ,必须为40
	bmInfo.biWidth = width;					//图像宽度 单位是像素
	bmInfo.biHeight = height;					//图像高度 单位是像素
	bmInfo.biPlanes = 1;						//必须为1
	bmInfo.biBitCount = bitCount;				//设置图像的位数。比如8位，16位，32位位数越高分辨率越高
	bmInfo.biCompression = BI_RGB;			//位图是否压缩 BI_RGB为不压缩
	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	BitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo.biSize;
	ImageSize = ((((bmInfo.biWidth * bmInfo.biBitCount) + 31) / 32) * 4) * bmInfo.biHeight;
	FileSize = BitsOffset + ImageSize;
	bmFileHeader.bfType = 0x4d42;//'B'+('M'<<8);
	bmFileHeader.bfSize = FileSize;
	bmFileHeader.bfOffBits = BitsOffset;
	hpal = GetStockObject(DEFAULT_PALETTE);
	if (hpal)
	{
		holdpal = SelectPalette(hMemDC, (HPALETTE)hpal, false);
		RealizePalette(hMemDC);
	}
	GetDIBits(hMemDC, hBitmap, 0, bmInfo.biHeight, lpData, (BITMAPINFO *)&bmInfo, DIB_RGB_COLORS);
	if (holdpal)
	{
		SelectPalette(hMemDC, (HPALETTE)holdpal, true);
		RealizePalette(hMemDC);
	}
	bmFile = CreateFile(sFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(bmFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, _T("Create File Error!"), _T("提示"), MB_OK | MB_ICONWARNING);
	}
	WriteFile(bmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten,NULL);
	WriteFile(bmFile, &bmInfo, sizeof(BITMAPINFOHEADER), &dwWritten,NULL);
	WriteFile(bmFile, lpData, ImageSize, &dwWritten, NULL);
	CloseHandle(bmFile);
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpData);
	::ReleaseDC(0,hScreenDC);
	DeleteDC(hMemDC);
	return hBitmap;
}

bool WndCtrl::KeyBoardPress( BYTE vk, bool bAlt, bool bShift, bool bCtrl )
{
	if(0 == vk)
	{
		return false;
	}

	if(bAlt)   { keybd_event(VK_MENU,    MapVirtualKey(VK_MENU,    0), 0, 0); }
	if(bShift) { keybd_event(VK_SHIFT,   MapVirtualKey(VK_SHIFT,   0), 0, 0); }
	if(bCtrl)  { keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), 0, 0); }

	keybd_event(vk, MapVirtualKey(vk, 0), 0, 0);
	Sleep(100);
	keybd_event(vk, MapVirtualKey(vk, 0), KEYEVENTF_KEYUP, 0);

	if(bAlt)   { keybd_event(VK_MENU,    MapVirtualKey(VK_MENU,    0), KEYEVENTF_KEYUP, 0); }
	if(bShift) { keybd_event(VK_SHIFT,   MapVirtualKey(VK_SHIFT,   0), KEYEVENTF_KEYUP, 0); }
	if(bCtrl)  { keybd_event(VK_CONTROL, MapVirtualKey(VK_CONTROL, 0), KEYEVENTF_KEYUP, 0); }

	return true;
}

bool WndCtrl::MouseClick( int nX, int nY )
{
	if(!SetCursorPos(nX, nY))
	{
		return false;
	}
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, NULL);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, NULL);
	return true;
}

bool WndCtrl::GetText(HWND hWnd, TCHAR* szText, LRESULT& nMaxCount)
{
	if (NULL == hWnd) return false;

	LONG dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	::SetWindowWord(hWnd, GWL_STYLE, 0);
	::SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~ES_PASSWORD);

	LRESULT nTextLen = ::SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
	if (nMaxCount < nTextLen)
	{
		nMaxCount = nTextLen;
		::SetWindowLong(hWnd, GWL_STYLE, dwStyle);
		return false;
	}

	nMaxCount = ::SendMessage(hWnd, WM_GETTEXT, (WPARAM)nMaxCount, (LPARAM)szText);

	::SetWindowLong(hWnd, GWL_STYLE, dwStyle);

	return nMaxCount >= 0;
}

void WndCtrl::Focus( HWND hWnd )
{
	if (hWnd == NULL) return;
	CWnd* pWnd = CWnd::FromHandle(hWnd);
	if (pWnd == NULL) return;
	pWnd->SetForegroundWindow();
	pWnd->SetFocus();
}

HWND WndCtrl::GetFocus()
{
	HWND hWnd = ::GetForegroundWindow();
	auto activeWindowThread = GetWindowThreadProcessId(hWnd, 0);
	auto thisThread = GetCurrentThreadId();
	if (AttachThreadInput(activeWindowThread, thisThread, TRUE)) {
		// 获取焦点所在控件
		HWND hFocus = ::GetFocus();

		AttachThreadInput(activeWindowThread, thisThread, FALSE);

		return hFocus;
	}
	return NULL;
}

void WndCtrl::SetWindowAlpha(float fAlpha, HWND hWnd)
{
	LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	::SetWindowLongPtr(hWnd, GWL_EXSTYLE, exStyle | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(hWnd, 0, BYTE(255 * fAlpha), LWA_ALPHA);
}

void WndCtrl::MouseThrough(bool bThrough, HWND hWnd)
{
	LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	if (bThrough) dwStyle |= WS_EX_TRANSPARENT;
	else dwStyle &= ~(WS_EX_TRANSPARENT);
	::SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwStyle);
}

bool WndCtrl::IsMouseThrough(HWND hWnd)
{
	LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	return (dwStyle & WS_EX_TRANSPARENT) != 0;
}

bool WndCtrl::IsWindowTopMost(HWND hWnd)
{
	LONG_PTR exStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	return ((exStyle & WS_EX_TOPMOST) != 0);
}

}