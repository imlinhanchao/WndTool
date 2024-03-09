#pragma once
#include "afxdialogex.h"


// CBorderDlg 对话框

class CBorderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBorderDlg)

public:
	CBorderDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBorderDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_BORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()

	void CheckRect();

	HWND m_hParent;
	int m_nBorderWidth;
	COLORREF m_crBorder;
public:
	void Init(HWND hWnd, int nBorderWidth = 20, COLORREF crBorder = RGB(3, 129, 254));
};
