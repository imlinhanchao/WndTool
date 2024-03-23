// SettingDlg.cpp: 实现文件
//

#include "pch.h"
#include "afxdialogex.h"
#include "SettingDlg.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SETTING, pParent)
{
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SIZE, &CSettingDlg::OnDeltaposSpinSize)
	ON_STN_CLICKED(IDC_STATIC_CLR, &CSettingDlg::OnStnClickedStaticClr)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_SIZE, &CSettingDlg::OnEnChangeEditSize)
END_MESSAGE_MAP()

BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetConfig(LoadConfig());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_brush;
}

void CSettingDlg::OnDeltaposSpinSize(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int nSize = GetDlgItemInt(IDC_EDIT_SIZE);
	if (pNMUpDown->iDelta >= 0)
	{
		SetDlgItemInt(IDC_EDIT_SIZE, nSize - 1);
	}
	else if (pNMUpDown->iDelta < 0)
	{
		SetDlgItemInt(IDC_EDIT_SIZE, max(0, nSize + 1));
	}

	Save();
	*pResult = 0;
}


void CSettingDlg::OnStnClickedStaticClr()
{
	CColorDialog clr(m_setting.clr);
	if (IDOK != clr.DoModal()) return;
	m_setting.clr = clr.GetColor();
	Save();
	SendMessage(WM_PAINT);
}


void CSettingDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	DC::DrawColor((CStatic*)GetDlgItem(IDC_STATIC_CLR), m_setting.clr);
	// 不为绘图消息调用 CDialogEx::OnPaint()
}

SETTING CSettingDlg::LoadConfig()
{
	SETTING setting;
	CString sConfig = Path::GetInstanceDirectory(_T("config.ini"));

	Ini ini(sConfig);

	ini.Read(_T("Setting"), _T("Size"), setting.nSize);
	ini.Read(_T("Setting"), _T("Color"), setting.clr);

	return setting;
}

void CSettingDlg::Save()
{
	SETTING setting = GetConfig();
	CString sConfig = Path::GetInstanceDirectory(_T("config.ini"));

	Ini ini(sConfig);

	ini.Write(_T("Setting"), _T("Size"), setting.nSize);
	ini.Write(_T("Setting"), _T("Color"), setting.clr);
}

SETTING CSettingDlg::GetConfig()
{
	SETTING setting;

	GetDlgItemInt(IDC_EDIT_SIZE, &setting.nSize);
	setting.clr = m_setting.clr;

	return setting;
}

void CSettingDlg::SetConfig(SETTING setting)
{
	SetDlgItemInt(IDC_EDIT_SIZE, setting.nSize);
	m_setting.clr = setting.clr;
	SendMessage(WM_PAINT);
}


void CSettingDlg::OnEnChangeEditSize()
{
	Save();
}
