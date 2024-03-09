#pragma once
#include "afxdialogex.h"


// CViewDlg 对话框

class CViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CViewDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_BG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	void PaintBG();
	void CheckMonitor();

	HWND m_hFocusWnd;
	MONITORINFO m_mi;
	CRect m_rcLast;
public:
	void Init(HWND hWnd);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
