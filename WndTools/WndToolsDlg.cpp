
// WndToolsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WndTools.h"
#include "WndToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWndToolsDlg 对话框



CWndToolsDlg::CWndToolsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WNDTOOLS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWndToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETTING, m_Tabs);
}

BEGIN_MESSAGE_MAP(CWndToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTING, &CWndToolsDlg::OnTcnSelchangeTabSetting)
END_MESSAGE_MAP()


// CWndToolsDlg 消息处理程序

BOOL CWndToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BOOL CWndToolsDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_HOTKEY)
	{
		CHotKey::Execute(pMsg->wParam);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWndToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWndToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWndToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWndToolsDlg::OnBnClickedOk()
{
	//m_pViewDlg = new CViewDlg();
	//m_pViewDlg->Create(IDD_DLG_BG, this);
	//m_pViewDlg->Init(m_hWnd);
	m_pBorderDlg = new CBorderDlg();
	m_pBorderDlg->Create(IDD_DLG_BORDER, this);
	m_pBorderDlg->Init(m_hWnd);
}

void CWndToolsDlg::OnTcnSelchangeTabSetting(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = m_Tabs.GetCurSel();
	CDialogEx* pDlgs [] = { &m_HotKeyDlg, m_AlwayOnTop };
	const int nSize = sizeof(pDlgs) / sizeof(CDialogEx*);
	for (int i = 0; i < nSize; i++) {
		pDlgs[i]->ShowWindow(nIndex == i ? SW_SHOW : SW_HIDE);
	}

	CRect rect;
	pDlgs[nIndex]->GetWindowRect(rect);
	Resize(rect);
	
	*pResult = 0;
}

void CWndToolsDlg::Init()
{
	// 设置Tab
	m_Tabs.InsertItem(0, _T("Hotkey"));

	// 设置Tab页
	m_HotKeyDlg.Create(IDD_DLG_HOTKEY, &m_Tabs);
	CRect rcHotKey, rcTab, rcClient;
	m_HotKeyDlg.GetWindowRect(rcHotKey);
	m_Tabs.GetWindowRect(rcTab);
	GetWindowRect(rcClient);

	HINSTANCE hDll = LoadLibrary(_T("plugin\\AlwayOnTop\\main.dll"));  // 替换为 DLL 的文件名

	if (hDll != NULL)
	{
		PFN_GET_SETTING pfnGetSettingDlg = (PFN_GET_SETTING)GetProcAddress(hDll, "GetSettingDlg");

		if (pfnGetSettingDlg != NULL)
		{
			m_AlwayOnTop = pfnGetSettingDlg(&m_Tabs);
			m_Tabs.InsertItem(1, _T("AlwayOnTop"));
			m_AlwayOnTop->SetWindowPos(NULL, 5, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}

	m_TabMargin.l = rcTab.left;
	m_TabMargin.t = rcTab.top;

	m_TabMargin.r = rcClient.right - rcTab.right;
	m_TabMargin.b = rcClient.bottom - rcTab.bottom;

	Resize(rcHotKey);

	// 显示Tab页
	m_HotKeyDlg.SetWindowPos(NULL, 5, 30, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);

	CenterWindow();
}

void CWndToolsDlg::Resize(CRect rcChild)
{
	int nTabWidth = rcChild.Width() + 10;
	int nTabHeight = rcChild.Height() + 35;
	int nClientWidth = m_TabMargin.l + nTabWidth + m_TabMargin.r;
	int nClientHeight = m_TabMargin.t + nTabHeight + m_TabMargin.b;

	SetWindowPos(NULL, 0, 0, nClientWidth, nClientHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
	m_Tabs.SetWindowPos(NULL, 0, 0, nTabWidth, nTabHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
}

