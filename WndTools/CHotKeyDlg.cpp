// CHotKeyDlg.cpp: 实现文件
//

#include "pch.h"
#include "WndTools.h"
#include "afxdialogex.h"
#include "CHotKeyDlg.h"


// CHotKeyDlg 对话框

IMPLEMENT_DYNAMIC(CHotKeyDlg, CDialogEx)

CHotKeyDlg::CHotKeyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_HOTKEY, pParent)
{
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CHotKeyDlg::~CHotKeyDlg()
{
}

void CHotKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHotKeyDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CHotKeyDlg 消息处理程序


BOOL CHotKeyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CHotKeyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}
