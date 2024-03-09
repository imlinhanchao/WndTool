// CViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "WndTools.h"
#include "afxdialogex.h"
#include "CViewDlg.h"


// CViewDlg 对话框

IMPLEMENT_DYNAMIC(CViewDlg, CDialogEx)

CViewDlg::CViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_BG, pParent)
{

}

CViewDlg::~CViewDlg()
{
}

void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CViewDlg 消息处理程序


BOOL CViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CViewDlg::Init(HWND hWnd)
{
	if (hWnd == NULL) return;

	m_hFocusWnd = hWnd;

	// 获取hWnd所在屏幕
	m_mi.cbSize = sizeof(m_mi);
	HMONITOR hMonitor = MonitorFromWindow(m_hFocusWnd, MONITOR_DEFAULTTONEAREST);
	GetMonitorInfo(hMonitor, &m_mi);

	// 获取屏幕宽高
	int cx = m_mi.rcMonitor.right - m_mi.rcMonitor.left;
	int cy = m_mi.rcMonitor.bottom - m_mi.rcMonitor.top;

	m_rcLast = CRect(m_mi.rcMonitor.left, m_mi.rcMonitor.top, m_mi.rcMonitor.right, m_mi.rcMonitor.bottom);

	// 将 m_hFocusWnd 置顶
	::SetWindowPos(m_hFocusWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// 设置窗体大小
	SetWindowPos(NULL, m_rcLast.left, m_rcLast.top, m_rcLast.Width(), m_rcLast.Height(), SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
	::SetWindowPos(m_hWnd, m_hFocusWnd, 0, 0, 0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	CWnd::FromHandle(m_hFocusWnd)->CenterWindow();

	// 填充背景
	SetBackgroundColor(RGB(0, 0, 0));

	// 设置窗体透明度
	WndCtrl::SetWindowAlpha(0.8, m_hWnd);

	SetTimer(1, 100, NULL);
}

void CViewDlg::PaintBG()
{
	CDC* pDC = GetDC();
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(m_rcLast, &brush);
	ReleaseDC(pDC);
}

void CViewDlg::CheckMonitor()
{
	m_mi.cbSize = sizeof(m_mi);
	HMONITOR hMonitor = MonitorFromWindow(m_hFocusWnd, MONITOR_DEFAULTTONEAREST);
	GetMonitorInfo(hMonitor, &m_mi);

	if (m_rcLast.left != m_mi.rcMonitor.left || m_rcLast.top != m_mi.rcMonitor.top) {
		m_rcLast = CRect(m_mi.rcMonitor.left, m_mi.rcMonitor.top, m_mi.rcMonitor.right, m_mi.rcMonitor.bottom);
		::SetWindowPos(m_hFocusWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(NULL, m_rcLast.left, m_rcLast.top, m_rcLast.Width(), m_rcLast.Height(), SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
	}
}


void CViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// 定时监听窗体 m_hFocusWnd 是否被关闭
		if (!IsWindow(m_hFocusWnd))
		{
			// 若 m_hFocusWnd 被关闭，则关闭窗体
			KillTimer(1);
			EndDialog(0);
		}

		// 监听窗体是否最小化
		else if (::IsIconic(m_hFocusWnd))
		{
			// 若最小化，则将窗体还原
			::SetWindowPos(m_hFocusWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		}
		else
		{
			// 检查m_hFocusWnd是否置顶
			if (!IsWindowVisible()) {
				::SetWindowPos(m_hWnd, m_hFocusWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_hFocusWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			if (!WndCtrl::IsWindowTopMost(m_hFocusWnd)) {
				::SetWindowPos(m_hFocusWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_hWnd, m_hFocusWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			}

			CheckMonitor();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// 不响应 Esc 和  Alt + F4
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CViewDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//PaintBG();
}


void CViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// 窗体还原时
	if (nID == SC_RESTORE)
	{
		// 将窗体置顶
		::SetWindowPos(m_hFocusWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		::SetWindowPos(m_hWnd, m_hFocusWnd, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
	}

	CDialogEx::OnSysCommand(nID, lParam);
}
