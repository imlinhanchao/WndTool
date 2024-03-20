// CBorderDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "CBorderDlg.h"
#include <Mmsystem.h>
#pragma comment(lib, "Dwmapi.lib")

// CBorderDlg 对话框

IMPLEMENT_DYNAMIC(CBorderDlg, CDialogEx)

CBorderDlg::CBorderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_BORDER, pParent)
{

}

CBorderDlg::~CBorderDlg()
{
}

void CBorderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBorderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBorderDlg 消息处理程序
BOOL CBorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CBorderDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CBorderDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CBorderDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcWindow;
	CPen pen;
	pen.CreatePen(PS_SOLID, m_nBorderWidth, m_crBorder);
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* pOldBrush = dc.SelectObject(pBrush);

	GetClientRect(&rcWindow);

	rcWindow.InflateRect(-m_nBorderWidth / 3, -m_nBorderWidth / 3, -m_nBorderWidth / 3, -m_nBorderWidth / 3);

	dc.FillRect(rcWindow, pBrush);

	dc.MoveTo(rcWindow.TopLeft());
	dc.LineTo(rcWindow.right, rcWindow.top);
	dc.LineTo(rcWindow.BottomRight());
	dc.LineTo(rcWindow.left, rcWindow.bottom);
	dc.LineTo(rcWindow.TopLeft());

	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}

void CBorderDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// 定时监听窗体 m_hFocusWnd 是否被关闭
		if (!IsWindow(m_hParent))
		{
			// 若 m_hFocusWnd 被关闭，则关闭窗体
			KillTimer(1);
			EndDialog(0);
		}

		// 监听窗体是否最小化
		else if (::IsIconic(m_hParent))
		{
			// 若最小化，则将窗体还原
			::SetWindowPos(m_hParent, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);
		}
		else
		{
			// 检查m_hFocusWnd是否置顶
			if (!IsWindowVisible()) {
				::SetWindowPos(m_hWnd, m_hParent, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_hParent, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			if (!WndCtrl::IsWindowTopMost(m_hParent)) {
				::SetWindowPos(m_hParent, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				::SetWindowPos(m_hWnd, m_hParent, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
			}

			CheckRect();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CBorderDlg::CheckRect()
{
	CRect rcParent;
	CRect rcTemp;

	::GetWindowRect(m_hParent, &rcParent);

	DwmGetWindowAttribute(m_hParent,
		DWMWA_EXTENDED_FRAME_BOUNDS,
		&rcParent, sizeof(rcParent));

	rcParent.InflateRect(m_nBorderWidth - m_nBorderWidth / 3, m_nBorderWidth - m_nBorderWidth / 3, m_nBorderWidth - m_nBorderWidth / 3, m_nBorderWidth - m_nBorderWidth / 3);

	SetWindowPos(NULL, rcParent.left, rcParent.top, rcParent.Width(), rcParent.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	Invalidate(TRUE);
}

void CBorderDlg::Init(HWND hWnd, int nBorderWidth, COLORREF crBorder)
{
	m_hParent = hWnd;
	m_nBorderWidth = nBorderWidth;
	m_crBorder = crBorder;

	CheckRect();
	::SetWindowPos(m_hParent, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	ModifyStyleEx(0, WS_EX_TRANSPARENT);

	::SetWindowPos(m_hWnd, m_hParent, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	SetTimer(1, 10, NULL);
	Invalidate(TRUE);
}
