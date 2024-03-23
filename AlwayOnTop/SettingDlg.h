#pragma once
#include "afxdialogex.h"

typedef struct _SETTING
{
	int nSize;
	DWORD clr;

	_SETTING()
	{
		nSize = 20;
		clr = RGB(3, 129, 254);
	}
}SETTING;
// CSettingDlg 对话框

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSettingDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_SETTING };

	SETTING setting() { return m_setting; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDeltaposSpinSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStnClickedStaticClr();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

	SETTING LoadConfig();
	void Save();

	SETTING GetConfig();
	void SetConfig(SETTING setting);

	CBrush m_brush;
	SETTING m_setting;
public:
	afx_msg void OnEnChangeEditSize();
};
