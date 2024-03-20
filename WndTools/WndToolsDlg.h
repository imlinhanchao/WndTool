
// WndToolsDlg.h: 头文件
//

#pragma once
#include "CViewDlg.h"
#include "CBorderDlg.h"
#include "CHotKeyDlg.h"


typedef struct _Margin
{
	int l;
	int t;
	int r;
	int b;
	_Margin() {
		l = t = r = b;
	}
} Margin, *PMargin;


// CWndToolsDlg 对话框
class CWndToolsDlg : public CDialogEx
{
// 构造
public:
	CWndToolsDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WNDTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTabSetting(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	void Init();
	void LoadPlugin();
	void Resize(CRect rcChild);
	
	CViewDlg* m_pViewDlg;
	CBorderDlg* m_pBorderDlg;

	std::map<CString, Plugin::PPLUGIN_EXECUTE> m_Plugins;

	CWnd* m_AlwayOnTop;
	std::vector<CWnd*> m_lstSettingDlg;
	CHotKeyDlg m_HotKeyDlg;
	CTabCtrl m_Tabs;

	Margin m_TabMargin;
public:
};
